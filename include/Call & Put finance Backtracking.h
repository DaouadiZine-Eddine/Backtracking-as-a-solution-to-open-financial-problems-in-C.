#ifndef ARBITRAGE_H
#define ARBITRAGE_H
typedef struct {
    char type;      // 'C' for call, 'P' for put
    double strike;
    double price;
} Option;


double payoff_call(double S, double K);
double payoff_put(double S, double K);
double portfolio_payoff(int quantities[], int n, Option options[], double S);
double portfolio_cost(int quantities[], int n, Option options[]);
int is_arbitrage(int quantities[], int n, Option options[], double grid[], int N);
void find_best_arbitrage(Option options[], int n,
                         double grid[], int N, int max_q,
                         int best_quantities[], double* best_cost);

#endif
