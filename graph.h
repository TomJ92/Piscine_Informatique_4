#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include <fstream>
#include <iostream>
/**************************************************************
    Ici sont proposÃ©es 3 classes fondamentales
            Vertex (=Sommet)
            Edge (=ArÃªte ou Arc)
            Graph (=Graphe)

    Les arÃªtes et les sommets et le graphe qu'ils constituent
    "travaillent" Ã©troitement ensemble : pour cette raison les
    Vertex et Edge se dÃ©clarent amis (friend) de Graph pour que
    ce dernier puisse librement accÃ©der aux membres (y compris
    protected ou private) de Vertex et Edge.

    Ces Classes peuvent Ãªtres complÃ©tÃ©es. Il est Ã©galement possible
    de les dÃ©river mais il est malheureusement assez difficile
    de dÃ©river le "triplet" des 3 classes en maintenant des relations
    cohÃ©rentes ( rechercher "c++ class covariance" et "c++ parallel inheritance"
    pour commencer .. ). Il est donc sans doute prÃ©fÃ©rable, si possible,
    de "customiser" ces classes de base directement, sans hÃ©ritage.

    Le modÃ¨le proposÃ© permet de reprÃ©senter un graphe orientÃ© Ã©ventuellement
    pondÃ©rÃ©, les arcs portent une ou des informations supplÃ©mentaire(s).
    Les relations/navigations Arcs -> Sommets et Sommets -> Arcs se font
    dans les 2 sens et utilisent des INDICES et NON PAS DES ADRESSES (pointeurs)
    de telle sorte que la topologie du graphe puisse Ãªtre assez facilement
    lue et Ã©crite en fichier, et bÃ©nÃ©ficie d'une bonne lisibilitÃ© en cas de bugs...

    Chaque arc possÃ¨de 2 attributs principaux (en plus de son Ã©ventuelle pondÃ©ration)
        -> m_from (indice du sommet de dÃ©part de l'arc )
        -> m_to (indice du sommet d'arrivÃ©e de l'arc )

    Chaque sommet possÃ¨de 2 liste d'arcs (en plus de ses attributs "internes", marquages...)
        -> m_in (liste des indices des arcs arrivant au sommet : accÃ¨s aux prÃ©dÃ©cesseurs)
        -> m_out (liste des indices des arcs partant du sommet : accÃ¨s aux successeurs)

    Cependant le problÃ¨me des indices (par rapport aux pointeurs) et qu'en cas
    de destruction d'une entitÃ© (un arc et/ou un sommet sont enlevÃ©s du graphe) alors :

    - Soit il faut reprendre toute la numÃ©rotation pour "boucher le trou"
      (par exemple on a supprimÃ© le sommet nÂ°4, le sommet nÂ°5 devient le 4, 6 devient 5 etc...)
      ce qui pose des problÃ¨mes de stabilitÃ© et de cohÃ©rence, et une difficultÃ© Ã  rÃ©-introduire
      le(s) mÃªme(s) Ã©lÃ©ment supprimÃ© (au mÃªme indice)

    - Soit on admet que la numÃ©rotation des sommets et arcs n'est pas contigue, cÃ d qu'il
      peut y avoir des "trous" : sommets 0 1 5 7 8, pas de sommets 2 ni 3 ni 4 ni 6. La numÃ©rotation
      est stable mais on ne peut plus utiliser un simple vecteur pour ranger la liste de tous
      les arcs et tous les sommets aux indices correspondants, on peut utiliser une map
      qui associe un objet arc ou sommet Ã  des indices arbitraires (pas forcÃ©ment contigus)

    C'est cette 2Ã¨me approche qui est proposÃ©e ici : dans la classe graphe vous trouverez
        -> map<int, Edge>   m_edges
        -> map<int, Vertex> m_vertices    (le pluriel de vertex est vertices)

    Il faudra Ãªtre attentif au fait que par rapport Ã  un simple vecteur, le parcours des Ã©lÃ©ments
    ne pourra PAS se faire avec un simple for (int i=0; i<m_edges.size(); ++i) ...m_edges[i]...
    et que les parcours Ã  itÃ©rateur ne donneront pas directement des Edge ou des Vertex
    mais des pairs, l'objet d'intÃ©rÃªt se trouvant dans "second" ("first" contenant l'indice)
                for (auto &it = m_edges.begin(); it!=m_edges.end(); ++it) ...it->second...
    ou bien     for (auto &e : m_edges) ...e.second...

    Il n'est pas obligatoire d'utiliser ces classes pour le projet, vous pouvez faire les votres

    Au niveau de l'interface, on dissocie une classe interface associÃ©e Ã  chaque classe fondamentale
    de telle sorte qu'il soit possible de travailler avec des graphes non reprÃ©sentÃ©s Ã  l'Ã©cran
    Imaginons par exemple qu'on doive gÃ©nÃ©rer 1000 permutations de graphes pour tester des
    combinaisons, on ne souhaite pas reprÃ©senter graphiquement ces 1000 graphes, et les
    interfaces pÃ¨sent lourd en ressource, avec cette organisation on est libre de rÃ©server ou
    pas une interface de prÃ©sentation associÃ©e aux datas (dÃ©couplage donnÃ©es/interface)

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
    // Les (methodes des) classes amies pourront accÃ©der
    // directement aux attributs (y compris privÃ©s)
    friend class Vertex;
    friend class EdgeInterface;
    friend class Graph;

private :

    /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de dÃ©clarer
    /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
    /// le paramÃ©trer ( voir l'implÃ©mentation du constructeur dans le .cpp )

    // La boite qui contient toute l'interface d'un sommet
    grman::WidgetBox m_top_box;

    // Un slider de visualisation/modification de la valeur du sommet
    grman::WidgetVSlider m_slider_value;

    // Un label de visualisation de la valeur du sommet
    grman::WidgetText m_label_value;

    // Une image de "remplissage"
    grman::WidgetImage m_img;
    grman::WidgetImage m_img2;
    // Un label indiquant l'index du sommet
    grman::WidgetText m_label_idx;

    // Une boite pour le label prÃ©cÃ©dent
    grman::WidgetText m_box_label_idx;
    ///croix pour supprimer un sommets
    grman::WidgetCheckBox m_cross;
    ///bouton pour savoir quelle sommets pour les arÃªtes
    grman::WidgetButton m_button_addEdge;


public :

    // Le constructeur met en place les Ã©lÃ©ments de l'interface
    // voir l'implÃ©mentation dans le .cpp
    VertexInterface(int idx, int x, int y, std::string pic_name="", int pic_idx=0);
};


class Vertex
{
    // Les (methodes des) classes amies pourront accÃ©der
    // directement aux attributs (y compris privÃ©s)
    friend class Graph;
    friend class VertexInterface;
    friend class Edge;
    friend class EdgeInterface;

private :
    /// liste des indices des arcs arrivant au sommet : accÃ¨s aux prÃ©dÃ©cesseurs
    std::vector<int> m_in;
    /// liste des indices des arcs partant du sommet : accÃ¨s aux successeurs
    std::vector<int> m_out;
        /// un exemple de donnée associée à l'arc, on peut en ajouter d'autres...
        double m_value;
        /// Cacapité de portage de l'environnement
        double k_capacite;
        /// Rythme de croissance
        double coeff_croissance;

    /// le POINTEUR sur l'interface associÃ©e, nullptr -> pas d'interface
    std::shared_ptr<VertexInterface> m_interface = nullptr;

    // Docu shared_ptr : https://msdn.microsoft.com/fr-fr/library/hh279669.aspx
    // La ligne prÃ©cÃ©dente est en gros Ã©quivalent Ã  la ligne suivante :
    // VertexInterface * m_interface = nullptr;

public:

    /// Les constructeurs sont Ã  complÃ©ter selon vos besoin...
    /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
    Vertex (double value=0, VertexInterface *interface=nullptr) :
        m_value(value), m_interface(interface)  {  }

    /// Vertex Ã©tant gÃ©rÃ© par Graph ce sera la mÃ©thode update de graph qui appellera
    /// le pre_update et post_update de Vertex (pas directement la boucle de jeu)
    /// Voir l'implÃ©mentation Graph::update dans le .cpp
    void pre_update();
    void post_update();
};



/***************************************************
                    EDGE
****************************************************/

class EdgeInterface
{
    // Les (methodes des) classes amies pourront accÃ©der
    // directement aux attributs (y compris privÃ©s)
    friend class Edge;
    friend class Graph;

private :

    /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de dÃ©clarer
    /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
    /// le paramÃ©trer ( voir l'implÃ©mentation du constructeur dans le .cpp )

    // Le WidgetEdge qui "contient" toute l'interface d'un arc
    grman::WidgetEdge m_top_edge;

    // Une boite pour englober les widgets de rÃ©glage associÃ©s
    grman::WidgetBox m_box_edge;

    // Un slider de visualisation/modification du poids valeur de l'arc
    grman::WidgetVSlider m_slider_weight;

    // Un label de visualisation du poids de l'arc
    grman::WidgetText m_label_weight;
    ///croix pour supprimer les arÃªtes
    grman::WidgetCheckBox m_cross;
    ///texte pour afficher les numÃ©ro de sommet de l'arÃªte
    grman::WidgetText m_text_number;

public :

    // Le constructeur met en place les Ã©lÃ©ments de l'interface
    // voir l'implÃ©mentation dans le .cpp
    EdgeInterface(Vertex& from, Vertex& to,int numS1, int numS2);
};


class Edge
{
    // Les (methodes des) classes amies pourront accÃ©der
    // directement aux attributs (y compris privÃ©s)
    friend class Graph;
    friend class EdgeInterface;
private :
    /// indice du sommet de dÃ©part de l'arc
    int m_from;

    /// indice du sommet d'arrivÃ©e de l'arc
    int m_to;

    /// un exemple de donnÃ©e associÃ©e Ã  l'arc, on peut en ajouter d'autres...
    double m_weight;
    
    ///Marqueur
    bool m_marque;

    /// le POINTEUR sur l'interface associÃ©e, nullptr -> pas d'interface
    std::shared_ptr<EdgeInterface> m_interface = nullptr;



public:

    /// Les constructeurs sont Ã  complÃ©ter selon vos besoin...
    /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
    Edge (double weight=0, EdgeInterface *interface=nullptr) :
        m_weight(weight), m_interface(interface)  {  }

    /// Edge Ã©tant gÃ©rÃ© par Graph ce sera la mÃ©thode update de graph qui appellera
    /// le pre_update et post_update de Edge (pas directement la boucle de jeu)
    /// Voir l'implÃ©mentation Graph::update dans le .cpp
    void pre_update();
    void post_update();
    ///accesseurs
    int getFrom()
    {
        return m_from;
    }
    int getTo()
    {
        return m_to;
    }
    void setFrom(int from)
    {
        m_from=from;
    }
    void setTo(int to)
    {
        m_to=to;
    }

};




/***************************************************
                    GRAPH
****************************************************/

class GraphInterface
{
    friend class Graph;

private :

    /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de dÃ©clarer
    /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
    /// le paramÃ©trer ( voir l'implÃ©mentation du constructeur dans le .cpp )

    /// La boite qui contient toute l'interface d'un graphe
    grman::WidgetBox m_top_box;

    /// Dans cette boite seront ajoutÃ©s les (interfaces des) sommets et des arcs...
    grman::WidgetBox m_main_box;

    /// Dans cette boite seront ajoutÃ©s des boutons de contrÃ´le etc...
    grman::WidgetBox m_tool_box;


    // A complÃ©ter Ã©ventuellement par des widgets de dÃ©coration ou
    // d'Ã©dition (boutons ajouter/enlever ...)
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
    ///bouton pour ajouter une arÃªte
    grman::WidgetButton m_button_addEdge;
    grman::WidgetText m_text_addE;
    grman::WidgetText m_text_edge;
    ///bouton pour ne plus ajouter sommet
    grman::WidgetButton m_button_noAddEdge;


public :

    // Le constructeur met en place les Ã©lÃ©ments de l'interface
    // voir l'implÃ©mentation dans le .cpp
    GraphInterface(int x, int y, int w, int h);
};


class Graph
{
private :

    /// La "liste" des arÃªtes
    std::map<int, Edge> m_edges;

    /// La liste des sommets
    std::map<int, Vertex> m_vertices;
    ///Valeur pour le chart
    std::map<int,int> m_chart;

    /// le POINTEUR sur l'interface associÃ©e, nullptr -> pas d'interface
    std::shared_ptr<GraphInterface> m_interface = nullptr;

    ///ordre du graphe
    int m_ordre;

    ///nb d'arÃªte du graphe
    int m_nbArete;

    ///NUMERO DU GRAPHE
    int m_numGraphe;

    ///bolÃ©en pour quitter le graphe
    bool m_quitGraphe;

    ///bolÃ©en qui dit si on doit ajouter une arÃªte
    bool m_ajouterNewEdge;

    ///SAVOIR LES SOMMET QUI VONT FORMER UNE ARRÃTE
    ///savoir si on ajoute le sommet1 ou 2
    bool m_addTo;
    bool m_addFrom;
    std::vector<int> m_vertexForNewEdge;
    ///pour Ã©viter de selectionner 2 fois le mÃªme sommets
    int m_vertexAlreadyUse;

public:

    /// Les constructeurs sont Ã  complÃ©ter selon vos besoin...
    /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
    Graph (GraphInterface *interface=nullptr) :
        m_interface(interface)  {  }

    void add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name="",int pic_idx=0 );
    void add_interfaced_edge(int idx, int vert1, int vert2, double weight=0);

    /// MÃ©thode spÃ©ciale qui construit un graphe arbitraire (dÃ©mo)
    /// Voir implÃ©mentation dans le .cpp
    /// Cette mÃ©thode est Ã  enlever et remplacer par un systÃ¨me
    /// de chargement de fichiers par exemple.
    void make_example();


    /// La mÃ©thode update Ã  appeler dans la boucle de jeu pour les graphes avec interface
    void update(clock_t ini,bool animation);
    /// Augmente la capacitÃ© des sommets au cours du temps
    void croissance_sommets(clock_t temps);
    ///Randomise les valeurs des sommets et des arrÃªtes
    void random_num();
    ///Calcul le coefficient k d'un sommet donnÃ©
    double calculK(std::vector<Edge> ar_arriv);
    ///ss prog qui remplit un graphe en fonction d'un fichier
    void ReadFile(std::string fileName, int num);
    ///ss prog qui sauve un graphe en remplissant un fichier
    void saveFile(std::string fileName);
    ///rÃ©initialise les sommet Ã  leurs position de dÃ©part
    void reinit(std::string fileName);
    ///initialisation du graphe
    void initialisation();
    ///setter
    bool getQuitGraph()
    {
        return m_quitGraphe;
    }
    void setQuitGraph(bool quitGraph)
    {
        m_quitGraphe=quitGraph;
    }
    ///enlever un sommet
    void removeVertex(int num);
    void rendreConti(int num);
    void rendreConti2(std::string fileName);
    ///enlever une arÃªte
    void removeEdge(int num);
    ///ajouter un sommet
    void addVertex();
    ///ajouter une arÃªte
    void addEdge(std::vector<int> m_newVertexForEdge);
    ///mettre tout les bouton Ã  false
    void initButton();
    ///k-sommet connexitÃ©
    void kVertexConnexite();
    Graph affichage_graphe_reduit();
    std::map <int,std::vector<int>> algo_forte_connexite();
    bool* recherche_cmp(int i);
    ///ajouter une combinaison
    void addCombi(int i);
    void initia();
    void croix_rouge();

};


#endif // GRAPH_H_INCLUDED
