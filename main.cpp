#include "grman/grman.h"
#include <iostream>
#include "menu_fonctions.cpp"
#include "graph.h"
#include "CROISSANCE_POPULATION.h"
#include <time.h>
#include <ctime>

int main()
{
<<<<<<< HEAD
        /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();
    bool animation=false;
=======
    /*
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

>>>>>>> master
    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    /// Un exemple de graphe
    Graph g;
<<<<<<< HEAD
    g.make_example();
    clock_t temps_ini;
    temps_ini=(double)clock();
    unsigned int compteur=0;
=======
    g.ReadFile("Graphe1");
    ///g.make_example();


>>>>>>> master
    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
    while ( !key[KEY_ESC] )
    {
<<<<<<< HEAD

        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
           g.update(temps_ini,animation);
        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
        if(key[KEY_1_PAD])
        {
            animation=true;
        }
        if(key[KEY_2_PAD])
        {
            animation=false;
        }
        compteur++;
        if (compteur%30==0)
            temps_ini=temps_ini+1000;
    }
    al_
    grman::fermer_allegro();
=======
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

    grman::fermer_allegro();*/
>>>>>>> master
    afficher();
    return 0;
}
END_OF_MAIN();
