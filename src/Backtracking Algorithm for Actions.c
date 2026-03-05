#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
const double tolerance=1e-9;
typedef struct {
    char nom[50];
    double prix;
    double rendement;
} action;

typedef struct Ensemble {
    int nb;
    action* tab;
} ensemble;

typedef struct Recueil {
    action x;
    struct Recueil* suivant;
} recueil_actions;

typedef recueil_actions* selection;

recueil_actions* ajouterElement(recueil_actions* alpha, action a) {
    recueil_actions* resultat=malloc(sizeof(recueil_actions));
    resultat->x=a;
    resultat->suivant=alpha;
    return resultat;
}

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

selection copierSelection(selection src) {
    if (src == NULL) return NULL;
    selection new = malloc(sizeof(recueil_actions));
    new->x = src->x;
    new->suivant = copierSelection(src->suivant);
    return new;
}


void libererSelection(selection s) {
    while (s!=NULL) {
        selection temp = s;
        s = s->suivant;
        free(temp);
    }
}

bool possible(action a, double budgetRestant ) {
    if (a.prix-budgetRestant<tolerance) {
        return true;
    }
    return false;
}

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
selection trouverMeilleureSelection(ensemble S, double budget) {
    selection meilleure = NULL;
    double meilleurRendement=-tolerance;
    backtrack(S, 0, NULL, budget, 0.0, &meilleure, &meilleurRendement);
    return meilleure;
}


int main()
{
    printf("Hello World");

    return 0;
}
