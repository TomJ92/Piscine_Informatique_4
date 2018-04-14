#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

/*!
 * \file graph.h
 * \brief Darwin simulator: classes
 * \author Adrien MOUFID, Tom JOUVET, Benoit RAMS, ING2 TD5
 * \version 0.1
 */

#include <fstream>
#include <queue>
#include <iostream>
#include <ctime>
/**************************************************************
    Ici sont proposées 3 classes fondamentales
            Vertex (=Sommet)
            Edge (=Arête ou Arc)
            Graph (=Graphe)

    Les arêtes et les sommets et le graphe qu'ils constituent
    "travaillent" étroitement ensemble : pour cette raison les
    Vertex et Edge se déclarent amis (friend) de Graph pour que
    ce dernier puisse librement accéder aux membres (y compris
    protected ou private) de Vertex et Edge.

    Ces Classes peuvent êtres complétées. Il est également possible
    de les dériver mais il est malheureusement assez difficile
    de dériver le "triplet" des 3 classes en maintenant des relations
    cohérentes ( rechercher "c++ class covariance" et "c++ parallel inheritance"
    pour commencer .. ). Il est donc sans doute préférable, si possible,
    de "customiser" ces classes de base directement, sans héritage.

    Le modèle proposé permet de représenter un graphe orienté éventuellement
    pondéré, les arcs portent une ou des informations supplémentaire(s).
    Les relations/navigations Arcs -> Sommets et Sommets -> Arcs se font
    dans les 2 sens et utilisent des INDICES et NON PAS DES ADRESSES (pointeurs)
    de telle sorte que la topologie du graphe puisse être assez facilement
    lue et écrite en fichier, et bénéficie d'une bonne lisibilité en cas de bugs...

    Chaque arc possède 2 attributs principaux (en plus de son éventuelle pondération)
        -> m_from (indice du sommet de départ de l'arc )
        -> m_to (indice du sommet d'arrivée de l'arc )

    Chaque sommet possède 2 liste d'arcs (en plus de ses attributs "internes", marquages...)
        -> m_in (liste des indices des arcs arrivant au sommet : accès aux prédécesseurs)
        -> m_out (liste des indices des arcs partant du sommet : accès aux successeurs)

    Cependant le problème des indices (par rapport aux pointeurs) et qu'en cas
    de destruction d'une entité (un arc et/ou un sommet sont enlevés du graphe) alors :

    - Soit il faut reprendre toute la numérotation pour "boucher le trou"
      (par exemple on a supprimé le sommet n°4, le sommet n°5 devient le 4, 6 devient 5 etc...)
      ce qui pose des problèmes de stabilité et de cohérence, et une difficulté à ré-introduire
      le(s) même(s) élément supprimé (au même indice)

    - Soit on admet que la numérotation des sommets et arcs n'est pas contigue, càd qu'il
      peut y avoir des "trous" : sommets 0 1 5 7 8, pas de sommets 2 ni 3 ni 4 ni 6. La numérotation
      est stable mais on ne peut plus utiliser un simple vecteur pour ranger la liste de tous
      les arcs et tous les sommets aux indices correspondants, on peut utiliser une map
      qui associe un objet arc ou sommet à des indices arbitraires (pas forcément contigus)

    C'est cette 2ème approche qui est proposée ici : dans la classe graphe vous trouverez
        -> map<int, Edge>   m_edges
        -> map<int, Vertex> m_vertices    (le pluriel de vertex est vertices)

    Il faudra être attentif au fait que par rapport à un simple vecteur, le parcours des éléments
    ne pourra PAS se faire avec un simple for (int i=0; i<m_edges.size(); ++i) ...m_edges[i]...
    et que les parcours à itérateur ne donneront pas directement des Edge ou des Vertex
    mais des pairs, l'objet d'intérêt se trouvant dans "second" ("first" contenant l'indice)
                for (auto &it = m_edges.begin(); it!=m_edges.end(); ++it) ...it->second...
    ou bien     for (auto &e : m_edges) ...e.second...

    Il n'est pas obligatoire d'utiliser ces classes pour le projet, vous pouvez faire les votres

    Au niveau de l'interface, on dissocie une classe interface associée à chaque classe fondamentale
    de telle sorte qu'il soit possible de travailler avec des graphes non représentés à l'écran
    Imaginons par exemple qu'on doive générer 1000 permutations de graphes pour tester des
    combinaisons, on ne souhaite pas représenter graphiquement ces 1000 graphes, et les
    interfaces pèsent lourd en ressource, avec cette organisation on est libre de réserver ou
    pas une interface de présentation associée aux datas (découplage données/interface)

***********************************************************************************************/

#include <vector>
#include <map>
#include <string>
#include <memory>

#include "grman/grman.h"

/***************************************************
                    VERTEX
****************************************************/
/*! 
 * \class VertexInterface
 *
 * \brief Interface d'un sommet
 */
 
class VertexInterface
{
    // Les (methodes des) classes amies pourront accéder
    // directement aux attributs (y compris privés)
    friend class Vertex; /*!< La classe Vertex y a accès*/
    friend class EdgeInterface; /*!< La classe EdgeIntreface y a accès*/
    friend class Graph; /*!< La classe Graph y a accès*/

    private :

        /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de déclarer
        /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
        /// le paramétrer ( voir l'implémentation du constructeur dans le .cpp )

        // La boite qui contient toute l'interface d'un sommet
        grman::WidgetBox m_top_box; /*!< Boite contenant l'interface du sommet*/

        // Un slider de visualisation/modification de la valeur du sommet
        grman::WidgetVSlider m_slider_value; /*!<Curseur sur le cote indiquant l'effectif d'un sommet*/

        // Un label de visualisation de la valeur du sommet
        grman::WidgetText m_label_value; /*!<Label qui indique l'effectif du sommet en question, en dessous du curseur*/

        // Une image de "remplissage"
        grman::WidgetImage m_img; /*!<Image du sommet*/

        // Un label indiquant l'index du sommet
        grman::WidgetText m_label_idx; /*!<Label indiquant le numero de sommet*/

        // Une boite pour le label précédent
        grman::WidgetText m_box_label_idx; /*!<Une petite boite qui contient le numero de sommet*/
        ///croix pour supprimer un sommets
        grman::WidgetCheckBox m_cross; /*!<Un symbole de croix sur lequel on clique pour supprimer le sommet*/
        ///bouton pour savoir quelle sommets pour les aretes
        grman::WidgetButton m_button_addEdge; /*!<Un carre bleu sur lequel on clique si on veut que l'arete ajoutee soit connectee au sommet*/
        grman::WidgetText m_textNum;

        grman::WidgetImage m_img2; /*!<Une image de croix a afficher si l'effectif d'un sommet est de 0*/


    public :
        /*! \brief Constructeur
        *
        *Constructeur d'interface de sommet
        *
        * \param idx (indice de sommet), x (abscisse), y (ordonnee), pic_name (nom de l'image du sommet), pic_idx (indice de l'image du sommet)
        */
        // Le constructeur met en place les éléments de l'interface
        // voir l'implémentation dans le .cpp
        VertexInterface(int idx, int x, int y, std::string pic_name="", int pic_idx=0);
};
 
/*! 
 * \class Vertex
 * \brief Sommet d'un graphe
 */

class Vertex
{
    // Les (methodes des) classes amies pourront accéder
    // directement aux attributs (y compris privés)
    friend class Graph; /*!< La classe Graph y a accès*/
    friend class VertexInterface; /*!< La classe VertexInterface y a accès*/
    friend class Edge; /*!< La classe Edge y a accès*/
    friend class EdgeInterface; /*!< La classe EdgeInterface y a accès*/

    private :
        /// liste des indices des arcs arrivant au sommet : accès aux prédécesseurs
        std::vector<int> m_in; /*!< Liste des prédecesseurs du sommet*/

        /// liste des indices des arcs partant du sommet : accès aux successeurs
        std::vector<int> m_out; /*!<Liste des successeurs du sommet*/

        /// un exemple de donnée associée à l'arc, on peut en ajouter d'autres...
        double m_value; /*!<Effectif du sommet, compris entre 0 et 100*/

        /// le POINTEUR sur l'interface associée, nullptr -> pas d'interface
        std::shared_ptr<VertexInterface> m_interface = nullptr; /*!< Pointeur interface pour pouvoir montrer le sommet à l'ecran*/

        ///indice des sommets
        int m_VertexIn; /*!<Indice d'un sommet predecesseur*/
        int m_VertexOut; /*!<Indice d'un sommet successeur*/

        bool m_existe; /*!<Booleen qui indique si le sommet existe ou non*/
        bool m_marqueur; /*!<Booleen de marquage*/
        bool m_marque; /*!<Booleen de marquage*/
        int m_numero_compo_connexe; /*!<Numero de la composante connexe a laquelle le sommet appartient*/
        /// Cacapité de portage de l'environnement
        double k_capacite; /*!<Capacite de portage du sommet dans l'environnement*/
        /// Rythme de croissance
        double coeff_croissance; /*!<Coefficient de croissance du sommet*/
        ///composante fortemenet connexe

        // Docu shared_ptr : https://msdn.microsoft.com/fr-fr/library/hh279669.aspx
        // La ligne précédente est en gros équivalent à la ligne suivante :
        // VertexInterface * m_interface = nullptr;

    public:
        /*!
         * \brief Constructeur
         *
         *\param value (effectif initialise a 0), *interface (interface du sommet initialise)
         */ 
        /// Les constructeurs sont à compléter selon vos besoin...
        /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
        Vertex (double value=0, VertexInterface *interface=nullptr) :
            m_value(value), m_interface(interface)  {  }

        /// Vertex étant géré par Graph ce sera la méthode update de graph qui appellera
        /// le pre_update et post_update de Vertex (pas directement la boucle de jeu)
        /// Voir l'implémentation Graph::update dans le .cpp
        /*!
         * \brief Permiere mise a jour du sommet avant l'appel a l'interface*/
        void pre_update(); 
        /*!
         * \brief Deuxieme mise a jour du sommet apres l'appel a l'interface*/
        void post_update(); 
};



/***************************************************
                    EDGE
****************************************************/
/*!
 * \class EdgeInterface
 * \brief Interface d'une arete
 */
class EdgeInterface
{
    // Les (methodes des) classes amies pourront accéder
    // directement aux attributs (y compris privés)
    friend class Edge; /*!< La classe Edge y a acces*/
    friend class Graph; /*!< La classe Graph y a acces*/

    private :

        /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de déclarer
        /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
        /// le paramétrer ( voir l'implémentation du constructeur dans le .cpp )

        // Le WidgetEdge qui "contient" toute l'interface d'un arc
        grman::WidgetEdge m_top_edge; /*!<Trait qui contient l'interface d'une arete*/

        // Une boite pour englober les widgets de réglage associés
        grman::WidgetBox m_box_edge; /*!<Petite boite pour contenir ce qui caracterise une arete*/

        // Un slider de visualisation/modification du poids valeur de l'arc
        grman::WidgetVSlider m_slider_weight; /*!<Curseur qui indique le poids de l'arete*/

        // Un label de visualisation du poids de l'arc
        grman::WidgetText m_label_weight; /*!<Texte qui indique le poids de l'arete*/
        ///croix pour supprimer les arêtes
        grman::WidgetCheckBox m_cross; /*!<Croix sur laquelle cliquer si on veut supprimer l'arete*/
        ///texte pour afficher les numéro de sommet de l'arête
        grman::WidgetText m_text_number; /*!<Texte pour indiquer le sommet de depart et le sommet d'arrivee*/

    public :

        // Le constructeur met en place les éléments de l'interface
        // voir l'implémentation dans le .cpp
        /*! \brief Constructeur
        * \param from (Sommet de depart), to (sommet d'arrivee), numS1 (indice du sommet de depart), numS2 (indice du sommet d'arrivee)
        */
        EdgeInterface(Vertex& from, Vertex& to,int numS1, int numS2);
};

/*!
 * \class Edge
 * \brief Arete du graphe
 */
class Edge
{
    // Les (methodes des) classes amies pourront accéder
    // directement aux attributs (y compris privés)
    friend class Graph; /*!< La classe Graph y a acces*/
    friend class EdgeInterface; /*!<La classe EdgeInterface y a acces*/

    private :
        /// indice du sommet de départ de l'arc
        int m_from; /*!<Indice du sommet de depart*/

        /// indice du sommet d'arrivée de l'arc
        int m_to; /*!<Indice du sommet d'arrivee*/

        /// un exemple de donnée associée à l'arc, on peut en ajouter d'autres...
        double m_weight; /*!<Poids de l'arete*/

        /// le POINTEUR sur l'interface associée, nullptr -> pas d'interface
        std::shared_ptr<EdgeInterface> m_interface = nullptr; /*!<Pointeur sur l'interface initialise*/



    public:

        /// Les constructeurs sont à compléter selon vos besoin...
        /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
        /*!
         * \brief Constructeur
         *
         * \param weight (Poids initialise), *interface (pointeur sur interface initialise)
         */
        Edge (double weight=0, EdgeInterface *interface=nullptr) :
            m_weight(weight), m_interface(interface)  {  }

        /// Edge étant géré par Graph ce sera la méthode update de graph qui appellera
        /// le pre_update et post_update de Edge (pas directement la boucle de jeu)
        /// Voir l'implémentation Graph::update dans le .cpp
        /*!
         * \brief Permiere mise a jour de l'arete avant l'appel a l'interface*/
        void pre_update();
        /*!
         * \brief Deuxieme mise a jour de l'arete apres l'appel a l'interface*/
        void post_update();
        ///accesseurs
        /*!
         * \brief Accesseur au sommet de depart
         * \return Indice du sommet de depart
         */
        int getFrom() { return m_from; }
        /*!
         * \brief Accesseur au sommet d'arrivee
         * \return Indice du sommet d'arrivee
         */
        int getTo() { return m_to; }
        /*!
         * \brief Modificateur du sommet de depart
         * \param from (sommet de depart)
         */
        void setFrom(int from) {m_from=from;}
        /*!
         * \brief Modificateur du sommet d'arrivee
         * \param from (sommet d'arrivee)
         */
        void setTo(int to) {m_to=to;}

};




/***************************************************
                    GRAPH
****************************************************/
/*!
 * \class GraphInterface
 *
 * \brief Interface d'un graphe
 */
class GraphInterface
{
    friend class Graph; /*!<La classe graphe y a acces*/

    private :

        /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de déclarer
        /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
        /// le paramétrer ( voir l'implémentation du constructeur dans le .cpp )

        /// La boite qui contient toute l'interface d'un graphe
        grman::WidgetBox m_top_box; /*!<Boite pour contenir le graphe*/

        /// Dans cette boite seront ajoutés les (interfaces des) sommets et des arcs...
        grman::WidgetBox m_main_box; /*!<Boite pour contenir tous les sommets et toutes les aretes*/

        /// Dans cette boite seront ajoutés des boutons de contrôle etc...
        grman::WidgetBox m_tool_box; /*!<Boite sur le cote pour les boutons de controle: forte connexite, k-connexite, ajout, suppression...*/


        // A compléter éventuellement par des widgets de décoration ou
        // d'édition (boutons ajouter/enlever ...)
        ///bouton pour sauver avec le texte
        grman::WidgetButton m_button_save; /*!<Bouton pour sauver le graphe*/
        grman::WidgetText m_text_save; /*!<Marquage "SAUVER" sur le bouton*/
        ///bouton pour reset les postion et les sommets de notre graphe
        grman::WidgetButton m_button_reset; /*!<Bouton pour reinitialiser le graphe*/
        grman::WidgetText m_text_reset; /*!<Marquage "RESET" sur le bouton*/
        ///bouton pour quitter notre graphe
        grman::WidgetButton m_button_quit;/*!<Bouton pour quitter le graphe*/
        grman::WidgetText m_text_quit; /*!<Marquage "QUITTER" sur le bouton*/
        ///bouton pour ajouter un sommet
        grman::WidgetButton m_button_addVertex;/*!<Bouton pour ajouter un sommet*/
        grman::WidgetText m_text_addV;/*!<Marquage "ADD SOMMET" sur le bouton*/
        grman::WidgetText m_text_vertex;
        ///bouton pour ajouter une arête
        grman::WidgetButton m_button_addEdge; /*!<Bouton pour ajouter une arete*/
        grman::WidgetText m_text_addE;/*!<Marquage "ADD EDGE" sur le bouton*/
        grman::WidgetText m_text_edge;
        ///bouton pour ne plus ajouter sommet
        grman::WidgetButton m_button_noAddEdge; /*!<Bouton pour ne plus ajouter d'arete*/
        ///bouton pour afficher les combinaison possible
        grman::WidgetButton m_button_k_connex; /*!<Bouton pour afficher la k-connexite sur la console*/
        grman::WidgetText m_text_k; /*!<Marquage "K CONNEX" sur le bouton*/
        grman::WidgetText m_text_connex;
        ///bouton pour afficher les composante fortement connexe
        grman::WidgetButton m_button_connex; /*!<Bouton pour afficher les composantes fortement connexes*/
        grman::WidgetText m_text_compo; /*!<Marquage "COMPO CONNEX" sur le bouton*/
        grman::WidgetText m_text_connexe;
        ///bouton pour lancer la simulation
        grman::WidgetButton m_button_start;/*!<Bouton pour lancer la simulation du graphe dans le temps*/
        grman::WidgetText m_text_run; /*!<Marquage du bouton*/
        ///bouton pour arreter la simulation
        grman::WidgetButton m_button_stop; /*!<Bouton pour arrete la simulation du graphe dans le temps*/
        grman::WidgetText m_text_stop; /*!<Marquage sur le bouton*/
        ///bouton pour mettre tout aléatoire
        grman::WidgetButton m_button_random;/*!<Bouton pour donner des valeurs aleatoires aux effectifs de somemts et poids des aretes*/
        grman::WidgetText m_text_random; /*!<Marquage du bouton*/
        ///petit bouton pour plus afficher les composante connexe
        grman::WidgetButton m_button_restartConnex;/*!<Bouton pour ne plus afficher les composantes fortement connexes*/
        ///bouton pour afficher graphe réduit

    public :
        /*!
         *
         * \brief Constructeur
         * \param x (Abscisse de depart), y (Ordonnee de depart), w (Longueur de la boite du graphe), h (Hauteur de la boite du graphe)
         */
        // Le constructeur met en place les éléments de l'interface
        // voir l'implémentation dans le .cpp
        GraphInterface(int x, int y, int w, int h);
};

/*!
 * \class Graph
 * \brief Graphe consulte
 */

class Graph
{
    private :

        /// La "liste" des arêtes
        std::map<int, Edge> m_edges; /*!<Liste des aretes du graphe*/

        /// La liste des sommets
        std::map<int, Vertex> m_vertices; /*!<Liste des sommets du graphe*/

        /// le POINTEUR sur l'interface associée, nullptr -> pas d'interface
        std::shared_ptr<GraphInterface> m_interface = nullptr; /*!<Pointeur sur l'interface*/

        ///ordre du graphe
        int m_ordre; /*!<Ordre du graphe*/

        ///nb d'arête du graphe
        int m_nbArete; /*!<Nombre d'aretes du graphe*/

        ///NUMERO DU GRAPHE
        int m_numGraphe; /*!<Numero du graphe: 1,2 ou 3*/

        ///boléen pour quitter le graphe
        bool m_quitGraphe; /*!< Booleen pour quitter le graphe*/

        ///boléen qui dit si on doit ajouter une arête
        bool m_ajouterNewEdge; /*!<Booleen pour l'ajout d'arete*/

        ///SAVOIR LES SOMMET QUI VONT FORMER UNE ARRËTE
        ///savoir si on ajoute le sommet1 ou 2
        bool m_addTo; /*!<Booleen du 1er sommet de l'arete*/
        bool m_addFrom; /*!<Booleen du 2eme sommet de l'arete*/
        std::vector<int> m_vertexForNewEdge; /*!<Liste de sommets eligibles pour ajouter une arete*/
        ///pour éviter de selectionner 2 fois le même sommets
        int m_vertexAlreadyUse; /*!<Sommet 1 deja marque pour ne pas le rechoisir quand on ajoute une arete*/

        int m_toursDeBoucle; /*!<Tour de boucle*/
        int m_toursDeBoucleMax;/*!< Tour de boucle max*/

    public:

        /// Les constructeurs sont à compléter selon vos besoin...
        /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
        /*!
        * 
        * \brief Constructeur
        * \param *interface (Pointeur initialise)
        */
        Graph (GraphInterface *interface=nullptr) :
            m_interface(interface)  {  }
         /*!
        * \brief Ajout d'un sommet
        * \param idx (Indice), value (effectif), x (abscisse), y (ordonnee), pic_name (nom de l'image), pic_idx (indice de l'image)
        */
        void add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name="",int pic_idx=0 );
         /*!
        * 
        * \brief Ajout d'une arete
        * \param idx (indice), vert1 (indice du sommet de depart), vert2 (indice du sommet d'aretes), weight (poids de l'arete)
        */
        void add_interfaced_edge(int idx, int vert1, int vert2, double weight=0);

        /// Méthode spéciale qui construit un graphe arbitraire (démo)
        /// Voir implémentation dans le .cpp
        /// Cette méthode est à enlever et remplacer par un système
        /// de chargement de fichiers par exemple.
         /*!
        * \brief Methode non utilisee de chargement de graphe
        */
        void make_example();


        /// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
        /*!
         * \brief Mise a jout du graphe
         * \param ini (instant t), animation (verifier si l'animation est activee)
         */
        void update(clock_t ini, bool animation);
        ///ss prog qui remplit un graphe en fonction d'un fichier
        /*!
         * \brief Chargement de graphe depuis un fichier
         * \param fileName (Nom du fichier sans le .txt), num (numero de graphe)
         */
        void ReadFile(std::string fileName, int num);
        ///ss prog qui sauve un graphe en remplissant un fichier
        /*!
         * \brief Sauvegarde du graphe dans un fichier
         * \param fileName (Nom du fichier sans le .txt)
         */
        void saveFile(std::string fileName);
        ///réinitialise les sommet à leurs position de départ
        /*!
         * \brief Reinitialisation du graphe a son etat d'origine
         * \param fileName (Nom du fichier appele sans le .txt)
         */
        void reinit(std::string fileName);
        ///initialisation du graphe
         /*!
         * \brief Initialisateur du graphe
         */
        void initialisation();
        ///setter
        /*!
         * \brief Accesseur au booleen pour quitter le graphe
         * \return Booleen pour quitter le graphe
         */
        bool getQuitGraph() {return m_quitGraphe;}
        /*!
         * \brief Modificateur du booleen pour quitter le graphe
         * \param Booleen pour quitter le graphe
         */
        void setQuitGraph(bool quitGraph) {m_quitGraphe=quitGraph; }
        ///enlever un sommet
        /*!
         * \brief Suppression d'un sommet
         * \param num (indice du sommet a supprimer)
         */
        void removeVertex(int num);
        ///enlever une arête
        /*!
         * \brief Suppression d'une arete
         * \param num (indice de l'arete a supprimer)
         */
        void removeEdge(int num);
        ///ajouter un sommet
        /*!
         * \brief Ajout d'un sommet
         */
        void addVertex();
        ///ajouter une arête
        /*!
         * \brief Ajout d'un arete
         * \param m_newVertexForEdge (vecteur de sommets eligibles pour y ajouter une arete)
         */
        void addEdge(std::vector<int> m_newVertexForEdge);
        ///mettre tout les bouton à false
        /*!
         * \brief Mise de tous les booleens de bouton a FALSE
         */
        void initButton();
        ///k-sommet connexité
         /*!
         * \brief Activation de la k-connexite pour les sommets
         */
        void kVertexConnexite();
        ///ajouter une combinaison
        /*!
         * \brief Ajout d'une combinaison de sommets a supprimer pour la k-connexite
         * \param j, k, m_allCombi (vecteur ou se trouvent les combinaisons possibles), i
         */
        void addCombi(int j, int k, int nb,std::vector<std::vector <int>> &m_allCombi,int i=0);
        ///cheminInverse
        ///ajouter une combinaison
        /*!
         * \brief Booleen determinant la possibilite de faire le chemin inverse entre deux sommets
         * \param depart (sommet de depart), arrivee (sommet d'arrivee)
         * \return true ou false selon la possibilite de faire le chemin inverse entre deux sommets indiques
         */
        bool cheminInverse(int depart,int arrive);
        ///debut k connexité
        /*!
         * \brief Initialisation de la k-connexite
         */
        void init_k_connex();
        ///k connexité
        /*!
         * \brief k-connexite
         * \param inter (combinaison), m_allCombi (toutes les combinaisons possibles trouvees)
         */
        void k_connex(std::vector<int>& inter,std::vector<std::vector <int>>& m_allCombi);
        ///boléen pour savoir si le chemin est bon
        /*!
         * \brief Booleen indiquant si le chemin est bon
         * \param idx (indice du sommet)
         * \return true ou false 
         */
        bool graphConnex(int idx);
        ///afficher les combinaison possible
        /*!
         * \brief affichage des combinaisons possibles
         * \param inter (combinaison), m_allCombi (toutes les combinaisons possibles trouvees)
         */
        void afficher_k_connex(std::vector<int>& inter,std::vector<std::vector <int>>& allCombi);
        ///forte Connex
        /*!
         * \brief affichage des composantes fortement connexes
         * \return map contenant toutes les composantes fortement connexes
         */
        std::map<int,std::vector<int>> algo_forte_connexite();
        /*!
         * \brief Booleen affichant la composante fortement connexe
         * \param i (numero de la composante)
         * \return ligne de booleens avec un true pour chaque sommet se trouvant dans la composante. A partir d'un true, la composante existe
         */
        bool* recherche_cmp(int i);
        /*!
         * \brief Rangement d'un sommet pour le rendre contigu a un autre
         * \param num (indice du sommet)
         */
        void rendreConti(int num);
        /*!
         * \brief Rangement des sommets pour avoir une map de sommets continue
         * \param fileName (Nom du fichier)
         */
        void rendreConti2(std::string fileName);
        /*!
         * \brief Croissance des populations
         * \param temps (instant t)
         */
        void croissance_sommets(clock_t temps);
        /*!
         * \brief Calcul de la capacite d'un sommet
         * \param ar_arriv (aretes arrivant a un sommet)
         * \return nouvelle capacite du sommet
         */
        double calculK(std::vector<Edge> ar_arriv);
        /*!
         * \brief Initialisation de la simulation
         */
        void initia();
        /*!
         * \brief Affichage d'une croix rouge sur les sommets a l'effectif 0
         */
        void croix_rouge();
        /*!
         * \brief Octroiement de valeurs random a des effectifs de sommets ou des poids d'aretes*/
         */
        void random();
};


#endif // GRAPH_H_INCLUDED
