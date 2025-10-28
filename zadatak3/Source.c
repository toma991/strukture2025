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
void freeList(Person* head);

// --- Funkcije ---
Person* createPerson() {
    Person* newPerson = (Person*)malloc(sizeof(Person));
    if (!newPerson) {
        perror("Greška: neuspjela alokacija memorije");
        return NULL;
    }

    printf("Unesite ime: ");
    if (scanf_s("%s", newPerson->ime, (unsigned)_countof(newPerson->ime)) != 1) {
        printf("Neispravan unos.\n");
        free(newPerson);
        return NULL;
    }

    printf("Unesite prezime: ");
    if (scanf_s("%s", newPerson->prezime, (unsigned)_countof(newPerson->prezime)) != 1) {
        printf("Neispravan unos.\n");
        free(newPerson);
        return NULL;
    }

    printf("Unesite godinu rođenja: ");
    if (scanf_s("%d", &newPerson->godina) != 1) {
        printf("Neispravan unos.\n");
        free(newPerson);
        return NULL;
    }

    newPerson->next = NULL;
    return newPerson;
}

// A. Dodaj na početak
Person* addToBeginning(Person* head) {
    Person* newPerson = createPerson();
    if (!newPerson) return head;
    newPerson->next = head;
    return newPerson;
}

// C. Dodaj na kraj
Person* addToEnd(Person* head) {
    Person* newPerson = createPerson();
    if (!newPerson) return head;

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
        return 0;
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
        printf("Osoba s prezimenom '%s' nije pronađena.\n", prezime);
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

// 3A. Dodaj iza određenog elementa
Person* addAfter(Person* head, Person* target) {
    if (target == NULL) {
        printf("Nevažeći element.\n");
        return head;
    }

    Person* newPerson = createPerson();
    if (!newPerson) return head;

    newPerson->next = target->next;
    target->next = newPerson;
    return head;
}

// 3B. Dodaj ispred određenog elementa
Person* addBefore(Person* head, Person* target) {
    if (target == NULL)
        return addToBeginning(head);

    if (head == target) {
        Person* newPerson = createPerson();
        if (!newPerson) return head;
        newPerson->next = head;
        return newPerson;
    }

    Person* current = head;
    while (current != NULL && current->next != target)
        current = current->next;

    if (current == NULL) {
        printf("Element nije pronađen.\n");
        return head;
    }

    Person* newPerson = createPerson();
    if (!newPerson) return head;

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
        return 1;
    }

    while (head != NULL) {
        fprintf(fp, "%s %s %d\n", head->ime, head->prezime, head->godina);
        head = head->next;
    }

    fclose(fp);
    printf("Lista je upisana u datoteku '%s'.\n", filename);
    return 0;
}

// 3E. Čitanje iz datoteke
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

    while (fscanf_s(fp, "%s %s %d", temp.ime, (unsigned)_countof(temp.ime),
        temp.prezime, (unsigned)_countof(temp.prezime), &temp.godina) == 3) {
        Person* newPerson = (Person*)malloc(sizeof(Person));
        if (!newPerson) {
            perror("Greška pri alokaciji memorije");
            freeList(head);
            fclose(fp);
            return NULL;
        }

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
    printf("Lista je učitana iz datoteke '%s'.\n", filename);
    return head;
}

// ✅ Oslobađanje memorije
void freeList(Person* head) {
    Person* temp = NULL;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
    printf("Sva memorija je oslobođena.\n");
}

// --- MAIN ---
int main() {
    Person* head = NULL;
    int choice;

    while (1) {
        printf("\nIzbornik:\n");
        printf("1. Dodaj osobu na početak\n");
        printf("2. Ispiši listu\n");
        printf("3. Dodaj osobu na kraj\n");
        printf("4. Dodaj osobu iza određenog elementa (po indeksu)\n");
        printf("5. Dodaj osobu ispred određenog elementa (po indeksu)\n");
        printf("6. Sortiraj po prezimenima\n");
        printf("7. Upis u datoteku\n");
        printf("8. Učitaj iz datoteke\n");
        printf("9. Obriši po prezimenu\n");
        printf("10. Izlaz\n");
        printf("Odabir: ");

        if (scanf_s("%d", &choice) != 1) {
            printf("Neispravan unos. Pokušajte ponovno.\n");
            while (getchar() != '\n'); // očisti ulazni buffer
            continue;
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
            if (scanf_s("%d", &idx) != 1) {
                printf("Neispravan unos indeksa.\n");
                while (getchar() != '\n');
                break;
            }
            Person* current = head;
            for (int i = 0; i < idx && current != NULL; i++)
                current = current->next;
            head = addAfter(head, current);
            break;
        }
        case 5: {
            int idx;
            printf("Unesite indeks: ");
            if (scanf_s("%d", &idx) != 1) {
                printf("Neispravan unos indeksa.\n");
                while (getchar() != '\n');
                break;
            }
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
            freeList(head);
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
            freeList(head);
            printf("Izlaz iz programa.\n");
            return 0;
        default:
            printf("Pogrešan odabir.\n");
        }
    }
}
