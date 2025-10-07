#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024
#define MAX_NAME 50
#define MAX_BODOVI 100.0   

typedef struct {
    char ime[MAX_NAME];
    char prezime[MAX_NAME];
    double bodovi;
} Student;

//funkcija za prebrojavanje redova
int prebrojiRedove(const char* imeDatoteke) {
    FILE* fp = fopen(imeDatoteke, "r");
    if (!fp) {
        printf("Ne mogu otvoriti datoteku %s!\n", imeDatoteke);
        return -1;
    }

    int count = 0;
    char buffer[MAX_LINE];
    while (fgets(buffer, MAX_LINE, fp)) {
        count++;
    }

    fclose(fp);
    return count;
}

//odma u mainu sam napravia i alokaciju i upis da ne razbijam to u vise manjih funkcija
int main() {
    const char* imeDatoteke = "studenti.txt";

    int brojStudenata = prebrojiRedove(imeDatoteke);
    if (brojStudenata <= 0) {
        printf("Datoteka je prazna ili ne postoji.\n");
        return 1;
    }

    Student* studenti = (Student*)malloc(brojStudenata * sizeof(Student));
    if (!studenti) {
        printf("Neuspjesno alociranje memorije!\n");
        return -1;
    }

    FILE* fp = fopen(imeDatoteke, "r");
    if (!fp) {
        printf("Ne mogu ponovno otvoriti datoteku!\n");
        free(studenti);
        return 1;
    }

    for (int i = 0; i < brojStudenata; i++) {
        fscanf(fp, "%s %s %lf", studenti[i].ime, studenti[i].prezime, &studenti[i].bodovi);
    }

    fclose(fp);

    printf("\n%-15s %-15s %-10s %-10s\n", "Ime", "Prezime", "Bodovi", "Relativni(%)");
    printf("------------------------------------------------------\n");

    for (int i = 0; i < brojStudenata; i++) {
        double relativni = (studenti[i].bodovi / MAX_BODOVI) * 100.0;
        printf("%-15s %-15s %-10.2f %-10.2f\n",
            studenti[i].ime, studenti[i].prezime,
            studenti[i].bodovi, relativni);
    }

    free(studenti);
    return 0;
}
