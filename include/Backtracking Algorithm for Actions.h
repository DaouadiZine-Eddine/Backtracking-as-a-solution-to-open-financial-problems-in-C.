#ifndef STOCK_SELECTION_H
#define STOCK_SELECTION_H

#include <stdbool.h>   

typedef struct {
    char nom[50];      
    double prix;       
    double rendement;  
} action;


typedef struct {
    int nb;            
    action* tab;       
} ensemble;

typedef struct Recueil {
    action x;                 
    struct Recueil* suivant;  
} recueil_actions;


typedef recueil_actions* selection;


recueil_actions* ajouterElement(recueil_actions* alpha, action a);


recueil_actions* retirerElement(recueil_actions* alpha, action a);

selection copierSelection(selection src);


void libererSelection(selection s);


bool possible(action a, double budgetRestant);


selection trouverMeilleureSelection(ensemble S, double budget);

#endif 
