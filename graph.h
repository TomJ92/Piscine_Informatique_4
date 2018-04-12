#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
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

class VertexInterface
{
    // Les (methodes des) classes amies pourront accéder
    // directement aux attributs (y compris privés)
    friend class Vertex;
    friend class EdgeInterface;
    friend class Graph;

    private :

        /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de déclarer
        /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
        /// le paramétrer ( voir l'implémentation du constructeur dans le .cpp )

        // La boite qui contient toute l'interface d'un sommet
        grman::WidgetBox m_top_box;

        // Un slider de visualisation/modification de la valeur du sommet
        grman::WidgetVSlider m_slider_value;

        // Un label de visualisation de la valeur du sommet
        grman::WidgetText m_label_value;

        // Une image de "remplissage"
        grman::WidgetImage m_img;

        // Un label indiquant l'index du sommet
        grman::WidgetText m_label_idx;

        // Une boite pour le label précédent
        grman::WidgetText m_box_label_idx;
        ///croix pour supprimer un sommets
        grman::WidgetCheckBox m_cross;
        ///bouton pour savoir quelle sommets pour les arêtes
        grman::WidgetButton m_button_addEdge;
        grman::WidgetText m_textNum;

        grman::WidgetImage m_img2;


    public :

        // Le constructeur met en place les éléments de l'interface
        // voir l'implémentation dans le .cpp
        VertexInterface(int idx, int x, int y, std::string pic_name="", int pic_idx=0);
};


class Vertex
{
    // Les (methodes des) classes amies pourront accéder
    // directement aux attributs (y compris privés)
    friend class Graph;
    friend class VertexInterface;
    friend class Edge;
    friend class EdgeInterface;

    private :
        /// liste des indices des arcs arrivant au sommet : accès aux prédécesseurs
        std::vector<int> m_in;

        /// liste des indices des arcs partant du sommet : accès aux successeurs
        std::vector<int> m_out;

        /// un exemple de donnée associée à l'arc, on peut en ajouter d'autres...
        double m_value;

        /// le POINTEUR sur l'interface associée, nullptr -> pas d'interface
        std::shared_ptr<VertexInterface> m_interface = nullptr;

        ///indice des sommets
        int m_VertexIn;
        int m_VertexOut;

        bool m_existe;
        bool m_marqueur;
        bool m_marque;
        int m_numero_compo_connexe;
        /// Cacapit� de portage de l'environnement
        double k_capacite;
        /// Rythme de croissance
        double coeff_croissance;
        ///composante fortemenet connexe

        // Docu shared_ptr : https://msdn.microsoft.com/fr-fr/library/hh279669.aspx
        // La ligne précédente est en gros équivalent à la ligne suivante :
        // VertexInterface * m_interface = nullptr;

    public:

        /// Les constructeurs sont à compléter selon vos besoin...
        /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
        Vertex (double value=0, VertexInterface *interface=nullptr) :
            m_value(value), m_interface(interface)  {  }

        /// Vertex étant géré par Graph ce sera la méthode update de graph qui appellera
        /// le pre_update et post_update de Vertex (pas directement la boucle de jeu)
        /// Voir l'implémentation Graph::update dans le .cpp
        void pre_update();
        void post_update();
};



/***************************************************
                    EDGE
****************************************************/

class EdgeInterface
{
    // Les (methodes des) classes amies pourront accéder
    // directement aux attributs (y compris privés)
    friend class Edge;
    friend class Graph;

    private :

        /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de déclarer
        /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
        /// le paramétrer ( voir l'implémentation du constructeur dans le .cpp )

        // Le WidgetEdge qui "contient" toute l'interface d'un arc
        grman::WidgetEdge m_top_edge;

        // Une boite pour englober les widgets de réglage associés
        grman::WidgetBox m_box_edge;

        // Un slider de visualisation/modification du poids valeur de l'arc
        grman::WidgetVSlider m_slider_weight;

        // Un label de visualisation du poids de l'arc
        grman::WidgetText m_label_weight;
        ///croix pour supprimer les arêtes
        grman::WidgetCheckBox m_cross;
        ///texte pour afficher les numéro de sommet de l'arête
        grman::WidgetText m_text_number;

    public :

        // Le constructeur met en place les éléments de l'interface
        // voir l'implémentation dans le .cpp
        EdgeInterface(Vertex& from, Vertex& to,int numS1, int numS2);
};


class Edge
{
    // Les (methodes des) classes amies pourront accéder
    // directement aux attributs (y compris privés)
    friend class Graph;
    friend class EdgeInterface;

    private :
        /// indice du sommet de départ de l'arc
        int m_from;

        /// indice du sommet d'arrivée de l'arc
        int m_to;

        /// un exemple de donnée associée à l'arc, on peut en ajouter d'autres...
        double m_weight;

        /// le POINTEUR sur l'interface associée, nullptr -> pas d'interface
        std::shared_ptr<EdgeInterface> m_interface = nullptr;



    public:

        /// Les constructeurs sont à compléter selon vos besoin...
        /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
        Edge (double weight=0, EdgeInterface *interface=nullptr) :
            m_weight(weight), m_interface(interface)  {  }

        /// Edge étant géré par Graph ce sera la méthode update de graph qui appellera
        /// le pre_update et post_update de Edge (pas directement la boucle de jeu)
        /// Voir l'implémentation Graph::update dans le .cpp
        void pre_update();
        void post_update();
        ///accesseurs
        int getFrom() { return m_from; }
        int getTo() { return m_to; }
        void setFrom(int from) {m_from=from;}
        void setTo(int to) {m_to=to;}

};




/***************************************************
                    GRAPH
****************************************************/

class GraphInterface
{
    friend class Graph;

    private :

        /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de déclarer
        /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
        /// le paramétrer ( voir l'implémentation du constructeur dans le .cpp )

        /// La boite qui contient toute l'interface d'un graphe
        grman::WidgetBox m_top_box;

        /// Dans cette boite seront ajoutés les (interfaces des) sommets et des arcs...
        grman::WidgetBox m_main_box;

        /// Dans cette boite seront ajoutés des boutons de contrôle etc...
        grman::WidgetBox m_tool_box;


        // A compléter éventuellement par des widgets de décoration ou
        // d'édition (boutons ajouter/enlever ...)
        ///bouton pour sauver avec le texte
        grman::WidgetButton m_button_save;
        grman::WidgetText m_text_save;
        ///bouton pour reset les postion et les sommets de notre graphe
        grman::WidgetButton m_button_reset;
        grman::WidgetText m_text_reset;
        ///bouton pour quitter notre graphe
        grman::WidgetButton m_button_quit;
        grman::WidgetText m_text_quit;
        ///bouton pour ajouter un sommet
        grman::WidgetButton m_button_addVertex;
        grman::WidgetText m_text_addV;
        grman::WidgetText m_text_vertex;
        ///bouton pour ajouter une arête
        grman::WidgetButton m_button_addEdge;
        grman::WidgetText m_text_addE;
        grman::WidgetText m_text_edge;
        ///bouton pour ne plus ajouter sommet
        grman::WidgetButton m_button_noAddEdge;
<<<<<<< HEAD
        ///bouton pour afficher les combinaison possible
        grman::WidgetButton m_button_k_connex;
        grman::WidgetText m_text_k;
        grman::WidgetText m_text_connex;
        ///bouton pour afficher les composante fortement connexe
        grman::WidgetButton m_button_connex;
        grman::WidgetText m_text_compo;
        grman::WidgetText m_text_connexe;
        ///bouton pour lancer la simulation
        grman::WidgetButton m_button_start;
        grman::WidgetText m_text_run;
        ///bouton pour arreter la simulation
        grman::WidgetButton m_button_stop;
        grman::WidgetText m_text_stop;
        ///bouton pour mettre tout al�atoire
        grman::WidgetButton m_button_random;
        grman::WidgetText m_text_random;
        ///petit bouton pour plus afficher les composante connexe
        grman::WidgetButton m_button_restartConnex;
        ///bouton pour afficher graphe r�duit
=======

>>>>>>> 3e970ea5c73e26c768074acc39a950b1286b675d

    public :

        // Le constructeur met en place les éléments de l'interface
        // voir l'implémentation dans le .cpp
        GraphInterface(int x, int y, int w, int h);
};


class Graph
{
    private :

        /// La "liste" des arêtes
        std::map<int, Edge> m_edges;

        /// La liste des sommets
        std::map<int, Vertex> m_vertices;

        /// le POINTEUR sur l'interface associée, nullptr -> pas d'interface
        std::shared_ptr<GraphInterface> m_interface = nullptr;

        ///ordre du graphe
        int m_ordre;

        ///nb d'arête du graphe
        int m_nbArete;

        ///NUMERO DU GRAPHE
        int m_numGraphe;

        ///boléen pour quitter le graphe
        bool m_quitGraphe;

        ///boléen qui dit si on doit ajouter une arête
        bool m_ajouterNewEdge;

        ///SAVOIR LES SOMMET QUI VONT FORMER UNE ARRËTE
        ///savoir si on ajoute le sommet1 ou 2
        bool m_addTo;
        bool m_addFrom;
        std::vector<int> m_vertexForNewEdge;
        ///pour éviter de selectionner 2 fois le même sommets
        int m_vertexAlreadyUse;

        int m_toursDeBoucle;
        int m_toursDeBoucleMax;

    public:

        /// Les constructeurs sont à compléter selon vos besoin...
        /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
        Graph (GraphInterface *interface=nullptr) :
            m_interface(interface)  {  }

        void add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name="",int pic_idx=0 );
        void add_interfaced_edge(int idx, int vert1, int vert2, double weight=0);

        /// Méthode spéciale qui construit un graphe arbitraire (démo)
        /// Voir implémentation dans le .cpp
        /// Cette méthode est à enlever et remplacer par un système
        /// de chargement de fichiers par exemple.
        void make_example();


<<<<<<< HEAD
        /// La m�thode update � appeler dans la boucle de jeu pour les graphes avec interface
        void update(clock_t ini, bool animation);
=======
        /// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
        void update();
>>>>>>> 3e970ea5c73e26c768074acc39a950b1286b675d
        ///ss prog qui remplit un graphe en fonction d'un fichier
        void ReadFile(std::string fileName, int num);
        ///ss prog qui sauve un graphe en remplissant un fichier
        void saveFile(std::string fileName);
        ///réinitialise les sommet à leurs position de départ
        void reinit(std::string fileName);
        ///initialisation du graphe
        void initialisation();
        ///setter
        bool getQuitGraph() {return m_quitGraphe;}
        void setQuitGraph(bool quitGraph) {m_quitGraphe=quitGraph; }
        ///enlever un sommet
        void removeVertex(int num);
        ///enlever une arête
        void removeEdge(int num);
        ///ajouter un sommet
        void addVertex();
        ///ajouter une arête
        void addEdge(std::vector<int> m_newVertexForEdge);
        ///mettre tout les bouton à false
        void initButton();
<<<<<<< HEAD
        ///k-sommet connexit�
        void kVertexConnexite();
        ///ajouter une combinaison
        void addCombi(int j, int k, int nb,std::vector<std::vector <int>> &m_allCombi,int i=0);
        ///cheminInverse
        bool cheminInverse(int depart,int arrive);
        ///debut k connexit�
        void init_k_connex();
        ///k connexit�
        void k_connex(std::vector<int>& inter,std::vector<std::vector <int>>& m_allCombi);
        ///bol�en pour savoir si le chemin est bon
        bool graphConnex(int idx);
        ///afficher les combinaison possible
        void afficher_k_connex(std::vector<int>& inter,std::vector<std::vector <int>>& allCombi);
        ///forte Connex
        std::map<int,std::vector<int>> algo_forte_connexite();
        bool* recherche_cmp(int i);
        void rendreConti(int num);
        void rendreConti2(std::string fileName);
        void croissance_sommets(clock_t temps);
        double calculK(std::vector<Edge> ar_arriv);
        void initia();
        void croix_rouge();
        void random();
=======
        ///k-sommet connexité
        void kVertexConnexite();
        ///ajouter une combinaison
        void addCombi(int i);
>>>>>>> 3e970ea5c73e26c768074acc39a950b1286b675d
};


#endif // GRAPH_H_INCLUDED
