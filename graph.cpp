#include "graph.h"

/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
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

    m_top_box.add_child(m_cross);
    m_cross.set_bg_color(ROUGE);
    m_cross.set_dim(10,10);
    m_cross.set_pos(115,0);
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
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de r�glage associ�s
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de r�glage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 100.0);  // Valeurs arbitraires, � adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

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

    m_top_box.add_child(m_button_save);
    m_button_save.set_dim(50,35);
    m_button_save.set_pos(15,15);
    m_button_save.set_bg_color(BLEUCLAIR);

    m_button_save.add_child(m_text_save);
    m_text_save.set_pos(2,13);
    m_text_save.set_message("Sauver");

    m_top_box.add_child(m_button_reset);
    m_button_reset.set_dim(50,35);
    m_button_reset.set_pos(15,65);
    m_button_reset.set_bg_color(ROUGE);

    m_button_reset.add_child(m_text_reset);
    m_text_reset.set_pos(6,13);
    m_text_reset.set_message("Reset");

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

void Graph::reinit(std::string fileName)
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    std::string nom;
    int xdep,ydep,numS;
    double valeur;
    std::string pic_name;
    std::ifstream fichier(fileName,std::ios::in);
    for(auto it = m_vertices.begin(); it != m_vertices.end();)
    {
        it=m_vertices.erase(it);
    }
    for(auto it=m_edges.begin(); it!=m_edges.end();)
    {
        it=m_edges.erase(it);
    }
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
        fichier>>m_nbArete;
        getline(fichier,nom);
        for(int i=0; i<m_nbArete; i++)
        {
            ///on r�cup�re le num�ro du sommet
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
            ///on ajoute une nouvelle ar�te
            add_interfaced_edge(numS,xdep,ydep,valeur);

        }
        std::cout<<"RESET"<<std::endl;
        fichier.close();
    }
    else
    {
        throw std::string("Il n'exite pas de fichier portant ce nom");
    }
    setQuitGraph(false);
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
        ///en fonction de nb d'ar�te
        for(int i=0; i<m_nbArete; i++)
        {
            ///on r�cup�re le num�ro du sommet
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
    setQuitGraph(false);
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
    switch(m_numGraphe)
    {
    case 1:
        name="Mer";
        break;
    case 2:
        name="Dragon";
        break;
    case 3:
        name="Poke";
        break;
    }
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
            number=std::to_string(el.first+1);
            fichier<<name+number+".bmp"<<std::endl;
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
    }
    ///sinon on envoie une exception
    else
    {
        throw std::string("Il n'exite pas de fichier portant ce nom");
    }
}

/// La m�thode update � appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
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
        if(elt.second.m_interface->m_cross.get_value())
        {
            removeVertex(elt.first);
        }
    }
    for (auto &elt : m_edges)
        elt.second.post_update();
    if(m_interface->m_button_save.get_value())
    {
        switch(m_numGraphe)
        {
        case 1:
            try
            {
                saveFile("Graphe1");
                m_interface->m_button_save.set_value(false);
            }
            catch(const std::string & e)
            {
                std::cout << e << "\n\n";
            }
            break;
        case 2:
            try
            {
                saveFile("Graphe2");
                m_interface->m_button_save.set_value(false);
            }
            catch(const std::string & e)
            {
                std::cout << e << "\n\n";
            }
            break;
        case 3:
            try
            {
                saveFile("Graphe3");
                m_interface->m_button_save.set_value(false);
            }
            catch(const std::string & e)
            {
                std::cout << e << "\n\n";
            }
            break;

        }
    }
    if(m_interface->m_button_reset.get_value())
    {

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
    if(m_interface->m_button_quit.get_value())
    {
        setQuitGraph(true);
    }

}

void Graph::removeVertex(int num)
{
    while(m_vertices[num].m_in.size()<0)
    {
        removeEdge(m_vertices[num].m_in[0]);
    }
    while(m_vertices[num].m_out.size()<0)
    {
        removeEdge(m_vertices[num].m_out[0]);
    }
    m_interface->m_main_box.remove_child( m_vertices[num].m_interface->m_top_box );
    m_vertices.erase(num);
}

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
void Graph::addVertex()
{
    int indice;
    std::string picName= "clown1.jpg";
    for(auto it=m_vertices.begin(); it!=m_vertices.end();it++)
    {
        indice= it->first;
    }
    add_interfaced_vertex(indice+1,50,300,300,picName);

}
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
    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei);
    ///on donne les valeurs de m_from et m_to
    m_edges[idx].setFrom(id_vert1);
    m_edges[idx].setTo(id_vert2);
    m_vertices[id_vert1].m_out.push_back(idx);
    m_vertices[id_vert2].m_in.push_back(idx);


}
