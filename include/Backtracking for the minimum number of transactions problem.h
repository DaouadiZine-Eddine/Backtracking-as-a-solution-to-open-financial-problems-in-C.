#ifndef MIN_TRANSACTIONS_H
#define MIN_TRANSACTIONS_H

#include <stdbool.h>

typedef struct {
    double id;
    double balance;
} person;

typedef struct List_Person {
    person p;
    struct List_Person* next;
} person_list;

person_list* add_person(person_list* l, person p);
person_list* remove_person(person_list* l, person p);
bool search_person(person_list* l, double id);

void set_balances(double new_balances[], int n);
bool is_possible(int i, int j);
double perform_transaction(int i, int j);
void cancel_transaction(int i, int j, double amount);
bool is_final_state(void);
void backtrack(int transaction_count, int* best_found);

#endif
