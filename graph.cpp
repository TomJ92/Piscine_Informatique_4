<<<<<<< HEAD
#include "graph.h"
#include <ctime>
/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    std::string num;
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de réglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 100.0);  // Valeurs arbitraires, à adapter...
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) );

    ///box qui affiche une croix pour supprimer un sommet
    m_top_box.add_child(m_cross);
    m_cross.set_bg_color(ROUGE);
    m_cross.set_dim(10,10);
    m_cross.set_pos(115,0);

    ///boite qui permet de savoir si on ajoute une arête dans ce sommet
    m_top_box.add_child(m_button_addEdge);
    m_button_addEdge.set_bg_color(BLEU);
    m_button_addEdge.set_dim(10,10);
    m_button_addEdge.set_pos(95,0);
    m_top_box.add_child(m_img2);
    m_top_box.add_child(m_textNum);
    m_textNum.set_pos(37,88);
    m_textNum.set_message("");
}


/// Gestion du Vertex avant l'appel à l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_value vers le slider associé
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donnée m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}


/// Gestion du Vertex après l'appel à l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_value locale
    m_value = m_interface->m_slider_value.get_value();
}

/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les éléments de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to,int numS1, int numS2)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    ///variable qui permet d'afficher les sommmets des arêtes
    std::string name;
    std::string sommet1;
    std::string sommet2;

    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de réglage associés
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(40,75);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de réglage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 200.0);  // Valeurs arbitraires, à adapter...
    m_slider_weight.set_dim(10,45);
    m_slider_weight.set_pos(13,13);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_pos(15,65);

    ///croix pour supprimer une arête
    m_box_edge.add_child(m_cross);
    m_cross.set_bg_color(BLEUCLAIR);
    m_cross.set_dim(7,7);
    m_cross.set_pos(0,63);

    ///texte qui affiche les sommets de l'arête
    m_box_edge.add_child(m_text_number);
    sommet1=std::to_string(numS1);
    sommet2=std::to_string(numS2);
    name=sommet1+"-"+sommet2;
    m_text_number.set_pos(0,1);
    m_text_number.set_message(name);
}


/// Gestion du Edge avant l'appel à l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_weight vers le slider associé
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donnée m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );

    ///faire grossir la flêche en fonction de son poids
    m_interface->m_top_edge.grossir(m_weight);
}

/// Gestion du Edge après l'appel à l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de préparer un cadre d'accueil des
/// éléments qui seront ensuite ajoutés lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);
    ///bouton pour sauvegarder
    m_top_box.add_child(m_button_save);
    m_button_save.set_dim(50,35);
    m_button_save.set_pos(15,15);
    m_button_save.set_bg_color(BLEUCLAIR);

    ///texte de sauvegarde
    m_button_save.add_child(m_text_save);
    m_text_save.set_pos(2,13);
    m_text_save.set_message("Sauver");

    ///bouton pour reset les position
    m_top_box.add_child(m_button_reset);
    m_button_reset.set_dim(50,35);
    m_button_reset.set_pos(15,65);
    m_button_reset.set_bg_color(ROUGE);

    ///texte de reset
    m_button_reset.add_child(m_text_reset);
    m_text_reset.set_pos(6,13);
    m_text_reset.set_message("Reset");

    ///bouton pour ajouter un sommet
    m_top_box.add_child(m_button_addVertex);
    m_button_addVertex.set_dim(50,35);
    m_button_addVertex.set_pos(15,115);
    m_button_addVertex.set_bg_color(VERTFLUOCLAIR);

    ///texte d'ajout de sommet
    m_button_addVertex.add_child(m_text_addV);
    m_text_addV.set_pos(13,5);
    m_text_addV.set_message("Add");

    m_button_addVertex.add_child(m_text_vertex);
    m_text_vertex.set_pos(3,20);
    m_text_vertex.set_message("Sommet");

    ///bouton pour ajouter
    m_top_box.add_child(m_button_addEdge);
    m_button_addEdge.set_dim(50,35);
    m_button_addEdge.set_pos(15,165);
    m_button_addEdge.set_bg_color(SABLE);

    ///texte d'ajout d'arête
    m_button_addEdge.add_child(m_text_addE);
    m_text_addE.set_pos(13,5);
    m_text_addE.set_message("Add");

    m_button_addEdge.add_child(m_text_edge);
    m_text_edge.set_pos(10,20);
    m_text_edge.set_message("Edge");

    m_tool_box.add_child(m_button_noAddEdge);
    m_button_noAddEdge.set_dim(7,7);
    m_button_noAddEdge.set_pos(68,176);
    m_button_noAddEdge.set_bg_color(BLEUSOMBRE);

    ///texte d'ajout d'arête
    m_top_box.add_child(m_button_k_connex);
    m_button_k_connex.set_dim(50,35);
    m_button_k_connex.set_pos(15,215);
    m_button_k_connex.set_bg_color(VERTCLAIR);

    ///texte d'ajout
    m_button_k_connex.add_child(m_text_k);
    m_text_k.set_pos(20,5);
    m_text_k.set_message("K");

    m_button_k_connex.add_child(m_text_connex);
    m_text_connex.set_pos(0,20);
    m_text_connex.set_message("Connex");

    ///bouton pour afficher les composante fortement connexe
    m_top_box.add_child(m_button_connex);
    m_button_connex.set_dim(50,35);
    m_button_connex.set_pos(15,265);
    m_button_connex.set_bg_color(VIOLETCLAIR);

    ///TEXTE POUR LA CONNEXITE
    m_button_connex.add_child(m_text_compo);
    m_text_compo.set_pos(5,5);
    m_text_compo.set_message("Compo");

    m_button_connex.add_child(m_text_connexe);
    m_text_connexe.set_pos(0,20);
    m_text_connexe.set_message("Connex");

    ///bouton pour reset les compo connex
    m_tool_box.add_child(m_button_restartConnex);
    m_button_restartConnex.set_dim(7,7);
    m_button_restartConnex.set_pos(68,276);
    m_button_restartConnex.set_bg_color(CYAN);

    ///bouton  pour quitter le programme
    m_top_box.add_child(m_button_quit);
    m_button_quit.set_dim(60,35);
    m_button_quit.set_pos(11,520);
    m_button_quit.set_bg_color(ORANGE);

    m_button_quit.add_child(m_text_quit);
    m_text_quit.set_pos(2,13);
    m_text_quit.set_message("Quitter");
}


/// Méthode spéciale qui construit un graphe arbitraire (démo)
/// Cette méthode est à enlever et remplacer par un système
/// de chargement de fichiers par exemple.
/// Bien sûr on ne veut pas que vos graphes soient construits
/// "à la main" dans le code comme ça.
void Graph::make_example()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne précédente est en gros équivalente à :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    /// Les sommets doivent être définis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
    add_interfaced_vertex(0, 30.0, 200, 100, "clown1.jpg");
    add_interfaced_vertex(1, 60.0, 400, 100, "clown2.jpg");
    add_interfaced_vertex(2,  50.0, 200, 300, "clown3.jpg");
    add_interfaced_vertex(3,  0.0, 400, 300, "clown4.jpg");
    add_interfaced_vertex(4,  100.0, 600, 300, "clown5.jpg");
    add_interfaced_vertex(5,  0.0, 100, 500, "bad_clowns_xx3xx.jpg", 0);
    add_interfaced_vertex(6,  0.0, 300, 500, "bad_clowns_xx3xx.jpg", 1);
    add_interfaced_vertex(7,  0.0, 500, 500, "bad_clowns_xx3xx.jpg", 2);

    /// Les arcs doivent être définis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
    add_interfaced_edge(0, 1, 2, 50.0);
    add_interfaced_edge(1, 0, 1, 50.0);
    add_interfaced_edge(2, 1, 3, 75.0);
    add_interfaced_edge(3, 4, 1, 25.0);
    add_interfaced_edge(4, 6, 3, 25.0);
    add_interfaced_edge(5, 7, 3, 25.0);
    add_interfaced_edge(6, 3, 4, 0.0);
    add_interfaced_edge(7, 2, 0, 100.0);
    add_interfaced_edge(8, 5, 2, 20.0);
    add_interfaced_edge(9, 3, 7, 80.0);
}

///sous programe qui réinitialise les postions et les sommets initials en fonction d'un fichier texte
void Graph::reinit(std::string fileName)
{
    ///interface graphique
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    ///variable pour stocker les valeur lu dans le fichier
    std::string nom;
    int xdep,ydep,numS;
    double valeur;
    std::string pic_name;
    ///fichier que l'on lit
    std::ifstream fichier(fileName,std::ios::in);
    ///on efface tout
    for(auto it = m_vertices.begin(); it != m_vertices.end();)
    {
        it=m_vertices.erase(it);
    }
    for(auto it=m_edges.begin(); it!=m_edges.end();)
    {
        it=m_edges.erase(it);
    }
    ///si on lit le fichier
    if(fichier)
    {
        ///on récupére l'ordre du graphe et on saute une ligne
        fichier>>m_ordre;
        getline(fichier,nom);
        ///on se base sur le nb de sommet présent dans le graphe
        for(int i=0; i<m_ordre; i++)
        {
            ///on remplit le numéro
            fichier>>numS;
            getline(fichier,nom);
            ///sa valeur
            fichier>> valeur;
            getline(fichier,nom);
            ///sa position
            fichier>>xdep;
            getline(fichier,nom);
            fichier>>ydep;
            getline(fichier,nom);
            ///son image
            fichier>>pic_name;
            getline(fichier,nom);
            ///on ajoute un vecteur
            add_interfaced_vertex(numS,valeur,xdep,ydep,pic_name);
        }
        ///on stocke le nb d'arête
        fichier>>m_nbArete;
        getline(fichier,nom);
        for(int i=0; i<m_nbArete; i++)
        {
            ///on récupére le numéro du sommet
            fichier>>numS;
            getline(fichier,nom);
            ///sa position
            fichier>>xdep;
            getline(fichier,nom);
            fichier>>ydep;
            getline(fichier,nom);
            ///sa valeur
            fichier>>valeur;
            getline(fichier,nom);
            ///on ajoute une nouvelle arête
            add_interfaced_edge(numS,xdep,ydep,valeur);
        }
        ///on indique un RESET
        std::cout<<"RESET"<<std::endl;
        fichier.close();
    }
    else
    {
        throw std::string("Il n'exite pas de fichier portant ce nom");
    }
    ///tout les paramétre permettant l'ajout deviennent faux
    setQuitGraph(false);
    m_ajouterNewEdge=false;
    m_addFrom=false;
    m_addTo=false;
}
///Ss programme qui remplit un graphe en fonction d'un fichier
void Graph::ReadFile(std::string fileName, int num)
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    ///varialbe string pour sauter des lignes
    std::string nom;
    m_numGraphe=num;
    ///information utile pour faire le sommet
    int numS, x, y;
    double valeur;
    std::string pic_name;
    ///lecture du fichier
    std::ifstream fichier(fileName+".txt",std::ios::in);
    ///si on lit le fichier
    if(fichier)
    {
        ///on récupére l'ordre du graphe et on saute une ligne
        fichier>>m_ordre;
        getline(fichier,nom);
        ///on se base sur le nb de sommet présent dans le graphe
        for(int i=0; i<m_ordre; i++)
        {
            ///on remplit le numéro
            fichier>>numS;
            getline(fichier,nom);
            ///sa valeur
            fichier>>valeur;
            getline(fichier,nom);
            ///sa position
            fichier>>x;
            getline(fichier,nom);
            fichier>>y;
            getline(fichier,nom);
            ///son image
            fichier>>pic_name;
            getline(fichier,nom);
            ///on ajoute un vecteur
            add_interfaced_vertex(numS,valeur,x,y,pic_name);

        }
        ///on récupére le nb d'arête présente dans notre graphe
        fichier>>m_nbArete;
        std::cout<<m_nbArete<<std::endl;
        ///en fonction de nb d'arête
        for(int i=0; i<m_nbArete; i++)
        {
            ///on récupére le numéro du sommet
            fichier>>numS;
            getline(fichier,nom);
            ///sa position
            fichier>>x;
            getline(fichier,nom);
            fichier>>y;
            getline(fichier,nom);
            ///sa valeur
            fichier>>valeur;
            getline(fichier,nom);
            ///on ajoute une nouvelle arête
            add_interfaced_edge(numS,x,y,valeur);

        }
        ///on ferme le fichier
        fichier.close();
    }
    ///si on arrive pas à lire le fichier, on envoies un exception
    else
    {
        throw std::string("Il n'exite pas de fichier portant ce nom");
    }
    ///tout les paramétre permettant l'ajout deviennet faux
    setQuitGraph(false);
    m_ajouterNewEdge=false;
    m_addFrom=false;
    m_addTo=false;
    ///si le vecteur n'a pas encore été rempli, on le remplit
    if(m_vertexForNewEdge.size()==0)
    {
        m_vertexForNewEdge.push_back(0);
        m_vertexForNewEdge.push_back(0);
    }
}

///ss programme qui sauvegarde un graphe en le remplissant dans un fichier
void Graph::saveFile(std::string fileName)
{
    ///variable pour la valeur de l'image
    std::string number;
    ///position du sommet
    int posInter=0;
    ///string du nom de l'image qui change en focntion du numéro de graphe
    std::string name;
    ///notre fichier txt
    std::ofstream fichier(fileName+ ".txt",std::ios::out);
    ///si on lit un fichier
    if(fichier)
    {
        ///on regarde l'ordre du graphe
        fichier<<m_vertices.size()<<std::endl;
        ///pour tout les sommets existants
        for(auto &el:m_vertices)
        {
            ///on écrit le numéro de sommet, sa valeur, sa position et le nom de son image
            fichier<<el.first<<std::endl;
            fichier<<el.second.m_value<<std::endl;
            posInter=el.second.m_interface->m_top_box.get_posx()+2;
            fichier<<posInter<<std::endl;
            posInter=el.second.m_interface->m_top_box.get_posy()+2;
            fichier<<posInter<<std::endl;
            name=el.second.m_interface->m_img.getName();
            fichier<<name<<std::endl;
        }
        ///on lit le nb d'arête
        fichier<<m_edges.size()<<std::endl;
        ///pour toutes les arêtes existantes
        for(auto &el:m_edges)
        {
            ///on lit leur numéros, les sommets et leurs valeurs
            fichier<<el.first<<std::endl;
            fichier<<el.second.m_to<<std::endl;
            fichier<<el.second.m_from<<std::endl;
            fichier<<el.second.m_weight<<std::endl;
        }
        fichier.close();
    }

    ///sinon on envoie une exception
    else
    {
        throw std::string("Il n'exite pas de fichier portant ce nom");
    }
}
///Dynamique de populations du graphe
void Graph::croissance_sommets(clock_t temps)
{
    ///Toutes les 5 secondes
    if (((temps/CLOCKS_PER_SEC)%1==0)&&(temps/CLOCKS_PER_SEC!=0))
        ///if (key[KEY_7])
    {
        ///Pour chaque sommet
        for (auto &elt : m_vertices)
        {
            ///On crée deux tableaux de vecteurs
            std::vector<Edge> arretes_arrivantes;
            std::vector<Edge> arretes_partantes;
            elt.second.coeff_croissance=0.0001;
            ///Pour chaque arrête
            for(auto &elm : m_edges)
            {

                if(elt.first==elm.second.m_to)
                {
                    arretes_arrivantes.push_back(elm.second);
                }
                if(elt.first==elm.second.m_from)
                {
                    arretes_partantes.push_back(elm.second);
                }
                ///Si l'arrête va vers le sommet
                elt.second.k_capacite=0;
                /** if (elm.second.m_to==elt.first)
                 {
                     ///On l'ajoute au vecteur
                     arretes_arrivantes.push_back(elm.second);
                 }
                 ///Si l'arrête part du sommet
                 if (elm.second.m_from==elt.first)
                 {
                     ///On l'ajoute au sommet
                     arretes_partantes.push_back(elm.second);
                 }**/
            }
            ///On crée un entier qu'on initialise à 0
            /**
            for (auto &elm : bidon->m_in)
            {
                Vertex* new_sommet = new Vertex;
                Edge* new_arrete = new Edge;
                *new_arrete=m_edges[elm];
                *new_sommet=new_arrete->m_from;
                voisins[new_sommet]=*new_arrete;
                delete new_arrete;
                delete new_sommet;
            =======
            ///on donne les valeurs de m_from et m_to
            m_edges[idx].setFrom(id_vert1);
            m_edges[idx].setTo(id_vert2);
            m_vertices[id_vert1].m_out.push_back(idx);
            m_vertices[id_vert2].m_in.push_back(idx);
            }
            ///Dynamique de populations du graphe
            void Graph::croissance_sommets(clock_t temps)
            {
            ///Toutes les 5 secondes
            /// if (((temps/CLOCKS_PER_SEC)%1==0)&&(temps/CLOCKS_PER_SEC!=0))
            if (key[KEY_7])
            {
            ///Pour chaque sommet
            for (auto &elt : m_vertices)
            {
            ///On crée deux tableaux de vecteurs
            std::vector<Edge> arretes_arrivantes;
            std::vector<Edge> arretes_partantes;
            ///Pour chaque arrête
            for(auto &elm : m_edges)
            {
                if(elt.first==elm.second.m_to)
                {
                    arretes_arrivantes.push_back(elm.second);
                }
                if(elt.first==elm.second.m_from)
                {
                    arretes_partantes.push_back(elm.second);
                }
                ///Si l'arrête va vers le sommet
                elt.second.k_capacite=0;
                if (elm.second.m_to==elt.first)
                 {
                     ///On l'ajoute au vecteur
                     arretes_arrivantes.push_back(elm.second);
                 }
                 ///Si l'arrête part du sommet
                 if (elm.second.m_from==elt.first)
                 {
                     ///On l'ajoute au sommet
                     arretes_partantes.push_back(elm.second);
                 }**/
            ///On crée un entier qu'on initialise Ã  0
            /**
            for (auto &elm : bidon->m_in)
            {
                Vertex* new_sommet = new Vertex;
                Edge* new_arrete = new Edge;
                *new_arrete=m_edges[elm];
                *new_sommet=new_arrete->m_from;
                voisins[new_sommet]=*new_arrete;
                delete new_arrete;
                delete new_sommet;
            >>>>>>> master
            }
            for (auto &elm : bidon->m_out)
            {
                Vertex* new_som0met = new Vertex;
                Edge* new_arrete = new Edge;
                *new_arrete=m_edges[elm];
                *new_sommet=new_arrete->m_to;
                voisins[*new_sommet]=*new_arrete;
                delete new_arrete;
                delete new_sommet;
            }
            delete bidon;
            for (auto &elm : voisins)
            {
                *kkk+=(elm.first.m_value)*(elm.second.m_weight);
            }
            **/
            ///Pour chaque arrête arrivantes
            unsigned int nombre_arrivantes=0;
            for(auto &lol : arretes_arrivantes)
            {
                ///Si la pop du sommet n'est pas égale à 0
                if(m_vertices[lol.m_from].m_value!=0)
                {
                    nombre_arrivantes++; ///On a une population qui influe
                }
            }
            std::cout<<"Sommet : "<<elt.first<<std::endl;
            elt.second.k_capacite=calculK(arretes_arrivantes);
            double coeff_out=0;
            unsigned int nombre_partantes=0;
            for(auto &elm : arretes_partantes)
            {
                ///Si la pop du sommet
                if (m_vertices[elm.m_to].m_value!=0)
                {
                    nombre_partantes++;
                }
                coeff_out+=(m_vertices[elm.m_to].m_value)*(m_vertices[elm.m_to].k_capacite);
            }
            /// bool arrivOK = false;
            /// bool partOK=false;
            /**
            if(nombre_arrivantes!=0)
            {
              arrivOK=true;
                std::cout<<"A VRAI"<<std::endl;
            }
            else if(nombre_arrivantes==0)
            {
                arrivOK=false;
                std::cout<<"A FAUX"<<std::endl;
            }
            if(nombre_partantes!=0)
            {
                partOK=false;
                std::cout<<"P FAUX"<<std::endl;
            }
            else if(nombre_partantes==0)
            {
                partOK=true;
                std::cout<<"P VRAI"<<std::endl;
            }
            ///Proies et prédateurs présents = population varie selon proies et prédateurs
            if((elt.second.m_value>1)&&(elt.second.m_value<100)&&(!arrivOK)&&(nombre_partantes!=0))
            {
                std::cout<<"LES DEUX"<<std::endl;
                if(elt.second.k_capacite!=0)
                {
                    elt.second.k_capacite=50;
                }
                elt.second.m_value+=(elt.second.coeff_croissance)*(elt.second.m_value)*(1-((elt.second.m_value)/(elt.second.k_capacite)));
                for (auto &elm : arretes_partantes)
                {
                    ///On soustrait le produit de son poids avec le sommet relié au coeff k
                    elt.second.m_value-=0.000015*(elm.m_weight)*(m_vertices[elm.m_to].m_value);
                }
            }
            ///Proies et pas de prédateurs = population augmente beaucoup
            else if((elt.second.m_value>1)&&(elt.second.m_value<100)&&(nombre_arrivantes=0)&&(nombre_partantes!=0))
            {
                std::cout<<"PROIES"<<std::endl;
                if(elt.second.k_capacite==0)
                {
                    elt.second.k_capacite=100000000;
                }
                elt.second.coeff_croissance*=10;
                elt.second.m_value+=(elt.second.coeff_croissance)*(elt.second.m_value)*(1-((elt.second.m_value)/(elt.second.k_capacite)));
            }
            ///Pas de proies et prédateurs = population diminue beaucoup
            else if((elt.second.m_value>1)&&(elt.second.m_value<100)&&(nombre_arrivantes!=0)&&(nombre_partantes=0))
            {
                std::cout<<"PREDA"<<std::endl;
                elt.second.k_capacite=0.00001;
                elt.second.coeff_croissance*=10;
                elt.second.m_value+=(elt.second.coeff_croissance)*(elt.second.m_value)*(1-((elt.second.m_value)/(elt.second.k_capacite)));
                for (auto &elm : arretes_partantes)
                {
                    ///On soustrait le produit de son poids avec le sommet relié au coeff k
                    elt.second.m_value-=0.000015*(elm.m_weight)*(m_vertices[elm.m_to].m_value);
                }
            }
            ///Pas de proies et pas de prédateurs = population diminue car ne peut pas se reproduire, s'éteint progressivement
            else if((elt.second.m_value>1)&&(elt.second.m_value<100)&&(nombre_arrivantes=0)&&(nombre_partantes=0))
            {
                std::cout<<"AUCUNE"<<std::endl;
                elt.second.k_capacite=0.001;
                elt.second.coeff_croissance/=10;
                elt.second.m_value+=(elt.second.coeff_croissance)*(elt.second.m_value)*(1-((elt.second.m_value)/(elt.second.k_capacite)));
            }
            std::cout<<"Valeur : "<<elt.second.m_value<<std::endl;
            **/
            std::cout<<"k vaut "<<elt.second.k_capacite<<std::endl;
            ///Equation de dynamique de population si on a assez de population et qu'on a des proies et des prédateurs
            if((elt.second.m_value>1)&&(elt.second.m_value<=100)&&(nombre_arrivantes!=0)&&(nombre_partantes!=0))
            {
                std::cout<<"Valeur pop AVANT : "<<elt.second.m_value<<std::endl;
                if (elt.second.k_capacite==0)
                {
                    elt.second.k_capacite=elt.second.m_value/3;
                }
                elt.second.m_value+=(elt.second.coeff_croissance)*(elt.second.m_value)*(1-((elt.second.m_value)/(elt.second.k_capacite)));
                std::cout<<"Valeur pop APRES1 : "<<elt.second.m_value<<std::endl;
                for (auto &elm : arretes_partantes)
                {
                    ///On soustrait le produit de son poids avec le sommet relié au coeff k
                    elt.second.m_value-=0.000015*(elm.m_weight)*(m_vertices[elm.m_to].m_value);
                }
                std::cout<<"Valeur pop APRES2: "<<elt.second.m_value<<std::endl;
            }
            ///Pas de proies et pas de prédateurs : pop diminue
            else if((elt.second.m_value>1)&&(elt.second.m_value<=100)&&(nombre_arrivantes==0)&&(nombre_partantes==0))
            {
                ///k petit pour que diminue
                elt.second.k_capacite=0.01;
                ///diminue légèrement
                elt.second.coeff_croissance/=150;
                std::cout<<"Valeur pop AVANT: "<<elt.second.m_value<<std::endl;
                elt.second.m_value+=(elt.second.coeff_croissance)*(elt.second.m_value)*(1-((elt.second.m_value)/(elt.second.k_capacite)));
                std::cout<<"Valeur pop APRES1: "<<elt.second.m_value<<std::endl;
            }
            ///Proie et pas de prédateur : augmente beaucoup
            else if((elt.second.m_value>1)&&(elt.second.m_value<=100)&&(nombre_arrivantes!=0)&&(nombre_partantes==0))
            {
                if(elt.second.k_capacite==0)
                {
                    elt.second.k_capacite=elt.second.m_value/1000;
                }
                elt.second.coeff_croissance*=100;
                std::cout<<"Valeur pop AVANT: "<<elt.second.m_value<<std::endl;
                elt.second.m_value+=(elt.second.coeff_croissance)*(elt.second.m_value)*(1-((elt.second.m_value)/(elt.second.k_capacite)));
                std::cout<<"Valeur pop APRES1: "<<elt.second.m_value<<std::endl;
            }
            ///Pas de proie et prédateur
            else if((elt.second.m_value>1)&&(elt.second.m_value<=100)&&(nombre_arrivantes==0)&&(nombre_partantes!=0))
            {
                std::cout<<"Valeur pop AVANT: "<<elt.second.m_value<<std::endl;
                for (auto &elm : arretes_partantes)
                {
                    ///On soustrait le produit de son poids avec le sommet relié au coeff k
                    elt.second.m_value-=0.000015*(elm.m_weight)*(m_vertices[elm.m_to].m_value);
                }
                std::cout<<"Valeur pop APRES1: "<<elt.second.m_value<<std::endl;
            }
            ///Si valeur superieure a 100
            if (elt.second.m_value>100)
            {
                ///On ramene a 100
                elt.second.m_value=100;
            }
            ///Si ça vaut 100
            if((elt.second.m_value==100)&&(elt.second.k_capacite!=0))
            {
                elt.second.coeff_croissance=-0.0003;
                elt.second.m_value+=(elt.second.coeff_croissance)*(elt.second.m_value)*(1-((elt.second.m_value)/(elt.second.k_capacite)));
                for (auto &elm : arretes_partantes)
                {
                    ///On soustrait le produit de son poids avec le sommet relié au coeff k
                    elt.second.m_value-=(coeff_out)*(elm.m_weight/100)*(m_vertices[elm.m_to].m_value);
                }
            }
            ///Si l'espèce ne peut plus se reproduire
            if(elt.second.m_value<=1)
            {
                elt.second.m_value=0;
            }
            ///Pas de pop négative
            if(elt.second.m_value<0)
            {
                elt.second.m_value=0;
                ///Croix rouge sur le sommet
            }
            elt.second.k_capacite=0;
        }
    }
}
void Graph::random_num()
{
    for (auto &elm : m_vertices)
    {
        elm.second.m_value=rand()%100;
    }
    for (auto &elm : m_edges)
    {
        elm.second.m_weight=rand()%100;
    }
}

double Graph::calculK(std::vector<Edge> ar_arriv)
{
    double *k = new double;
    *k=0;
    for(auto &elm : ar_arriv)
    {
        *k+=(elm.m_weight)*(m_vertices[elm.m_from].m_value);
    }
    return *k;
    delete k;
}

void Graph::initia()
{
    for(auto &elm : m_vertices)
    {
        elm.second.coeff_croissance=0;
    }
}
void Graph::croix_rouge()
{
    for(auto &elm:m_vertices)
    {
        if(elm.second.m_value==0)
        {
            elm.second.m_interface->m_img2.set_pic_name("croix_rouge.bmp");
        }
        else if(elm.second.m_value!=0)
        {
            elm.second.m_interface->m_img2.set_pic_name("");
        }
    }
}

/// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update(clock_t ini, bool animation)
{
    int num=m_numGraphe;
    if (!m_interface)
        return;
    for (auto &elt : m_vertices)
        elt.second.pre_update();
    for (auto &elt : m_edges)
        elt.second.pre_update();

    m_interface->m_top_box.update();

    for (auto &elt : m_vertices)
    {
        elt.second.post_update();
        ///si on ajoute un nouvelle arête
        if(m_ajouterNewEdge)
        {
            ///si on appuie sur le bouton bleu
            if(elt.second.m_interface->m_button_addEdge.get_value())
            {
                ///si on a pas encore selectionné de sommet
                if(m_addFrom==false)
                {
                    ///on stocke l'indice du sommet
                    m_vertexForNewEdge[0]=elt.first;
                    m_addFrom=true;
                    m_vertexAlreadyUse=m_vertexForNewEdge[0];
                }
                /// si on a selectionné un autre sommet
                else if(m_addTo==false&&m_addFrom==true&&m_vertexAlreadyUse!=elt.first)
                {
                    ///on ajoute une arrête et reset tout les paramétre
                    m_vertexForNewEdge[1]=elt.first;
                    m_addFrom=false;
                    m_addTo=false;
                    addEdge(m_vertexForNewEdge);
                    m_ajouterNewEdge=false;
                    m_interface->m_button_connex.set_value(false);
                    saveFile("Graphe1Inter");
                    initialisation();
                    ReadFile("Graphe1Inter",num);
                }
            }
        }
    }
    for (auto &elt : m_edges)
    {
        elt.second.post_update();
        ///si on clique sur la croix d'une arête

    }
    for(auto & elt:m_edges)
    {
        if(elt.second.m_interface->m_cross.get_value())
        {
            ///on la supprime
            removeEdge(elt.first);
            m_interface->m_button_connex.set_value(false);
            saveFile("Graphe1Inter");
            initialisation();
            ReadFile("Graphe1Inter",num);
            break;
        }
    }
    for(auto & elt:m_vertices)
    {
        if(elt.second.m_interface->m_cross.get_value())
        {
            ///on la supprime
            removeVertex(elt.first);
            m_interface->m_button_connex.set_value(false);
            break;
        }
    }
    ///si on clique sur le bouton sauvegarder
    if(m_interface->m_button_save.get_value())
    {
        ///selon le numéro du graphe
        switch(m_numGraphe)
        {
        case 1:
            ///on sauvegarde
            try
            {
                saveFile("Graphe1");
                m_interface->m_button_save.set_value(false);
                std::cout<<"Sauvegarde du Graphe 1"<<std::endl;
            }
            catch(const std::string & e)
            {
                std::cout << e << "\n\n";
            }
            break;
        case 2:
            ///on sauvegarde
            try
            {
                saveFile("Graphe2");
                m_interface->m_button_save.set_value(false);
                std::cout<<"Sauvegarde du Graphe 2"<<std::endl;
            }
            catch(const std::string & e)
            {
                std::cout << e << "\n\n";
            }
            break;
        case 3:
            ///on sauvegarde
            try
            {
                saveFile("Graphe3");
                m_interface->m_button_save.set_value(false);
                std::cout<<"Sauvegarde du Graphe 3"<<std::endl;
            }
            catch(const std::string & e)
            {
                std::cout << e << "\n\n";
            }
            break;

        }
        saveFile("Graphe1Inter");
        initialisation();
        ReadFile("Graphe1Inter",num);
    }
    ///on appuie sur le bouton reset
    if(m_interface->m_button_reset.get_value())
    {
        std::cout<<"good";
        ///en fonction du numéro du graphe
        ///on lit le fichier Reset.txt
        switch(m_numGraphe)
        {
        case 1:
            try
            {
                reinit("Graphe1Reset.txt");
                m_interface->m_button_reset.set_value(false);
            }
            catch(const std::string & e)
            {
                std::cout << e << "\n\n";
            }
            break;
        case 2:
            try
            {
                reinit("Graphe2Reset.txt");
                m_interface->m_button_reset.set_value(false);
            }
            catch(const std::string & e)
            {
                std::cout << e << "\n\n";
            }
            break;
        case 3:
            try
            {
                reinit("Graphe3Reset.txt");
                m_interface->m_button_reset.set_value(false);
            }
            catch(const std::string & e)
            {
                std::cout << e << "\n\n";
            }
            break;

        }
    }
    ///si on appuie sur ce bouton
    ///on quitte le graphe
    if(m_interface->m_button_quit.get_value())
    {
        setQuitGraph(true);
        std::cout<<"Retour au menu"<<std::endl;
    }
    ///si on appuie sur le bouton
    ///on ajoute un sommet
    if(m_interface->m_button_addVertex.get_value())
    {
        m_interface->m_button_addVertex.set_value(false);
        addVertex();
        saveFile("Graphe1Inter");
        initialisation();
        ReadFile("Graphe1Inter",num);
    }
    ///si on appuie sur le bouton addEdge
    if(m_interface->m_button_addEdge.get_value())
    {
        m_interface->m_button_addEdge.set_value(false);
        m_ajouterNewEdge=true;
        initButton();
        std::cout<<"Selectionner 2 sommet"<<std::endl;
    }
    if(m_interface->m_button_noAddEdge.get_value())
    {
        m_ajouterNewEdge=false;
        m_interface->m_button_noAddEdge.set_value(false);
        std::cout<<"Vous n'ajoutez plus une nouvelle arete"<<std::endl;
    }
    if(m_interface->m_button_k_connex.get_value())
    {
        m_interface->m_button_k_connex.set_value(false);
        init_k_connex();
        rest(300);
    }
    if(m_interface->m_button_connex.get_value())
    {
        m_interface->m_button_connex.set_value(false);
        algo_forte_connexite();
        for(auto &el:m_vertices)
        {
            el.second.m_interface->m_textNum.set_message(std::to_string(el.second.m_numero_compo_connexe));
        }
    }
    if(m_interface->m_button_restartConnex.get_value())
    {
        m_interface->m_button_connex.set_value(false);
        saveFile("Graphe1Inter");
        initialisation();
        ReadFile("Graphe1Inter",num);
    }
     if(animation)
    {
        croissance_sommets(ini);
    }
    if(key[KEY_SPACE])
    {
        ///Mettre au hasard la valeur des sommets
        random_num();
    }
    if(key[KEY_R])
    {
        for(auto &elm : m_edges)
        {
            elm.second.m_weight=0;
        }
    }
    croix_rouge();
}

///la méthode qui ajoute une arete
void Graph::addEdge(std::vector<int> m_newVertexForEdge)
{
    ///variable
    int indice;
    bool good=true;
    ///on explore toutes les arete
    for(auto elt:m_edges)
    {
        ///si on a une arete similaire dans notre map d'arete
        if(elt.second.m_from==m_vertexForNewEdge[0]&&elt.second.m_to==m_vertexForNewEdge[1])
        {
            ///alors on ajoute pas de nouvelle arete
            good=false;
            std::cout<<"On peut pas rajouter une nouvelle arete"<<std::endl;
        }
    }
    ///si l'arete n'existe alors
    if(good)
    {
        ///on regarde l'indice le plus grand de la map d'arete
        for(auto it=m_edges.begin(); it!=m_edges.end(); it++)
        {
            indice=it->first;
        }
        ///et on ajoute +1
        indice++;
        ///on ajoute une nouvelle arete
        add_interfaced_edge(indice,m_vertexForNewEdge[0],m_vertexForNewEdge[1],60);
        std::cout<<"Ajout d'une nouvelle arete"<<std::endl;
    }

}

///la methode qui enleve un sommet
void Graph::removeVertex(int num)
{
    ///on enleve les aretes qui sont connectés par des sommets
    for(int i=m_vertices[num].m_in.size()-1; i>=0; i--)
    {
        removeEdge(m_vertices[num].m_in[i]);
    }
    for(int i=m_vertices[num].m_out.size()-1; i>=0; i--)
    {
        removeEdge(m_vertices[num].m_out[i]);
    }
    ///on enleve l'interface du sommet et on enleve le sommet
    m_interface->m_main_box.remove_child( m_vertices[num].m_interface->m_top_box );
    m_vertices.erase(num);
    std::cout<<"Suppresion d'un sommet"<<std::endl;
    rendreConti(num);
}

void Graph::rendreConti(int num)
{
    int **indiceForI;
    int idx=0;
    int inter=0;
    inter=m_numGraphe;
    indiceForI=new int *[2];
    for (int i=0; i<2; i++)
    {
        indiceForI[i]=new int [m_vertices.size()];
    }
    for(auto el:m_vertices)
    {
        indiceForI[0][idx]=idx;
        indiceForI[1][idx]=el.first;
        idx++;
    }
    for(int i=0; i<(int)m_vertices.size(); i++)
    {
        std::cout<<indiceForI[0][i]<<" "<<indiceForI[1][i]<<std::endl;

    }
    idx=0;
    for(auto & el:m_edges)
    {
        for(int j=0; j<(int)m_vertices.size(); j++)
        {
            if(indiceForI[1][j]==el.second.m_from&&indiceForI[1][j]>=num)
            {
                el.second.m_from=indiceForI[0][j];
            }
        }
        for(int j=0; j<(int)m_vertices.size(); j++)
        {
            if(indiceForI[1][j]==el.second.m_to&&indiceForI[1][j]>=num)
            {
                el.second.m_to=indiceForI[0][j];
            }
        }
    }
    for(auto & el:m_edges)
    {
        std::cout<<el.second.m_from<<"  "<<el.second.m_to<<std::endl;
    }
   rendreConti2("Graphe1Inter");
   initialisation();
   ReadFile("Graphe1Inter",inter);
   std::cout<<std::endl<<m_numGraphe;
}
void Graph::rendreConti2(std::string fileName)
{
    ///variable pour la valeur de l'image
    std::string number;
    ///position du sommet
    int posInter=0;
    ///string du nom de l'image qui change en focntion du numéro de graphe
    std::string name;
    ///notre fichier txt
    std::ofstream fichier(fileName+ ".txt",std::ios::out);
    int indice=0;
    ///si on lit un fichier
    if(fichier)
    {
        ///on regarde l'ordre du graphe
        fichier<<m_vertices.size()<<std::endl;
        ///pour tout les sommets existants
        for(auto &el:m_vertices)
        {
            ///on écrit le numéro de sommet, sa valeur, sa position et le nom de son image
            fichier<<indice<<std::endl;
            fichier<<el.second.m_value<<std::endl;
            posInter=el.second.m_interface->m_top_box.get_posx()+2;
            fichier<<posInter<<std::endl;
            posInter=el.second.m_interface->m_top_box.get_posy()+2;
            fichier<<posInter<<std::endl;
            name=el.second.m_interface->m_img.getName();
            fichier<<name<<std::endl;
            indice++;
        }
        ///on lit le nb d'arête
        fichier<<m_edges.size()<<std::endl;
        ///pour toutes les arêtes existantes
        for(auto &el:m_edges)
        {
            ///on lit leur numéros, les sommets et leurs valeurs
            fichier<<el.first<<std::endl;
            fichier<<el.second.m_to<<std::endl;
            fichier<<el.second.m_from<<std::endl;
            fichier<<el.second.m_weight<<std::endl;
        }
        fichier.close();
    }

    ///sinon on envoie une exception
    else
    {
        throw std::string("Il n'exite pas de fichier portant ce nom");
    }
}

///la méthode qui ajoute un sommet
void Graph::addVertex()
{
    ///on regarde l'indice le plus grand de la map d'arete
    int indice;
    std::string name;
    std::string number;
    std::string picName="clown1.jpg";
    switch(m_numGraphe)
    {
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    }
    for(auto it=m_vertices.begin(); it!=m_vertices.end(); it++)
    {
        indice= it->first;
    }
    ///on ajoute +1 sur l'indice
    indice++;
    ///on ajoute un sommet
    add_interfaced_vertex(indice,50,300,300,picName);
}

///la méthode qui enleve une arete
void Graph::removeEdge(int num)
{
    /// référence vers le Edge à enlever
    Edge &remed=m_edges[num];
    //std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    //std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    //std::cout << m_edges.size() << std::endl;
    /// test : on a bien des éléments interfacés
    if (m_interface && remed.m_interface)
    {
        /// Ne pas oublier qu'on a fait ça à l'ajout de l'arc :
        /* EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]); */
        /* m_interface->m_main_box.add_child(ei->m_top_edge);  */
        /* m_edges[idx] = Edge(weight, ei); */
        /// Le new EdgeInterface ne nécessite pas de delete car on a un shared_ptr
        /// Le Edge ne nécessite pas non plus de delete car on n'a pas fait de new (sémantique par valeur)
        /// mais il faut bien enlever le conteneur d'interface m_top_edge de l'arc de la main_box du graphe
        m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );
    }
    /// Il reste encore à virer l'arc supprimé de la liste des entrants et sortants des 2 sommets to et from
    /// References sur les listes de edges des sommets from et to
    std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;
    std::vector<int> &veto = m_vertices[remed.m_to].m_in;
    vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), num ), vefrom.end() );
    veto.erase( std::remove( veto.begin(), veto.end(), num ), veto.end() );
    /// Le Edge ne nécessite pas non plus de delete car on n'a pas fait de new (sémantique par valeur)
    /// Il suffit donc de supprimer l'entrée de la map pour supprimer à la fois l'Edge et le EdgeInterface
    /// mais malheureusement ceci n'enlevait pas automatiquement l'interface top_edge en tant que child de main_box !
    m_edges.erase( num );
    /// Tester la cohérence : nombre d'arc entrants et sortants des sommets 1 et 2
}
//void Graph::kVertexConnexite()
//{
//    std::vector<std::vector<int>> allCombinaison;
//    ///toute les combinaison de sommets
//    for(int i=1;i<=m_vertices.size();i++)
//    {
//        addCombi(i,allCombinaison);
//    }
//}

//void Graph::addCombi(int i,std::vector<std::vector<int>> allCombinaison)
//{
//    if(m_vertices.size()>=i)
//    {
//        {
//
//        }
//    }
//}

///ss prog qui efface toute les sommets et arete d'un graphe et on set son numéro
void Graph::initialisation()
{
    ///on détruit les map de sommet et d'arêtes
    for(auto it = m_vertices.begin(); it != m_vertices.end();)
    {
        it=m_vertices.erase(it);
    }
    for(auto it=m_edges.begin(); it!=m_edges.end();)
    {
        it=m_edges.erase(it);
    }
    m_numGraphe=0;
}

void Graph::init_k_connex()
{
    int indice;
    bool noNeed=false;
    int nbAdj=0;
    std::vector<std::vector <int>> allCombi;
    std::vector<int> inter;
    for(auto& el:m_vertices)
    {

        nbAdj=el.second.m_out.size()+el.second.m_in.size();
        if(nbAdj==0)
        {
            noNeed=true;
        }
    }
    if(noNeed)
    {
        std::cout<<"le graphe est deja non connexe"<<std::endl;
    }
    else if(!noNeed)
    {
        for(auto &el:m_vertices)
        {
            indice=el.first;
            el.second.m_existe=true;
        }
        indice++;
        m_toursDeBoucle=0;
        m_toursDeBoucleMax=9999;
        k_connex(inter,allCombi);
        afficher_k_connex(inter,allCombi);
    }

//    for(int i=0;i<m_allCombi.size();i++)
//    {
//        for(int j=0;j<m_allCombi[i].size();j++)
//        {
//            std::cout<<m_allCombi[i][j]<<" ";
//        }
//        std::cout<<std::endl;
//    }
}

void Graph::k_connex(std::vector<int>& inter, std::vector<std::vector <int>>& allCombi)
{
    bool good;
    for(auto & el:m_vertices)
    {
        if(el.second.m_existe)
        {
            el.second.m_existe=false;
            if(graphConnex(el.first))
            {
                inter.push_back(el.first);
                m_toursDeBoucle++;
                if(m_toursDeBoucle<m_toursDeBoucleMax)
                {
                    k_connex(inter,allCombi);
                }
                inter.pop_back();
            }
            else if(!graphConnex(el.first))
            {
                m_toursDeBoucle++;
                good=true;
                for(int i=0; i<(int)allCombi.size(); i++)
                {
                    if(allCombi[i]==inter)
                    {
                        good=false;
                    }
                }
                if(good)
                    allCombi.push_back(inter);
                if(m_toursDeBoucle<m_toursDeBoucleMax)
                {
                    m_toursDeBoucleMax=m_toursDeBoucle;
                }
            }
            m_toursDeBoucle--;
            el.second.m_existe=true;

        }
    }
}

bool Graph::graphConnex(int idx)
{
    int inter;
    int indice1=0;
    int indice2=0;
    std::queue<int> file;
    for(auto & el:m_vertices)
    {
        el.second.m_marqueur=false;
    }
    m_vertices[idx].m_marqueur=true;
    file.push(idx);
    while(file.size()!=0)
    {
        inter=file.front();
        file.pop();
        for(int i=0; i<(int)m_vertices[inter].m_in.size(); i++)
        {
            if(m_vertices[m_edges[m_vertices[inter].m_in[i]].m_from].m_existe&&!m_vertices[m_edges[m_vertices[inter].m_in[i]].m_from].m_marqueur)
            {
                file.push(m_edges[m_vertices[inter].m_in[i]].m_from);
                m_vertices[m_edges[m_vertices[inter].m_in[i]].m_from].m_marqueur=true;
            }

        }
        for(int i=0; i<(int)m_vertices[inter].m_out.size(); i++)
        {
            if(m_vertices[m_edges[m_vertices[inter].m_out[i]].m_to].m_existe&&!m_vertices[m_edges[m_vertices[inter].m_out[i]].m_to].m_marqueur)
            {
                file.push(m_edges[m_vertices[inter].m_out[i]].m_to);
                m_vertices[m_edges[m_vertices[inter].m_out[i]].m_to].m_marqueur=true;
            }
        }
    }
    for(auto& el:m_vertices)
    {
        if(el.second.m_existe)
        {
            indice1++;
        }
        if(el.second.m_marqueur)
        {
            indice2++;
        }
    }
    if(indice1>indice2)
    {
        return false;
    }
    else if(indice1==indice2)
    {
        return true;
    }
}

void Graph::afficher_k_connex(std::vector<int>& inter,std::vector<std::vector <int>>& allCombi)
{
    int poids_min=888888;
    std::vector<std::vector <int>> allCombMin;
    std::vector<std::vector <int>> allCombiMin2;
    std::vector<std::vector <int>> alreadyUse;
    int indice=0;
    bool good;
    for(int i=0; i<(int)allCombi.size(); i++)
    {
        if((int)allCombi[i].size()<poids_min)
        {
            poids_min=allCombi[i].size();
        }
    }
    for(int i=0; i<(int)allCombi.size(); i++)
    {
        if((int)allCombi[i].size()==poids_min)
        {
            allCombMin.push_back(allCombi[i]);
        }
    }
    for(int i=0; i<(int)allCombMin.size(); i++)
    {
        inter=allCombMin[i];

        indice=0;
        good=true;
        for(int j=0; j<(int)alreadyUse.size(); j++)
        {
            for(int k=0; k<(int)alreadyUse[i].size(); i++)
            {
                for(int l=0; l<(int)inter.size(); l++)
                {
                    if(inter[l]==alreadyUse[j][k])
                    {
                        indice++;
                    }
                    if(indice==poids_min)
                    {
                        good=false;
                    }
                }
            }
        }
        if(good)
        {
            allCombiMin2.push_back(inter);
            alreadyUse.push_back(inter);
        }
    }
    /// std::cout<<poids_min<<std::endl;
    std::cout<<"Pour avoir un graphe connexe il faut enlever les combinaisons de sommets suivant:"<<std::endl;
//    for(int i=0; i<m_allCombi.size(); i++)
//    {
//        if(m_allCombi[i].size()==poids_min)
//        {
//            std::cout<<"- ";
//            for(int j=0; j<m_allCombi[i].size(); j++)
//            {
//                std::cout<<m_allCombi[i][j]<<" ";
//            }
//
//            std::cout<<std::endl;
//        }
//    }
    for(int i=0; i<(int)allCombiMin2.size(); i++)
    {
        if((int)allCombiMin2.size()!=0)
        {
            std::cout<<"- ";
            for(int j=0; j<(int)allCombiMin2[i].size(); j++)
            {
                std::cout<<allCombiMin2[i][j]<<" ";
            }
            std::cout<<std::endl;
        }

    }

}

std::map<int,std::vector<int>>  Graph::algo_forte_connexite()
{
    int couleurs[20]={ROUGE, ROUGECLAIR, BLEU, VERT, JAUNE, ORANGE, SABLE, GRIS, BLANC, VIOLETCLAIR, MARRONCLAIR, BLEUCLAIR, VERTCLAIR, BLANCBLEU, JAUNECLAIR,FUCHSIASOMBRE,FUCHSIA,CYAN,KAKI,CYANSOMBRE};
    int nb_compos=1; ///Nombre de composantes fortement connexes
    int nb_arete(0);
    std::vector<Edge> aretes_composantes_connexes;
    bool presence_cfc;
    std::vector<int> list_sommets;
    std::map<int,std::vector<int>> composantes_connexes;
    bool** compos_connexes; ///Tableau qui détermine les composantes fortement connexes.
    compos_connexes=new bool*[(int)m_vertices.size()]; ///1ere allocation de mémoire
    for(int i=0; i<(int)m_vertices.size(); i++) ///2ème allocation de mémoire
        compos_connexes[i]=new bool[(int)m_vertices.size()];
    for(auto &elem: m_vertices) ///Initialisation des tableaux
    {
        elem.second.m_marque=false;
    }
    for(auto &elem: m_vertices) ///Pour chaque sommet du graphe
    {
        if(!elem.second.m_marque) ///Si il n'a pas été traité
        {
            elem.second.m_marque=true; ///On le marque
            compos_connexes[elem.first]=recherche_cmp(elem.first); ///On cherche si le sommet fait partie d'une composante fortement connexe
            for(auto &elem2: m_vertices) ///Pour chaque sommet j du graphe
            {
                if(compos_connexes[elem.first][elem2.first] && !elem2.second.m_marque) ///Si i et j font partie d'une composante fortement connexe et que j n'a pas été traité
                {
                    m_vertices[elem2.first].m_marque=true; ///On le marque
                }
            }
        }
    }
    for(auto &elem:m_vertices)
        elem.second.m_marque=false;
    for(auto &elem:m_vertices) ///Affichage du tableau de booléens pour les composantes fortement connexes
    {
        presence_cfc=false;
        for(auto &elem2: m_vertices)
        {
            if(compos_connexes[elem.first][elem2.first] && !elem2.second.m_marque) ///On affiche 1 si la valeur est true, 0 sinon
            {
                elem2.second.m_marque=true;
                list_sommets.push_back(elem2.first); ///On ajoute j parmi les sommets de la composante fortement connexe potentiellement détectée
                presence_cfc=true;
            }
        }
        if(presence_cfc) ///Si une composante fortement connexe (cfc) est détectée, on augmente le nb de cfc de 1.
        {
            composantes_connexes.emplace(nb_compos, list_sommets); ///On ajoute une composante fortement connexe avec son numéro et les indices des sommets qui s'y trouvent
            nb_compos++;
        }
        list_sommets.clear();

    }
    for(auto &elem: composantes_connexes)
    {
        for(unsigned int i=0; i<elem.second.size(); i++)
        {
            m_vertices[elem.second[i]].m_numero_compo_connexe=elem.first;
        }
    }
    std::cout << std::endl << "Il y a " << nb_compos-1 << " composantes fortement connexes dans ce graphe." << std::endl; ///Affichage du nombre de composantes fortement connexes
    for(auto &elem: composantes_connexes)
    {
        std::cout << std::endl << "Composante numero " << elem.first <<", constituee des sommets d'indice ";
        for(unsigned int j=0; j<elem.second.size(); j++)
            std::cout << elem.second[j] << " ";
        std::cout << std::endl;
    }

    std::ofstream fichier("Compo_connexe"+std::to_string(m_numGraphe)+".txt", std::ios::out); ///Ouverture d'un fichier pour le graphe réduit
    if(fichier) ///S'il est ouvert
    {
        fichier << composantes_connexes.size() << std::endl ; ///Ordre du graphe réduit, qui est soit inférieur, soit égal à l'ordre du graphe
        for(auto &elem: composantes_connexes) ///Sauvegarde des sommets
        {
            fichier << elem.first-1 << std::endl;
            fichier << 5 << std::endl;
            fichier << m_vertices[elem.second.front()].m_interface->m_top_box.get_posx()+2 << std::endl; ///En cas de sommet général,
            fichier << m_vertices[elem.second.front()].m_interface->m_top_box.get_posy()+2 << std::endl;
            if(elem.second.size()>1)
                fichier << "sommet_general.jpg" << std::endl; ///Les sommets généraux auront une image neutre
            if(elem.second.size()==1)
                fichier << m_vertices[elem.second.front()].m_interface->m_img.getName() << std::endl;
                ///Les sommets étant d'eux-mêmes une composante fortement connexe gardent leur image
        }
        if(composantes_connexes.size()==m_vertices.size()) ///Si tous les sommets forment eux-memes des composantes
        {                                                    ///fortement connexes, on reprend les arcs du graphe sans les modifier
            fichier << m_nbArete << std::endl;
            for(auto &elem :m_edges)
            {
                fichier << elem.first << std::endl;
                fichier << elem.second.m_to << std::endl;
                fichier << elem.second.m_from << std::endl;
                fichier << elem.second.m_weight << std::endl;
            }
        }
        if(composantes_connexes.size()<m_vertices.size())///Si il y a des sommets généraux
        {
            for(auto &elem: m_edges) ///On calcule le nombre d'aretes en question
            ///Les arêtes en pleine composante fortement connexe sont ignorées
            {
                if(m_vertices[elem.second.m_from].m_numero_compo_connexe!=m_vertices[elem.second.m_to].m_numero_compo_connexe)
                {
                    aretes_composantes_connexes.push_back(elem.second);
                    nb_arete++;
                }
            }
            fichier << nb_arete << std::endl;
            for(int j=0; j<nb_arete; j++)
            {
                aretes_composantes_connexes[j].m_from=composantes_connexes[m_vertices[aretes_composantes_connexes[j].m_from].m_numero_compo_connexe].front();
                aretes_composantes_connexes[j].m_to=composantes_connexes[m_vertices[aretes_composantes_connexes[j].m_to].m_numero_compo_connexe].front();
            }
            for(int j=0; j<nb_arete; j++)
            {
                fichier << j << std::endl;
                fichier << aretes_composantes_connexes[j].m_to << std::endl;
                fichier << aretes_composantes_connexes[j].m_from << std::endl;
                fichier << 5 << std::endl;
            }
        }
        fichier.close();
    }
    for(auto &elem: composantes_connexes)
    {
        for(int i=0; i<(int)elem.second.size(); i++)
            m_vertices[elem.second[i]].m_interface->m_top_box.set_bg_color(couleurs[elem.first]);
    }
    return composantes_connexes;
}

bool* Graph::recherche_cmp(int i)
{
     bool* marquage = new bool[(int)m_vertices.size()]; ///Tableau de booléens qui dit si un sommet a été visité ou non
    bool* pred, *succ; ///Tableau de booléens pour dire si chaque sommet admet un prédecesseur (pred) ou un successeur (succ)
    bool* cmp = new bool[(int)m_vertices.size()];; ///Tableau de booléens pour dire si un sommet fait partie ou non de la composante fortement connexe retournée
    bool suivant=true; ///Booléen pour dire si il reste des sommets à visiter
    pred=new bool[(int)m_vertices.size()]; ///Allocation de mémoire pour les 2 tableaux de predecesseurs et de successeurs
    succ=new bool[(int)m_vertices.size()];
    for(int j=0; j<(int)m_vertices.size(); j++) ///Initialisation des 3 tableaux
    {
        marquage[j]=false;
        pred[j]=false;
        succ[j]=false;
        cmp[j]=false;
    }
    pred[i]=true; ///On fait en sorte que notre sommet de départ admette un prédecesseur et un successeur
    succ[i]=true;
    while(suivant) ///Tant que tous les sommets n'ont pas été visités
    {
        suivant=false;
        for(int j=0; j<(int)m_vertices.size(); j++) ///Pour tout sommet j du graphe
        {
            if(!marquage[j] && pred[j]) ///Si il n'a pas été marqué et qu'il admet un prédecesseur
            {
                marquage[j]=true; ///Le sommet j a été visité
                for(int k=0; k<(int)m_vertices.size(); k++) ///Pout tout sommet k du graphe
                {
                    for(auto &elem: m_edges) ///Parmi les aretes dispos
                    {
                        if(k==elem.second.getTo() && j==elem.second.getFrom()) ///Si k est un successeur de j
                        {
                            pred[k]=true;///On dit que k a un prédecesseur (j)
                            suivant=true; ///Et on fait continuer la boucle
                        }
                    }
                }
            }
        }
    }
    for(int j=0; j<(int)m_vertices.size(); j++)
        marquage[j]=false;
    suivant=true;
    while(suivant) ///Tant que tous les sommets n'ont pas été visités
    {
        suivant=false;
        for(int j=0; j<(int)m_vertices.size(); j++) ///Pour tout sommet j du graphe
        {
            if(!marquage[j] && succ[j]) ///Si il n'a pas été marqué et qu'il admet un successeur
            {
                marquage[j]=true; ///Le sommet j a été visité
                for(int k=0; k<(int)m_vertices.size(); k++) ///Pout tout sommet k du graphe
                {
                    for(auto &elem: m_edges) ///Parmi les aretes dispos
                    {
                        if(k==elem.second.getFrom() && j==elem.second.getTo()) ///Si k est un prédécesseur de j
                        {
                            succ[k]=true;///On dit que k a un successeur (j)
                            suivant=true; ///Et on fait continuer la boucle
                        }
                    }
                }
            }
        }
    }
    for(int j=0; j<(int)m_vertices.size(); j++)
    {
        cmp[j]=pred[j]&succ[j]; ///On met en place la composante fortement connexe.
    }
    return cmp;
}
/// Aide à l'ajout de sommets interfacés
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Création d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
}

///ss prog qui initialise les bouton
void Graph::initButton()
{
    for(auto & elt: m_vertices)
    {
        elt.second.m_interface->m_button_addEdge.set_value(false);
    }
}

/// Aide à l'ajout d'arcs interfacés
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
{
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }
    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }
    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2],id_vert1,id_vert2);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei);
    ///on donne les valeurs de m_from et m_to
    m_edges[idx].setFrom(id_vert1);
    m_edges[idx].setTo(id_vert2);
    m_vertices[id_vert1].m_out.push_back(idx);
    m_vertices[id_vert2].m_in.push_back(idx);
}
=======

#include "graph.h"

/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    std::string num;
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de r�glage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 100.0);  // Valeurs arbitraires, � adapter...
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) );

    ///box qui affiche une croix pour supprimer un sommet
    m_top_box.add_child(m_cross);
    m_cross.set_bg_color(ROUGE);
    m_cross.set_dim(10,10);
    m_cross.set_pos(115,0);

    ///boite qui permet de savoir si on ajoute une ar�te dans ce sommet
    m_top_box.add_child(m_button_addEdge);
    m_button_addEdge.set_bg_color(BLEU);
    m_button_addEdge.set_dim(10,10);
    m_button_addEdge.set_pos(95,0);

    m_top_box.add_child(m_textNum);
    m_textNum.set_pos(37,88);
    m_textNum.set_message("");

    m_top_box.add_child(m_img2);
    m_img2.set_dim(100,100);
    m_img2.set_gravity_x(grman::GravityX::Right);
    m_img2.set_pic_name("");

}


/// Gestion du Vertex avant l'appel � l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_value vers le slider associ�
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donn�e m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}


/// Gestion du Vertex apr�s l'appel � l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_value locale
    m_value = m_interface->m_slider_value.get_value();
}

/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to,int numS1, int numS2)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    ///variable qui permet d'afficher les sommmets des ar�tes
    std::string name;
    std::string sommet1;
    std::string sommet2;

    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de r�glage associ�s
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(40,75);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de r�glage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 200.0);  // Valeurs arbitraires, � adapter...
    m_slider_weight.set_dim(10,45);
    m_slider_weight.set_pos(13,13);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_pos(15,65);

    ///croix pour supprimer une ar�te
    m_box_edge.add_child(m_cross);
    m_cross.set_bg_color(BLEUCLAIR);
    m_cross.set_dim(7,7);
    m_cross.set_pos(0,63);

    ///texte qui affiche les sommets de l'ar�te
    m_box_edge.add_child(m_text_number);
    sommet1=std::to_string(numS1);
    sommet2=std::to_string(numS2);
    name=sommet1+"-"+sommet2;
    m_text_number.set_pos(0,1);
    m_text_number.set_message(name);
}


/// Gestion du Edge avant l'appel � l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_weight vers le slider associ�
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donn�e m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );

    ///faire grossir la fl�che en fonction de son poids
    m_interface->m_top_edge.grossir(m_weight);
}

/// Gestion du Edge apr�s l'appel � l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de pr�parer un cadre d'accueil des
/// �l�ments qui seront ensuite ajout�s lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);
    ///bouton pour sauvegarder
    m_top_box.add_child(m_button_save);
    m_button_save.set_dim(50,35);
    m_button_save.set_pos(15,15);
    m_button_save.set_bg_color(BLEUCLAIR);

    ///texte de sauvegarde
    m_button_save.add_child(m_text_save);
    m_text_save.set_pos(2,13);
    m_text_save.set_message("Sauver");

    ///bouton pour reset les position
    m_top_box.add_child(m_button_reset);
    m_button_reset.set_dim(50,35);
    m_button_reset.set_pos(15,65);
    m_button_reset.set_bg_color(ROUGE);

    ///texte de reset
    m_button_reset.add_child(m_text_reset);
    m_text_reset.set_pos(6,13);
    m_text_reset.set_message("Reset");

    ///bouton pour ajouter un sommet
    m_top_box.add_child(m_button_addVertex);
    m_button_addVertex.set_dim(50,35);
    m_button_addVertex.set_pos(15,115);
    m_button_addVertex.set_bg_color(VERTFLUOCLAIR);

    ///texte d'ajout de sommet
    m_button_addVertex.add_child(m_text_addV);
    m_text_addV.set_pos(13,5);
    m_text_addV.set_message("Add");

    m_button_addVertex.add_child(m_text_vertex);
    m_text_vertex.set_pos(3,20);
    m_text_vertex.set_message("Sommet");

    ///bouton pour ajouter
    m_top_box.add_child(m_button_addEdge);
    m_button_addEdge.set_dim(50,35);
    m_button_addEdge.set_pos(15,165);
    m_button_addEdge.set_bg_color(SABLE);

    ///texte d'ajout d'ar�te
    m_button_addEdge.add_child(m_text_addE);
    m_text_addE.set_pos(13,5);
    m_text_addE.set_message("Add");

    m_button_addEdge.add_child(m_text_edge);
    m_text_edge.set_pos(10,20);
    m_text_edge.set_message("Edge");

    m_top_box.add_child(m_button_noAddEdge);
    m_button_noAddEdge.set_dim(7,7);
    m_button_noAddEdge.set_pos(68,176);
    m_button_noAddEdge.set_bg_color(BLEUSOMBRE);

    ///texte d'ajout d'ar�te
    m_top_box.add_child(m_button_k_connex);
    m_button_k_connex.set_dim(50,35);
    m_button_k_connex.set_pos(15,215);
    m_button_k_connex.set_bg_color(VERTCLAIR);

    ///texte d'ajout
    m_button_k_connex.add_child(m_text_k);
    m_text_k.set_pos(20,5);
    m_text_k.set_message("K");

    m_button_k_connex.add_child(m_text_connex);
    m_text_connex.set_pos(0,20);
    m_text_connex.set_message("Connex");

    ///bouton pour afficher les composante fortement connexe
    m_top_box.add_child(m_button_connex);
    m_button_connex.set_dim(50,35);
    m_button_connex.set_pos(15,265);
    m_button_connex.set_bg_color(VIOLETCLAIR);

    ///TEXTE POUR LA CONNEXITE
    m_button_connex.add_child(m_text_compo);
    m_text_compo.set_pos(5,5);
    m_text_compo.set_message("Compo");

    m_button_connex.add_child(m_text_connexe);
    m_text_connexe.set_pos(0,20);
    m_text_connexe.set_message("Connex");

    ///bouton pour reset les compo connex
    m_tool_box.add_child(m_button_restartConnex);
    m_button_restartConnex.set_dim(7,7);
    m_button_restartConnex.set_pos(68,276);
    m_button_restartConnex.set_bg_color(BLEU);

    ///bouton pour lancer la simulation
    m_top_box.add_child(m_button_start);
    m_button_start.set_dim(50,35);
    m_button_start.set_pos(15,315);
    m_button_start.set_bg_color(CYAN);
    ///texte pour lancer la simulation
    m_button_start.add_child(m_text_run);
    m_text_run.set_pos(15,13);
    m_text_run.set_message("Run");

    ///bouton pour lancer la simulation
    m_top_box.add_child(m_button_stop);
    m_button_stop.set_dim(50,35);
    m_button_stop.set_pos(15,365);
    m_button_stop.set_bg_color(JAUNE);
    ///texte pour lancer la simulation
    m_button_stop.add_child(m_text_stop);
    m_text_stop.set_pos(9,13);
    m_text_stop.set_message("Stop");

    ///bouton pour mettre les nombre al�atoire
    m_top_box.add_child(m_button_random);
    m_button_random.set_dim(50,35);
    m_button_random.set_pos(15,415);
    m_button_random.set_bg_color(ROSE);

    ///TEXTE RANDOM
    m_button_random.add_child(m_text_random);
    m_text_random.set_pos(2,13);
    m_text_random.set_message("Random");

    ///bouton  pour quitter le programme
    m_top_box.add_child(m_button_quit);
    m_button_quit.set_dim(60,35);
    m_button_quit.set_pos(11,520);
    m_button_quit.set_bg_color(ORANGE);

    m_button_quit.add_child(m_text_quit);
    m_text_quit.set_pos(2,13);
    m_text_quit.set_message("Quitter");
}


/// M�thode sp�ciale qui construit un graphe arbitraire (d�mo)
/// Cette m�thode est � enlever et remplacer par un syst�me
/// de chargement de fichiers par exemple.
/// Bien s�r on ne veut pas que vos graphes soient construits
/// "� la main" dans le code comme �a.
void Graph::make_example()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne pr�c�dente est en gros �quivalente � :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    /// Les sommets doivent �tre d�finis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
    add_interfaced_vertex(0, 30.0, 200, 100, "clown1.jpg");
    add_interfaced_vertex(1, 60.0, 400, 100, "clown2.jpg");
    add_interfaced_vertex(2,  50.0, 200, 300, "clown3.jpg");
    add_interfaced_vertex(3,  0.0, 400, 300, "clown4.jpg");
    add_interfaced_vertex(4,  100.0, 600, 300, "clown5.jpg");
    add_interfaced_vertex(5,  0.0, 100, 500, "bad_clowns_xx3xx.jpg", 0);
    add_interfaced_vertex(6,  0.0, 300, 500, "bad_clowns_xx3xx.jpg", 1);
    add_interfaced_vertex(7,  0.0, 500, 500, "bad_clowns_xx3xx.jpg", 2);

    /// Les arcs doivent �tre d�finis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
    add_interfaced_edge(0, 1, 2, 50.0);
    add_interfaced_edge(1, 0, 1, 50.0);
    add_interfaced_edge(2, 1, 3, 75.0);
    add_interfaced_edge(3, 4, 1, 25.0);
    add_interfaced_edge(4, 6, 3, 25.0);
    add_interfaced_edge(5, 7, 3, 25.0);
    add_interfaced_edge(6, 3, 4, 0.0);
    add_interfaced_edge(7, 2, 0, 100.0);
    add_interfaced_edge(8, 5, 2, 20.0);
    add_interfaced_edge(9, 3, 7, 80.0);
}

///sous programe qui r�initialise les postions et les sommets initials en fonction d'un fichier texte
void Graph::reinit(std::string fileName)
{
    ///interface graphique
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    ///variable pour stocker les valeur lu dans le fichier
    std::string nom;
    int xdep,ydep,numS;
    double valeur;
    std::string pic_name;
    ///fichier que l'on lit
    std::ifstream fichier(fileName,std::ios::in);
    ///on efface tout
    for(auto it = m_vertices.begin(); it != m_vertices.end();)
    {
        it=m_vertices.erase(it);
    }
    for(auto it=m_edges.begin(); it!=m_edges.end();)
    {
        it=m_edges.erase(it);
    }
    ///si on lit le fichier
    if(fichier)
    {
        ///on r�cup�re l'ordre du graphe et on saute une ligne
        fichier>>m_ordre;
        getline(fichier,nom);
        ///on se base sur le nb de sommet pr�sent dans le graphe
        for(int i=0; i<m_ordre; i++)
        {
            ///on remplit le num�ro
            fichier>>numS;
            getline(fichier,nom);
            ///sa valeur
            fichier>> valeur;
            getline(fichier,nom);
            ///sa position
            fichier>>xdep;
            getline(fichier,nom);
            fichier>>ydep;
            getline(fichier,nom);
            ///son image
            fichier>>pic_name;
            getline(fichier,nom);
            ///on ajoute un vecteur
            add_interfaced_vertex(numS,valeur,xdep,ydep,pic_name);
        }
        ///on stocke le nb d'ar�te
        fichier>>m_nbArete;
        getline(fichier,nom);
        for(int i=0; i<m_nbArete; i++)
        {
            ///on r�cup�re le num�ro du sommet
            fichier>>numS;
            getline(fichier,nom);
            ///sa position
            fichier>>ydep;
            getline(fichier,nom);
            fichier>>xdep;
            getline(fichier,nom);
            ///sa valeur
            fichier>>valeur;
            getline(fichier,nom);
            ///on ajoute une nouvelle ar�te
            add_interfaced_edge(numS,xdep,ydep,valeur);
        }
        ///on indique un RESET
        std::cout<<"RESET"<<std::endl;
        fichier.close();
    }
    else
    {
        throw std::string("Il n'exite pas de fichier portant ce nom");
    }
    ///tout les param�tre permettant l'ajout deviennent faux
    setQuitGraph(false);
    m_ajouterNewEdge=false;
    m_addFrom=false;
    m_addTo=false;
}
///Ss programme qui remplit un graphe en fonction d'un fichier
void Graph::ReadFile(std::string fileName, int num)
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    ///varialbe string pour sauter des lignes
    std::string nom;
    m_numGraphe=num;
    ///information utile pour faire le sommet
    int numS, x, y;
    double valeur;
    std::string pic_name;
    ///lecture du fichier
    std::ifstream fichier(fileName+".txt",std::ios::in);
    ///si on lit le fichier
    if(fichier)
    {
        ///on r�cup�re l'ordre du graphe et on saute une ligne
        fichier>>m_ordre;
        getline(fichier,nom);
        ///on se base sur le nb de sommet pr�sent dans le graphe
        for(int i=0; i<m_ordre; i++)
        {
            ///on remplit le num�ro
            fichier>>numS;
            getline(fichier,nom);
            ///sa valeur
            fichier>>valeur;
            getline(fichier,nom);
            ///sa position
            fichier>>x;
            getline(fichier,nom);
            fichier>>y;
            getline(fichier,nom);
            ///son image
            fichier>>pic_name;
            getline(fichier,nom);
            ///on ajoute un vecteur
            add_interfaced_vertex(numS,valeur,x,y,pic_name);

        }
        ///on r�cup�re le nb d'ar�te pr�sente dans notre graphe
        fichier>>m_nbArete;
        std::cout<<m_nbArete<<std::endl;
        ///en fonction de nb d'ar�te
        for(int i=0; i<m_nbArete; i++)
        {
            ///on r�cup�re le num�ro du sommet
            fichier>>numS;
            getline(fichier,nom);
            ///sa position
            fichier>>y;
            getline(fichier,nom);
            fichier>>x;
            getline(fichier,nom);
            ///sa valeur
            fichier>>valeur;
            getline(fichier,nom);
            ///on ajoute une nouvelle ar�te
            add_interfaced_edge(numS,x,y,valeur);

        }
        ///on ferme le fichier
        fichier.close();
    }
    ///si on arrive pas � lire le fichier, on envoies un exception
    else
    {
        throw std::string("Il n'exite pas de fichier portant ce nom");
    }
    ///tout les param�tre permettant l'ajout deviennet faux
    setQuitGraph(false);
    m_ajouterNewEdge=false;
    m_addFrom=false;
    m_addTo=false;
    ///si le vecteur n'a pas encore �t� rempli, on le remplit
    if(m_vertexForNewEdge.size()==0)
    {
        m_vertexForNewEdge.push_back(0);
        m_vertexForNewEdge.push_back(0);
    }
}

///ss programme qui sauvegarde un graphe en le remplissant dans un fichier
void Graph::saveFile(std::string fileName)
{
    ///variable pour la valeur de l'image
    std::string number;
    ///position du sommet
    int posInter=0;
    ///string du nom de l'image qui change en focntion du num�ro de graphe
    std::string name;
    ///notre fichier txt
    std::ofstream fichier(fileName+ ".txt",std::ios::out);
    ///si on lit un fichier
    if(fichier)
    {
        ///on regarde l'ordre du graphe
        fichier<<m_vertices.size()<<std::endl;
        ///pour tout les sommets existants
        for(auto &el:m_vertices)
        {
            ///on �crit le num�ro de sommet, sa valeur, sa position et le nom de son image
            fichier<<el.first<<std::endl;
            fichier<<el.second.m_value<<std::endl;
            posInter=el.second.m_interface->m_top_box.get_posx()+2;
            fichier<<posInter<<std::endl;
            posInter=el.second.m_interface->m_top_box.get_posy()+2;
            fichier<<posInter<<std::endl;
            name=el.second.m_interface->m_img.getName();
            fichier<<name<<std::endl;
        }
        ///on lit le nb d'ar�te
        fichier<<m_edges.size()<<std::endl;
        ///pour toutes les ar�tes existantes
        for(auto &el:m_edges)
        {
            ///on lit leur num�ros, les sommets et leurs valeurs
            fichier<<el.first<<std::endl;
            fichier<<el.second.m_to<<std::endl;
            fichier<<el.second.m_from<<std::endl;
            fichier<<el.second.m_weight<<std::endl;
        }
        fichier.close();
    }

    ///sinon on envoie une exception
    else
    {
        throw std::string("Il n'exite pas de fichier portant ce nom");
    }
}

/// La m�thode update � appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update(clock_t ini, bool animation)
{
    bool good;
    int num=m_numGraphe;
    if (!m_interface)
        return;

    for (auto &elt : m_vertices)
        elt.second.pre_update();
    for (auto &elt : m_edges)
        elt.second.pre_update();

    m_interface->m_top_box.update();

    for (auto &elt : m_vertices)
    {
        elt.second.post_update();
        ///si on ajoute un nouvelle ar�te
        if(m_ajouterNewEdge)
        {
            ///si on appuie sur le bouton bleu
            if(elt.second.m_interface->m_button_addEdge.get_value())
            {
                ///si on a pas encore selectionn� de sommet
                if(m_addFrom==false)
                {
                    ///on stocke l'indice du sommet
                    m_vertexForNewEdge[0]=elt.first;
                    m_addFrom=true;
                    m_vertexAlreadyUse=m_vertexForNewEdge[0];
                }
                /// si on a selectionn� un autre sommet
                else if(m_addTo==false&&m_addFrom==true&&m_vertexAlreadyUse!=elt.first)
                {
                    good=true;
                    ///on ajoute une arr�te et reset tout les param�tre
                    m_vertexForNewEdge[1]=elt.first;
                    m_addFrom=false;
                    m_addTo=false;
                    for(auto &elm:m_edges)
                    {
                        if(m_vertexForNewEdge[0]==elm.second.m_from&&m_vertexForNewEdge[1]==elm.second.m_to)
                        {
                            good=false;
                        }
                    }
                    if(good)
                    {
                        addEdge(m_vertexForNewEdge);
                    }
                    else if(!good)
                    {
                        std::cout<<"cet arete existe d�j�"<<std::endl;
                    }
                    m_ajouterNewEdge=false;
                    m_interface->m_button_connex.set_value(false);
                }
            }
        }
    }
    for (auto &elt : m_edges)
    {
        elt.second.post_update();
        ///si on clique sur la croix d'une ar�te

    }
    for(auto & elt:m_edges)
    {
        if(elt.second.m_interface->m_cross.get_value())
        {
            ///on la supprime
            removeEdge(elt.first);
            m_interface->m_button_connex.set_value(false);
            saveFile("Graphe1Inter");
            initialisation();
            ReadFile("Graphe1Inter",num);
            break;
        }
    }
    for(auto & elt:m_vertices)
    {
        if(elt.second.m_interface->m_cross.get_value())
        {
            ///on la supprime
            removeVertex(elt.first);
            m_interface->m_button_connex.set_value(false);
            break;
        }
    }
    ///si on clique sur le bouton sauvegarder
    if(m_interface->m_button_save.get_value())
    {
        ///selon le num�ro du graphe
        switch(m_numGraphe)
        {
        case 1:
            ///on sauvegarde
            try
            {
                saveFile("Graphe1");
                m_interface->m_button_save.set_value(false);
                std::cout<<"Sauvegarde du Graphe 1"<<std::endl;
            }
            catch(const std::string & e)
            {
                std::cout << e << "\n\n";
            }
            break;
        case 2:
            ///on sauvegarde
            try
            {
                saveFile("Graphe2");
                m_interface->m_button_save.set_value(false);
                std::cout<<"Sauvegarde du Graphe 2"<<std::endl;
            }
            catch(const std::string & e)
            {
                std::cout << e << "\n\n";
            }
            break;
        case 3:
            ///on sauvegarde
            try
            {
                saveFile("Graphe3");
                m_interface->m_button_save.set_value(false);
                std::cout<<"Sauvegarde du Graphe 3"<<std::endl;
            }
            catch(const std::string & e)
            {
                std::cout << e << "\n\n";
            }
            break;

        }
        saveFile("Graphe1Inter");
        initialisation();
        ReadFile("Graphe1Inter",num);
    }
    ///on appuie sur le bouton reset
    if(m_interface->m_button_reset.get_value())
    {
        std::cout<<"good";
        ///en fonction du num�ro du graphe
        ///on lit le fichier Reset.txt
        switch(m_numGraphe)
        {
        case 1:
            try
            {
                reinit("Graphe1Reset.txt");
                m_interface->m_button_reset.set_value(false);
            }
            catch(const std::string & e)
            {
                std::cout << e << "\n\n";
            }
            break;
        case 2:
            try
            {
                reinit("Graphe2Reset.txt");
                m_interface->m_button_reset.set_value(false);
            }
            catch(const std::string & e)
            {
                std::cout << e << "\n\n";
            }
            break;
        case 3:
            try
            {
                reinit("Graphe3Reset.txt");
                m_interface->m_button_reset.set_value(false);
            }
            catch(const std::string & e)
            {
                std::cout << e << "\n\n";
            }
            break;

        }
    }
    ///si on appuie sur ce bouton
    ///on quitte le graphe
    if(m_interface->m_button_quit.get_value())
    {
        setQuitGraph(true);
        std::cout<<"Retour au menu"<<std::endl;
    }
    ///si on appuie sur le bouton
    ///on ajoute un sommet
    if(m_interface->m_button_addVertex.get_value())
    {
        m_interface->m_button_addVertex.set_value(false);
        addVertex();
        saveFile("Graphe1Inter");
        initialisation();
        ReadFile("Graphe1Inter",num);
    }
    ///si on appuie sur le bouton addEdge
    if(m_interface->m_button_addEdge.get_value())
    {
        m_interface->m_button_addEdge.set_value(false);
        m_ajouterNewEdge=true;
        initButton();
        std::cout<<"Selectionner 2 sommet"<<std::endl;

    }
    if(m_interface->m_button_noAddEdge.get_value())
    {
        m_ajouterNewEdge=false;
        m_interface->m_button_noAddEdge.set_value(false);
        std::cout<<"Vous n'ajoutez plus une nouvelle arete"<<std::endl;
    }
    if(m_interface->m_button_k_connex.get_value())
    {
        m_interface->m_button_k_connex.set_value(false);
        init_k_connex();
        rest(300);
    }
    if(m_interface->m_button_connex.get_value())
    {
        m_interface->m_button_connex.set_value(false);
        algo_forte_connexite();
        for(auto &el:m_vertices)
        {
            el.second.m_interface->m_textNum.set_message(std::to_string(el.second.m_numero_compo_connexe));
        }
    }
    if(m_interface->m_button_restartConnex.get_value())
    {
        m_interface->m_button_connex.set_value(false);
        saveFile("Graphe1Inter");
        initialisation();
        ReadFile("Graphe1Inter",num);
    }
    if(m_interface->m_button_start.get_value())
    {
        animation=true;
    }
    if(m_interface->m_button_stop.get_value())
    {
        animation=false;
        m_interface->m_button_start.set_value(false);
        m_interface->m_button_stop.set_value(false);
    }
    if(animation)
    {
        croissance_sommets(ini);
        croix_rouge();
    }
    else if (!animation)
    {
        for(auto & el:m_vertices)
        {
            el.second.m_interface->m_img2.set_pic_name("");
        }
    }
    if(m_interface->m_button_random.get_value())
    {
        m_interface->m_button_random.set_value(false);
        random();
    }
}

///la m�thode qui ajoute une arete
void Graph::addEdge(std::vector<int> m_newVertexForEdge)
{
    ///variable
    int indice;
    bool good=true;
    ///on explore toutes les arete
    for(auto elt:m_edges)
    {
        ///si on a une arete similaire dans notre map d'arete
        if(elt.second.m_from==m_vertexForNewEdge[0]&&elt.second.m_to==m_vertexForNewEdge[1])
        {
            ///alors on ajoute pas de nouvelle arete
            good=false;
            std::cout<<"On peut pas rajouter une nouvelle arete"<<std::endl;
        }
    }
    ///si l'arete n'existe alors
    if(good)
    {
        ///on regarde l'indice le plus grand de la map d'arete
        for(auto it=m_edges.begin(); it!=m_edges.end(); it++)
        {
            indice=it->first;
        }
        ///et on ajoute +1
        indice++;
        ///on ajoute une nouvelle arete
        add_interfaced_edge(indice,m_vertexForNewEdge[0],m_vertexForNewEdge[1],60);
        std::cout<<"Ajout d'une nouvelle arete"<<std::endl;
    }

}

///la methode qui enleve un sommet
void Graph::removeVertex(int num)
{
    ///on enleve les aretes qui sont connect�s par des sommets
    for(int i=m_vertices[num].m_in.size()-1; i>=0; i--)
    {
        removeEdge(m_vertices[num].m_in[i]);
    }
    for(int i=m_vertices[num].m_out.size()-1; i>=0; i--)
    {
        removeEdge(m_vertices[num].m_out[i]);
    }
    ///on enleve l'interface du sommet et on enleve le sommet
    m_interface->m_main_box.remove_child( m_vertices[num].m_interface->m_top_box );
    m_vertices.erase(num);
    std::cout<<"Suppresion d'un sommet"<<std::endl;
    rendreConti(num);
}

void Graph::rendreConti(int num)
{
    int **indiceForI;
    int idx=0;
    int inter=0;
    inter=m_numGraphe;
    indiceForI=new int *[2];
    for (int i=0; i<2; i++)
    {
        indiceForI[i]=new int [m_vertices.size()];
    }
    for(auto el:m_vertices)
    {
        indiceForI[0][idx]=idx;
        indiceForI[1][idx]=el.first;
        idx++;
    }
    for(int i=0; i<m_vertices.size(); i++)
    {
        std::cout<<indiceForI[0][i]<<" "<<indiceForI[1][i]<<std::endl;

    }
    idx=0;
    for(auto & el:m_edges)
    {
        for(int j=0; j<m_vertices.size(); j++)
        {
            if(indiceForI[1][j]==el.second.m_from&&indiceForI[1][j]>=num)
            {
                el.second.m_from=indiceForI[0][j];
            }
        }
        for(int j=0; j<m_vertices.size(); j++)
        {
            if(indiceForI[1][j]==el.second.m_to&&indiceForI[1][j]>=num)
            {
                el.second.m_to=indiceForI[0][j];
            }
        }
    }
    for(auto & el:m_edges)
    {
        std::cout<<el.second.m_from<<"  "<<el.second.m_to<<std::endl;
    }
    rendreConti2("Graphe1Inter");
    initialisation();
    ReadFile("Graphe1Inter",inter);
    std::cout<<std::endl<<m_numGraphe;
}

void Graph::rendreConti2(std::string fileName)
{
    ///variable pour la valeur de l'image
    std::string number;
    ///position du sommet
    int posInter=0;
    ///string du nom de l'image qui change en focntion du num�ro de graphe
    std::string name;
    ///notre fichier txt
    std::ofstream fichier(fileName+ ".txt",std::ios::out);
    int indice=0;
    ///si on lit un fichier
    if(fichier)
    {
        ///on regarde l'ordre du graphe
        fichier<<m_vertices.size()<<std::endl;
        ///pour tout les sommets existants
        for(auto &el:m_vertices)
        {
            ///on �crit le num�ro de sommet, sa valeur, sa position et le nom de son image
            fichier<<indice<<std::endl;
            fichier<<el.second.m_value<<std::endl;
            posInter=el.second.m_interface->m_top_box.get_posx()+2;
            fichier<<posInter<<std::endl;
            posInter=el.second.m_interface->m_top_box.get_posy()+2;
            fichier<<posInter<<std::endl;
            name=el.second.m_interface->m_img.getName();
            fichier<<name<<std::endl;
            indice++;
        }
        ///on lit le nb d'ar�te
        fichier<<m_edges.size()<<std::endl;
        ///pour toutes les ar�tes existantes
        for(auto &el:m_edges)
        {
            ///on lit leur num�ros, les sommets et leurs valeurs
            fichier<<el.first<<std::endl;
            fichier<<el.second.m_to<<std::endl;
            fichier<<el.second.m_from<<std::endl;
            fichier<<el.second.m_weight<<std::endl;
        }
        fichier.close();
    }

    ///sinon on envoie une exception
    else
    {
        throw std::string("Il n'exite pas de fichier portant ce nom");
    }
}

void Graph::addVertex()
{
    ///on regarde l'indice le plus grand de la map d'arete
    int indice;
    //int number1;
    std::string name;
    std::string number;
    std::string picName;
    std::vector<std::string> allName;
    bool good=false;
    int idx=1;
    for(auto & el:m_vertices)
    {
        allName.push_back(el.second.m_interface->m_img.getName());
    }
    switch(m_numGraphe)
    {
    case 1:
    {

        while(!good)
        {
            good=true;
            if(idx>12)
            {
                number=std::to_string(idx-12);
                picName="Poke"+number+".bmp";
            }
            else if(idx<=12)
            {
                number=std::to_string(idx);
                picName="Dragon"+number+".bmp";
            }
            for(int i=0;i<allName.size();i++)
            {
                if(picName==allName[i])
                {
                    good=false;
                }
            }
            if(!good)
            {
                idx++;
            }
        }
    break;
    }
case 2:
    {
       while(!good)
        {
            good=true;
            if(idx>11)
            {
                number=std::to_string(idx-11);
                picName="Poke"+number+".bmp";
            }
            else if(idx<=11)
            {
                number=std::to_string(idx);
                picName="Mer"+number+".bmp";
            }
            for(int i=0;i<allName.size();i++)
            {
                if(picName==allName[i])
                {
                    good=false;
                }
            }
            if(!good)
            {
                idx++;
            }
        }
        break;
    }
case 3:
    {
        while(!good)
        {
            good=true;
            if(idx>12)
            {
                number=std::to_string(idx-11);
                picName="Mer"+number+".bmp";
            }
            else if(idx<=12)
            {
                number=std::to_string(idx);
                picName="Dragon"+number+".bmp";
            }
            for(int i=0;i<allName.size();i++)
            {
                if(picName==allName[i])
                {
                    good=false;
                }
            }
            if(!good)
            {
                idx++;
            }
        }
        break;
    }
}

for(auto it=m_vertices.begin(); it!=m_vertices.end(); it++)
{
    indice= it->first;
}
///on ajoute +1 sur l'indice
indice++;
///on ajoute un sommet
add_interfaced_vertex(indice,50,300,300,picName);
}
///la m�thode qui enleve une arete
void Graph::removeEdge(int num)
{
    /// r�f�rence vers le Edge � enlever
    Edge &remed=m_edges[num];
    //std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    //std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    //std::cout << m_edges.size() << std::endl;
    /// test : on a bien des �l�ments interfac�s
    if (m_interface && remed.m_interface)
    {
        /// Ne pas oublier qu'on a fait �a � l'ajout de l'arc :
        /* EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]); */
        /* m_interface->m_main_box.add_child(ei->m_top_edge);  */
        /* m_edges[idx] = Edge(weight, ei); */
        /// Le new EdgeInterface ne n�cessite pas de delete car on a un shared_ptr
        /// Le Edge ne n�cessite pas non plus de delete car on n'a pas fait de new (s�mantique par valeur)
        /// mais il faut bien enlever le conteneur d'interface m_top_edge de l'arc de la main_box du graphe
        m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );
    }
    /// Il reste encore � virer l'arc supprim� de la liste des entrants et sortants des 2 sommets to et from
    /// References sur les listes de edges des sommets from et to
    std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;
    std::vector<int> &veto = m_vertices[remed.m_to].m_in;
    vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), num ), vefrom.end() );
    veto.erase( std::remove( veto.begin(), veto.end(), num ), veto.end() );
    /// Le Edge ne n�cessite pas non plus de delete car on n'a pas fait de new (s�mantique par valeur)
    /// Il suffit donc de supprimer l'entr�e de la map pour supprimer � la fois l'Edge et le EdgeInterface
    /// mais malheureusement ceci n'enlevait pas automatiquement l'interface top_edge en tant que child de main_box !
    m_edges.erase( num );
    /// Tester la coh�rence : nombre d'arc entrants et sortants des sommets 1 et 2
}
//void Graph::kVertexConnexite()
//{
//    std::vector<std::vector<int>> allCombinaison;
//    ///toute les combinaison de sommets
//    for(int i=1;i<=m_vertices.size();i++)
//    {
//        addCombi(i,allCombinaison);
//    }
//}

//void Graph::addCombi(int i,std::vector<std::vector<int>> allCombinaison)
//{
//    if(m_vertices.size()>=i)
//    {
//        {
//
//        }
//    }
//}

///ss prog qui efface toute les sommets et arete d'un graphe et on set son num�ro
void Graph::initialisation()
{
    ///on d�truit les map de sommet et d'ar�tes
    for(auto it = m_vertices.begin(); it != m_vertices.end();)
    {
        it=m_vertices.erase(it);
    }
    for(auto it=m_edges.begin(); it!=m_edges.end();)
    {
        it=m_edges.erase(it);
    }
    m_numGraphe=0;
}

void Graph::init_k_connex()
{
    int indice;
    bool noNeed=false;
    int nbAdj=0;
    std::vector<std::vector <int>> allCombi;
    std::vector<int> inter;
    for(auto& el:m_vertices)
    {

        nbAdj=el.second.m_out.size()+el.second.m_in.size();
        if(nbAdj==0)
        {
            noNeed=true;
        }
    }
    if(noNeed)
    {
        std::cout<<"le graphe est deja non connexe"<<std::endl;
    }
    else if(!noNeed)
    {
        for(auto &el:m_vertices)
        {
            indice=el.first;
            el.second.m_existe=true;
        }
        indice++;
        m_toursDeBoucle=0;
        m_toursDeBoucleMax=9999;
        k_connex(inter,allCombi);
        afficher_k_connex(inter,allCombi);
    }

//    for(int i=0;i<m_allCombi.size();i++)
//    {
//        for(int j=0;j<m_allCombi[i].size();j++)
//        {
//            std::cout<<m_allCombi[i][j]<<" ";
//        }
//        std::cout<<std::endl;
//    }
}

void Graph::k_connex(std::vector<int>& inter, std::vector<std::vector <int>>& allCombi)
{
    bool good;
    for(auto & el:m_vertices)
    {
        if(el.second.m_existe)
        {
            el.second.m_existe=false;
            if(graphConnex(el.first))
            {
                inter.push_back(el.first);
                m_toursDeBoucle++;
                if(m_toursDeBoucle<m_toursDeBoucleMax)
                {
                    k_connex(inter,allCombi);
                }
                inter.pop_back();
            }
            else if(!graphConnex(el.first))
            {
                m_toursDeBoucle++;
                good=true;
                for(int i=0; i<allCombi.size(); i++)
                {
                    if(allCombi[i]==inter)
                    {
                        good=false;
                    }
                }
                if(good)
                    allCombi.push_back(inter);
                if(m_toursDeBoucle<m_toursDeBoucleMax)
                {
                    m_toursDeBoucleMax=m_toursDeBoucle;
                }
            }
            m_toursDeBoucle--;
            el.second.m_existe=true;

        }
    }
}

bool Graph::graphConnex(int idx)
{

    int inter;
    int indice1=0;
    int indice2=0;
    std::queue<int> file;
    for(auto & el:m_vertices)
    {
        el.second.m_marqueur=false;
    }
    m_vertices[idx].m_marqueur=true;
    file.push(idx);
    while(file.size()!=0)
    {
        inter=file.front();
        file.pop();
        for(int i=0; i<m_vertices[inter].m_in.size(); i++)
        {
            if(m_vertices[m_edges[m_vertices[inter].m_in[i]].m_from].m_existe&&!m_vertices[m_edges[m_vertices[inter].m_in[i]].m_from].m_marqueur)
            {
                file.push(m_edges[m_vertices[inter].m_in[i]].m_from);
                m_vertices[m_edges[m_vertices[inter].m_in[i]].m_from].m_marqueur=true;
            }

        }
        for(int i=0; i<m_vertices[inter].m_out.size(); i++)
        {
            if(m_vertices[m_edges[m_vertices[inter].m_out[i]].m_to].m_existe&&!m_vertices[m_edges[m_vertices[inter].m_out[i]].m_to].m_marqueur)
            {
                file.push(m_edges[m_vertices[inter].m_out[i]].m_to);
                m_vertices[m_edges[m_vertices[inter].m_out[i]].m_to].m_marqueur=true;
            }
        }
    }
    for(auto& el:m_vertices)
    {
        if(el.second.m_existe)
        {
            indice1++;
        }
        if(el.second.m_marqueur)
        {
            indice2++;
        }
    }
    if(indice1>indice2)
    {
        return false;
    }
    else if(indice1==indice2)
    {
        return true;
    }

}

void Graph::afficher_k_connex(std::vector<int>& inter,std::vector<std::vector <int>>& allCombi)
{
    int poids_min=888888;
    std::vector<std::vector <int>> allCombMin;
    std::vector<std::vector <int>> allCombiMin2;
    std::vector<std::vector <int>> alreadyUse;
    int indice=0;
    bool good;
    for(int i=0; i<allCombi.size(); i++)
    {
        if(allCombi[i].size()<poids_min)
        {
            poids_min=allCombi[i].size();
        }
    }
    for(int i=0; i<allCombi.size(); i++)
    {
        if(allCombi[i].size()==poids_min)
        {
            allCombMin.push_back(allCombi[i]);
        }
    }
    for(int i=0; i<allCombMin.size(); i++)
    {
        inter=allCombMin[i];

        indice=0;
        good=true;
        for(int j=0; j<alreadyUse.size(); j++)
        {
            for(int k=0; k<alreadyUse[i].size(); i++)
            {
                for(int l=0; l<inter.size(); l++)
                {
                    if(inter[l]==alreadyUse[j][k])
                    {
                        indice++;
                    }
                    if(indice==poids_min)
                    {
                        good=false;
                    }
                }
            }
        }
        if(good)
        {
            allCombiMin2.push_back(inter);
            alreadyUse.push_back(inter);
        }
    }
    /// std::cout<<poids_min<<std::endl;
    std::cout<<"Pour avoir un graphe connexe il faut enlever les combinaisons de sommets suivant:"<<std::endl;
//    for(int i=0; i<m_allCombi.size(); i++)
//    {
//        if(m_allCombi[i].size()==poids_min)
//        {
//            std::cout<<"- ";
//            for(int j=0; j<m_allCombi[i].size(); j++)
//            {
//                std::cout<<m_allCombi[i][j]<<" ";
//            }
//
//            std::cout<<std::endl;
//        }
//    }
    for(int i=0; i<allCombiMin2.size(); i++)
    {
        if(allCombiMin2.size()!=0)
        {
            std::cout<<"- ";
            for(int j=0; j<allCombiMin2[i].size(); j++)
            {
                std::cout<<allCombiMin2[i][j]<<" ";
            }
            std::cout<<std::endl;
        }

    }

}

std::map<int,std::vector<int>>  Graph::algo_forte_connexite()
{
    int couleurs[20]= {ROUGE, ROUGECLAIR, BLEU, VERT, JAUNE, ORANGE, SABLE, GRIS, BLANC, VIOLETCLAIR, MARRONCLAIR, BLEUCLAIR, VERTCLAIR, BLANCBLEU, JAUNECLAIR,FUCHSIASOMBRE,FUCHSIA,CYAN,KAKI,CYANSOMBRE};
    int nb_compos=1; ///Nombre de composantes fortement connexes
    int nb_arete(0);
    bool presence_cfc;
    std::vector<int> list_sommets;
    std::map<int,std::vector<int>> composantes_connexes;
    bool** compos_connexes; ///Tableau qui d�termine les composantes fortement connexes.
    compos_connexes=new bool*[(int)m_vertices.size()]; ///1ere allocation de m�moire
    for(int i=0; i<(int)m_vertices.size(); i++) ///2�me allocation de m�moire
        compos_connexes[i]=new bool[(int)m_vertices.size()];
    for(auto &elem: m_vertices) ///Initialisation des tableaux
    {
        elem.second.m_marque=false;
    }
    for(auto &elem: m_vertices) ///Pour chaque sommet du graphe
    {
        if(!elem.second.m_marque) ///Si il n'a pas �t� trait�
        {
            elem.second.m_marque=true; ///On le marque
            compos_connexes[elem.first]=recherche_cmp(elem.first); ///On cherche si le sommet fait partie d'une composante fortement connexe
            for(auto &elem2: m_vertices) ///Pour chaque sommet j du graphe
            {
                if(compos_connexes[elem.first][elem2.first] && !elem2.second.m_marque) ///Si i et j font partie d'une composante fortement connexe et que j n'a pas �t� trait�
                {
                    m_vertices[elem2.first].m_marque=true; ///On le marque
                }
            }
        }
    }
    for(auto &elem:m_vertices)
        elem.second.m_marque=false;
    for(auto &elem:m_vertices) ///Affichage du tableau de bool�ens pour les composantes fortement connexes
    {
        presence_cfc=false;
        for(auto &elem2: m_vertices)
        {
            if(compos_connexes[elem.first][elem2.first] && !elem2.second.m_marque) ///On affiche 1 si la valeur est true, 0 sinon
            {
                elem2.second.m_marque=true;
                list_sommets.push_back(elem2.first); ///On ajoute j parmi les sommets de la composante fortement connexe potentiellement d�tect�e
                presence_cfc=true;
            }
        }
        if(presence_cfc) ///Si une composante fortement connexe (cfc) est d�tect�e, on augmente le nb de cfc de 1.
        {
            composantes_connexes.emplace(nb_compos, list_sommets); ///On ajoute une composante fortement connexe avec son num�ro et les indices des sommets qui s'y trouvent
            nb_compos++;
        }
        list_sommets.clear();

    }
    for(auto &elem: composantes_connexes)
    {
        for(unsigned int i=0; i<elem.second.size(); i++)
        {
            m_vertices[elem.second[i]].m_numero_compo_connexe=elem.first;
        }
    }
    std::cout << std::endl << "Il y a " << nb_compos-1 << " composantes fortement connexes dans ce graphe." << std::endl; ///Affichage du nombre de composantes fortement connexes
    for(auto &elem: composantes_connexes)
    {
        std::cout << std::endl << "Composante numero " << elem.first <<", constituee des sommets d'indice ";
        for(unsigned int j=0; j<elem.second.size(); j++)
            std::cout << elem.second[j] << " ";
        std::cout << std::endl;
    }

    std::ofstream fichier("Compo_connexe"+std::to_string(m_numGraphe)+".txt", std::ios::out); ///Ouverture d'un fichier pour le graphe r�duit
    if(fichier) ///S'il est ouvert
    {
        fichier << composantes_connexes.size() << std::endl ; ///Ordre du graphe r�duit, qui est soit inf�rieur, soit �gal � l'ordre du graphe
        for(auto &elem: composantes_connexes) ///Sauvegarde des sommets
        {
            fichier << elem.first-1 << std::endl;
            fichier << 5 << std::endl;
            fichier << m_vertices[elem.second.front()].m_interface->m_top_box.get_posx()+2 << std::endl; ///En cas de sommet g�n�ral,
            fichier << m_vertices[elem.second.front()].m_interface->m_top_box.get_posy()+2 << std::endl;
            if(elem.second.size()>1)
                fichier << "sommet_general.jpg" << std::endl; ///Les sommets g�n�raux auront une image neutre
            if(elem.second.size()==1)
                fichier << m_vertices[elem.second.front()].m_interface->m_img.getName() << std::endl;
            ///Les sommets �tant d'eux-m�mes une composante fortement connexe gardent leur image
        }
        if(composantes_connexes.size()==m_vertices.size()) ///Si tous les sommets forment eux-memes des composantes
        {
            ///fortement connexes, on reprend les arcs du graphe sans les modifier
            fichier << m_nbArete << std::endl;
            for(auto &elem :m_edges)
            {
                fichier << elem.first << std::endl;
                fichier << elem.second.m_to << std::endl;
                fichier << elem.second.m_from << std::endl;
                fichier << elem.second.m_weight << std::endl;
            }
        }
        if(composantes_connexes.size()<m_vertices.size())///Si il y a des sommets g�n�raux
        {
            for(auto &elem: m_edges) ///On calcule le nombre d'aretes en question
                ///Les ar�tes en pleine composante fortement connexe sont ignor�es
            {
                if(m_vertices[elem.second.m_from].m_numero_compo_connexe!=m_vertices[elem.second.m_to].m_numero_compo_connexe)
                    nb_arete++;
            }
            fichier << nb_arete << std::endl;
//            for(int j=0; j<nb_arete; j++) ///PARTIE BOURBIER PAS COMPLETE AVEC LES SOUCIS D'ARETES
//            {
//                for(auto &elem: m_edges)
//                {
//                    if(m_vertices[elem.second.m_from].m_numero_compo_connexe!=m_vertices[elem.second.m_to].m_numero_compo_connexe && !elem.second.m_marque)
//                    {
//                            elem.second.m_marque=true;
//
//                    }
//                }
//                fichier << j << std::endl;
//                /// fichier << successeur
//                /// fichier << predecesseur
//                fichier << 5 << std::endl;
//            }
        }
        fichier.close();
    }
    for(auto &elem: composantes_connexes)
    {
        for(int i=0; i<(int)elem.second.size(); i++)
            m_vertices[elem.second[i]].m_interface->m_top_box.set_bg_color(couleurs[elem.first]);
    }
    return composantes_connexes;
}

void Graph::croissance_sommets(clock_t temps)
{
    ///Toutes les 5 secondes
    if (((temps/CLOCKS_PER_SEC)%1==0)&&(temps/CLOCKS_PER_SEC!=0))
        ///if (key[KEY_7])
    {
        ///Pour chaque sommet
        for (auto &elt : m_vertices)
        {
            ///On cr�e deux tableaux de vecteurs
            std::vector<Edge> arretes_arrivantes;
            std::vector<Edge> arretes_partantes;
            elt.second.coeff_croissance=0.0001;
            ///Pour chaque arr�te
            for(auto &elm : m_edges)
            {

                if(elt.first==elm.second.m_to)
                {
                    arretes_arrivantes.push_back(elm.second);
                }
                if(elt.first==elm.second.m_from)
                {
                    arretes_partantes.push_back(elm.second);
                }
                ///Si l'arr�te va vers le sommet
                elt.second.k_capacite=0;
                /** if (elm.second.m_to==elt.first)
                 {
                     ///On l'ajoute au vecteur
                     arretes_arrivantes.push_back(elm.second);
                 }
                 ///Si l'arr�te part du sommet
                 if (elm.second.m_from==elt.first)
                 {
                     ///On l'ajoute au sommet
                     arretes_partantes.push_back(elm.second);
                 }**/
            }
            ///On cr�e un entier qu'on initialise � 0
            /**
            for (auto &elm : bidon->m_in)
            {
                Vertex* new_sommet = new Vertex;
                Edge* new_arrete = new Edge;
                *new_arrete=m_edges[elm];
                *new_sommet=new_arrete->m_from;
                voisins[new_sommet]=*new_arrete;
                delete new_arrete;
                delete new_sommet;
            =======
            ///on donne les valeurs de m_from et m_to
            m_edges[idx].setFrom(id_vert1);
            m_edges[idx].setTo(id_vert2);
            m_vertices[id_vert1].m_out.push_back(idx);
            m_vertices[id_vert2].m_in.push_back(idx);
            }
            ///Dynamique de populations du graphe
            void Graph::croissance_sommets(clock_t temps)
            {
            ///Toutes les 5 secondes
            /// if (((temps/CLOCKS_PER_SEC)%1==0)&&(temps/CLOCKS_PER_SEC!=0))
            if (key[KEY_7])
            {
            ///Pour chaque sommet
            for (auto &elt : m_vertices)
            {
            ///On crée deux tableaux de vecteurs
            std::vector<Edge> arretes_arrivantes;
            std::vector<Edge> arretes_partantes;
            ///Pour chaque arrête
            for(auto &elm : m_edges)
            {
                if(elt.first==elm.second.m_to)
                {
                    arretes_arrivantes.push_back(elm.second);
                }
                if(elt.first==elm.second.m_from)
                {
                    arretes_partantes.push_back(elm.second);
                }
                ///Si l'arrête va vers le sommet
                elt.second.k_capacite=0;
                if (elm.second.m_to==elt.first)
                 {
                     ///On l'ajoute au vecteur
                     arretes_arrivantes.push_back(elm.second);
                 }
                 ///Si l'arrête part du sommet
                 if (elm.second.m_from==elt.first)
                 {
                     ///On l'ajoute au sommet
                     arretes_partantes.push_back(elm.second);
                 }**/
            ///On crée un entier qu'on initialise �  0
            /**
            for (auto &elm : bidon->m_in)
            {
                Vertex* new_sommet = new Vertex;
                Edge* new_arrete = new Edge;
                *new_arrete=m_edges[elm];
                *new_sommet=new_arrete->m_from;
                voisins[new_sommet]=*new_arrete;
                delete new_arrete;
                delete new_sommet;
            >>>>>>> master
            }
            for (auto &elm : bidon->m_out)
            {
                Vertex* new_som0met = new Vertex;
                Edge* new_arrete = new Edge;
                *new_arrete=m_edges[elm];
                *new_sommet=new_arrete->m_to;
                voisins[*new_sommet]=*new_arrete;
                delete new_arrete;
                delete new_sommet;
            }
            delete bidon;
            for (auto &elm : voisins)
            {
                *kkk+=(elm.first.m_value)*(elm.second.m_weight);
            }
            **/
            ///Pour chaque arr�te arrivantes
            unsigned int nombre_arrivantes=0;
            for(auto &lol : arretes_arrivantes)
            {
                ///Si la pop du sommet n'est pas �gale � 0
                if(m_vertices[lol.m_from].m_value!=0)
                {
                    nombre_arrivantes++; ///On a une population qui influe
                }
            }
            std::cout<<"Sommet : "<<elt.first<<std::endl;
            elt.second.k_capacite=calculK(arretes_arrivantes);
            double coeff_out=0;
            unsigned int nombre_partantes=0;
            for(auto &elm : arretes_partantes)
            {
                ///Si la pop du sommet
                if (m_vertices[elm.m_to].m_value!=0)
                {
                    nombre_partantes++;
                }
                coeff_out+=(m_vertices[elm.m_to].m_value)*(m_vertices[elm.m_to].k_capacite);
            }
            /// bool arrivOK = false;
            /// bool partOK=false;
            /**
            if(nombre_arrivantes!=0)
            {
              arrivOK=true;
                std::cout<<"A VRAI"<<std::endl;
            }
            else if(nombre_arrivantes==0)
            {
                arrivOK=false;
                std::cout<<"A FAUX"<<std::endl;
            }
            if(nombre_partantes!=0)
            {
                partOK=false;
                std::cout<<"P FAUX"<<std::endl;
            }
            else if(nombre_partantes==0)
            {
                partOK=true;
                std::cout<<"P VRAI"<<std::endl;
            }
            ///Proies et pr�dateurs pr�sents = population varie selon proies et pr�dateurs
            if((elt.second.m_value>1)&&(elt.second.m_value<100)&&(!arrivOK)&&(nombre_partantes!=0))
            {
                std::cout<<"LES DEUX"<<std::endl;
                if(elt.second.k_capacite!=0)
                {
                    elt.second.k_capacite=50;
                }
                elt.second.m_value+=(elt.second.coeff_croissance)*(elt.second.m_value)*(1-((elt.second.m_value)/(elt.second.k_capacite)));
                for (auto &elm : arretes_partantes)
                {
                    ///On soustrait le produit de son poids avec le sommet relié au coeff k
                    elt.second.m_value-=0.000015*(elm.m_weight)*(m_vertices[elm.m_to].m_value);
                }
            }
            ///Proies et pas de pr�dateurs = population augmente beaucoup
            else if((elt.second.m_value>1)&&(elt.second.m_value<100)&&(nombre_arrivantes=0)&&(nombre_partantes!=0))
            {
                std::cout<<"PROIES"<<std::endl;
                if(elt.second.k_capacite==0)
                {
                    elt.second.k_capacite=100000000;
                }
                elt.second.coeff_croissance*=10;
                elt.second.m_value+=(elt.second.coeff_croissance)*(elt.second.m_value)*(1-((elt.second.m_value)/(elt.second.k_capacite)));
            }
            ///Pas de proies et pr�dateurs = population diminue beaucoup
            else if((elt.second.m_value>1)&&(elt.second.m_value<100)&&(nombre_arrivantes!=0)&&(nombre_partantes=0))
            {
                std::cout<<"PREDA"<<std::endl;
                elt.second.k_capacite=0.00001;
                elt.second.coeff_croissance*=10;
                elt.second.m_value+=(elt.second.coeff_croissance)*(elt.second.m_value)*(1-((elt.second.m_value)/(elt.second.k_capacite)));
                for (auto &elm : arretes_partantes)
                {
                    ///On soustrait le produit de son poids avec le sommet relié au coeff k
                    elt.second.m_value-=0.000015*(elm.m_weight)*(m_vertices[elm.m_to].m_value);
                }
            }
            ///Pas de proies et pas de pr�dateurs = population diminue car ne peut pas se reproduire, s'�teint progressivement
            else if((elt.second.m_value>1)&&(elt.second.m_value<100)&&(nombre_arrivantes=0)&&(nombre_partantes=0))
            {
                std::cout<<"AUCUNE"<<std::endl;
                elt.second.k_capacite=0.001;
                elt.second.coeff_croissance/=10;
                elt.second.m_value+=(elt.second.coeff_croissance)*(elt.second.m_value)*(1-((elt.second.m_value)/(elt.second.k_capacite)));
            }
            std::cout<<"Valeur : "<<elt.second.m_value<<std::endl;
            **/
            std::cout<<"k vaut "<<elt.second.k_capacite<<std::endl;
            ///Equation de dynamique de population si on a assez de population et qu'on a des proies et des pr�dateurs
            if((elt.second.m_value>1)&&(elt.second.m_value<=100)&&(nombre_arrivantes!=0)&&(nombre_partantes!=0))
            {
                std::cout<<"Valeur pop AVANT : "<<elt.second.m_value<<std::endl;
                if (elt.second.k_capacite==0)
                {
                    elt.second.k_capacite=elt.second.m_value/3;
                }
                elt.second.m_value+=(elt.second.coeff_croissance)*(elt.second.m_value)*(1-((elt.second.m_value)/(elt.second.k_capacite)));
                std::cout<<"Valeur pop APRES1 : "<<elt.second.m_value<<std::endl;
                for (auto &elm : arretes_partantes)
                {
                    ///On soustrait le produit de son poids avec le sommet relié au coeff k
                    elt.second.m_value-=0.000015*(elm.m_weight)*(m_vertices[elm.m_to].m_value);
                }
                std::cout<<"Valeur pop APRES2: "<<elt.second.m_value<<std::endl;
            }
            ///Pas de proies et pas de pr�dateurs : pop diminue
            else if((elt.second.m_value>1)&&(elt.second.m_value<=100)&&(nombre_arrivantes==0)&&(nombre_partantes==0))
            {
                ///k petit pour que diminue
                elt.second.k_capacite=0.01;
                ///diminue l�g�rement
                elt.second.coeff_croissance/=150;
                std::cout<<"Valeur pop AVANT: "<<elt.second.m_value<<std::endl;
                elt.second.m_value+=(elt.second.coeff_croissance)*(elt.second.m_value)*(1-((elt.second.m_value)/(elt.second.k_capacite)));
                std::cout<<"Valeur pop APRES1: "<<elt.second.m_value<<std::endl;
            }
            ///Proie et pas de pr�dateur : augmente beaucoup
            else if((elt.second.m_value>1)&&(elt.second.m_value<=100)&&(nombre_arrivantes!=0)&&(nombre_partantes==0))
            {
                if(elt.second.k_capacite==0)
                {
                    elt.second.k_capacite=elt.second.m_value/1000;
                }
                elt.second.coeff_croissance*=100;
                std::cout<<"Valeur pop AVANT: "<<elt.second.m_value<<std::endl;
                elt.second.m_value+=(elt.second.coeff_croissance)*(elt.second.m_value)*(1-((elt.second.m_value)/(elt.second.k_capacite)));
                std::cout<<"Valeur pop APRES1: "<<elt.second.m_value<<std::endl;
            }
            ///Pas de proie et pr�dateur
            else if((elt.second.m_value>1)&&(elt.second.m_value<=100)&&(nombre_arrivantes==0)&&(nombre_partantes!=0))
            {
                std::cout<<"Valeur pop AVANT: "<<elt.second.m_value<<std::endl;
                for (auto &elm : arretes_partantes)
                {
                    ///On soustrait le produit de son poids avec le sommet relié au coeff k
                    elt.second.m_value-=0.000015*(elm.m_weight)*(m_vertices[elm.m_to].m_value);
                }
                std::cout<<"Valeur pop APRES1: "<<elt.second.m_value<<std::endl;
            }
            ///Si valeur superieure a 100
            if (elt.second.m_value>100)
            {
                ///On ramene a 100
                elt.second.m_value=100;
            }
            ///Si �a vaut 100
            if((elt.second.m_value==100)&&(elt.second.k_capacite!=0))
            {
                elt.second.coeff_croissance=-0.0003;
                elt.second.m_value+=(elt.second.coeff_croissance)*(elt.second.m_value)*(1-((elt.second.m_value)/(elt.second.k_capacite)));
                for (auto &elm : arretes_partantes)
                {
                    ///On soustrait le produit de son poids avec le sommet relié au coeff k
                    elt.second.m_value-=(coeff_out)*(elm.m_weight/100)*(m_vertices[elm.m_to].m_value);
                }
            }
            ///Si l'esp�ce ne peut plus se reproduire
            if(elt.second.m_value<=1)
            {
                elt.second.m_value=0;
            }
            ///Pas de pop n�gative
            if(elt.second.m_value<0)
            {
                elt.second.m_value=0;
                ///Croix rouge sur le sommet
            }
            elt.second.k_capacite=0;
        }
    }
}

double Graph::calculK(std::vector<Edge> ar_arriv)
{
    double *k = new double;
    *k=0;
    for(auto &elm : ar_arriv)
    {
        *k+=(elm.m_weight)*(m_vertices[elm.m_from].m_value);
    }
    return *k;
    delete k;
}

void Graph::initia()
{
    for(auto &elm : m_vertices)
    {
        elm.second.coeff_croissance=0;
    }
}

void Graph::random()
{
    for (auto &elm : m_vertices)
    {
        elm.second.m_value=rand()%100+1;
    }
    for (auto &elm : m_edges)
    {
        elm.second.m_weight=rand()%100+1;
    }
}

void Graph::croix_rouge()
{
    for(auto &elm:m_vertices)
    {
        if(elm.second.m_value==0)
        {
            elm.second.m_interface->m_img2.set_pic_name("croix_rouge.bmp");
        }
        else if(elm.second.m_value!=0)
        {
            elm.second.m_interface->m_img2.set_pic_name("");
        }
    }
}
bool* Graph::recherche_cmp(int i)
{
    bool* marquage = new bool[(int)m_vertices.size()]; ///Tableau de bool�ens qui dit si un sommet a �t� visit� ou non
    bool* pred, *succ; ///Tableau de bool�ens pour dire si chaque sommet admet un pr�decesseur (pred) ou un successeur (succ)
    bool* cmp = new bool[(int)m_vertices.size()];; ///Tableau de bool�ens pour dire si un sommet fait partie ou non de la composante fortement connexe retourn�e
    bool suivant=true; ///Bool�en pour dire si il reste des sommets � visiter
    pred=new bool[(int)m_vertices.size()]; ///Allocation de m�moire pour les 2 tableaux de predecesseurs et de successeurs
    succ=new bool[(int)m_vertices.size()];
    for(int j=0; j<(int)m_vertices.size(); j++) ///Initialisation des 3 tableaux
    {
        marquage[j]=false;
        pred[j]=false;
        succ[j]=false;
        cmp[j]=false;
    }
    pred[i]=true; ///On fait en sorte que notre sommet de d�part admette un pr�decesseur et un successeur
    succ[i]=true;
    while(suivant) ///Tant que tous les sommets n'ont pas �t� visit�s
    {
        suivant=false;
        for(int j=0; j<(int)m_vertices.size(); j++) ///Pour tout sommet j du graphe
        {
            if(!marquage[j] && pred[j]) ///Si il n'a pas �t� marqu� et qu'il admet un pr�decesseur
            {
                marquage[j]=true; ///Le sommet j a �t� visit�
                for(int k=0; k<(int)m_vertices.size(); k++) ///Pout tout sommet k du graphe
                {
                    for(auto &elem: m_edges) ///Parmi les aretes dispos
                    {
                        if(k==elem.second.getTo() && j==elem.second.getFrom()) ///Si k est un successeur de j
                        {
                            pred[k]=true;///On dit que k a un pr�decesseur (j)
                            suivant=true; ///Et on fait continuer la boucle
                        }
                    }
                }
            }
        }
    }
    for(int j=0; j<(int)m_vertices.size(); j++)
        marquage[j]=false;
    suivant=true;
    while(suivant) ///Tant que tous les sommets n'ont pas �t� visit�s
    {
        suivant=false;
        for(int j=0; j<(int)m_vertices.size(); j++) ///Pour tout sommet j du graphe
        {
            if(!marquage[j] && succ[j]) ///Si il n'a pas �t� marqu� et qu'il admet un successeur
            {
                marquage[j]=true; ///Le sommet j a �t� visit�
                for(int k=0; k<(int)m_vertices.size(); k++) ///Pout tout sommet k du graphe
                {
                    for(auto &elem: m_edges) ///Parmi les aretes dispos
                    {
                        if(k==elem.second.getFrom() && j==elem.second.getTo()) ///Si k est un pr�d�cesseur de j
                        {
                            succ[k]=true;///On dit que k a un successeur (j)
                            suivant=true; ///Et on fait continuer la boucle
                        }
                    }
                }
            }
        }
    }
    for(int j=0; j<(int)m_vertices.size(); j++)
    {
        cmp[j]=pred[j]&succ[j]; ///On met en place la composante fortement connexe.
    }
    return cmp;
}

/// Aide � l'ajout de sommets interfac�s
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Cr�ation d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
}

///ss prog qui initialise les bouton
void Graph::initButton()
{
    for(auto & elt: m_vertices)
    {
        elt.second.m_interface->m_button_addEdge.set_value(false);
    }
}

/// Aide � l'ajout d'arcs interfac�s
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
{
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }
    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }
    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2],id_vert1,id_vert2);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei);
    ///on donne les valeurs de m_from et m_to
    m_edges[idx].setFrom(id_vert1);
    m_edges[idx].setTo(id_vert2);
    m_vertices[id_vert1].m_out.push_back(idx);
    m_vertices[id_vert2].m_in.push_back(idx);
}
>>>>>>> GRAPHISME
