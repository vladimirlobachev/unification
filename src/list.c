
#include "list.h"
#include "entry.h"

Expression* head_left = NULL;
Expression* head_right = NULL;

void create_expression(bool side, Entry* entry) {
    Expression* new_entry = malloc(sizeof(Expression));
    
    if (side) {
        head_left = new_entry;
    }
    else {
        head_right = new_entry;
    }
    
    new_entry->entry = entry;
    new_entry->next = NULL;

}

void concat(bool side, Entry* entry) {
    Expression* pointer = malloc(sizeof(Expression));

    
    if (side) {
        if (head_left == NULL) {
            create_expression(side, entry);
            return;
        }
        pointer = head_left;
    }
    else {
        if (head_right == NULL) {
            create_expression(side, entry);
            return;
        }
        pointer = head_right;
    }
    
    while (pointer->next != NULL) {
        pointer = pointer->next;
    }
    
    Expression* new_entry = malloc(sizeof(Expression));
    new_entry->entry = entry;
    new_entry->next = NULL;
    
    pointer->next = new_entry;
    
}

void nullExpressions() {
    head_left = NULL;
    head_right = NULL;
}

void printExpressions(Expression* left, Expression* right) {
    
    printExpression(left);
    printf("=");
    printExpression(right);

}

void printExpression(Expression* ptr) {
    Expression* pointer = ptr;

    if (pointer == NULL) {
        printf("empty");
        return;
    }
    bool head = true;
    while (pointer != NULL) {
        if (!head) {
            printf(":");
        }
        printf("%s", pointer->entry->term);
        pointer = pointer->next;
        if (head) {
            head = !head;
        }
    }
}

