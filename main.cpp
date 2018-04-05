#include "grman/grman.h"
#include <iostream>
#include "menu_fonctions.cpp"
#include "graph.h"
#include "CROISSANCE_POPULATION.h"
#include <time.h>
#include <ctime>

int main()
{
        /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    /// Un exemple de graphe
    Graph g;
    g.make_example();
    clock_t temps_ini;
    temps_ini=clock();
    unsigned int compteur=0;


    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
    while ( !key[KEY_ESC] )
    {

        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        g.update(temps_ini);

        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
        compteur++;
        if (compteur%30==0)
            temps_ini=temps_ini+1000;
        std::cout<<(unsigned int)temps_ini/CLOCKS_PER_SEC<<std::endl;
    }

    grman::fermer_allegro();
    afficher();
    return 0;
}
END_OF_MAIN();
