#include "grman/grman.h"
#include <iostream>
#include "menu_fonctions.cpp"
#include "graph.h"

int main()
{
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    /// Un exemple de graphe
    Graph g;
    g.ReadFile("Graphe1");
    ///g.make_example();


    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
    while ( !key[KEY_ESC] )
    {
        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        g.update();


        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
        if(key[KEY_S])
        {
            try
            {
                g.saveFile("Graphe1");
            }
            catch(const std::string & e)
            {
                std::cout << e << "\n\n";
            }
        }
    }

    grman::fermer_allegro();

    return 0;
    return 0;
}
END_OF_MAIN();


