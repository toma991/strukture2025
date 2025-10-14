/*
2. Definirati strukturu osoba(ime, prezime, godina roðenja) i napisati program koji :
A.dinamièki dodaje novi element na poèetak liste,
B.ispisuje listu,
C.dinamièki dodaje novi element na kraj liste,
D.pronalazi element u listi(po prezimenu),
E.briše odreðeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Person {
    char first_name[50];
    char last_name[50];
    int birth_year;
    struct Person* next;
} Person;

// A. Dodavanje na poèetak
Person* addToBeginning(Person* head) {
    Person* newPerson = (Person*)malloc(sizeof(Person));
    if (!newPerson) {
        perror("Memory allocation failed.");
        exit(1);
    }

    printf("Enter first name: ");
    if (scanf_s("%s", newPerson->first_name, sizeof(newPerson->first_name)) != 1) {
        perror("Input error.");
        exit(1);
    }
    printf("Enter last name: ");
    if (scanf_s("%s", newPerson->last_name, sizeof(newPerson->last_name)) != 1) {
        perror("Input error.");
        exit(1);
    }
    printf("Enter birth year: ");
    if (scanf_s("%d", &newPerson->birth_year) != 1) {
        perror("Input error.");
        exit(1);
    }

    newPerson->next = head;
    return newPerson;
}

// B. Ispis liste
int printList(Person* head) {
    Person* current = head;
    while (current) {
        printf("First Name: %s, Last Name: %s, Birth Year: %d\n", current->first_name, current->last_name, current->birth_year);
        current = current->next;
    }
    return 0;
}

// C. Dodavanje na kraj
Person* addToEnd(Person* head) {
    Person* newPerson = (Person*)malloc(sizeof(Person));
    if (!newPerson) {
        perror("Memory allocation failed.");
        exit(1);
    }

    printf("Enter first name: ");
    if (scanf_s("%s", newPerson->first_name, sizeof(newPerson->first_name)) != 1) {
        perror("Input error.");
        exit(1);
    }
    printf("Enter last name: ");
    if (scanf_s("%s", newPerson->last_name, sizeof(newPerson->last_name)) != 1) {
        perror("Input error.");
        exit(1);
    }
    printf("Enter birth year: ");
    if (scanf_s("%d", &newPerson->birth_year) != 1) {
        perror("Input error.");
        exit(1);
    }

    newPerson->next = NULL;

    if (!head) {
        return newPerson;
    }

    Person* current = head;
    while (current->next) {
        current = current->next;
    }
    current->next = newPerson;

    return head;
}


// D. Pronalaženje po prezimenu
Person* findByLastName(Person* head, const char* lastName) {
    Person* current = head;
    while (current) {
        if (strcmp(current->last_name, lastName) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

// E. Brisanje odreðenog elementa po prezimenu
Person* deleteByLastName(Person* head, const char* lastName) {
    Person* current = head;
    Person* previous = NULL;

    while (current && strcmp(current->last_name, lastName) != 0) {
        previous = current;
        current = current->next;
    }

    if (!current) {
        printf("Person with last name '%s' not found.\n", lastName);
        return head;
    }

    if (!previous) {
        // brišemo prvi element
        head = current->next;
    }
    else {
        previous->next = current->next;
    }

    free(current);
    printf("Person with last name '%s' deleted successfully.\n", lastName);
    return head;
}

// Glavni izbornik
int main() {
    Person* head = NULL;
    int choice;
    char lastName[50];

    while (1) {
        printf("\nChoose an option:\n");
        printf("1. Add a person to the beginning of the list\n");
        printf("2. Print the list\n");
        printf("3. Add a person to the end of the list\n");
        printf("4. Find a person by last name\n");
        printf("5. Delete a person by last name\n");
        printf("6. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            head = addToBeginning(head);
            break;
        case 2:
            printList(head);
            break;
        case 3:
            head = addToEnd(head);
            break;
        case 4:
            printf("Enter last name to search: ");
            scanf("%49s", lastName);
            {
                Person* found = findByLastName(head, lastName);
                if (found)
                    printf("Found: %s %s, born %d\n",
                        found->first_name, found->last_name, found->birth_year);
                else
                    printf("No person with last name '%s' found.\n", lastName);
            }
            break;
        case 5:
            printf("Enter last name to delete: ");
            scanf("%49s", lastName);
            head = deleteByLastName(head, lastName);
            break;
        case 6:
            // oslobaðanje memorije prije izlaza
            while (head) {
                Person* temp = head;
                head = head->next;
                free(temp);
            }
            printf("Exiting program.\n");
            return 0;
        default:
            printf("Invalid option. Please enter again.\n");
        }
    }
}
