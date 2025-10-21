/*2. Definirati strukturu osoba(ime, prezime, godina roðenja) i napisati program koji :
A.dinamièki dodaje novi element na poèetak liste,
B.ispisuje listu,
C.dinamièki dodaje novi element na kraj liste,
D.pronalazi element u listi(po prezimenu),
E.briše odreðeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.

3. Prethodnom zadatku dodati funkcije :
A.dinamièki dodaje novi element iza odreðenog elementa,
B.dinamièki dodaje novi element ispred odreðenog elementa,
C.sortira listu po prezimenima osoba,
D.upisuje listu u datoteku,
E.èita listu iz datoteke.
Samo nadogradit prosli zadatak
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Person {
    char ime[50];
    char prezime[50];
    int godina;
    struct Person* next;
} Person;

Person* createPerson();
Person* addToBeginning(Person* head);
Person* addToEnd(Person* head);
int printList(Person* head);
Person* findBySurname(Person* head, const char* prezime);
Person* deleteBySurname(Person* head, const char* prezime);
Person* addAfter(Person* head, Person* target);
Person* addBefore(Person* head, Person* target);
Person* sortList(Person* head);
int writeToFile(Person* head, const char* filename);
Person* readFromFile(const char* filename);

// --- Funkcije ---
Person* createPerson() {
    Person* newPerson = (Person*)malloc(sizeof(Person));
    if (!newPerson) {
        perror("Allocation error");
        exit(1);
    }

    printf("Unesite ime: ");
    scanf_s("%s", newPerson->ime, (unsigned)_countof(newPerson->ime));
    printf("Unesite prezime: ");
    scanf_s("%s", newPerson->prezime, (unsigned)_countof(newPerson->prezime));
    printf("Unesite godinu roðenja: ");
    scanf_s("%d", &newPerson->godina);

    newPerson->next = NULL;
    return newPerson;
}

// A. Dodaj na poèetak
Person* addToBeginning(Person* head) {
    Person* newPerson = createPerson();
    newPerson->next = head;
    return newPerson;
}

// C. Dodaj na kraj
Person* addToEnd(Person* head) {
    Person* newPerson = createPerson();
    if (head == NULL)
        return newPerson;

    Person* current = head;
    while (current->next != NULL)
        current = current->next;

    current->next = newPerson;
    return head;
}

// B. Ispis liste
int printList(Person* head) {
    int index = 0;
    if (!head) {
        printf("Lista je prazna.\n");
        return;
    }

    while (head != NULL) {
        printf("[%d] %s %s, %d\n", index++, head->ime, head->prezime, head->godina);
        head = head->next;
    }
    return 0;
}

// D. Pronalaženje po prezimenu
Person* findBySurname(Person* head, const char* prezime) {
    while (head != NULL) {
        if (strcmp(head->prezime, prezime) == 0)
            return head;
        head = head->next;
    }
    return NULL;
}

// E. Brisanje osobe po prezimenu
Person* deleteBySurname(Person* head, const char* prezime) {
    Person* current = head;
    Person* prev = NULL;

    while (current != NULL && strcmp(current->prezime, prezime) != 0) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Osoba s prezimenom '%s' nije pronaðena.\n", prezime);
        return head;
    }

    if (prev == NULL)
        head = current->next;
    else
        prev->next = current->next;

    free(current);
    printf("Osoba obrisana.\n");
    return head;
}

// 3A. Dodaj iza odreðenog elementa
Person* addAfter(Person* head, Person* target) {
    if (target == NULL) {
        printf("Nevažeæi element.\n");
        return head;
    }

    Person* newPerson = createPerson();
    newPerson->next = target->next;
    target->next = newPerson;
    return head;
}

// 3B. Dodaj ispred odreðenog elementa
Person* addBefore(Person* head, Person* target) {
    if (target == NULL)
        return addToBeginning(head);

    if (head == target) {
        Person* newPerson = createPerson();
        newPerson->next = head;
        return newPerson;
    }

    Person* current = head;
    while (current != NULL && current->next != target)
        current = current->next;

    if (current == NULL) {
        printf("Element nije pronaðen.\n");
        return head;
    }

    Person* newPerson = createPerson();
    newPerson->next = target;
    current->next = newPerson;
    return head;
}

// 3C. Sortiranje po prezimenima
Person* sortList(Person* head) {
    if (head == NULL || head->next == NULL)
        return head;

    Person* i, * j;
    char tempIme[50], tempPrezime[50];
    int tempGodina;

    for (i = head; i->next != NULL; i = i->next) {
        for (j = i->next; j != NULL; j = j->next) {
            if (strcmp(i->prezime, j->prezime) > 0) {
                strcpy(tempIme, i->ime);
                strcpy(tempPrezime, i->prezime);
                tempGodina = i->godina;

                strcpy(i->ime, j->ime);
                strcpy(i->prezime, j->prezime);
                i->godina = j->godina;

                strcpy(j->ime, tempIme);
                strcpy(j->prezime, tempPrezime);
                j->godina = tempGodina;
            }
        }
    }
    return head;
}

// 3D. Upis u datoteku
int writeToFile(Person* head, const char* filename) {
    FILE* fp;
    fopen_s(&fp, filename, "w");
    if (!fp) {
        perror("Greška pri otvaranju datoteke");
        return;
    }

    while (head != NULL) {
        fprintf(fp, "%s %s %d\n", head->ime, head->prezime, head->godina);
        head = head->next;
    }

    fclose(fp);
    printf("Lista je upisana u datoteku '%s'.\n", filename);

    return 0;
}

// 3E. Èitanje iz datoteke
Person* readFromFile(const char* filename) {
    FILE* fp;
    fopen_s(&fp, filename, "r");
    if (!fp) {
        perror("Greška pri otvaranju datoteke");
        return NULL;
    }

    Person* head = NULL;
    Person* tail = NULL;
    Person temp;

    while (fscanf_s(fp, "%s %s %d", temp.ime, (unsigned)_countof(temp.ime), temp.prezime, (unsigned)_countof(temp.prezime), &temp.godina) == 3) {
        Person* newPerson = (Person*)malloc(sizeof(Person));
        strcpy(newPerson->ime, temp.ime);
        strcpy(newPerson->prezime, temp.prezime);
        newPerson->godina = temp.godina;
        newPerson->next = NULL;

        if (head == NULL)
            head = newPerson;
        else
            tail->next = newPerson;

        tail = newPerson;
    }

    fclose(fp);
    printf("Lista je uèitana iz datoteke '%s'.\n", filename);
    return head;
}

// --- MAIN ---
int main() {
    Person* head = NULL;
    int choice;

    while (1) {
        printf("\nIzbornik:\n");
        printf("1. Dodaj osobu na poèetak\n");
        printf("2. Ispiši listu\n");
        printf("3. Dodaj osobu na kraj\n");
        printf("4. Dodaj osobu iza odreðenog elementa (po indeksu)\n");
        printf("5. Dodaj osobu ispred odreðenog elementa (po indeksu)\n");
        printf("6. Sortiraj po prezimenima\n");
        printf("7. Upis u datoteku\n");
        printf("8. Uèitaj iz datoteke\n");
        printf("9. Obriši po prezimenu\n");
        printf("10. Izlaz\n");
        printf("Odabir: ");

        if (scanf_s("%d", &choice) != 1) {
            perror("Greška unosa");
            exit(1);
        }

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
        case 4: {
            int idx;
            printf("Unesite indeks: ");
            scanf_s("%d", &idx);
            Person* current = head;
            for (int i = 0; i < idx && current != NULL; i++)
                current = current->next;
            head = addAfter(head, current);
            break;
        }
        case 5: {
            int idx;
            printf("Unesite indeks: ");
            scanf_s("%d", &idx);
            if (idx == 0)
                head = addBefore(head, head);
            else {
                Person* current = head;
                for (int i = 0; i < idx - 1 && current != NULL; i++)
                    current = current->next;
                if (current)
                    head = addBefore(head, current->next);
            }
            break;
        }
        case 6:
            head = sortList(head);
            printf("Lista sortirana.\n");
            break;
        case 7: {
            char filename[100];
            printf("Unesite ime datoteke: ");
            scanf_s("%s", filename, (unsigned)_countof(filename));
            writeToFile(head, filename);
            break;
        }
        case 8: {
            char filename[100];
            printf("Unesite ime datoteke: ");
            scanf_s("%s", filename, (unsigned)_countof(filename));
            // Oslobaðanje stare liste
            while (head) {
                Person* tmp = head;
                head = head->next;
                free(tmp);
            }
            head = readFromFile(filename);
            break;
        }
        case 9: {
            char prezime[50];
            printf("Unesite prezime za brisanje: ");
            scanf_s("%s", prezime, (unsigned)_countof(prezime));
            head = deleteBySurname(head, prezime);
            break;
        }
        case 10:
            while (head) {
                Person* tmp = head;
                head = head->next;
                free(tmp);
            }
            return 0;
        default:
            printf("Pogrešan odabir.\n");
        }
    }
}
