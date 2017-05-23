//AUTEURS :
//NAME
//FORM

//Date
//SCHOOL

//Version 1.6
//NAMES

//Appel des bibliothèques
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h> //pour la fonction aléatoire
#include <conio.h> //pour kbhit et getch
#include <math.h> //pour la valeur absolue

//Définition de l'unique structure
struct info //On définit la structure info qui permettra d'enregistrer toute les informations des éléments de la grille
{
    int type, NbJSansManger, NbJSansBoire, NbJour, NbJSConsom, Ressource;
};
typedef struct info sinfo;
//type 0 savane
//Type 1 Buisson
//Type 2 Eau
//Type 3 Animal


//Declaration des sous-programmes
void Color(int couleurDuTexte, int couleurDeFond);
int AttendreTouche(void);
int Loadgame(int * NbLigne, int * NbColonne, int * NbAnimaux, int * NbAnimauxC, int * NbBuisson, int * NbPtEau, int * DureeSimulation, int * Jour);
void Parametres(int * NbColonne, int * NbLigne, int * NbAnimaux, int * NbAnimauxC, int * NbBuisson, int * NbPtEau, int * DureeSimulation);
void Initialisation(int NbLigne, int NbColonne, int * NbAnimaux, int * NbAnimauxC, int * NbBuisson, int * NbPtEau, sinfo Grille[NbLigne][NbColonne], int * NoFichier, int * DureeSimulation, int * Jour);
void deplacement(int NbLigne, int NbColonne, sinfo Grille[NbLigne][NbColonne], int Jour);
void Affichage(int NbColonne, int NbLigne, sinfo Grille[NbLigne][NbColonne], int * AnimauxRestant, int * AnimauxCRestant, int * BuissonsRestant, int * PtEauRestant);
void Vivre(int NbLigne, int NbColonne, sinfo Grille[NbLigne][NbColonne]);
void Legende(int Jour, int AnimauxRestant, int AnimauxCRestant, int BuissonsRestant, int PtEauRestant);
int MenuPause(int NbLigne, int NbColonne, int NbAnimaux, int NbAnimauxC, int NbBuisson, int NbPtEau, int DureeSimulation, int Jour, sinfo Grille[NbLigne][NbColonne]);
void Savegame(int NbLigne, int NbColonne, int NbAnimaux, int NbAnimauxC, int NbBuisson, int NbPtEau, int DureeSimulation, int Jour, sinfo Grille[NbLigne][NbColonne]);
void Resulats(int NbAnimaux, int NbAnimauxC, int NbBuisson, int NbPtEau, int AnimauxRestant, int AnimauxCRestant, int BuissonsRestant, int PtEauRestant, int Jour, int DureeSimulation);

int i, j; //initialisation des variables communes

int main()
{
    Color(15,0);

    int Touche, NoFichier = 666, choixPause; //On associera cette variable a la touche saisie par l'utilisateur

    //DEBUT DE L'INITIALISATION DU PROGRAMME
    int NbColonne, NbLigne, NbAnimaux, NbAnimauxC, NbBuisson, NbPtEau, AnimauxCRestant, AnimauxRestant, BuissonsRestant, PtEauRestant, Jour, DureeSimulation;

    system("cls");
    printf("Bienvenue dans le programme de simulation d'environnement,\nDans ce programme vous allez definir la configuration que vous souhaitez pour la simulation.\nLes animaux vont pouvoir se deplacer aleatoirement et vont se nourrir ou boire \ns'ils sont a cote d'une ressource en eau ou en nourriture.\n\n");
    printf("Si vous souhaitez charger une simulation enregistree, appuyez sur Entree\nSinon appuyez sur une autre touche\n");
    Touche = AttendreTouche();

    if (Touche == 13)
    {
ChargerSimulation:
        ;
        NoFichier = Loadgame( & NbLigne, & NbColonne, & NbAnimaux, & NbAnimauxC, & NbBuisson, & NbPtEau, & DureeSimulation, & Jour); //Cette fonction va permettre d'initialiser la simulation à partir d'une sauvegarde
        Touche = 13;
    }
    if (Touche != 13 || NoFichier == 0) //Dans ce bloc l'utilisateur va definir les differents parametres pour la simulation
    {
debut:; //Ce saut va permettre de redefinir les parametres de la simulation, si l'utilisateur le souhaite à la fin de la simulation
        Parametres( & NbColonne, & NbLigne, & NbAnimaux, & NbAnimauxC, & NbBuisson, & NbPtEau, & DureeSimulation);
memeparametres:; //Ce saut va permettre de recommencer la simulation avec les mêmes parametres
        NoFichier = 0;
        Jour = 0;

    }


    sinfo Grille[NbLigne][NbColonne]; //on associe la structure info à un tableau que l'on utilisera pour stocker les informations de chaque emplacements de la carte
    Initialisation(NbLigne, NbColonne, & NbAnimaux, & NbAnimauxC, & NbBuisson, & NbPtEau, Grille, & NoFichier, & DureeSimulation, & Jour); //On fait un appel à la fonction Initialisation

    AnimauxRestant = NbAnimaux; //On associe le nombre d'animaux restant au nombre d'animaux voulus sinon la boucle while ne serait jamais executée
    AnimauxCRestant = NbAnimauxC; //Idem pour le nombre d'animaux carnivores

    while ((AnimauxRestant > 0 || AnimauxCRestant > 0) && DureeSimulation > Jour) //Cette boucle fait tourner le programme tant qu'il reste des animaux en vie ou tant que le nombre de jour simulé est inférieur à la duree de simulation saisie
    {
        system("cls");
        Jour++;
        AnimauxRestant = 0; //On réinitialise la variable nous informant sur le nombre d'animaux restant
        AnimauxCRestant = 0;
        BuissonsRestant = 0;
        PtEauRestant = 0;
        deplacement(NbLigne, NbColonne, Grille, Jour); //on fait appel à la fonction déplacement, qui va pouvoir permettre le déplacement aléatoire des animaux
        Affichage(NbColonne, NbLigne, Grille, & AnimauxRestant, & AnimauxCRestant, & BuissonsRestant, & PtEauRestant); //Cette fonction va pouvoir affiche la carte
        Vivre(NbLigne, NbColonne, Grille); //Cette fonction correspond à toutes les étapes liées à la vie des animaux et des autres éléments , comme le fait de se nourrir et de boire pour les animaux, la régénération des ressources...
        Legende(Jour, AnimauxRestant, AnimauxCRestant, BuissonsRestant, PtEauRestant); //Cette fonction affiche la légende de la carte ainsi que le nombre d'éléments restant

        if (kbhit()) // Si l'utilisateur presse une touche
        {
            Touche = getch();
            if (Touche == 32) //Si cette touche est la barre espace, le menu pause se lance
            {
                choixPause = MenuPause(NbLigne, NbColonne, NbAnimaux, NbAnimauxC, NbBuisson, NbPtEau, DureeSimulation, Jour, Grille);
                if (choixPause == 1)
                {
                    goto memeparametres;
                }
                if (choixPause == 2)
                {
                    goto debut;
                }
                if (choixPause == 3)
                {
                    goto ChargerSimulation;
                }
            }
        }

    }
    Resulats(NbAnimaux, NbAnimauxC, NbBuisson, NbPtEau, AnimauxRestant, AnimauxCRestant, BuissonsRestant, PtEauRestant, Jour, DureeSimulation);
    printf("Appuyez sur ENTREE pour quitter\nAppuyez sur a pour recommencer une simulation avec les memes parametres\nAppuyez sur b pour refaire une simulation avec des nouveaux parametres\nAppuyez sur c pour charger une simulation\n");
    Touche = AttendreTouche();

    while (Touche != 13 && Touche != 97 && Touche != 98 && Touche != 99) //Attendre que la touche saisie soit Entree, a ou b
    {
        system("cls");
        printf("Appuyez sur ENTREE pour quitter\nAppuyez sur a pour recommencer une simulation avec les memes parametres\nAppuyez sur b pour refaire une simulation avec des nouveaux parametres\nAppuyez sur c pour charger une simulation\n");
        Color(12, 0);
        printf("Appuyez sur Entree, a, b ou c \n"); //Message d'erreur qui s'affiche en rouge
        Color(15, 0);
        Touche = AttendreTouche();
    }
    if (Touche == 13) //Si la touche est entrée alors, le programme est terminé
    {
        exit(0);
    }
    if (Touche == 97) //Si la touche est a , la simulation est refaite avec les memes parametres
    {
        goto memeparametres;
    }
    if (Touche == 98) //Si la touche est b, l'utilisateur va pouvoir saisir de nouveaux parametres
    {
        goto debut;
    }
    if (Touche == 99) //Si la touche est b, l'utilisateur va pouvoir saisir de nouveaux parametres
    {
        goto ChargerSimulation;
    }
    return 0;
}
void Color(int couleurDuTexte, int couleurDeFond) //On définit la fonction pour donner une couleur au texte et au fond de la console
{
    HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H, couleurDeFond * 16 + couleurDuTexte);
}
int AttendreTouche() //On definie cette fonction qui permet d'attendre que l'utilisateur saisisse une touche
{
    int Touche; //On associera cette variable a la touche saisie par l'utilisateur
    while (!kbhit());
    Touche = getch();
    if (Touche == 0) //Certaines touche retourne d'abord un 0 puis une autre valeur (exemple la touche F1) qui retourne 0 puis 59, grace à cette étape la fonction retournera bien 59
    {
        Touche = getch();
    }

    return ((int) Touche); //ce return permettra de donner la valeur de la touche pressée
}
int Loadgame(int * NbLigne, int * NbColonne, int * NbAnimaux, int * NbAnimauxC, int * NbBuisson, int * NbPtEau, int * DureeSimulation, int * Jour)
{
    int Choix;
    int NoFichier;
    FILE * fichier = NULL;
    system("cls");
    printf("Quelle sauvegarde souhaitez-vous charger?\n");
    printf("Appuyez sur 1 pour charger la premiere sauvegarde\n");
    printf("Appuyez sur 2 pour charger la deuxieme sauvegarde\n");
    printf("Appuyez sur 3 pour charger la troisieme sauvegarde\n");
    Choix = AttendreTouche();

    while (Choix != 49 && Choix != 50 && Choix != 51)
    {
        system("cls");
        printf("Quelle sauvegarde souhaitez-vous charger?\n");
        printf("Appuyez sur 1 pour charger la premiere sauvegarde\n");
        printf("Appuyez sur 2 pour charger la deuxieme sauvegarde\n");
        printf("Appuyez sur 3 pour charger la troisieme sauvegarde\n\n");
        Color(12, 0);
        printf("Appuyez sur 1, 2 ou 3\n"); //Message d'erreur si la personne ne saisit pas une des touches demandées
        Color(15, 0);
        Choix = AttendreTouche();
    }
    if (Choix == 49)
    {
        fichier = fopen("Sauvegarde_1.txt", "r"); //ouvre le fichier 1
        NoFichier = 1;
    }
    if (Choix == 50)
    {
        fichier = fopen("Sauvegarde_2.txt", "r");
        NoFichier = 2;
    }
    if (Choix == 51)
    {
        fichier = fopen("Sauvegarde_3.txt", "r");
        NoFichier = 3;
    }
    if (fichier != NULL)
    {

        int y = 0;
        char buf[10];
        //sinfo Grille[*NbLigne][*NbColonne];
        while (fgets(buf, 10, fichier) != NULL) //Cette boucle va permettre de lire une nouvelle ligne du fichier à chaque tour, jusqu'à la fin du fichier
        {
            y++; //Cette variable correspondra donc au numéro de la ligne
            if (y == 1)
            {
                * NbLigne = atoi(buf);
                printf("Nombre de Lignes :%d\n", * NbLigne);
            }
            if (y == 2)
            {
                * NbColonne = atoi(buf);
                printf("Nombre de Colonnes :%d\n", * NbColonne);
            }
            if (y == 3)
            {
                * NbAnimaux = atoi(buf);
                printf("Nombre de Animaux :%d\n", * NbAnimaux);
            }
            if (y == 4)
            {
                * NbAnimauxC = atoi(buf);
                printf("Nombre de carnivores :%d\n", * NbAnimauxC);
            }
            if (y == 5)
            {
                * NbBuisson = atoi(buf);
                printf("Nombre de buissons :%d\n", * NbBuisson);
            }
            if (y == 6)
            {
                * NbPtEau = atoi(buf);
                printf("Nombre de points d'eau :%d\n", * NbPtEau);
            }
            if (y == 7)
            {
                * DureeSimulation = atoi(buf);
                printf("DureeSimulation %d\n", * DureeSimulation);
            }
            if (y == 8)
            {
                * Jour = atoi(buf);
                printf("Nombre de jours :%d\n", * Jour);
            }
        }
        fclose(fichier); //Cela ferme le fichier ouvert
    }
    else
    {
        // On affiche un message d'erreur si on veut
        NoFichier = 0;
        Color(12, 0);
        printf("\nImpossible d'ouvrir le fichier de sauvegarde, vous allez devoir definir les parametres de la simulation");
        Color(15, 0);
        AttendreTouche();
    }
    system("cls");
    return NoFichier; //Retourne le no du fichier qui a été utliser pour charger la simulation
}
void Parametres(int * NbColonne, int * NbLigne, int * NbAnimaux, int * NbAnimauxC, int * NbBuisson, int * NbPtEau, int * DureeSimulation)
{
    system("cls");
    printf("Saisissez le nombre de colonnes\n");
    scanf("%d", NbColonne);
    printf("\rSaisissez le nombre de lignes\n");
    scanf("%d", NbLigne);
    printf("Saisissez le nombre d'animaux herbivores\n");
    scanf("%d", NbAnimaux);
    printf("Saisissez le nombre d'animaux carnivores\n");
    scanf("%d", NbAnimauxC);
    printf("Saisissez le nombre de buissons\n");
    scanf("%d", NbBuisson);
    printf("Saisissez le nombre de ressources d'eau\n");
    scanf("%d", NbPtEau);
    printf("Saisissez le nombre de jour que vous souhaitez observer\n");
    scanf("%d", DureeSimulation);
}
void Initialisation(int NbLigne, int NbColonne, int * NbAnimaux, int * NbAnimauxC, int * NbBuisson, int * NbPtEau, sinfo Grille[NbLigne][NbColonne], int * NoFichier, int * DureeSimulation, int * Jour) //Ce sous programme va permettre d'attribuer un emplacement précedement vide à chaque élément de la simulation
{
    for (i = 0; i < NbLigne; i++) //Attribue à toutes les cases le type savane
    {
        for (j = 0; j < NbColonne; j++)
        {
            Grille[i][j].type = 0;
        }

    }
    srand(time(NULL)); //fonction nécessaire pour générer des variables aléatoires
    int PosX, PosY; //on initialise les variables PosX et PosY qui définiront les nouveaux emplacements des animaux
    if ( * NoFichier == 0)
    {
        int a = 0;
        for (a = 0; a < * NbBuisson; a++) //Etape qui attribue une case aléatoire pour le nombre de buissons voulus
        {
            PosX = rand() % (NbLigne);
            PosY = rand() % (NbColonne);
            while (Grille[PosX][PosY].type != 0) //Cette boucle permet d'attribuer à un buisson une case sans élément
            {
                PosX = rand() % (NbLigne);
                PosY = rand() % (NbColonne);
            }
            if (Grille[PosX][PosY].type == 0)
            {
                Grille[PosX][PosY].type = 1; //on attribue le Type 1 aux buissons
                Grille[PosX][PosY].Ressource = 100; // on attribue un nombre de ressource aux buisson
                Grille[PosX][PosY].NbJSConsom = 0;
            }
        }

        for (a = 0; a < * NbPtEau; a++) //Etape qui attribue une case aléatoire pour le nombre de point d'eau voulus
        {
            PosX = rand() % (NbLigne);
            PosY = rand() % (NbColonne);
            while (Grille[PosX][PosY].type != 0)
            {
                PosX = rand() % (NbLigne);
                PosY = rand() % (NbColonne);
            }
            if (Grille[PosX][PosY].type == 0)
            {
                Grille[PosX][PosY].type = 2; //on attribue le Type 2 aux points d'eau
                Grille[PosX][PosY].Ressource = 100;
                Grille[PosX][PosY].NbJSConsom = 0;
            }
        }

        for (a = 0; a < * NbAnimaux; a++) //Etape qui attribue une case aléatoire pour le nombre d'animaux
        {
            PosX = rand() % (NbLigne);
            PosY = rand() % (NbColonne);
            while (Grille[PosX][PosY].type != 0)
            {
                PosX = rand() % (NbLigne);
                PosY = rand() % (NbColonne);
            }
            if (Grille[PosX][PosY].type == 0)
            {
                Grille[PosX][PosY].type = 3; //on attribue le type 3 aux animaux herbivores
                Grille[PosX][PosY].NbJSansManger = 0;
                Grille[PosX][PosY].NbJSansBoire = 0;
                Grille[PosX][PosY].NbJour = 0;
            }
        }
        for (a = 0; a < * NbAnimauxC; a++) //Etape qui attribue une case aléatoire pour le nombre d'animaux carnivores
        {
            PosX = rand() % (NbLigne);
            PosY = rand() % (NbColonne);
            while (Grille[PosX][PosY].type != 0)
            {
                PosX = rand() % (NbLigne);
                PosY = rand() % (NbColonne);
            }
            if (Grille[PosX][PosY].type == 0)
            {
                Grille[PosX][PosY].type = 4; // on attribue le type 4 aux animaux herbivores
                Grille[PosX][PosY].NbJSansManger = 0;
                Grille[PosX][PosY].NbJSansBoire = 0;
                Grille[PosX][PosY].NbJour = 0;
            }
        }
    }
    int y;
    PosX = 0; //reinitialisation de la variable PosX
    PosY = 0;
    y = 0;
    //char buf[100]; //Longueur maximum d'un ligne sera 10
    char * ValeurUnique = (char * ) malloc(100); //on créer ces variable car elle serviront pour diviser les lignes
    char * ValeurLigne = (char * ) malloc(100);
    FILE * fichier = NULL;
    if ( * NoFichier != 0)
    {
        char buf[100]; //Longueur maximum d'un ligne sera 10
        if ( * NoFichier == 1) //Si la partie a été chargée depuis le fichier de sauvegarde 1
        {
            fichier = fopen("Sauvegarde_1.txt", "r"); //ouverture du fichier de sauvegarde
        }
        if ( * NoFichier == 2) //Si la partie a été chargée depuis le fichier de sauvegarde 1
        {
            fichier = fopen("Sauvegarde_2.txt", "r"); //ouverture du fichier de sauvegarde
        }
        if ( * NoFichier == 3) //Si la partie a été chargée depuis le fichier de sauvegarde 1
        {
            fichier = fopen("Sauvegarde_3.txt", "r"); //ouverture du fichier de sauvegarde
        }
        while (fgets(buf, 100, fichier) != NULL) //Tant que la fin du fichier n'a pas été atteinte
        {
            y++; //Cette varaiable va etre utilisée pour déterminer le numéro de la ligne pour chaque tour de la boucle
            if (y == 1)
            {
                NbLigne = atoi(buf); //Cette fontion permet de convertir des caracteres en int
                printf("Nombre de Lignes :%d\n", NbLigne);
            }
            if (y == 2)
            {
                NbColonne = atoi(buf);
                printf("Nombre de Colonnes :%d\n", NbColonne);
            }
            if (y == 3)
            {
                * NbAnimaux = atoi(buf);
                printf("Nombre de Animaux :%d\n", * NbAnimaux);
            }
            if (y == 4)
            {
                * NbAnimauxC = atoi(buf);
                printf("Nombre de carnivores :%d\n", * NbAnimauxC);
            }
            if (y == 5)
            {
                * NbBuisson = atoi(buf);
                printf("Nombre de buissons :%d\n", * NbBuisson);
            }
            if (y == 6)
            {
                * NbPtEau = atoi(buf);
                printf("Nombre de points d'eau :%d\n", * NbPtEau);
            }
            if (y == 7)
            {
                * DureeSimulation = atoi(buf);
                printf("DureeSimulation %d\n", * DureeSimulation);
            }
            if (y == 8)
            {
                * Jour = atoi(buf);
                printf("Nombre de jours :%d\n", * Jour);
            }

            if (y > 8)
            {
                ValeurLigne = strtok(buf, "\n"); //Cette étape permet de "retirer" le \n de la ligne
                ValeurUnique = strtok(ValeurLigne, ":"); //Cette fonction va permettre de séparer les valeurs séparées par des : dans le fichier de sauvegarde
                int z = 0;
                while (ValeurUnique != NULL) //Tant que la fin de la ligne n'a pas été atteinte
                {

                    if (z == 0) //Chaque premier nombre de la ligne correspond au type de la case
                    {
                        Grille[PosX][PosY].type = atoi(ValeurUnique);
                    }
                    if (z == 1 && (Grille[PosX][PosY].type == 1 || Grille[PosX][PosY].type == 2)) //Si le type==1 ou 2 le deuxieme nombre de la ligne (séparé par un : ) correspond au nombre de jour sans etre consommé
                    {
                        Grille[PosX][PosY].NbJSConsom = atoi(ValeurUnique);
                    }
                    if (z == 2 && (Grille[PosX][PosY].type == 1 || Grille[PosX][PosY].type == 2)) //Si le type==1 ou 2 le troisieme nombre de la ligne (séparé par un : ) correspond au nombre de ressource de l'élément
                    {
                        Grille[PosX][PosY].Ressource = atoi(ValeurUnique);
                    }
                    if (z == 1 && (Grille[PosX][PosY].type == 3 || Grille[PosX][PosY].type == 4)) //Si le type==3 ou 4 le deuxieme nombre de la ligne (séparé par un : ) correspond au nombre de jour sans manger
                    {
                        Grille[PosX][PosY].NbJSansManger = atoi(ValeurUnique);
                    }
                    if (z == 2 && (Grille[PosX][PosY].type == 3 || Grille[PosX][PosY].type == 4)) //Si le type==3 ou 4 le troisieme nombre de la ligne (séparé par un : ) correspond au nombre de jour sans boire
                    {
                        Grille[PosX][PosY].NbJSansBoire = atoi(ValeurUnique);
                    }
                    if (z == 3 && (Grille[PosX][PosY].type == 3 || Grille[PosX][PosY].type == 4)) //Si le type==3 ou 4 le quatrieme nombre de la ligne (séparé par un : ) correspond au nombre de jour de l'animal
                    {
                        Grille[PosX][PosY].NbJour = atoi(ValeurUnique);
                    }
                    z++;
                    ValeurUnique = strtok(NULL, ":");
                }
                PosX++; //Pour chaque tour dans cette boucle, on augmente de un PosX

                if (PosX == NbLigne) //Si PosX atteint le nombre de ligne , alors on le reinitialise à 0 et on ajoute un au nombre de colonne
                {
                    PosX = 0;
                    PosY++;
                }
            }
        }

        fclose(fichier); //on ferme le fichier ouvert
    }
        free(ValeurLigne);
        free(ValeurUnique);
}
void deplacement(int NbLigne, int NbColonne, sinfo Grille[NbLigne][NbColonne], int Jour) //Ce sous-programme permet le déplacement aléatoire des animaux
{
    int NewPosX, NewPosY;
    int Dif;
    for (i = 0; i < NbLigne; i++)
    {
        for (j = 0; j < NbColonne; j++)
        {
            if ((Grille[i][j].type == 3 || Grille[i][j].type == 4) && Grille[i][j].NbJour < Jour)
            {
                Grille[i][j].NbJSansManger++; //l'animal se deplace une seule fois par jour, on augmente donc son nombre de our sans manger et son nombre de jour sans boire
                Grille[i][j].NbJSansBoire++;
                do
                {
                    NewPosX = (i + (rand() % 5) - 2);
                    NewPosY = (j + (rand() % 5) - 2);
                    Dif = fabs(NewPosX - i) + fabs(NewPosY - j); //La varibale Dif correspond à la sommme des valeur absolues des déplacements horizontzux et verticaux

                }
                while (NewPosX >= NbLigne || NewPosY >= NbColonne || NewPosX < 0 || NewPosY < 0 || (NewPosX == i && NewPosY == j) || Grille[NewPosX][NewPosY].type != 0 || Dif > 2);
                //Condition au déplacement : le nouvelle postion n'est pas plus grande que le nombre de ligne ou le nombre de colonne,
                if (Grille[NewPosX][NewPosY].type == 0)
                {
                    Grille[NewPosX][NewPosY] = Grille[i][j]; // On attribue les variables de l'animal à sa nouvelle position dans le tableau
                    Grille[NewPosX][NewPosY].NbJour++; //Cette variable permet d'éviter que l'animal ce déplace plusieurs fois dans la boucle for
                    Grille[i][j].type = 0; //On libere l'ancienne case de l'animal

                }
            }
        }
    }
}
void Affichage(int NbColonne, int NbLigne, sinfo Grille[NbLigne][NbColonne], int * AnimauxRestant, int * AnimauxCRestant, int * BuissonsRestant, int * PtEauRestant) //Sous programmes permttant d'afficher la carte dans la console
{
    for (i = 0; i < NbLigne; i++) //Cette boucle for correspond à l'affichage de la simulation
    {
        for (j = 0; j < NbColonne; j++)
        {
            if (Grille[i][j].type == 0)
            {
                Color(14, 14); //On associe la couleur jaune à la savane
                printf("  ");
            }
            if (Grille[i][j].type == 1 && Grille[i][j].Ressource > 50)
            {
                Color(2, 2); // on associe la couleur verte aux buissons en bonne santé
                printf("  ");
                * BuissonsRestant += 1;
            }
            if (Grille[i][j].type == 1 && Grille[i][j].Ressource <= 50)
            {
                Color(6, 6); //on associe la couleur kaki aux buissons déja consommés
                printf("  ");
                * BuissonsRestant += 1;
            }
            if (Grille[i][j].type == 2)
            {
                Color(1, 1); //on associe la couleur bleu aux points d'eau
                printf("  ");
                * PtEauRestant += 1;
            }
            if (Grille[i][j].type == 3)
            {
                Color(12, 12); //on associe cette couleur aux animaux herbivores
                printf("00");
                * AnimauxRestant += 1;
            }
            if (Grille[i][j].type == 4)
            {
                Color(4, 4); //on associe cette couleur aux animaux carnivores
                printf("  ");
                * AnimauxCRestant += 1;
            }
            Color(15, 0); // on redefinit les couleurs initaiales de la consoles
        }
        printf("\n");
    }
}
void Vivre(int NbLigne, int NbColonne, sinfo Grille[NbLigne][NbColonne]) //Ce sous-programme va permettre aux animaux de se nourrir et de boire
{
    for (i = 0; i < NbLigne; i++)
    {
        for (j = 0; j < NbColonne; j++)
        {
            if (Grille[i][j].type == 2 || Grille[i][j].type == 1)
            {
                Grille[i][j].NbJSConsom++; //Pour tout les éléments de type buisson ou eau, on définit une variable qui correspond aux nombre de jour sans etre consommé par un animal, afin de permettre leur régénération
            }
            if (Grille[i][j].type == 3) //Cette boucle correspond permet de déterminer si une ressource d'eau et/ou de nourriture est présente autour de l'animal, si oui le nombre de jour sans boire et/ou sans ma,ger de l'animal retourne à 0 et la ressource perd 20 points de vie sur les 100 qu'elle peut avoir
            {
                if (i < NbLigne - 1 && Grille[i + 1][j].type == 1 && Grille[i][j].NbJSansManger > 0 && Grille[i + 1][j].Ressource > 0)
                {
                    Grille[i][j].NbJSansManger = 0; //le nombre de jour sans manger de l'animal revient à 0, il n'aura donc plus besoin de se nourrir si d'autres plantes se trouvent sur une case adjacente
                    Grille[i + 1][j].Ressource = Grille[i + 1][j].Ressource - 20; //Le buisson perd 20 ressources
                    Grille[i + 1][j].NbJSConsom = 0; //Le nombre de jour sans etre consommé du buisson revient à 0
                }
                if (i > 0 && Grille[i - 1][j].type == 1 && Grille[i][j].NbJSansManger > 0 && Grille[i - 1][j].Ressource > 0)
                {
                    Grille[i][j].NbJSansManger = 0;
                    Grille[i - 1][j].Ressource = (Grille[i - 1][j].Ressource - 20);
                    Grille[i - 1][j].NbJSConsom = 0;
                }
                if (j < NbColonne - 1 && Grille[i][j + 1].type == 1 && Grille[i][j].NbJSansManger > 0 && Grille[i][j + 1].Ressource > 0)
                {
                    Grille[i][j].NbJSansManger = 0;
                    Grille[i][j + 1].Ressource = (Grille[i][j + 1].Ressource - 20);
                    Grille[i][j + 1].NbJSConsom = 0;
                }
                if (j > 0 && Grille[i][j - 1].type == 1 && Grille[i][j].NbJSansManger > 0 && Grille[i][j - 1].Ressource > 0)
                {
                    Grille[i][j].NbJSansManger = 0;
                    Grille[i][j - 1].Ressource = (Grille[i][j - 1].Ressource - 20);
                    Grille[i][j - 1].NbJSConsom = 0;
                }
                if (i < NbLigne - 1 && j > 0 && Grille[i + 1][j - 1].type == 1 && Grille[i][j].NbJSansManger > 0 && Grille[i + 1][j - 1].Ressource > 0)
                {
                    Grille[i][j].NbJSansManger = 0;
                    Grille[i + 1][j - 1].Ressource = (Grille[i + 1][j - 1].Ressource - 20);
                    Grille[i + 1][j - 1].NbJSConsom = 0;
                }
                if (i > 0 && j > 0 && Grille[i - 1][j - 1].type == 1 && Grille[i][j].NbJSansManger > 0 && Grille[i - 1][j - 1].Ressource > 0)
                {
                    Grille[i][j].NbJSansManger = 0;
                    Grille[i - 1][j - 1].Ressource = (Grille[i - 1][j - 1].Ressource - 20);
                    Grille[i - 1][j - 1].NbJSConsom = 0;
                }
                if (i > 0 && j < NbLigne - 1 && Grille[i - 1][j + 1].type == 1 && Grille[i][j].NbJSansManger > 0 && Grille[i - 1][j + 1].Ressource > 0)
                {
                    Grille[i][j].NbJSansManger = 0;
                    Grille[i - 1][j + 1].Ressource = (Grille[i - 1][j + 1].Ressource - 20);
                    Grille[i - 1][j + 1].NbJSConsom = 0;
                }
                if (i < NbLigne - 1 && j < NbColonne - 1 && Grille[i + 1][j + 1].type == 1 && Grille[i][j].NbJSansManger > 0 && Grille[i + 1][j + 1].Ressource > 0)
                {
                    Grille[i][j].NbJSansManger = 0;
                    Grille[i + 1][j + 1].Ressource = (Grille[i + 1][j + 1].Ressource - 20);
                    Grille[i + 1][j + 1].NbJSConsom = 0;
                }


                if (i < NbLigne - 1 && Grille[i + 1][j].type == 2 && Grille[i][j].NbJSansBoire > 0 && Grille[i + 1][j].Ressource > 0) //Les huits conditions suivantes effectuent les memes opération mais pour l'eau
                {
                    Grille[i][j].NbJSansBoire = 0;
                    Grille[i + 1][j].Ressource = (Grille[i + 1][j].Ressource - 34);
                    Grille[i + 1][j].NbJSConsom = 0;
                }
                if (i > 0 && Grille[i - 1][j].type == 2 && Grille[i][j].NbJSansBoire > 0 && Grille[i - 1][j].Ressource > 0)
                {
                    Grille[i][j].NbJSansBoire = 0;
                    Grille[i - 1][j].Ressource = (Grille[i - 1][j].Ressource - 34);
                    Grille[i - 1][j].NbJSConsom = 0;
                }
                if (j < NbColonne - 1 && Grille[i][j + 1].type == 2 && Grille[i][j].NbJSansBoire > 0 && Grille[i][j + 1].Ressource > 0)
                {
                    Grille[i][j].NbJSansBoire = 0;
                    Grille[i][j + 1].Ressource = (Grille[i][j + 1].Ressource - 34);
                    Grille[i][j + 1].NbJSConsom = 0;
                }
                if (j > 0 && Grille[i][j - 1].type == 2 && Grille[i][j].NbJSansBoire > 0 && Grille[i][j - 1].Ressource > 0)
                {
                    Grille[i][j].NbJSansBoire = 0;
                    Grille[i][j - 1].Ressource = (Grille[i][j - 1].Ressource - 34);
                    Grille[i][j - 1].NbJSConsom = 0;
                }
                if (i < NbLigne - 1 && j > 0 && Grille[i + 1][j - 1].type == 2 && Grille[i][j].NbJSansBoire > 0 && Grille[i + 1][j - 1].Ressource > 0)
                {
                    Grille[i][j].NbJSansBoire = 0;
                    Grille[i + 1][j - 1].Ressource = (Grille[i + 1][j - 1].Ressource - 34);
                    Grille[i + 1][j - 1].NbJSConsom = 0;
                }
                if (i > 0 && j > 0 && Grille[i - 1][j - 1].type == 2 && Grille[i][j].NbJSansBoire > 0 && Grille[i - 1][j - 1].Ressource > 0)
                {
                    Grille[i][j].NbJSansBoire = 0;
                    Grille[i - 1][j - 1].Ressource = (Grille[i - 1][j - 1].Ressource - 34);
                    Grille[i - 1][j - 1].NbJSConsom = 0;
                }
                if (i > 0 && j < NbColonne - 1 && Grille[i - 1][j + 1].type == 2 && Grille[i][j].NbJSansBoire > 0 && Grille[i - 1][j + 1].Ressource > 0)
                {
                    Grille[i][j].NbJSansBoire = 0;
                    Grille[i - 1][j + 1].Ressource = (Grille[i - 1][j + 1].Ressource - 34);
                    Grille[i - 1][j + 1].NbJSConsom = 0;
                }
                if (i < NbLigne - 1 && j < NbColonne - 1 && Grille[i + 1][j + 1].type == 2 && Grille[i][j].NbJSansBoire > 0 && Grille[i + 1][j + 1].Ressource > 0)
                {
                    Grille[i][j].NbJSansBoire = 0;
                    Grille[i + 1][j + 1].Ressource = Grille[i + 1][j + 1].Ressource - 34;
                    Grille[i + 1][j + 1].NbJSConsom = 0;
                }

                if (Grille[i][j].NbJSansManger == 10 || Grille[i][j].NbJSansBoire == 3) //L'animal meurt s'il n'a pas mangé depuis 10 jours ou s'il n'a pas bu depuis 3 jours
                {
                    Grille[i][j].type = 0; // De la savane remplace alors la case de l'animal
                }


            }
            if (Grille[i][j].type == 4) //Cette boucle permet de déterminer si une ressource d'eau et/ou un herbivore sont présents autour de l'animal, si oui le nombre de jour sans boire et/ou sans ma,ger de l'animal retourne à 0 et la ressource perd, et 20 points de vie sur les 100 qu'elle peut avoir et l'herbivore meurt
            {
                if (i < NbLigne - 1 && Grille[i + 1][j].type == 3 && Grille[i][j].NbJSansManger > 0)
                {
                    Grille[i][j].NbJSansManger = 0;
                    Grille[i + 1][j].Ressource = Grille[i + 1][j].Ressource - 20;
                    Grille[i + 1][j].NbJSConsom = 0;
                }
                if (i > 0 && Grille[i - 1][j].type == 3 && Grille[i][j].NbJSansManger > 0)
                {
                    Grille[i][j].NbJSansManger = 0;
                    Grille[i - 1][j].type = 0;
                }
                if (j < NbColonne - 1 && Grille[i][j + 1].type == 3 && Grille[i][j].NbJSansManger > 0)
                {
                    Grille[i][j].NbJSansManger = 0;
                    Grille[i][j + 1].type = 0;
                }
                if (j > 0 && Grille[i][j - 1].type == 3 && Grille[i][j].NbJSansManger > 0)
                {
                    Grille[i][j].NbJSansManger = 0;
                    Grille[i][j - 1].type = 0;
                }
                if (i < NbLigne - 1 && j > 0 && Grille[i + 1][j - 1].type == 3 && Grille[i][j].NbJSansManger > 0)
                {
                    Grille[i][j].NbJSansManger = 0;
                    Grille[i + 1][j - 1].type = 0;
                }
                if (i > 0 && j > 0 && Grille[i - 1][j - 1].type == 3 && Grille[i][j].NbJSansManger > 0)
                {
                    Grille[i][j].NbJSansManger = 0;
                    Grille[i - 1][j - 1].type = 0;
                }
                if (i > 0 && j < NbLigne - 1 && Grille[i - 1][j + 1].type == 3 && Grille[i][j].NbJSansManger > 0)
                {
                    Grille[i][j].NbJSansManger = 0;
                    Grille[i - 1][j + 1].type = 0;
                }
                if (i < NbLigne - 1 && j < NbColonne - 1 && Grille[i + 1][j + 1].type == 3 && Grille[i][j].NbJSansManger > 0)
                {
                    Grille[i][j].NbJSansManger = 0;
                    Grille[i + 1][j + 1].type = 0;
                }
                if (i < NbLigne - 1 && Grille[i + 1][j].type == 2 && Grille[i][j].NbJSansBoire > 0 && Grille[i + 1][j].Ressource > 0)
                {
                    Grille[i][j].NbJSansBoire = 0;
                    Grille[i + 1][j].Ressource = (Grille[i + 1][j].Ressource - 34);
                    Grille[i + 1][j].NbJSConsom = 0;
                }
                if (i > 0 && Grille[i - 1][j].type == 2 && Grille[i][j].NbJSansBoire > 0 && Grille[i - 1][j].Ressource > 0)
                {
                    Grille[i][j].NbJSansBoire = 0;
                    Grille[i - 1][j].Ressource = (Grille[i - 1][j].Ressource - 34);
                    Grille[i - 1][j].NbJSConsom = 0;
                }
                if (j < NbColonne - 1 && Grille[i][j + 1].type == 2 && Grille[i][j].NbJSansBoire > 0 && Grille[i][j + 1].Ressource > 0)
                {
                    Grille[i][j].NbJSansBoire = 0;
                    Grille[i][j + 1].Ressource = (Grille[i][j + 1].Ressource - 34);
                    Grille[i][j + 1].NbJSConsom = 0;
                }
                if (j > 0 && Grille[i][j - 1].type == 2 && Grille[i][j].NbJSansBoire > 0 && Grille[i][j - 1].Ressource > 0)
                {
                    Grille[i][j].NbJSansBoire = 0;
                    Grille[i][j - 1].Ressource = (Grille[i][j - 1].Ressource - 34);
                    Grille[i][j - 1].NbJSConsom = 0;
                }
                if (i < NbLigne - 1 && j > 0 && Grille[i + 1][j - 1].type == 2 && Grille[i][j].NbJSansBoire > 0 && Grille[i + 1][j - 1].Ressource > 0)
                {
                    Grille[i][j].NbJSansBoire = 0;
                    Grille[i + 1][j - 1].Ressource = (Grille[i + 1][j - 1].Ressource - 34);
                    Grille[i + 1][j - 1].NbJSConsom = 0;
                }
                if (i > 0 && j > 0 && Grille[i - 1][j - 1].type == 2 && Grille[i][j].NbJSansBoire > 0 && Grille[i - 1][j - 1].Ressource > 0)
                {
                    Grille[i][j].NbJSansBoire = 0;
                    Grille[i - 1][j - 1].Ressource = (Grille[i - 1][j - 1].Ressource - 34);
                    Grille[i - 1][j - 1].NbJSConsom = 0;
                }
                if (i > 0 && j < NbColonne - 1 && Grille[i - 1][j + 1].type == 2 && Grille[i][j].NbJSansBoire > 0 && Grille[i - 1][j + 1].Ressource > 0)
                {
                    Grille[i][j].NbJSansBoire = 0;
                    Grille[i - 1][j + 1].Ressource = (Grille[i - 1][j + 1].Ressource - 34);
                    Grille[i - 1][j + 1].NbJSConsom = 0;
                }
                if (i < NbLigne - 1 && j < NbColonne - 1 && Grille[i + 1][j + 1].type == 2 && Grille[i][j].NbJSansBoire > 0 && Grille[i + 1][j + 1].Ressource > 0)
                {
                    Grille[i][j].NbJSansBoire = 0;
                    Grille[i + 1][j + 1].Ressource = Grille[i + 1][j + 1].Ressource - 34;
                    Grille[i + 1][j + 1].NbJSConsom = 0;
                }

                if (Grille[i][j].NbJSansManger == 10 || Grille[i][j].NbJSansBoire == 3) //L'animal meurt
                {
                    Grille[i][j].type = 0;
                }


            }

            if (Grille[i][j].type == 2 && Grille[i][j].Ressource <= 0) //Cette condition permet de remplacer les ressources entierement consommées et les remplace par de la savane
            {
                Grille[i][j].type = 0;
            }
            if (Grille[i][j].type == 1 && Grille[i][j].NbJSConsom == 4) //Si un buisson n'a pas été consommé depuis 3 jours, il se régénère automatiquement
            {
                Grille[i][j].Ressource = 100;
            }
            if (Grille[i][j].type == 1 && Grille[i][j].Ressource <= 0)
            {
                Grille[i][j].type = 0;
            }
            if (Grille[i][j].type == 2) // si l'eau n'est pas entièrement consommée en un jour elle se régénère entièrement
            {
                Grille[i][j].Ressource = 100;
            }
        }
    }

}
void Legende(int Jour, int AnimauxRestant, int AnimauxCRestant, int BuissonsRestant, int PtEauRestant) //Ce sous-programme affiche la légende en bas de la carte
{
    printf("\n\n");
    Color(14, 14);
    printf("  ");
    Color(15, 0);
    printf(" : Savane\n");

    Color(2, 2);
    printf("  ");
    Color(15, 0);
    printf(" : Buisson\n");

    Color(6, 6);
    printf("  ");
    Color(15, 0);
    printf(" : Buisson avec de faibles ressources\n");

    Color(1, 1);
    printf("  ");
    Color(15, 0);
    printf(" : Eau\n");

    Color(12, 12);
    printf("  ");
    Color(15, 0);
    printf(" : Animal\n");

    Color(4, 4);
    printf("  ");
    Color(15, 0);
    printf(" : Animal Carnivore\n\n");

    printf("Jour %d\n", Jour);
    if (AnimauxRestant > 1) //Affiche le nombre d'animaux restant avec une orthographe cohérente
    {
        printf("Il reste %d animaux herbivores\n", AnimauxRestant);
    }
    else
    {
        printf("Il reste %d animal herbivore\n", AnimauxRestant);
    }

    if (AnimauxCRestant > 1)
    {
        printf("Il reste %d animaux carnivores\n", AnimauxCRestant);
    }
    else
    {
        printf("Il reste %d animal carnivore\n", AnimauxCRestant);
    }
    if (BuissonsRestant > 1)
    {
        printf("Il reste %d buissons\n", BuissonsRestant);
    }
    else
    {
        printf("Il reste %d buisson\n", BuissonsRestant);
    }
    if (PtEauRestant > 1)
    {
        printf("Il reste %d ressources en eau\n", PtEauRestant);
    }
    else
    {
        printf("Il reste %d ressource en eau\n", PtEauRestant);
    }
    printf("\nAppuyez sur ESPACE pour mettre en pause la simulation");
    Sleep(1000);
}
void Savegame(int NbLigne, int NbColonne, int NbAnimaux, int NbAnimauxC, int NbBuisson, int NbPtEau, int DureeSimulation, int Jour, sinfo Grille[NbLigne][NbColonne])
{
    int Choix;
    FILE * fichier = NULL;
    system("cls");
    Color(15, 0);
    printf("~~~~Menu de Sauvegarde de la simulation~~~~\n\n");
    printf("Quel emplacement de sauvegarde souhaitez-vous choisir ?\n  -Appuyez sur 1 pour choisir l'emplacement 1\n  -Appuyez sur 2 pour choisir l'emplacement 2 \n  -Appuyez sur 3 pour choisir l'emplacement 3");
    Choix = AttendreTouche();
    while (Choix != 49 && Choix != 50 && Choix != 51)
    {
        system("cls");
        printf("~~~~Menu de Sauvegarde de la simulation~~~~\n\n");
        printf("Quel emplacement de sauvegarde souhaitez-vous choisir ?\n  -Appuyez sur 1 pour choisir l'emplacement 1\n  -Appuyez sur 2 pour choisir l'emplacement 2 \n  -Appuyez sur 3 pour l'emplacement 3\n");
        Color(12, 0);
        printf("Appuyez sur une touche valide");
        Color(15, 0);
        Choix = AttendreTouche();

    }
    if (Choix == 49)
    {
        fichier = fopen("Sauvegarde_1.txt", "w");
    }
    if (Choix == 50)
    {
        fichier = fopen("Sauvegarde_2.txt", "w");
    }
    if (Choix == 51)
    {
        fichier = fopen("Sauvegarde_3.txt", "w");
    }

    if (fichier != NULL) //Enregistre les variables de la simulation en cours dans le fichier sélectionné
    {
        fprintf(fichier, "%d\n", NbLigne);
        fprintf(fichier, "%d\n", NbColonne);
        fprintf(fichier, "%d\n", NbAnimaux);
        fprintf(fichier, "%d\n", NbAnimauxC);
        fprintf(fichier, "%d\n", NbBuisson);
        fprintf(fichier, "%d\n", NbPtEau);
        fprintf(fichier, "%d\n", DureeSimulation);
        fprintf(fichier, "%d\n", Jour);
        for (j = 0; j < NbColonne; j++) //Cette boucle for correspond à l'utilisation de toute les valeurs du tableau
            for (i = 0; i < NbLigne; i++)
            {
                if (Grille[i][j].type == 0)
                {
                    fprintf(fichier, "%d:\n", Grille[i][j].type);
                }
                if (Grille[i][j].type == 1)
                {
                    fprintf(fichier, "%d:", Grille[i][j].type);
                    fprintf(fichier, "%d:", Grille[i][j].NbJSConsom);
                    fprintf(fichier, "%d:\n", Grille[i][j].Ressource);
                }
                if (Grille[i][j].type == 2)
                {
                    fprintf(fichier, "%d:", Grille[i][j].type);
                    fprintf(fichier, "%d:", Grille[i][j].NbJSConsom);
                    fprintf(fichier, "%d:\n", Grille[i][j].Ressource);
                }
                if (Grille[i][j].type == 3)
                {
                    fprintf(fichier, "%d:", Grille[i][j].type);
                    fprintf(fichier, "%d:", Grille[i][j].NbJSansManger);
                    fprintf(fichier, "%d:", Grille[i][j].NbJSansBoire);
                    fprintf(fichier, "%d:\n", Grille[i][j].NbJour);
                }
                if (Grille[i][j].type == 4)
                {
                    fprintf(fichier, "%d:", Grille[i][j].type);
                    fprintf(fichier, "%d:", Grille[i][j].NbJSansManger);
                    fprintf(fichier, "%d:", Grille[i][j].NbJSansBoire);
                    fprintf(fichier, "%d:\n", Grille[i][j].NbJour);
                }
            }
            fclose(fichier); // ferme le fichier
    }




    system("cls");
    Color(2, 0);
    printf("La sauvegarde s'est correctement terminee !");
    Color(15, 0);
    printf("\n\n\nAppuyez sur Echap pour quitter\nAppuyez sur Entree pour reprendre la simulation");
    do
    {
        Choix = AttendreTouche();
    }
    while (Choix != 27 && Choix != 13);

    if (Choix == 27)
    {
        exit(0);
    }
    if (Choix == 13)
    {
        system("exit");
    }
    else
    {
        Color(12, 0);
        printf("Impossible d'ouvrir le fichier");
        Color(15, 0);
    }
}
int MenuPause(int NbLigne, int NbColonne, int NbAnimaux, int NbAnimauxC, int NbBuisson, int NbPtEau, int DureeSimulation, int Jour, sinfo Grille[NbLigne][NbColonne])
{
    int Touche;

    do
    {
        system("cls"); //Cette fonction efface le texte de la console


        for (i = 0; i < NbLigne; i++) //Cette boucle for correspond à l'affichage de la simulation
        {
            for (j = 0; j < NbColonne; j++)
            {
                if (Grille[i][j].type == 0)
                {
                    Color(14, 14); //On associe la couleur jaune à la savane
                    printf("  ");
                }
                if (Grille[i][j].type == 1 && Grille[i][j].Ressource > 50)
                {
                    Color(2, 2); // on associe la couleur verte aux buissons en bonne santé
                    printf("  ");
                    //* BuissonsRestant += 1;
                }
                if (Grille[i][j].type == 1 && Grille[i][j].Ressource <= 50)
                {
                    Color(6, 6); //on associe la couleur kaki aux buissons déja consommés
                    printf("  ");
                    //* BuissonsRestant += 1;
                }
                if (Grille[i][j].type == 2)
                {
                    Color(1, 1); //on associe la couleur bleu aux points d'eau
                    printf("  ");
                    //* PtEauRestant += 1;
                }
                if (Grille[i][j].type == 3)
                {
                    Color(12, 12); //on associe cette couleur aux animaux herbivores
                    printf("00");
                    //* AnimauxRestant += 1;
                }
                if (Grille[i][j].type == 4)
                {
                    Color(4, 4); //on associe cette couleur aux animaux carnivores
                    printf("  ");
                    //* AnimauxCRestant += 1;
                }
                Color(15, 0); // on redefinit les couleurs initaiales de la consoles
            }
            printf("\n");
        }
        int couleur;
        couleur = 1 + rand() % 13; //on génère une couleur aléatoire pour le texte
        Color(couleur, 0);
        printf("############################\n");
        printf("#                          #\n");
        printf("#      La simulation       #\n");
        printf("#      est en pause        #\n");
        printf("#  appuyez sur une touche  #\n");
        printf("#     pour reprendre !     #\n");
        printf("#                          #\n");
        printf("############################\n");
        Color(15, 0);
        printf("Appuyez sur s pour enregistrer la partie !!\n");
        printf("Appuyez sur a pour recommencer une simulation avec les memes parametres\n");
        printf("Appuyez sur b pour refaire une simulation avec des nouveaux parametres\n");
        printf("Appuyez sur c pour charger une simulation\n");
        Sleep(200); //on instaure un cours délais entre chaque changement de couleur
    }
    while (!kbhit());

    Touche = getch();
    if (Touche == 115)
    {
        Savegame(NbLigne, NbColonne, NbAnimaux, NbAnimauxC, NbBuisson, NbPtEau, DureeSimulation, Jour, Grille); //on va pouvoir acceder au menu de sauvegarde
        return 0;
    }
    else if (Touche == 97)   //Si la touche est a , la simulation est refaite avec les memes parametres
    {
        return 1;
    }
    else if (Touche == 98)   //Si la touche est b, l'utilisateur va pouvoir saisir de nouveaux parametres
    {
        return 2;
    }
    else if (Touche == 99)   //Si la touche est c, l'utilisateur va pouvoir charger une simulation
    {
        return 3;
    }
    else
    {
        return 0;
    }
}

void Resulats(int NbAnimaux, int NbAnimauxC, int NbBuisson, int NbPtEau, int AnimauxRestant, int AnimauxCRestant, int BuissonsRestant, int PtEauRestant, int Jour, int DureeSimulation)
{
    system("cls"); //on efface le texte de la console

    int BuissonsDisparus, PtEauDisparus, AnimauxMorts, AnimauxCMorts; //on initialise les variables qui nous donneront le nombre d'éléments disparus

    BuissonsDisparus = NbBuisson - BuissonsRestant;
    PtEauDisparus = NbPtEau - PtEauRestant;
    AnimauxMorts = NbAnimaux - AnimauxRestant;
    AnimauxCMorts = NbAnimauxC - AnimauxCRestant;

    if (AnimauxRestant <= 0) //Si la simulation se termine car il ne reste plus d'animaux
    {
        printf("Tous les animaux sont morts apres %d jours,\nau debut de la simulation il y avait\n", Jour);
        if (NbAnimaux > 1) //Les conditions suivantes permettent d'écrire les résultats de la simulation avec une orthographe correcte
        {
            printf(" - %d animaux\n", NbAnimaux);
        }
        else
        {
            printf(" - %d animal\n", NbAnimaux);
        }
        if (NbBuisson > 1)
        {
            printf(" - %d buissons\n", NbBuisson);
        }
        else
        {
            printf(" - %d buisson\n", NbBuisson);
        }
        if (NbPtEau > 1)
        {
            printf(" - %d points d'eau\n\n", NbPtEau);
        }
        else
        {
            printf(" - %d point d'eau\n\n", NbPtEau);
        }

        if (BuissonsRestant > 1)
        {
            printf("Il reste:\n - %d buissons\n ", BuissonsRestant);
        }
        else
        {
            printf("Il reste:\n - %d buisson\n ", BuissonsRestant);
        }
        if (PtEauRestant > 1)
        {
            printf("- %d points d'eau\n\n", PtEauRestant);
        }
        else
        {
            printf("- %d point d'eau\n\n", PtEauRestant);
        }
        if (BuissonsDisparus > 1)
        {
            printf("%d buissons ont ete consommes", BuissonsDisparus);
        }
        else if (BuissonsDisparus == 1)
        {
            printf("%d buisson a ete consomme", BuissonsDisparus);
        }
        else
        {
            printf("aucun buisson a ete consomme");
        }

        if (PtEauDisparus > 1)
        {
            printf(" et %d ressources en eau ont disparu\n\n", PtEauDisparus);
        }
        else if (PtEauDisparus == 1)
        {
            printf(" et %d ressource en eau a disparu\n\n", PtEauDisparus);
        }
        else
        {
            printf(" aucune ressource en eau a disparu\n\n");
        }

    }
    if (Jour >= DureeSimulation) //Si la simulation se termine car la duree de simulation a été atteinte
    {
        int TauxSurvie, TauxSurvieC;
        TauxSurvie = 100 * AnimauxRestant / NbAnimaux; //Cela permet de donner le taux de survie en % pour les animaux herbivores
        TauxSurvieC = 100 * AnimauxCRestant / NbAnimauxC;
        printf("Le nombre de jour d'observation a ete atteint, au debut de la simulation il y avait\n");
        if (NbAnimaux > 1)
        {
            printf(" - %d animaux herbivores\n", NbAnimaux);
        }
        else
        {
            printf(" - %d animal herbivore\n", NbAnimaux);
        }
        if (NbAnimauxC > 1)
        {
            printf(" - %d animaux carnivores\n", NbAnimauxC);
        }
        else
        {
            printf(" - %d animal carnivore\n", NbAnimauxC);
        }

        if (NbBuisson > 1)
        {
            printf(" - %d buissons\n", NbBuisson);
        }
        else
        {
            printf(" - %d buisson\n", NbBuisson);
        }
        if (NbPtEau > 1)
        {
            printf(" - %d points d'eau\n\n", NbPtEau);
        }
        else
        {
            printf(" - %d point d'eau\n\n", NbPtEau);
        }
        if (AnimauxRestant > 1)
        {
            printf("Il reste:\n - %d animaux herbivores\n", AnimauxRestant);
        }
        else
        {
            printf("Il reste:\n - %d animal herbivore\n", AnimauxRestant);
        }

        if (AnimauxCRestant > 1)
        {
            printf(" - %d animaux carnivores\n", AnimauxCRestant);
        }
        else
        {
            printf(" - %d animal carnivore\n", AnimauxCRestant);
        }

        if (BuissonsRestant > 1)
        {
            printf(" - %d buissons\n ", BuissonsRestant);
        }
        else
        {
            printf(" - %d buisson\n ", BuissonsRestant);
        }
        if (PtEauRestant > 1)
        {
            printf("- %d points d'eau\n\n", PtEauRestant);
        }
        else
        {
            printf("- %d point d'eau\n\n", PtEauRestant);
        }

        if (AnimauxMorts > 1)
        {
            printf("%d animaux herbivores sont morts, ", AnimauxMorts);
        }
        else if (AnimauxMorts == 1)
        {
            printf("%d animal herbivore est mort, ", AnimauxMorts);
        }
        else
        {
            printf("Aucun animal herbivore est mort, ");
        }

        if (AnimauxCMorts > 1)
        {
            printf("%d animaux carnivores sont morts, ", AnimauxCMorts);
        }
        else if (AnimauxCMorts == 1)
        {
            printf("%d animal carnivore est mort, ", AnimauxCMorts);
        }
        else
        {
            printf("aucun animal carnivore est mort, ");
        }

        if (BuissonsDisparus > 1)
        {
            printf("%d buissons ont ete consommes", BuissonsDisparus);
        }
        else if (BuissonsDisparus == 1)
        {
            printf("%d buisson a ete consomme", BuissonsDisparus);
        }
        else
        {
            printf("aucun buisson a ete consomme");
        }

        if (PtEauDisparus > 1)
        {
            printf(" et %d ressources en eau ont disparu.\n\n", PtEauDisparus);
        }
        else if (PtEauDisparus == 1)
        {
            printf(" et %d ressource en eau a disparu.\n\n", PtEauDisparus);
        }
        else
        {
            printf("et aucune ressource en eau a disparu.\n\n");
        }

        printf("Le taux de survie est de %d %% pour les animaux herbivores et de %d %% pour les animaux carnivores\n\n\n", TauxSurvie, TauxSurvieC);

    }
}
