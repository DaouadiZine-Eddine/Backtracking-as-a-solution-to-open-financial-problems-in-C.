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

int min_transactions(double balances[], int n);

#endif
