#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// Global data
double balances[] = {2, 3, 5}; // modify as needed
const double tolerance = 1e-9;
int count = sizeof(balances) / sizeof(double); // number of persons

// Data types
typedef struct Person {
    double id;
    double balance;
} person;

typedef struct List_Person {
    person p;
    struct List_Person* next;
} person_list;


person_list* add_person(person_list* l, person p) {
    person_list* result = malloc(sizeof(person_list));
    result->p = p;
    result->next = l;
    return result;
}


person_list* remove_person(person_list* l, person p) {
    if (l == NULL) return l;
    else if (fabs(l->p.id - p.id) <= tolerance && fabs(l->p.balance - p.balance) <= tolerance) {
        person_list* to_free = l;
        l = l->next;
        free(to_free);
    } else {
        l->next = remove_person(l->next, p);
    }
    return l;
}


bool search_person(person_list* l, double id) {
    if (l == NULL) return false;
    else if (fabs(l->p.id - id) <= tolerance) return true;
    return search_person(l->next, id);
}

bool is_possible(int i, int j) {
    if (balances[i] < -tolerance && balances[j] > tolerance) return true;
    return false;
}


double perform_transaction(int i, int j) {
    double amount = (-balances[i] < balances[j]) ? -balances[i] : balances[j];
    balances[i] += amount;
    balances[j] -= amount;
    return amount;
}


void cancel_transaction(int i, int j, double amount) {
    if (i < 0 || i >= count || j < 0 || j >= count) exit(EXIT_FAILURE);
    balances[i] -= amount;
    balances[j] += amount;
}


bool is_final_state() {
    int non_zero = 0;
    for (int i = 0; i < count; i++) {
        if (fabs(balances[i]) > tolerance) non_zero++;
    }
    return non_zero <= 1;
}


void backtrack(int transaction_count, int* best_found) {
    if (transaction_count >= *best_found) return;

    if (is_final_state()) {
        if (transaction_count < *best_found) {
            *best_found = transaction_count;
        }
        return;
    }

    for (int i = 0; i < count; i++) {
        for (int j = 0; j < count; j++) {
            if (is_possible(i, j)) {
                double amount = perform_transaction(i, j);
                backtrack(transaction_count + 1, best_found);
                cancel_transaction(i, j, amount);
            }
        }
    }
}

int main() {
    int best = count;
    backtrack(0, &best);
    printf("Minimum number of transactions: %d\n", best);
    return 0;
}
