#include <stdio.h>
#include <stdlib.h>
#include <math.h>
const double tolerance=1e-9;
typedef struct {
    char type;
    double strike;
    double price;
} Option;


double S_grid[] = {
    80, 82, 84, 86, 88, 90, 92, 94, 96, 98,
    100, 102, 104, 106, 108, 110, 112, 114, 116, 118, 120
};
int N = sizeof(S_grid) / sizeof(double);


Option options[] = {
    {'C', 90,  1.0},
    {'C', 100, 10.0},
    {'C', 110, 15.0},
    {'P', 100,  5.0},
    {'P', 110,  2.0}
};
int nb_options = sizeof(options) / sizeof(Option);

int max_q = 10;


double payoff_call(double S, double K) {
    return (S-K<tolerance) ?  0.0 : (S-K);
}

double payoff_put(double S, double K) {
    return (K-S<tolerance) ?  0.0 : (K-S);
}

double portfolio_payoff(int quantities[], int n, Option options[], double S) {
    double resultat=0.0;
    for (int i=0; i<n; i++) {
        if (options[i].type=='C') {
            resultat+=quantities[i]*payoff_call(S, options[i].strike );
        }
        else if (options[i].type=='P') {
            resultat+=quantities[i]*payoff_put(S, options[i].strike);
        }
    }
    return resultat;
}

double portfolio_cost(int quantities[], int n, Option options[]) {
    double resultat=0;
    for (int i=0; i<n; i++) {
        resultat+= quantities[i]*options[i].price;
    }
    return resultat;
}


int is_arbitrage(int quantities[], int n, Option options[], double grid[], int N) {
    for (int i=0; i<N; i++) {
        if (portfolio_payoff(quantities, n, options, grid[i]) < -tolerance) {
            return 0;
        }
    }
    if (portfolio_cost(quantities, n, options)<-tolerance) return 1;
    return 0;
}


void backtrack(int pos, int n, int current[],
               Option options[], double grid[], int N, int max_q,
               int best_quantities[], double *best_cost) {
    if (pos == n) {
        if (is_arbitrage(current, n, options, grid, N)) {
            double cost = portfolio_cost(current, n, options);
            if (cost < *best_cost) {
                *best_cost = cost;
                for (int i = 0; i < n; i++)
                    best_quantities[i] = current[i];
            }
        }
        return;
    }
    for (int q = -max_q; q <= max_q; q++) {
        current[pos] = q;
        backtrack(pos + 1, n, current, options, grid, N, max_q, best_quantities, best_cost);
    }
}


void find_best_arbitrage(Option options[], int n,
                         double grid[], int N, int max_q,
                         int best_quantities[], double* best_cost) {
    *best_cost=1e9;
    int current[n];
    backtrack(0, n, current, options, grid, N, max_q, best_quantities, best_cost);
}

int main() {
    int best_quantities[nb_options];
    double best_cost;

    find_best_arbitrage(options, nb_options, S_grid, N, max_q, best_quantities, &best_cost);

    if (best_cost >= 1e8) {  // si aucun arbitrage trouvé (proche de l'initial)
        printf("Aucune opportunité d'arbitrage trouvée.\n");
    } else {
        printf("Meilleur arbitrage trouvé :\nQuantités : [");
        for (int i = 0; i < nb_options; i++)
            printf("%d%s", best_quantities[i], (i < nb_options-1) ? ", " : "");
        printf("]\nCoût initial = %f\n", best_cost);
    }
    return 0;
}
