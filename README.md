# Système de Gestion de Réservations 📅

**Projet de Programmation C Avancée | 2025-2026**
**Sujet :** Projet 3 - Solution de gestion pour espaces de coworking et petites entreprises.


---

## 📝 Description
Ce logiciel est une application console permettant de gérer intégralement la réservation de salles de réunion. Il automatise la vérification des disponibilités, le calcul des coûts et la génération de rapports financiers, répondant ainsi aux besoins de gestion de ressources partagées.

L'application garantit la persistance des données (salles, réservations, factures) et offre une interface utilisateur intuitive.

---

## ✅ Réalisation des Tâches (Conformité Projet 3)

Ce projet implémente l'ensemble des tâches requises par le cahier des charges :

### Tâche 1 : Modèle de Données 
* **Structure Salle :** Gestion complète (Nom, capacité, tarif horaire) avec un dictionnaire dynamique pour les équipements (Dictionnaire).
* **Structure Reservation :** Gestion des créneaux avec timestamps (Date/Heure début et fin), identification client et statut.

### Tâche 2 : Système de Réservation Intelligent 
* *Détection de conflits :* Algorithme vérifiant qu'aucune réservation ne chevauche une autre pour la même salle.
* *Validation :* Vérification de la capacité de la salle et de la cohérence des dates.

### Tâche 3 & 4 : Tarification et Facturation 
* *Calcul automatique :* Le coût total est calculé en temps réel (Tarif_horaire * Durée).
* *Facturation :* Génération automatique de factures détaillées sauvegardées dans le fichier factures.txt.

### Tâche 5 : Statistiques et Reporting 
* Calcul du chiffre d'affaires global et par salle.
* Identification des salles les plus populaires.
* Analyse mensuelle des réservations.

### Tâche 6 : Persistance des Données 
* Sauvegarde automatique des données dans salles.txt et reservations.txt après chaque modification.
* Chargement automatique de l'état du système au lancement.

---

## 🚀 Tâches Supplémentaires (Bonus)

Conformément à la consigne *"Ajouter au minimum une autre tâche"*, deux fonctionnalités de recherche avancée ont été développées :

1.  *Recherche par Capacité (Option 7) :*
    Permet de filtrer les salles pouvant accueillir un nombre minimum de personnes. Idéal pour trouver rapidement une salle pour un grand groupe.

2.  *Recherche par Équipement (Option 8) :*
    Permet de trouver toutes les salles disposant d'un matériel spécifique (ex: "Projecteur", "Tableau blanc") grâce à une recherche par mot-clé dans le dictionnaire des équipements.

---

## 📂 Organisation des Fichiers

| Fichier | Description |
| :--- | :--- |
| **main.c** | Code source complet de l'application (contient le main et les définitions de fonctions). |
| **salles.txt** | Base de données des salles (Format : Nom Capacité Tarif {Equipements}). |
| **reservations.txt** | Historique des réservations (Format : ID Client Salle DateDebut...). |
| **factures.txt** | Registre cumulatif des factures générées pour les clients. |
| **statistiques.txt** | 
| **chiffres_affaires.txt** | 


---

## 🛠️ Compilation et Exécution

### Prérequis
* Compilateur GCC installé.
* Système d'exploitation : Windows

### Instructions
1.  *Compilation :*
    Ouvrez un terminal dans le dossier du projet et exécutez :
    
    gcc "PRO C-1.c" -o gestion_reservations
    


2.  *Lancement :*
    * *Windows :* gestion_reservations.exe


---

## 💻 Guide d'Utilisation (Menu)

L'application propose un menu interactif numéroté :

1.  *Ajouter une salle :* Configuration d'une nouvelle salle.
2.  *Voir catalogue :* Liste toutes les salles disponibles.
3.  *Ajouter une réservation :* Création d'une réservation (Nom client, Salle, Dates).
4.  *Afficher une facture :* Recherche d'une facture client.
5.  *Statistiques complètes :* Rapport d'activité détaillé.
6.  *Chiffre d'affaires par salle :* Focus financier.
7.  *Recherche par capacité :* (Bonus) Filtrage par taille.
8.  *Recherche par équipement :* (Bonus) Filtrage par matériel.
9.  *Quitter :* Sauvegarde et sortie.

---

## 👥 Auteurs
* *Nehdi Oussema*
* *Amar Mohamed*
* * Saad Mohamed Aziz*

Projet réalisé à l'École Nationale des Sciences de l'Informatique (ENSI)
