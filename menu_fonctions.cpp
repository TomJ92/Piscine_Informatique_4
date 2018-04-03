#include "grman/grman.h"
#include <iostream>
#define LARGEURECRAN 2056
#define HAUTEURECRAN 1156
#include "MENU_FONCTIONS.h"
void regles()
{
    BITMAP* regles=create_bitmap(LARGEURECRAN,HAUTEURECRAN);
    regles=load_bitmap("regles.bmp",0);
    blit(regles,screen,0,0,0,0,LARGEURECRAN,HAUTEURECRAN);
    readkey();
}
void afficher()
{
    ///On initialise allegro
    grman::init();
    ///On déclare les bitmaps
    BITMAP* menu_p,*menu_graphe1,*menu_graphe2,*menu_graphe3,*menu_explications,*menu_quitter,*buffer=create_bitmap(HAUTEURECRAN,LARGEURECRAN);
    menu_p=load_bitmap("MENU_P.bmp",0);
    menu_graphe1=load_bitmap("MENu-Graphe1.bmp",0);
    menu_graphe2=load_bitmap("MENU-Graphe2.bmp",0);
    menu_graphe3=load_bitmap("MENU-Graphe3.bmp",0);
    menu_explications=load_bitmap("MENU-Explications.bmp",0);
    menu_quitter=load_bitmap("MENU-Quitter.bmp",0);
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
        case 0 :
            break;
        case 1 :
            break;
        case 2:
            break;
        case 3:
            break;
        ///Si on clique sur "Explications"
        case 4:
            regles();
            choix=0;
            break;
        default :
            quitter=true;
            break;
        }
    }
    grman::fermer_allegro();
}
