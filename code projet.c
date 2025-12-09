#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SALLES 50
#define MAX_RES 500
#define MAX_EQUIPEMENTS 20

/********************************************
 * STRUCTURES
 *******************************************/
typedef struct {
    int jour, mois, annee;
} Date;

typedef struct {
    int heure, minute;
} Temps;

typedef struct {
    char cle[50];
    int quantite;
} Paire;

typedef struct {
    Paire items[MAX_EQUIPEMENTS];
    int taille;
} Dictionnaire;

typedef struct {
    char nom[50];
    int capacite;
    float tarif_horaire;
    Dictionnaire equipements;
} Salle;

typedef struct {
    int id;
    char nom_client[50];
    char salle[50];
    Date debut_date;
    Temps debut_temps;
    Date fin_date;
    Temps fin_temps;
    float prix_total;
    char statut[20];
} Reservation;

/********************************************
 * VARIABLES GLOBALES
 *******************************************/
Salle salles[MAX_SALLES];
Reservation reservations[MAX_RES];
int nb_salles = 10;   // 10 salles par défaut
int nb_res = 0;

/********************************************
 * OUTILS TEMPS-DATES
 *******************************************/
int dateToJours(Date d){
    return d.annee*365 + d.mois*30 + d.jour;
}

int differenceMinutes(Date d1, Temps t1, Date d2, Temps t2){
    int m1 = dateToJours(d1)*1440 + t1.heure*60 + t1.minute;
    int m2 = dateToJours(d2)*1440 + t2.heure*60 + t2.minute;
    return m2 - m1;
}

/********************************************
 * INITIALISATION DES 10 SALLES DIRECTEMENT
 *******************************************/
void initialiserSallesParDefaut(){

    // #1
    strcpy(salles[0].nom, "MeetingRoom1");
    salles[0].capacite = 12;
    salles[0].tarif_horaire = 18;
    salles[0].equipements.taille = 0;
    strcpy(salles[0].equipements.items[0].cle,"chaises"); salles[0].equipements.items[0].quantite=12;
    strcpy(salles[0].equipements.items[1].cle,"television"); salles[0].equipements.items[1].quantite=1;
    strcpy(salles[0].equipements.items[2].cle,"tableau_blanc"); salles[0].equipements.items[2].quantite=1;
    strcpy(salles[0].equipements.items[3].cle,"climatisation"); salles[0].equipements.items[3].quantite=1;
    salles[0].equipements.taille = 4;

    // #2
    strcpy(salles[1].nom, "MeetingRoom2");
    salles[1].capacite = 18;
    salles[1].tarif_horaire = 20;
    salles[1].equipements.taille = 0;
    strcpy(salles[1].equipements.items[0].cle,"chaises"); salles[1].equipements.items[0].quantite=18;
    strcpy(salles[1].equipements.items[1].cle,"projecteur"); salles[1].equipements.items[1].quantite=1;
    strcpy(salles[1].equipements.items[2].cle,"haut_parleurs"); salles[1].equipements.items[2].quantite=2;
    salles[1].equipements.taille = 3;

    // #3
    strcpy(salles[2].nom, "StudyRoom1");
    salles[2].capacite = 10;
    salles[2].tarif_horaire = 10;
    salles[2].equipements.taille = 0;
    strcpy(salles[2].equipements.items[0].cle,"tables"); salles[2].equipements.items[0].quantite=10;
    strcpy(salles[2].equipements.items[1].cle,"chaises"); salles[2].equipements.items[1].quantite=10;
    salles[2].equipements.taille = 2;

    // #4
    strcpy(salles[3].nom, "StudyRoom2");
    salles[3].capacite = 20;
    salles[3].tarif_horaire = 12;
    salles[3].equipements.taille = 0;
    strcpy(salles[3].equipements.items[0].cle,"tables"); salles[3].equipements.items[0].quantite=20;
    strcpy(salles[3].equipements.items[1].cle,"chaises"); salles[3].equipements.items[1].quantite=20;
    strcpy(salles[3].equipements.items[2].cle,"television"); salles[3].equipements.items[2].quantite=1;
    salles[3].equipements.taille = 3;

    // #5
    strcpy(salles[4].nom, "ConferenceHall1");
    salles[4].capacite = 120;
    salles[4].tarif_horaire = 40;
    salles[4].equipements.taille = 0;
    strcpy(salles[4].equipements.items[0].cle,"chaises"); salles[4].equipements.items[0].quantite=120;
    strcpy(salles[4].equipements.items[1].cle,"microphones"); salles[4].equipements.items[1].quantite=4;
    strcpy(salles[4].equipements.items[2].cle,"haut_parleurs"); salles[4].equipements.items[2].quantite=6;
    salles[4].equipements.taille = 3;

    // #6
    strcpy(salles[5].nom, "ConferenceHall2");
    salles[5].capacite = 80;
    salles[5].tarif_horaire = 35;
    salles[5].equipements.taille = 0;
    strcpy(salles[5].equipements.items[0].cle,"chaises"); salles[5].equipements.items[0].quantite=80;
    strcpy(salles[5].equipements.items[1].cle,"ecran_geant"); salles[5].equipements.items[1].quantite=1;
    salles[5].equipements.taille = 2;

    // #7
    strcpy(salles[6].nom, "CoworkingSpace1");
    salles[6].capacite = 25;
    salles[6].tarif_horaire = 15;
    salles[6].equipements.taille = 0;
    strcpy(salles[6].equipements.items[0].cle,"bureaux"); salles[6].equipements.items[0].quantite=25;
    strcpy(salles[6].equipements.items[1].cle,"ordinateurs"); salles[6].equipements.items[1].quantite=10;
    salles[6].equipements.taille = 2;

    // #8
    strcpy(salles[7].nom, "CoworkingSpace2");
    salles[7].capacite = 40;
    salles[7].tarif_horaire = 14;
    salles[7].equipements.taille = 0;
    strcpy(salles[7].equipements.items[0].cle,"bureaux"); salles[7].equipements.items[0].quantite=40;
    strcpy(salles[7].equipements.items[1].cle,"televisions"); salles[7].equipements.items[1].quantite=2;
    salles[7].equipements.taille = 2;

    // #9
    strcpy(salles[8].nom, "MultiUseRoom1");
    salles[8].capacite = 30;
    salles[8].tarif_horaire = 22;
    salles[8].equipements.taille = 0;
    strcpy(salles[8].equipements.items[0].cle,"tapis_sol"); salles[8].equipements.items[0].quantite=30;
    strcpy(salles[8].equipements.items[1].cle,"enceintes"); salles[8].equipements.items[1].quantite=2;
    salles[8].equipements.taille = 2;

    // #10
    strcpy(salles[9].nom, "MultiUseRoom2");
    salles[9].capacite = 20;
    salles[9].tarif_horaire = 25;
    salles[9].equipements.taille = 0;
    strcpy(salles[9].equipements.items[0].cle,"ecrans_interactifs"); salles[9].equipements.items[0].quantite=2;
    strcpy(salles[9].equipements.items[1].cle,"webcams"); salles[9].equipements.items[1].quantite=2;
    salles[9].equipements.taille = 2;
}

/********************************************
 * CHARGEMENT DES RESERVATIONS
 *******************************************/
void chargerReservations(){
    FILE *f = fopen("reservations.txt","r");
    if(!f) return;

    while(!feof(f)){
        Reservation r;
        if(fscanf(f,"%d %s %s %d/%d/%d %d:%d %d/%d/%d %d:%d %f %s",
            &r.id, r.nom_client, r.salle,
            &r.debut_date.jour,&r.debut_date.mois,&r.debut_date.annee,
            &r.debut_temps.heure,&r.debut_temps.minute,
            &r.fin_date.jour,&r.fin_date.mois,&r.fin_date.annee,
            &r.fin_temps.heure,&r.fin_temps.minute,
            &r.prix_total, r.statut)==14){
            reservations[nb_res++] = r;
        }
    }
    fclose(f);
}

/********************************************
 * DISPONIBILITE
 *******************************************/
int estDisponible(char *nomSalle, Date d1, Temps t1, Date d2, Temps t2){

    int startN = differenceMinutes((Date){0},(Temps){0}, d1,t1);
    int endN   = differenceMinutes((Date){0},(Temps){0}, d2,t2);

    for(int i=0;i<nb_res;i++){
        if(strcmp(reservations[i].salle, nomSalle)==0){
            int startE = differenceMinutes((Date){0},(Temps){0},
                            reservations[i].debut_date, reservations[i].debut_temps);
            int endE   = differenceMinutes((Date){0},(Temps){0},
                            reservations[i].fin_date, reservations[i].fin_temps);

            if(!(endN <= startE || startN >= endE))
                return 0;
        }
    }

    return 1;
}

/********************************************
 * AJOUT SALLE
 *******************************************/
void ajouterSalle(){
    if(nb_salles>=MAX_SALLES){
        printf("Limite atteinte.\n");
        return;
    }

    Salle *s = &salles[nb_salles];
    s->equipements.taille = 0;

    printf("Nom salle : ");
    scanf("%s", s->nom);

    printf("Capacité : ");
    scanf("%d", &s->capacite);

    printf("Tarif horaire : ");
    scanf("%f", &s->tarif_horaire);

    nb_salles++;

    printf("Salle ajoutée.\n");
}

/********************************************
 * FACTURE
 *******************************************/
void enregistrerFacture(Reservation r){

    FILE *f = fopen("factures.txt","a");
    if(!f) return;

    fprintf(f,"FACTURE %d\n", r.id);
    fprintf(f,"Client : %s\n",r.nom_client);
    fprintf(f,"Salle  : %s\n",r.salle);

    fprintf(f,"Début  : %02d/%02d/%04d %02d:%02d\n",
        r.debut_date.jour,r.debut_date.mois,r.debut_date.annee,
        r.debut_temps.heure,r.debut_temps.minute);

    fprintf(f,"Fin    : %02d/%02d/%04d %02d:%02d\n",
        r.fin_date.jour,r.fin_date.mois,r.fin_date.annee,
        r.fin_temps.heure,r.fin_temps.minute);

    fprintf(f,"Montant : %.2f DT\n", r.prix_total);
    fprintf(f,"-------------------------\n");

    fclose(f);
}

/********************************************
 * CHIFFRE D'AFFAIRES
 *******************************************/
void mettreAJourChiffreAffaires(){

    FILE *f = fopen("chiffre_affaires.txt","w");
    if(!f) return;

    fprintf(f,"===== CHIFFRE D'AFFAIRES =====\n");

    float total=0;

    for(int i=0;i<nb_salles;i++){
        float ca=0;

        for(int j=0;j<nb_res;j++){
            if(strcmp(reservations[j].salle, salles[i].nom)==0 &&
                strcmp(reservations[j].statut,"confirme")==0)
                ca += reservations[j].prix_total;
        }

        fprintf(f,"%s : %.2f DT\n", salles[i].nom, ca);
        total += ca;
    }

    fprintf(f,"TOTAL : %.2f DT\n", total);

    fclose(f);
}

/********************************************
 * STATISTIQUES S1b (append)
 *******************************************/
void mettreAJourStatistiques(){

    FILE *f = fopen("statistiques.txt","a");
    if(!f) return;

    fprintf(f,"\n===== NOUVEAU RAPPORT =====\n");

    // salle la plus populaire
    int max=0; 
    char pop[50]="";
    int mois[13]={0};

    for(int i=0;i<nb_salles;i++){
        int cnt=0;
        for(int j=0;j<nb_res;j++){
            if(strcmp(reservations[j].salle,salles[i].nom)==0 &&
                strcmp(reservations[j].statut,"confirme")==0){
                cnt++;
                mois[reservations[j].debut_date.mois]++;
            }
        }

        if(cnt>max){
            max=cnt;
            strcpy(pop, salles[i].nom);
        }
    }

    fprintf(f,"Salle la plus populaire : %s (%d réservations)\n", pop, max);

    fprintf(f,"Réservations par mois :\n");
    for(int m=1;m<=12;m++)
        fprintf(f,"Mois %02d : %d\n", m, mois[m]);

    fclose(f);
}

/********************************************
 * AJOUT RESERVATION
 *******************************************/
void ajouterReservation(){

    Reservation r;

    r.id = (nb_res==0?1:reservations[nb_res-1].id+1);

    printf("Nom client : ");
    scanf("%s",r.nom_client);

    printf("Salle : ");
    scanf("%s",r.salle);

    int idx=-1;
    for(int i=0;i<nb_salles;i++)
        if(strcmp(salles[i].nom,r.salle)==0) idx=i;

    if(idx==-1){
        printf("Salle introuvable !\n");
        return;
    }

    char ds[20],ts[20],df[20],tf[20];

    printf("Date début (jj/mm/aaaa) : ");
    scanf("%s",ds);
    sscanf(ds,"%d/%d/%d",&r.debut_date.jour,&r.debut_date.mois,&r.debut_date.annee);

    printf("Heure début (hh:mm) : ");
    scanf("%s",ts);
    sscanf(ts,"%d:%d",&r.debut_temps.heure,&r.debut_temps.minute);

    printf("Date fin (jj/mm/aaaa) : ");
    scanf("%s",df);
    sscanf(df,"%d/%d/%d",&r.fin_date.jour,&r.fin_date.mois,&r.fin_date.annee);

    printf("Heure fin (hh:mm) : ");
    scanf("%s",tf);
    sscanf(tf,"%d:%d",&r.fin_temps.heure,&r.fin_temps.minute);

    int duree = differenceMinutes(r.debut_date,r.debut_temps, r.fin_date,r.fin_temps);

    if(duree<=0){
        printf("Erreur : fin avant début.\n");
        strcpy(r.statut,"refuse");
    }
    else if(!estDisponible(r.salle,r.debut_date,r.debut_temps,r.fin_date,r.fin_temps)){
        printf("Créneau occupé !\n");
        strcpy(r.statut,"refuse");
    }
    else{
        strcpy(r.statut,"confirme");
        r.prix_total = (duree/60.0)*salles[idx].tarif_horaire;
        printf("Réservation confirmée : %.2f DT\n",r.prix_total);
        enregistrerFacture(r);
    }

    // sauvegarde dans tableau
    reservations[nb_res++] = r;

    // sauvegarde dans fichier
    FILE *f = fopen("reservations.txt","a");
    fprintf(f,"%d %s %s %02d/%02d/%04d %02d:%02d %02d/%02d/%04d %02d:%02d %.2f %s\n",
        r.id,r.nom_client,r.salle,
        r.debut_date.jour,r.debut_date.mois,r.debut_date.annee,
        r.debut_temps.heure,r.debut_temps.minute,
        r.fin_date.jour,r.fin_date.mois,r.fin_date.annee,
        r.fin_temps.heure,r.fin_temps.minute,
        r.prix_total,r.statut);
    fclose(f);

    mettreAJourChiffreAffaires();
    mettreAJourStatistiques();
}

/********************************************
 * AFFICHER CATALOGUE
 *******************************************/
void afficherCatalogue(){
    printf("\n===== CATALOGUE SALLES =====\n");
    for(int i=0;i<nb_salles;i++){
        Salle *s=&salles[i];
        printf("%s | %d places | %.2f DT/h\n", s->nom, s->capacite, s->tarif_horaire);
    }
}

/********************************************
 * MENU
 *******************************************/
void menu(){
    int c;
    do{
        printf("\n===== MENU =====\n");
        printf("1. Ajouter une salle\n");
        printf("2. Voir catalogue\n");
        printf("3. Ajouter une réservation\n");
        printf("4. Quitter\n");
        printf("Votre choix : ");
        scanf("%d",&c);

        if(c==1) ajouterSalle();
        else if(c==2) afficherCatalogue();
        else if(c==3) ajouterReservation();

    }while(c!=4);
}

/********************************************
 * MAIN FINAL 4A
 *******************************************/
int main(){

    initialiserSallesParDefaut();
    chargerReservations();

    printf("\n=== SYSTEME DE RESERVATION ===\n");
    printf("%d salles chargées.\n", nb_salles);
    printf("%d réservations chargées.\n", nb_res);

    menu();

    return 0;
}