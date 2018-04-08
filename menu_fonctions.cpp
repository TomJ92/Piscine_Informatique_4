#include "grman/grman.h"
#include <iostream>
#define LARGEURECRAN 2056
#define HAUTEURECRAN 1156
#include "graph.h"
#include "MENU_FONCTIONS.h"
void regles()
{
    BITMAP* regles=create_bitmap(LARGEURECRAN,HAUTEURECRAN);
    regles=load_bitmap("menu/regles.bmp",0);
    blit(regles,screen,0,0,0,0,LARGEURECRAN,HAUTEURECRAN);
    readkey();
}
void afficher()
{
    ///On initialise allegro
    grman::init();
    grman::set_pictures_path("pics");
    ///On déclare les bitmaps
    BITMAP* menu_p,*menu_graphe1,*menu_graphe2,*menu_graphe3,*menu_explications,*menu_quitter,*buffer=create_bitmap(HAUTEURECRAN,LARGEURECRAN);
    Graph g;
    menu_p=load_bitmap("menu/MENU_P.bmp",0);
    menu_graphe1=load_bitmap("menu/MENu-Graphe1.bmp",0);
    menu_graphe2=load_bitmap("menu/MENU-Graphe2.bmp",0);
    menu_graphe3=load_bitmap("menu/MENU-Graphe3.bmp",0);
    menu_explications=load_bitmap("menu/MENU-Explications.bmp",0);
    menu_quitter=load_bitmap("menu/MENU-Quitter.bmp",0);
  //  int i=0;
    //bool m;
    ///Déclaration des variables
    bool quitter=false;
    unsigned int choix=0;
    ///Tant qu'on ne quitte pas le jeu
    while(!quitter)
    {
        ///Si la souris se trouve sur le mot "Graphe 1"
        if((mouse_x<145)&& (mouse_y>362) && (mouse_y<403))
        {
            ///On copie l'image du menu
            blit(menu_graphe1,buffer,0,0,0,0,HAUTEURECRAN,LARGEURECRAN);
            ///Si on clique gauche avec la souris
            if(mouse_b&1)
            {
                ///On sélectionne le premier choix
                choix=1;
            }
        }
        ///Si la souris est sur "Graphe 2"
        else if((mouse_x<153)&& (mouse_y>403) && (mouse_y<449))
        {
            ///On copie l'image correpondante
            blit(menu_graphe2,buffer,0,0,0,0,HAUTEURECRAN,LARGEURECRAN);
            ///Si on clique
            if(mouse_b&1)
            {
                ///On sélectionne le deuxième choix
                choix=2;
            }
        }
        ///Si la souris est sur "Graphe 3"
        else if((mouse_x<149)&& (mouse_y>449) && (mouse_y<489))
        {
            ///On copie l'image correspondante
            blit(menu_graphe3,buffer,0,0,0,0,HAUTEURECRAN,LARGEURECRAN);
            ///Si on clique avec la souris
            if(mouse_b&1)
            {
                ///On sélectionne le troisième choix
                choix=3;
            }
        }
        ///Si on sélectionne Explications
        else if((mouse_x<187)&& (mouse_y>489) && (mouse_y<533))
        {
            blit(menu_explications,buffer,0,0,0,0,HAUTEURECRAN,LARGEURECRAN);
            if(mouse_b&1)
            {
                choix=4;
            }
        }
        else if((mouse_x<110)&& (mouse_y>533))
        {
            blit(menu_quitter,buffer,0,0,0,0,HAUTEURECRAN,LARGEURECRAN);
            if(mouse_b&1)
            {
                quitter=true;
            }
        }
        else
        {
            blit(menu_p,buffer,0,0,0,0,HAUTEURECRAN,LARGEURECRAN);
        }
        blit(buffer,screen,0,0,0,0,HAUTEURECRAN,LARGEURECRAN);
        clear_bitmap(buffer);
        switch(choix)
        {
        case 0:
            break;
        case 1 :
            {
            clock_t temps_ini;
            unsigned int compteur=0;
            temps_ini=(double)clock();
            bool animation=false;
            g.ReadFile("Graphe1",1);
            /// Vous gardez la main sur la "boucle de jeu"
            /// ( contrairement à des frameworks plus avancés )
            while ( !g.getQuitGraph() )
            {
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
                if(key[KEY_S])
                {
                    g.algo_forte_connexite();
                    rest(300);
                }
                else if(key[KEY_O])
                {
                    g.algo_forte_connexite();
                    rest(200);
                }
            }
            g.initialisation();
            rest(300);
            break;
            }
        case 2:
            {
            clock_t temps_ini;
            unsigned int compteur=0;
            temps_ini=(double)clock();
            bool animation=false;
            g.ReadFile("Graphe2",2);
            /// Vous gardez la main sur la "boucle de jeu"
            /// ( contrairement à des frameworks plus avancés )
            while ( !g.getQuitGraph() )
            {
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
                if(key[KEY_S])
                {
                    g.init_k_connex();
                    rest(300);
                    std::cout<<"good"<<std::endl;
                }
                else if(key[KEY_O])
                {
                    g.algo_forte_connexite();
                    rest(200);
                }
            }
            g.initialisation();
            rest(300);
            break;
            }
        case 3:
            {
            clock_t temps_ini;
            unsigned int compteur=0;
            temps_ini=(double)clock();
            bool animation=false;
            g.ReadFile("Graphe3",3);
            /// Vous gardez la main sur la "boucle de jeu"
            /// ( contrairement à des frameworks plus avancés )
            while ( !g.getQuitGraph() )
            {
                /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
                g.update(temps_ini,animation);
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
                if(key[KEY_S])
                {
                    g.init_k_connex();
                    rest(300);
                }
                else if(key[KEY_O])
                {
                    g.algo_forte_connexite();
                    rest(200);
                }
                /// Mise à jour générale (clavier/souris/buffer etc...)
                grman::mettre_a_jour();
            }
            g.initialisation();
            rest(300);
            break;
            }
        ///Si on clique sur "Explications"
        case 4:
            regles();
            break;
        default :
            quitter=true;
            break;
        }
        choix=0;
    }
    grman::fermer_allegro();
}
