#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
// Tolérance pour les flottants.
const double tolerance=1e-9;

//Structure de données représentant une action. 
typedef struct {
    char nom[50];
    double prix;
    double rendement;
} action;

//Structure de données représentant une liste d'actions. 
typedef struct Ensemble {
    int nb;
    action* tab;
} ensemble;

//Structure de données représentant un recueil d'action. 
typedef struct Recueil {
    action x;
    struct Recueil* suivant;
} recueil_actions;

typedef recueil_actions* selection;

//Fonction permettant d'ajouter une action à un recueil d'actions.
recueil_actions* ajouterElement(recueil_actions* alpha, action a) {
    recueil_actions* resultat=malloc(sizeof(recueil_actions));
    resultat->x=a;
    resultat->suivant=alpha;
    return resultat;
}
//Fonction permettant de retirer une action à un recueil d'actions.
recueil_actions* retirerElement(recueil_actions* alpha, action a) {
    if (alpha==NULL) return alpha;
    if (strcmp(alpha->x.nom , a.nom)==0) {
        recueil_actions* temp=alpha;
        alpha=alpha->suivant;
        free(temp);
        return alpha;
    }
    alpha->suivant=retirerElement(alpha->suivant, a);
    return alpha;
}

//Fonction permettant de copier une selection. 
selection copierSelection(selection src) {
    if (src == NULL) return NULL;
    selection new = malloc(sizeof(recueil_actions));
    new->x = src->x;
    new->suivant = copierSelection(src->suivant);
    return new;
}

//Fonction permettant de libérer la mémoire allouée par une sélection. 
void libererSelection(selection s) {
    while (s!=NULL) {
        selection temp = s;
        s = s->suivant;
        free(temp);
    }
}
//Fonction permettant de vérifier si on peut acheter une action avec un budget donné. 
bool possible(action a, double budgetRestant ) {
    if (a.prix-budgetRestant<tolerance) {
        return true;
    }
    return false;
}

//Fonction de backtracking, trouvant à partir de plusieurs paramètres la solution optimale.
void backtrack(ensemble S, int index, selection courante, double budgetRestant, double rendementCourant, selection* meilleure, double* meilleurRendement) {
    if (index>=S.nb) return;
    backtrack(S, index + 1, courante, budgetRestant, rendementCourant, meilleure, meilleurRendement);
    if (possible(S.tab[index], budgetRestant)) {
        selection nouvelle = copierSelection(courante);
        nouvelle = ajouterElement(nouvelle, S.tab[index]);
        double nouveauBudget = budgetRestant - S.tab[index].prix;
        double nouveauRendement = rendementCourant + S.tab[index].rendement;
        if (nouveauRendement > *meilleurRendement + tolerance) {
            libererSelection(*meilleure);
            *meilleure = copierSelection(nouvelle);
            *meilleurRendement = nouveauRendement;
        }
        backtrack(S, index + 1, nouvelle, nouveauBudget, nouveauRendement, meilleure, meilleurRendement);
        libererSelection(nouvelle);
    }
}
//Fonction effectuant l'pplication de la fonction de backtracking à un budget et un ensemble S d'actions.
selection trouverMeilleureSelection(ensemble S, double budget) {
    selection meilleure = NULL;
    double meilleurRendement=-tolerance;
    backtrack(S, 0, NULL, budget, 0.0, &meilleure, &meilleurRendement);
    return meilleure;
}

//Partie principale où l'on effectue des tests. 
int main()
{
    //On crée une action qui vas servir aux tests.
    //Test 1. 
    action test;
    strcpy(test.nom, "Action 1.");
    test.prix=50.2;
    test.rendement=10.3; 
    //Test 2.
    action test2;
    strcpy(test2.nom, "Action 2.");
    test2.prix=50;
    test2.rendement=25; 
    //Test 3.
    action test3;
    strcpy(test3.nom, "Action 3.");
    test3.prix=100;
    test3.rendement=83.2; 
    //On crée un ensemble de portefeuilles. 
    ensemble portefeuille;
    portefeuille.nb=3;
    portefeuille.tab=malloc(portefeuille.nb*sizeof(action)); 
    portefeuille.tab[0]=test;
    portefeuille.tab[1]=test2;
    portefeuille.tab[2]=test3;
    //On effectue des tests.
    bool test_premier=possible(test, 50.2);
    bool test_second=possible(test2, 50.2);
    bool test_troisième=possible(test3, 50.2);
    if (test_premier==true) {
        printf("TEST 1 : On peut acheter.");
        printf("\n"); 
    }
    else {
        printf("TEST 1 : On ne peut pas acheter..."); 
        printf("\n"); 
    }
    if (test_second==true) {
        printf("TEST 2 : On peut acheter.");
        printf("\n"); 
    }
    else {
        printf("TEST 2 : On ne peut pas acheter..."); 
        printf("\n"); 
    }
    if (test_troisième==true) {
        printf("TEST 3 : On peut acheter.");
        printf("\n"); 
    }
    else {
        printf("TEST 3 : On ne peut pas acheter..."); 
        printf("\n"); 
    }
    if (test_premier && test_second && test_troisième) {
        printf("Tout les tests marchent : nous pouvons acheter n'importe quelle action."); 
        printf("\n"); 
    }
    else {
        printf("Nous ne pouvons pas acheter au moins une des actions disponibles avec le budget restant..."); 
        printf("\n"); 
    }
    // On teste avec un recueil. 
    recueil_actions a;
    selection liste = NULL;
    liste = ajouterElement(liste, test);
    liste = ajouterElement(liste, test2);
    liste = ajouterElement(liste, test3);
    printf("Les actions du recueil sont :"); 
    for (selection cur = liste; cur != NULL; cur = cur->suivant)
        printf("%s ", cur->x.nom);
        printf("\n");
    libererSelection(liste); 
    printf("\n"); 
    printf("La meilleure selection d'actions est :"); 
    printf("\n"); 
    //On test l'algorithme de backtracking. 
    selection meilleur_choix=trouverMeilleureSelection(portefeuille, 100); 
    if (meilleur_choix==NULL) {
        printf("Aucune action possible. ");
        printf("\n"); 
    }
    else {
        for (selection cur=meilleur_choix; cur!=NULL; cur=cur->suivant) {
            printf("%s ", cur->x.nom); 
            printf("\n"); 
        }
    }
    printf("\n"); 
    if (meilleur_choix != NULL && strcmp(meilleur_choix->x.nom, "Action 3.") == 0) {
        printf("L'algorithme fonctionne."); 
    }
    else {
        printf("Erreur de programmation."); 
    }
    libererSelection(meilleur_choix);
    free(portefeuille.tab);
    return 0;
}
