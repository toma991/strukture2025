/*
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
*/

#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>

#define MAX_SIZE (128)
#define MAX_LEN (1024)
#define MAX_POINTS (15)

typedef struct _student {

	char name[MAX_SIZE];
	char surname[MAX_SIZE];
	double points;

}student;

int readNoRowsInFile();
student* allocateMemoryAndReadStudents(int noStudents);
double calculateRelativePoints(double points);
int showStudents(int noStudents, student* students);

int main()
{
	int noRows = 0;
	student* students = NULL;

	noRows = readNoRowsInFile();
	if (noRows > 0)
	{
		students = allocateMemoryAndReadStudents(noRows);
		showStudents(noRows, students);

		free(students);
	}

	return 0;
}
jsjdka
int readNoRowsInFile() {

	int counter = 0;
	FILE* filepointer = NULL;
	char buffer[MAX_SIZE] = { 0 };

	filepointer = fopen("studenti.txt", "r");
	if (!filepointer) {
		printf("Neuspjesno otvaranje datoteke!\n");
		return -1;
	}

	while (!feof(filepointer)) {
		fgets(buffer, MAX_SIZE, filepointer);
		counter++;
	}

	fclose(filepointer);

	return counter;
}

student* allocateMemoryAndReadStudents(int noStudents) {

	int counter = 0;
	FILE* filepointer = NULL;
	student* students = NULL;


	students = (student*)malloc(noStudents * sizeof(student));
	if (!students) {
		printf("Neuspjesna alokacija memorije!\n");
		return NULL;
	}

	filepointer = fopen("student.txt", "r");
	if (!filepointer) {
		printf("Neuspjesno otvaranje datoteke!\n");
		return NULL;
	}

	while (!feof(filepointer))
	{
		fscanf(filepointer, " %s %s %lf", students[counter].name, students[counter].surname, &students[counter].points);
		counter++;
	}

	fclose(filepointer);

	return students;
}

double calculateRelativePoints(double points)
{
	return ((points / MAX_POINTS) * 100);
}

int showStudents(int noStudents, student* students)
{
	int counter = 0;

	for (counter; counter < noStudents; counter++)
	{
		printf("Name: %s\t Surname: %s\t Absolute points: %.2lf\t Relative points: %.2lf%%\t\n", students[counter].name,
			students[counter].surname, students[counter].points, calculateRelativePoints(students[counter].points));
	}

	return 0;
}