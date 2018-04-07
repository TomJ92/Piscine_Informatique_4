#include "graph.h"

/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
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

    ///bouton  pour quitter le programme
    m_top_box.add_child(m_button_quit);
    m_button_quit.set_dim(60,35);
    m_button_quit.set_pos(11,520);
    m_button_quit.set_bg_color(ORANGE);

    m_button_quit.add_child(m_text_quit);
    m_text_quit.set_pos(2,13);
    m_text_quit.set_message("Quitter");

    m_tool_box.add_child(m_button_noAddEdge);
    m_button_noAddEdge.set_dim(7,7);
    m_button_noAddEdge.set_pos(68,175);
    m_button_noAddEdge.set_bg_color(ROUGE);

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

/// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update(clock_t ini, bool animation)
{
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
        ///on appuie sur la croix pour supprimer
        if(elt.second.m_interface->m_cross.get_value())
        {
            ///on enléve le sommet
            removeVertex(elt.first);
        }
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
                }
            }
        }
    }
    for (auto &elt : m_edges)
    {
        elt.second.post_update();
        ///si on clique sur la croix d'une arête
        if(elt.second.m_interface->m_cross.get_value())
        {
            ///on la supprime
            removeEdge(elt.first);
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
    }
    ///on appuie sur le bouton reset
    if(m_interface->m_button_reset.get_value())
    {
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
}

///la méthode qui ajoute un sommet
void Graph::addVertex()
{
    ///on regarde l'indice le plus grand de la map d'arete
    int indice;
    //int number1;
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
            std::cout<<"Sommet : "<<elt.first<<std::endl;
            elt.second.k_capacite=calculK(arretes_arrivantes);
            double coeff_out=0;
            if(elt.second.k_capacite==0)
            {
                    elt.second.k_capacite=-1;
            }
            coeff_out=calcul_coeff_out(arretes_partantes,elt.second.k_capacite);
            ///Equation de dynamique de population si on a assez de population
            if((elt.second.m_value>1)&&(elt.second.m_value<100)&&(elt.second.k_capacite!=0))
            {
                std::cout<<"k vaut "<<elt.second.k_capacite<<std::endl;
                std::cout<<"Valeur pop AVANT : "<<elt.second.m_value<<std::endl;
                elt.second.m_value+=(elt.second.coeff_croissance)*(elt.second.m_value)*(1-((elt.second.m_value)/(elt.second.k_capacite)));
                std::cout<<"Valeur pop APRES1 : "<<elt.second.m_value<<std::endl;
                for (auto &elm : arretes_partantes)
                {
                    ///On soustrait le produit de son poids avec le sommet relié au coeff k
                elt.second.m_value-=(coeff_out)*(elm.m_weight)*(m_vertices[elm.m_to].m_value);
                }
                std::cout<<" Coeffout : "<<coeff_out<<std::endl;
                std::cout<<"Valeur pop APRES2: "<<elt.second.m_value<<std::endl;
            }
            ///Si valeur supérieur à 100
            if (elt.second.m_value>100)
            {
                ///On ramène à 100
                elt.second.m_value=100;
            }
            ///Si ça vaut 100
            if((elt.second.m_value==100)&&elt.second.k_capacite)
            {
                elt.second.coeff_croissance=-0.0003;
                elt.second.m_value+=(elt.second.coeff_croissance)*(elt.second.m_value)*(1-((elt.second.m_value)/(elt.second.k_capacite)));
                for (auto &elm : arretes_partantes)
                {
                    ///On soustrait le produit de son poids avec le sommet relié au coeff k
                elt.second.m_value-=(coeff_out)*(elm.m_weight/100)*(m_vertices[elm.m_to].m_value);
                }
            }
            if(elt.second.m_value<=1)
            {
                elt.second.m_value=0;
            }
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
        *k+=(elm.m_weight/100)*(m_vertices[elm.m_from].m_value);
    }
    return *k;
    delete k;
}
double Graph::calcul_coeff_out(std::vector<Edge> arr_part, double k_coeff)
{
    double k = 0;
    for(auto &elm : arr_part)
    {
        k+=(m_vertices[elm.m_to].m_value)*(elm.m_weight/100)*(m_vertices[elm.m_to].k_capacite);
    }
    return k;
}
void Graph::initia()
{
    for(auto &elm : m_vertices)
    {
        elm.second.coeff_croissance=0;
    }
}

void Graph::affichage_graphe_reduit()
{
    std::map<int, std::vector<int>> graphe_reduit = algo_forte_connexite();
    Graph reduit;
    reduit.m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    reduit.m_numGraphe=m_numGraphe;
    reduit.m_ordre=(int)graphe_reduit.size();
    reduit.m_nbArete=reduit.m_ordre-1;
    for(int i=0; i<reduit.m_ordre; i++)
    {
        if(graphe_reduit.size()>1)
            reduit.add_interfaced_vertex(i, 50, (i+1)*50, (i+1)*50, "sommet_general.bmp");
        if(graphe_reduit.size()==1) reduit.add_interfaced_vertex(i, 50, (i+1)*50, (i+1)*50, reduit.m_vertices[i].m_interface->m_img.getName());
    }
    ///A CONPLETER
}

std::map<int,std::vector<int>>  Graph::algo_forte_connexite()
{
    int nb_compos=1; ///Nombre de composantes fortement connexes
    bool presence_cfc;
    std::vector<int> list_sommets;
    std::map<int,std::vector<int>> composantes_connexes;
    bool* marques=new bool[m_ordre]; ///Déclaration du tableau de booléens qui dit si un sommet a été traité ou pas
    bool** compos_connexes; ///Tableau qui détermine les composantes fortement connexes.
    compos_connexes=new bool*[m_ordre]; ///1ere allocation de mémoire
    for(int i=0; i<m_ordre; i++) ///2ème allocation de mémoire
        compos_connexes[i]=new bool[m_ordre];
    for(int i=0; i<m_ordre; i++) ///Initialisation des tableaux
    {
        marques[i]=false;
        for(int j=0; j<m_ordre; j++)
            compos_connexes[i][j]=false;
    }
    marques[0]=true;
    for(int i=1; i<m_ordre; i++) ///Pour chaque sommet i du graphe
    {
        if(!marques[i]) ///Si il n'a pas été traité
        {
            marques[i]=true; ///On le marque
            compos_connexes[i]=recherche_cmp(i); ///On cherche si le sommet fait partie d'une composante fortement connexe
            for(int j=0; j<m_ordre; j++) ///Pour chaque sommet j du graphe
            {
                if(compos_connexes[i][j] && !marques[j]) ///Si i et j font partie d'une composante fortement connexe et que j n'a pas été traité
                {
                    marques[j]=true; ///On le marque
                }
            }
        }
    }
    std::cout << std::endl;
    for(int i=0; i<m_ordre; i++)
        marques[i]=false;
    for(int i=0; i<m_ordre; i++) ///Affichage du tableau de booléens pour les composantes fortement connexes
    {
        presence_cfc=false;
        for(int j=0; j<m_ordre; j++)
        {
            if(compos_connexes[i][j] && !marques[j]) ///On affiche 1 si la valeur est true, 0 sinon
            {
                std::cout << 1 << " ";
                marques[j]=true;
                list_sommets.push_back(j); ///On ajoute j parmi les sommets de la composante fortement connexe potentiellement détectée
                presence_cfc=true;
            }
            else std::cout << 0 << " ";
        }
        std::cout << std::endl << std::endl;
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
    return composantes_connexes;
}

bool* Graph::recherche_cmp(int i)
{
   bool* marquage = new bool[m_ordre]; ///Tableau de booléens qui dit si un sommet a été visité ou non
    bool* pred, *succ; ///Tableau de booléens pour dire si chaque sommet admet un prédecesseur (pred) ou un successeur (succ)
    bool* cmp = new bool[m_ordre];; ///Tableau de booléens pour dire si un sommet fait partie ou non de la composante fortement connexe retournée
    bool suivant=true; ///Booléen pour dire si il reste des sommets à visiter
    pred=new bool[m_ordre]; ///Allocation de mémoire pour les 3 tableaux
    succ=new bool[m_ordre];
    for(int j=0; j<m_ordre; j++) ///Initialisation des 3 tableaux
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
        for(int j=0; j<m_ordre; j++) ///Pour tout sommet j du graphe
        {
            if(!marquage[j] && pred[j]) ///Si il n'a pas été marqué et qu'il admet un prédecesseur
            {
                marquage[j]=true; ///Le sommet j a été visité
                for(int k=0; k<m_ordre; k++) ///Pout tout sommet k du graphe
                {
                    for(unsigned int l=0; l<m_edges.size(); l++) ///Parmi les successeurs l de j
                    {
                        if(k==m_edges[l].getTo() && j==m_edges[l].getFrom()) ///Si k est un successeur de j et qu'il n'a pas été visité
                        {
                            pred[k]=true;///On dit que k a un prédecesseur (j)
                            suivant=true; ///Et on fait continuer la boucle
                        }
                    }

                    std::cout << std::endl;
                }
            }
        }
    }
    for(int j=0; j<m_ordre; j++)
        marquage[j]=false;
    suivant=true;
    while(suivant) ///Tant que tous les sommets n'ont pas été visités
    {
        suivant=false;
        for(int j=0; j<m_ordre; j++) ///Pour tout sommet j du graphe
        {
            if(!marquage[j] && succ[j]) ///Si il n'a pas été marqué et qu'il admet un successeur
            {
                marquage[j]=true; ///Le sommet j a été visité
                for(int k=0; k<m_ordre; k++) ///Pout tout sommet k du graphe
                {
                    for(unsigned int l=0; l<m_edges.size(); l++) ///Parmi les successeurs l de j
                    {
                        if(k==m_edges[l].getFrom() && j==m_edges[l].getTo()) ///Si k est un prédécesseur de j et qu'il n'a pas été visité
                        {
                            succ[k]=true;///On dit que k a un successeur (j)
                            suivant=true; ///Et on fait continuer la boucle
                        }
                    }

                    std::cout << std::endl;
                }
            }
        }
    }
    for(int j=0; j<m_ordre; j++)
    {
        cmp[j]=pred[j]&succ[j]; ///On met en place la composante fortement connexe.
    }
    return cmp;
}

