#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>


double balances[] = {2, 3, 5}; 
const double tolerance = 1e-9;
int count = sizeof(balances) / sizeof(double);


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
    for (int i = 0; i < count; i++) {
        if (fabs(balances[i]) > tolerance) return false;
    }
    return true;
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


void set_balances(double new_balances[], int n) {
    for (int i = 0; i < n; i++) {
        balances[i] = new_balances[i];
    }
    count = n;
}


void run_test(double test_balances[], int n, int expected, const char* test_name) {
    set_balances(test_balances, n);
    int best = count;  
    backtrack(0, &best);
    if (best == expected) {
        printf("[PASS] %s\n", test_name);
    } else {
        printf("[FAIL] %s : expected %d, got %d\n", test_name, expected, best);
    }
}


void test_unit_functions() {
    // Test is_possible
    double bal[] = {-5, 3, 2};
    set_balances(bal, 3);
    assert(is_possible(0, 1) == true);
    assert(is_possible(0, 2) == true);
    assert(is_possible(1, 0) == false);
    assert(is_possible(2, 0) == false);
    assert(is_possible(1, 2) == false);
    
    
    set_balances(bal, 3);
    double amount = perform_transaction(0, 1);
    assert(fabs(amount - 3.0) <= tolerance); 
    assert(fabs(balances[0] - (-2.0)) <= tolerance);
    assert(fabs(balances[1] - 0.0) <= tolerance);
    cancel_transaction(0, 1, amount);
    assert(fabs(balances[0] - (-5.0)) <= tolerance);
    assert(fabs(balances[1] - 3.0) <= tolerance);
    
  
    double final1[] = {0, 0, 1e-10}; 
    set_balances(final1, 3);
    assert(is_final_state() == true);
    double final2[] = {0, 0.5, -0.5}; 
    set_balances(final2, 3);
    assert(is_final_state() == false);
    double final3[] = {5, 0, 0};
    set_balances(final3, 3);
    assert(is_final_state() == false); 
    
    printf("[PASS] Unit tests\n");
}

void test_backtrack_cases() {
    double t1[] = {0, 0, 0};
    run_test(t1, 3, 0, "All balances are zero");
    
    double t2[] = {5, 0, 0};
    run_test(t2, 3, 3, "Single positive person (sum != 0)");
    
    double t3[] = {-5, 5};
    run_test(t3, 2, 1, "Two persons - simple debt");
    
    double t4[] = {-2, -3, 5};
    run_test(t4, 3, 2, "Three persons: -2, -3, 5");
    
    double t5[] = {-1, -2, 1, 2};
    run_test(t5, 4, 2, "Four persons: -1, -2, 1, 2");
    
    double t6[] = {-1.5, 1.5};
    run_test(t6, 2, 1, "Two persons with decimals");
    
    double t7[] = {10, -20, 10, 0}; 
    run_test(t7, 4, 2, "Circular debt");
}

int main() {
    printf("=== Running tests ===\n");
    test_unit_functions();
    test_backtrack_cases();
    printf("=== All tests completed ===\n");
    return 0;
}
