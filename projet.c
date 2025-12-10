#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SALLES 50
#define MAX_RES 500
#define MAX_EQUIPEMENTS 20

/* STRUCTURES DE DONNÉES */

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

/* VARIABLES GLOBALES */

Salle salles[MAX_SALLES];
Reservation reservations[MAX_RES];
int nb_salles = 0;
int nb_res = 0;

/* FONCTIONS UTILITAIRES POUR DATES ET TEMPS */

int estBissextile(int annee){
    if ((annee % 400) == 0) return 1;
    if ((annee % 100) == 0) return 0;
    if ((annee % 4) == 0) return 1;
    return 0;
}

long joursDepuisOrigine(Date d){
    int moisLenNorm[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    long jours = 0;

    for (int y = 0; y < d.annee; y++)
        jours += 365 + estBissextile(y);

    for (int m = 1; m < d.mois; m++){
        if (m == 2)
            jours += 28 + estBissextile(d.annee);
        else
            jours += moisLenNorm[m-1];
    }

    jours += (d.jour - 1);

    return jours;
}

long differenceMinutes(Date d1, Temps t1, Date d2, Temps t2){
    long m1 = joursDepuisOrigine(d1) * 1440L + t1.heure * 60 + t1.minute;
    long m2 = joursDepuisOrigine(d2) * 1440L + t2.heure * 60 + t2.minute;
    return m2 - m1;
}

int estDateValide(int j, int m, int a) {
    if (a < 1900 || a > 2100) return 0; 
    if (m < 1 || m > 12) return 0; 
    if (j < 1) return 0; 
    int joursDansMois[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (m == 2) {
        if ((a % 4 == 0 && a % 100 != 0) || (a % 400 == 0)) {
            joursDansMois[2] = 29;
        }
    }
    if (j > joursDansMois[m]) return 0;

    return 1; 
}
int estDansLeFutur(Date d, Temps t) {
    time_t now = time(NULL);
    struct tm *curr = localtime(&now);

    Date today = { curr->tm_mday, curr->tm_mon + 1, curr->tm_year + 1900 };
    Temps nowTime = { curr->tm_hour, curr->tm_min };

    long diff = differenceMinutes(today, nowTime, d, t);
    return diff >= 0;
}

/* AJOUTER ÉQUIPEMENT DANS SALLE */
void ajouterEquipement(Salle *s, const char *cle, int quantite) {
    for (int i = 0; i < s->equipements.taille; i++) {
        if (strcmp(cle, s->equipements.items[i].cle) == 0) {
            s->equipements.items[i].quantite += quantite;
            return;
        }
    }
    strcpy(s->equipements.items[s->equipements.taille].cle, cle);
    s->equipements.items[s->equipements.taille].quantite = quantite;
    s->equipements.taille++;
}

/* INITIALISATION SALLES PAR DÉFAUT*/

void initialiserSallesParDefaut(){
    nb_salles = 10;

    strcpy(salles[0].nom, "MeetingRoom1");
    salles[0].capacite = 12;
    salles[0].tarif_horaire = 18;
    salles[0].equipements.taille = 0;
    ajouterEquipement(&salles[0], "chaises", 12);
    ajouterEquipement(&salles[0], "television", 1);
    ajouterEquipement(&salles[0], "tableau_blanc", 1);
    ajouterEquipement(&salles[0], "climatisation", 1);

    strcpy(salles[1].nom, "MeetingRoom2");
    salles[1].capacite = 18;
    salles[1].tarif_horaire = 20;
    salles[1].equipements.taille = 0;
    ajouterEquipement(&salles[1], "chaises", 18);
    ajouterEquipement(&salles[1], "projecteur", 1);
    ajouterEquipement(&salles[1], "haut_parleurs", 2);

    strcpy(salles[2].nom, "StudyRoom1");
    salles[2].capacite = 10;
    salles[2].tarif_horaire = 10;
    salles[2].equipements.taille = 0;
    ajouterEquipement(&salles[2], "tables", 10);
    ajouterEquipement(&salles[2], "chaises", 10);

    strcpy(salles[3].nom, "StudyRoom2");
    salles[3].capacite = 20;
    salles[3].tarif_horaire = 12;
    salles[3].equipements.taille = 0;
    ajouterEquipement(&salles[3], "tables", 20);
    ajouterEquipement(&salles[3], "chaises", 20);
    ajouterEquipement(&salles[3], "television", 1);

    strcpy(salles[4].nom, "ConferenceHall1");
    salles[4].capacite = 120;
    salles[4].tarif_horaire = 40;
    salles[4].equipements.taille = 0;
    ajouterEquipement(&salles[4], "chaises", 120);
    ajouterEquipement(&salles[4], "microphones", 4);
    ajouterEquipement(&salles[4], "haut_parleurs", 6);

    strcpy(salles[5].nom, "ConferenceHall2");
    salles[5].capacite = 80;
    salles[5].tarif_horaire = 35;
    salles[5].equipements.taille = 0;
    ajouterEquipement(&salles[5], "chaises", 80);
    ajouterEquipement(&salles[5], "ecran_geant", 1);

    strcpy(salles[6].nom, "CoworkingSpace1");
    salles[6].capacite = 25;
    salles[6].tarif_horaire = 15;
    salles[6].equipements.taille = 0;
    ajouterEquipement(&salles[6], "bureaux", 25);
    ajouterEquipement(&salles[6], "ordinateurs", 10);

    strcpy(salles[7].nom, "CoworkingSpace2");
    salles[7].capacite = 40;
    salles[7].tarif_horaire = 14;
    salles[7].equipements.taille = 0;
    ajouterEquipement(&salles[7], "bureaux", 40);
    ajouterEquipement(&salles[7], "televisions", 2);

    strcpy(salles[8].nom, "MultiUseRoom1");
    salles[8].capacite = 30;
    salles[8].tarif_horaire = 22;
    salles[8].equipements.taille = 0;
    ajouterEquipement(&salles[8], "tapis_sol", 30);
    ajouterEquipement(&salles[8], "enceintes", 2);

    strcpy(salles[9].nom, "MultiUseRoom2");
    salles[9].capacite = 20;
    salles[9].tarif_horaire = 25;
    salles[9].equipements.taille = 0;
    ajouterEquipement(&salles[9], "ecrans_interactifs", 2);
    ajouterEquipement(&salles[9], "webcams", 2);
}
/* CHARGEMENT DES SALLES DEPUIS FICHIER salles.txt*/

void chargerSalles() {
    FILE *f = fopen("salles.txt", "r");
    if (!f) {
        printf("\033[31m⚠️ Fichier salles.txt introuvable → utilisation des salles par défaut.\033[0m\n");
        initialiserSallesParDefaut();
        return;
    }

    int count = 0;
    fscanf(f, "%d\n", &count);
    if (count <= 0 || count > MAX_SALLES) {
        printf("\033[31m⚠️ Fichier salles.txt corrompu → utilisation des salles par défaut.\033[0m\n");
        fclose(f);
        initialiserSallesParDefaut();
        return;
    }

    nb_salles = count;

    for (int i = 0; i < count; i++) {
        Salle *s = &salles[i];
        s->equipements.taille = 0;
        int nb_eq = 0;
        char cle[50];
        int q;

        fscanf(f, "Salle=%49s\n", s->nom);
        fscanf(f, "Capacite=%d\n", &s->capacite);
        fscanf(f, "Tarif=%f\n", &s->tarif_horaire);
        fscanf(f, "Equipements=%d\n", &nb_eq);

        for (int j = 0; j < nb_eq; j++) {
            fscanf(f, "%49[^=]=%d\n", cle, &q);
            ajouterEquipement(s, cle, q);
        }
        fscanf(f, "---\n");
    }

    fclose(f);
}

/*RECHERCHE SALLE PAR NOM*/

int indexSalle(const char *nom) {
    for (int i = 0; i < nb_salles; i++)
        if (strcmp(salles[i].nom, nom) == 0)
            return i;
    return -1;
}

/*DISPONIBILITÉ*/

int estDisponible(const char *nomSalle, Date d1, Temps t1, Date d2, Temps t2) {

    long startN = differenceMinutes((Date){1,1,0}, (Temps){0,0}, d1, t1);
    long endN   = differenceMinutes((Date){1,1,0}, (Temps){0,0}, d2, t2);

    for (int i = 0; i < nb_res; i++) {

        if (strcmp(reservations[i].salle, nomSalle) == 0) {

            long startE = differenceMinutes((Date){1,1,0}, (Temps){0,0},
                        reservations[i].debut_date,
                        reservations[i].debut_temps);

            long endE   = differenceMinutes((Date){1,1,0}, (Temps){0,0},
                        reservations[i].fin_date,
                        reservations[i].fin_temps);

            if (!(endN <= startE || startN >= endE))
                return 0;
        }
    }
    return 1;
}

/*FACTURE (avec durée F3)*/

void enregistrerFacture(Reservation r) {

    long dureeMin = differenceMinutes(r.debut_date, r.debut_temps,
                                      r.fin_date, r.fin_temps);
    long heures = dureeMin / 60;
    long minutes = dureeMin % 60;

    FILE *f = fopen("factures.txt", "a");
    if (!f) return;

    fprintf(f, "FACTURE %d\n", r.id);
    fprintf(f, "Client : %s\n", r.nom_client);
    fprintf(f, "Salle : %s\n", r.salle);
    fprintf(f, "Début : %02d/%02d/%04d %02d:%02d\n",
            r.debut_date.jour, r.debut_date.mois, r.debut_date.annee,
            r.debut_temps.heure, r.debut_temps.minute);
    fprintf(f, "Fin   : %02d/%02d/%04d %02d:%02d\n",
            r.fin_date.jour, r.fin_date.mois, r.fin_date.annee,
            r.fin_temps.heure, r.fin_temps.minute);

    fprintf(f, "Durée : %ld heures %ld minutes\n", heures, minutes);
    fprintf(f, "Montant : %.2f DT\n", r.prix_total);
    fprintf(f, "-------------------------\n");

    fclose(f);
}

/* CHARGEMENT DES RESERVATIONS EXISTANTES*/

void chargerReservations(){
    FILE *f = fopen("reservations.txt","r");
    if(!f) return;

    while(!feof(f) && nb_res < MAX_RES){
        Reservation r;
        int read = fscanf(f,"%d %49s %49s %d/%d/%d %d:%d "
                            "%d/%d/%d %d:%d %f %19s",
            &r.id, r.nom_client, r.salle,
            &r.debut_date.jour,&r.debut_date.mois,&r.debut_date.annee,
            &r.debut_temps.heure,&r.debut_temps.minute,
            &r.fin_date.jour,&r.fin_date.mois,&r.fin_date.annee,
            &r.fin_temps.heure,&r.fin_temps.minute,
            &r.prix_total, r.statut);

        if(read >= 14){
            reservations[nb_res++] = r;
        } else {
            char buf[256];
            fgets(buf, sizeof(buf), f);
        }
    }
    fclose(f);
}

/* MISE À JOUR CA & STATISTIQUES (déclarations)*/

void mettreAJourChiffreAffaires();
void mettreAJourStatistiques();

/*AJOUT RESERVATION*/

void ajouterReservation() {

    Reservation r;
    r.id = (nb_res==0?1:reservations[nb_res-1].id+1);

    printf("Nom client : ");
    scanf("%49s", r.nom_client);

    printf("Salle : ");
    scanf("%49s", r.salle);

    int idx = indexSalle(r.salle);
    if (idx == -1) {
        printf("\033[31m❌ Salle introuvable !\033[0m \n");
        return;
    }

    int num_personnes;
    printf("Nombre de personnes : ");
    scanf("%d", &num_personnes);
    if (num_personnes > salles[idx].capacite){
        printf("\033[31m❌ Capacité insuffisante !\033[0m \n");
        return;
    }
    if(num_personnes <=0){
        printf("\033[31m❌ Nombre de personnes invalide !\033[0m \n");
        return;
    }

    char ds[20], ts[20], df[20], tf[20];

    printf("Date début (jj/mm/aaaa) : ");
    scanf("%19s", ds);
    
    if (sscanf(ds, "%d/%d/%d", &r.debut_date.jour, &r.debut_date.mois, &r.debut_date.annee) == 3) {

        if (estDateValide(r.debut_date.jour, r.debut_date.mois, r.debut_date.annee)) {
            printf("\033[32m Date valide enregistrée :\033[0m  %02d/%02d/%d\n",r.debut_date.jour, r.debut_date.mois, r.debut_date.annee);

            
        } else {
            printf("\033[31m Erreur : La date %s n'existe pas dans le calendrier.\033[0m \n", ds);
            return;
        }

    } else {
        printf("\033[31m Erreur : Format invalide. Veuillez utiliser jj/mm/aaaa.\033[0m \n");
        return;
    }


    printf("Heure début (hh:mm) : ");
    scanf("%19s", ts);

    if(sscanf(ts, "%d:%d", &r.debut_temps.heure, &r.debut_temps.minute) == 2){
        if(r.debut_temps.heure <0 || r.debut_temps.heure >23 ||
           r.debut_temps.minute <0 || r.debut_temps.minute >59){
            printf("\033[31m Erreur : Heure invalide.\033[0m \n");
            return;
        }
    } else {
        printf("\033[31m Erreur : Format invalide. Veuillez utiliser hh:mm.\033[0m \n");
        return;
    }
    if (!estDansLeFutur(r.debut_date, r.debut_temps)) {
    printf("\n\033[31m ❌ Erreur : impossible de réserver dans le passé.\033[0m \n");
    return;
    }

    printf("Date fin (jj/mm/aaaa) : ");
    scanf("%19s", df);
    sscanf(df, "%d/%d/%d", &r.fin_date.jour, &r.fin_date.mois, &r.fin_date.annee);
    if (sscanf(df, "%d/%d/%d", &r.fin_date.jour, &r.fin_date.mois, &r.fin_date.annee) == 3) {

        if (estDateValide(r.fin_date.jour, r.fin_date.mois, r.fin_date.annee)) {
            printf("\033[32m Date valide enregistrée :\033[0m  %02d/%02d/%d\n",r.fin_date.jour, r.fin_date.mois, r.fin_date.annee);

            
        } else {
            printf("\033[31m Erreur : La date %s n'existe pas dans le calendrier.\033[0m \n", ds);
            return;
        }

    } else {
        printf("\033[31m Erreur : Format invalide. Veuillez utiliser jj/mm/aaaa.\033[0m \n");
        return;
    }


    printf("Heure fin (hh:mm) : ");
    scanf("%19s", tf);
    if(sscanf(tf, "%d:%d", &r.fin_temps.heure, &r.fin_temps.minute) == 2){
        if(r.fin_temps.heure <0 || r.fin_temps.heure >23 ||
           r.fin_temps.minute <0 || r.fin_temps.minute >59){
            printf("\033[31m Erreur : Heure invalide.\033[0m \n");
            return;
        }
    } else {
        printf("\033[31m Erreur : Format invalide. Veuillez utiliser hh:mm.\033[0m \n");
        return;
    }


    long duree = differenceMinutes(r.debut_date, r.debut_temps,r.fin_date, r.fin_temps);

    if (duree <= 0) {
        printf("\033[31m ❌ Erreur : fin avant début.\033[0m \n");
        strcpy(r.statut, "refuse");
    }
    else if (!estDisponible(r.salle, r.debut_date, r.debut_temps, r.fin_date, r.fin_temps)) {

        printf("❌\033[31m  Créneau occupé !\033[0m \n");
        strcpy(r.statut, "refuse");
    }
    else {
        strcpy(r.statut, "confirme");
        r.prix_total = (duree / 60.0) * salles[idx].tarif_horaire;
        printf("\033[32m ✔ Réservation confirmée :\033[0m  %.2f DT\n", r.prix_total);
        enregistrerFacture(r);
    }

    reservations[nb_res++] = r;

    FILE *f = fopen("reservations.txt", "a");
    if (f) {
        fprintf(f, "%d %s %s %02d/%02d/%04d %02d:%02d "
                    "%02d/%02d/%04d %02d:%02d %.2f %s\n",
                r.id, r.nom_client, r.salle,
                r.debut_date.jour, r.debut_date.mois, r.debut_date.annee,
                r.debut_temps.heure, r.debut_temps.minute,
                r.fin_date.jour, r.fin_date.mois, r.fin_date.annee,
                r.fin_temps.heure, r.fin_temps.minute,
                r.prix_total, r.statut);
        fclose(f);
    }

    mettreAJourChiffreAffaires();
    mettreAJourStatistiques();
}

/*AJOUT DES 5 RESERVATIONS PAR DÉFAUT (une seule fois)*/

void ajouterReservationsParDefautSiFichierVide() {

    FILE *f = fopen("reservations.txt", "r");

    if (f) {
        int c = fgetc(f);
        fclose(f);
        if (c != EOF) return;   // fichier NON vide → ne rien ajouter
    }

    printf("Ajout des 5 réservations par défaut...\n");

    char *noms[5] = {
        "Entreprise_Talan",
        "Entreprise_Siemens",
        "Prof_Nader",
        "Prof_Ahmed",
        "Asso_Robotique_ENSI"
    };

    char salle[50] = "CoworkingSpace1";

    if (indexSalle(salle) == -1)
        strcpy(salle, salles[0].nom);

    for (int i = 0; i < 5; i++) {

        Reservation r;
        r.id = (nb_res==0?1:reservations[nb_res-1].id+1);

        strcpy(r.nom_client, noms[i]);
        strcpy(r.salle, salle);

        r.debut_date = (Date){1, 1, 2025};
        r.debut_temps = (Temps){8, 0};
        r.fin_date   = (Date){1, 1, 2025};
        r.fin_temps  = (Temps){12, 0};

        int idx = indexSalle(salle);
        r.prix_total = 4 * salles[idx].tarif_horaire;

        strcpy(r.statut, "confirme");

        reservations[nb_res++] = r;
        enregistrerFacture(r);

        FILE *fw = fopen("reservations.txt", "a");
        fprintf(fw, "%d %s %s %02d/%02d/%04d %02d:%02d "
                "%02d/%02d/%04d %02d:%02d %.2f %s\n",
                r.id, r.nom_client, r.salle,
                r.debut_date.jour, r.debut_date.mois, r.debut_date.annee,
                r.debut_temps.heure, r.debut_temps.minute,
                r.fin_date.jour, r.fin_date.mois, r.fin_date.annee,
                r.fin_temps.heure, r.fin_temps.minute,
                r.prix_total, r.statut);
        fclose(fw);
    }
}

/*AFFICHAGE FACTURE PAR RECHERCHE*/
void afficherFactureParInfos() {

    char nom[50], salleNom[50], ds[20], ts[20];
    Date d;
    Temps t;

    printf("Nom client : ");
    scanf("%49s", nom);

    printf("Salle : ");
    scanf("%49s", salleNom);

    printf("Date début (jj/mm/aaaa) : ");
    scanf("%19s", ds);
    sscanf(ds, "%d/%d/%d", &d.jour, &d.mois, &d.annee);

    printf("Heure début (hh:mm) : ");
    scanf("%19s", ts);
    sscanf(ts, "%d:%d", &t.heure, &t.minute);

    for (int i = 0; i < nb_res; i++) {

        Reservation *r = &reservations[i];

        if (strcmp(r->nom_client, nom) == 0 &&
            strcmp(r->salle, salleNom) == 0 &&
            r->debut_date.jour == d.jour &&
            r->debut_date.mois == d.mois &&
            r->debut_date.annee == d.annee &&
            r->debut_temps.heure == t.heure &&
            r->debut_temps.minute == t.minute) {

            printf("\n\033[34m === FACTURE TROUVÉE ===\033[0m \n");
            printf("Client : %s\n", r->nom_client);
            printf("Salle  : %s\n", r->salle);
            printf("Prix   : %.2f DT\n", r->prix_total);

            enregistrerFacture(*r);
            return;
        }
    }

    printf("\033[31m ❌ Aucune facture trouvée.\033[0m \n");
}

/*STATISTIQUES (mode complet — écrasement)*/

void statistiques() {

    FILE *f = fopen("statistiques.txt", "w");
    if (!f) return;

    fprintf(f, "\033[34m ===== STATISTIQUES SYSTÈME =====\033[0m \n");

    float totalCA = 0;
    int maxRes = -1;
    char sallePop[50] = "";
    int moisStats[13] = {0};

    for (int i = 0; i < nb_salles; i++) {
        float ca = 0;
        int count = 0;

        for (int j = 0; j < nb_res; j++) {

            Reservation *r = &reservations[j];

            if (strcmp(r->salle, salles[i].nom) == 0 &&
                strcmp(r->statut, "confirme") == 0) {

                ca += r->prix_total;
                count++;

                if (r->debut_date.mois >= 1 && r->debut_date.mois <= 12)
                    moisStats[r->debut_date.mois]++;
            }
        }

        fprintf(f, "%s : %.2f DT (%d réservations)\n",
                salles[i].nom, ca, count);

        if (count > maxRes) {
            maxRes = count;
            strcpy(sallePop, salles[i].nom);
        }

        totalCA += ca;
    }

    fprintf(f, "\nSalle la plus populaire : %s (%d réservations)\n",
            sallePop, maxRes);

    fprintf(f, "\nRéservations par mois :\n");
    for (int m = 1; m <= 12; m++)
        fprintf(f, "Mois %02d : %d réservations\n", m, moisStats[m]);

    fprintf(f, "\nChiffre d'affaires total : %.2f DT\n", totalCA);

    fclose(f);

    printf("Statistiques mises à jour.\n");
}

/* AFFICHAGE CHIFFRE D'AFFAIRES PAR SALLE*/

void afficherChiffreAffaireParSalle(){
    printf("\n\033[34m ===== CHIFFRE D'AFFAIRES PAR SALLE =====\033[0m \n");

    float totalCA = 0;

    for(int i=0;i<nb_salles;i++){
        float ca = 0;
        for(int j=0;j<nb_res;j++){
            if(strcmp(reservations[j].salle, salles[i].nom)==0 &&
                strcmp(reservations[j].statut,"confirme")==0)
                ca += reservations[j].prix_total;
        }
        printf("%s : %.2f DT\n", salles[i].nom, ca);
        totalCA += ca;
    }

    printf("TOTAL : %.2f DT\n", totalCA);
}

/*MISE À JOUR TEMPORAIRE (append)*/

void mettreAJourChiffreAffaires(){
    FILE *f = fopen("chiffre_affaires.txt","w");
    if(!f) return;

    float total=0;

    fprintf(f," ===== CHIFFRE D'AFFAIRES ===== \n");

    for(int i=0;i<nb_salles;i++){
        float ca = 0;
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

/*MISE À JOUR STATISTIQUES (append)*/
void mettreAJourStatistiques() {
    FILE *f = fopen("statistiques.txt", "a");
    if (!f) return;

    fprintf(f, "\n ===== NOUVELLE RÉSERVATION ===== \n");

    int mois[13]={0};
    int max=0;
    char pop[50]="";

    for(int i=0;i<nb_salles;i++){
        int cnt=0;

        for(int j=0;j<nb_res;j++){
            if(strcmp(reservations[j].salle, salles[i].nom)==0 &&
               strcmp(reservations[j].statut,"confirme")==0){

                cnt++;

                if (reservations[j].debut_date.mois >= 1 &&
                    reservations[j].debut_date.mois <= 12)
                    mois[reservations[j].debut_date.mois]++;
            }
        }

        if(cnt > max){
            max = cnt;
            strcpy(pop, salles[i].nom);
        }
    }

    fprintf(f,"Salle la plus populaire : %s (%d réservations)\n",
            pop, max);

    fprintf(f,"Reservations par mois :\n");
    for(int m=1;m<=12;m++)
        fprintf(f,"Mois %02d : %d\n", m, mois[m]);

    fclose(f);
}

/*AJOUT SALLE + CATALOGUE*/
void ajouterSalle() {
    if (nb_salles >= MAX_SALLES) {
        printf("\033[31m ❌ Limite atteinte.\033[0m \n");
        return;
    }

    Salle *s = &salles[nb_salles];
    s->equipements.taille = 0;

    printf("Nom salle : ");
    scanf("%49s", s->nom);

    printf("Capacité : ");
    scanf("%d", &s->capacite);

    printf("Tarif horaire : ");
    scanf("%f", &s->tarif_horaire);

    // Saisie des équipements
    int nbEq = 0;
    printf("Nombre d'équipements : ");
    scanf("%d", &nbEq);
    if (nbEq > MAX_EQUIPEMENTS) nbEq = MAX_EQUIPEMENTS;

    for (int i = 0; i < nbEq; i++) {
        char cle[50];
        int q;
        printf("Nom équipement %d : ", i+1);
        scanf("%49s", cle);
        printf("Quantité : ");
        scanf("%d", &q);
        ajouterEquipement(s, cle, q);  // Utilise la fonction existante
    }

    nb_salles++;

    // Sauvegarde toutes les salles dans salles.txt
    FILE *f = fopen("salles.txt", "w");
    if (!f) {
        printf("❌ Impossible d'ouvrir salles.txt pour écriture.\n");
        return;
    }

    fprintf(f, "%d\n", nb_salles);  // nombre de salles

    for (int i = 0; i < nb_salles; i++) {
        Salle *sal = &salles[i];
        fprintf(f, "Salle=%s\n", sal->nom);
        fprintf(f, "Capacite=%d\n", sal->capacite);
        fprintf(f, "Tarif=%.1f\n", sal->tarif_horaire);
        fprintf(f, "Equipements=%d\n", sal->equipements.taille);
        for (int j = 0; j < sal->equipements.taille; j++) {
            fprintf(f, "%s=%d\n", sal->equipements.items[j].cle,
                                   sal->equipements.items[j].quantite);
        }
        fprintf(f, "---\n");
    }

    fclose(f);

    printf("\033[32m ✔ Salle ajoutée et sauvegardée avec succès.\033[0m \n");
}

/* AFFICHAGE CATALOGUE DES SALLES*/

void afficherCatalogue() {
    printf("\n\033[34m ===== CATALOGUE DES SALLES =====\033[0m \n");

    FILE *f = fopen("salles.txt", "r");
    if (!f) {
        printf("⚠️\033[31m  Fichier salles.txt introuvable. Aucun catalogue disponible.\033[0m \n");
        return;
    }

    int totalSalles = 0;
    if (fscanf(f, "%d\n", &totalSalles) != 1 || totalSalles <= 0) {
        printf("⚠️\033[31m  Fichier salles.txt corrompu.\033[0m \n");
        fclose(f);
        return;
    }

    for (int i = 0; i < totalSalles; i++) {
        Salle s;
        s.equipements.taille = 0;

        int nbEq = 0;
        char cle[50];
        int q;

        if (fscanf(f, "Salle=%49s\n", s.nom) != 1) break;
        if (fscanf(f, "Capacite=%d\n", &s.capacite) != 1) break;
        if (fscanf(f, "Tarif=%f\n", &s.tarif_horaire) != 1) break;
        if (fscanf(f, "Equipements=%d\n", &nbEq) != 1) break;

        for (int j = 0; j < nbEq && j < MAX_EQUIPEMENTS; j++) {
            if (fscanf(f, "%49[^=]=%d\n", cle, &q) != 2) break;
            ajouterEquipement(&s, cle, q);
        }

        // Lire la ligne séparatrice ---
        char sep[10];
        fscanf(f, "%9s\n", sep);

        // Affichage
        printf("\n%s (%d places) - %.2f DT/h\n", s.nom, s.capacite, s.tarif_horaire);
        if (s.equipements.taille > 0) {
            printf("Équipements : ");
            for (int e = 0; e < s.equipements.taille; e++) {
                printf("%s(%d) ", s.equipements.items[e].cle, s.equipements.items[e].quantite);
            }
            printf("\n");
        }
    }

    fclose(f);
}


/*⭐ RECHERCHE PAR CAPACITÉ (F1)*/
void rechercherParCapacite(){
    int cap;
    printf("Capacité minimum recherchée : ");
    scanf("%d", &cap);

    printf("\nSalles correspondant :\n");

    int trouve = 0;
    for(int i=0;i<nb_salles;i++){
        if(salles[i].capacite >= cap){
            printf("- %s (%d places)\n",
                    salles[i].nom, salles[i].capacite);
            trouve = 1;
        }
    }

    if(!trouve)
        printf("\033[31m❌ Aucune salle trouvée.\033[0m\n");
}

/* ⭐ RECHERCHE PAR ÉQUIPEMENT (F2)*/
void rechercherParEquipement(){
    char eq[50];
    printf("Nom équipement : ");
    scanf("%49s", eq);

    printf("\nSalles contenant '%s' :\n", eq);

    int trouve = 0;

    for(int i=0;i<nb_salles;i++){
        for(int j=0;j<salles[i].equipements.taille;j++){
            if(strcmp(salles[i].equipements.items[j].cle, eq)==0){
                printf("- %s (%d disponibles)\n",
                       salles[i].nom,
                       salles[i].equipements.items[j].quantite);
                trouve = 1;
            }
        }
    }

    if(!trouve)
        printf("\033[31m ❌ Aucun résultat.\033[0m \n");
}

/* MENU PRINCIPAL*/
void menu() {

    int choix;

    do {
       printf("\n \033[34m===== Sélectionner Votre Commande =====  \n \033[0m\n");
        printf("1. Ajouter une salle\n");
        printf("2. Voir catalogue\n");
        printf("3. Ajouter une réservation\n");
        printf("4. Afficher une facture\n");
        printf("5. Statistiques complètes\n");
        printf("6. Chiffre d'affaires par salle\n");
        printf("7. Recherche par capacité\n");
        printf("8. Recherche par équipement\n");
        printf("9. Quitter\n");
        printf("Votre choix : ");

        scanf("%d", &choix);

        switch (choix) {
            case 1: ajouterSalle(); break;
            case 2: afficherCatalogue(); break;
            case 3: ajouterReservation(); break;
            case 4: afficherFactureParInfos(); break;
            case 5: statistiques(); break;
            case 6: afficherChiffreAffaireParSalle(); break;
            case 7: rechercherParCapacite(); break;
            case 8: rechercherParEquipement(); break;
            case 9: printf("Au revoir !\n"); break;
            default: printf("Choix invalide.\n");
        }

    } while (choix != 9);
}

/*MAIN PROGRAMME*/
int main() {

    chargerSalles();
    chargerReservations();
    ajouterReservationsParDefautSiFichierVide();

    printf("\n\033[34m=== SYSTEME DE RESERVATION CHARGÉ ===\033[0m\n");
    printf("%d salles disponibles.\n", nb_salles);
    printf("%d réservations chargées.\n", nb_res);

    menu();
    return 0;
}
