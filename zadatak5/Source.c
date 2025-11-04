#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct Node {
    double value;
    struct Node* next;
} Node;

int push(Node** top, double value);
int pop(Node** top, double* result);
int evaluatePostfix(const char* filename, double* rezultat);

int main() {
    char filename[100];
    printf("Unesite ime datoteke s postfiks izrazom: ");
    scanf("%s", filename);

    double rezultat;
    if (evaluatePostfix(filename, &rezultat))
        printf("Rezultat izraza: %.2f\n", rezultat);
    else
        printf("Došlo je do greške pri izraèunu.\n");

    return 0;
}

int push(Node** top, double value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Greška pri alokaciji memorije!\n");
        return -1;
    }
    newNode->value = value;
    newNode->next = *top;
    *top = newNode;

    return 0;
}

int pop(Node** top, double* result) {
    if (*top == NULL) {
        printf("Greška: stog je prazan!\n");
        return -2;
    }
    Node* temp = *top;
    *result = temp->value;
    *top = temp->next;
    free(temp);

    return 1;
}

int evaluatePostfix(const char* filename, double* rezultat) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Greška: ne mogu otvoriti datoteku '%s'\n", filename);
        return -3;
    }

    Node* stack = NULL;
    char token[50];

    while (fscanf(file, "%s", token) == 1) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            push(&stack, atof(token));
        }
        else if (strlen(token) == 1) {
            double b, a;
            if (!pop(&stack, &b) || !pop(&stack, &a)) {
                printf("Greška: premalo operanada!\n");
                fclose(file);
                return -4;
            }

            double result;
            switch (token[0]) {
            case '+': result = a + b; break;
            case '-': result = a - b; break;
            case '*': result = a * b; break;
            case '/':
                if (b == 0) {
                    printf("Greška: dijeljenje s nulom!\n");
                    fclose(file);
                    return -5;
                }
                result = a / b;
                break;
            default:
                printf("Nepoznat operator: %s\n", token);
                fclose(file);
                return -6;
            }
            push(&stack, result);
        }
        else {
            printf("Neispravan token: %s\n", token);
            fclose(file);
            return -7;
        }
    }

    if (!pop(&stack, rezultat)) {
        printf("Greška: nema rezultata na stogu!\n");
        fclose(file);
        return 0;
    }

    if (stack != NULL) {
        printf("Upozorenje: preostali elementi na stogu!\n");
    }

    fclose(file);
    return 1;
}