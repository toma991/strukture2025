/* 7. Napisati program koji pomoæu vezanih listi(stabala) predstavlja strukturu direktorija.
Omoguæiti unos novih direktorija i pod - direktorija, ispis sadržaja direktorija i
povratak u prethodni direktorij.Toènije program treba preko menija simulirati
korištenje DOS naredbi : 1 - "md", 2 - "cd dir", 3 - "cd..", 4 - "dir" i 5 – izlaz.*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 50

#define EMPTY_STACK 1
#define EXIT_SUCCESS 0
#define MALLOC_ERROR -1
#define SCANF_ERROR -2

struct directory;
typedef struct directory* DirectoryPosition;
typedef struct directory {
	char name[MAX_LENGTH];
	DirectoryPosition subdirectoryPosition;
	DirectoryPosition next;
} Directory;

struct levelStack;
typedef struct levelStack* LevelStackPosition;
typedef struct levelStack {
	DirectoryPosition directoryLevel;
	LevelStackPosition next;
} LevelStack;

DirectoryPosition createDirectory(char* directoryName);
int push(LevelStackPosition, DirectoryPosition);
int pop(LevelStackPosition);
int addDirectory(DirectoryPosition, DirectoryPosition);
int changeDirectory(LevelStackPosition, char*);
int printDirectory(DirectoryPosition, int);

int main()
{
	char input = 0;
	char directoryName[MAX_LENGTH] = { 0 };
	Directory headDirectory = { .name = {0}, .subdirectoryPosition = NULL, .next = NULL };
	DirectoryPosition rootDirectory = NULL;
	LevelStack headLevelStack = { .directoryLevel = NULL, .next = NULL };

	rootDirectory = createDirectory("C:");
	if (rootDirectory == NULL) return MALLOC_ERROR;

	headDirectory.next = rootDirectory;
	push(&headLevelStack, rootDirectory);

	printf("1 - make directory\n2 - change directory to <dir>\n3 - go to parent directory\n4 - view directory content\n5 - end program\n");

	do {
		printf("Enter command: ");
		if (scanf(" %c", &input) != 1) return SCANF_ERROR;

		switch (input) {
		case '1':
			// md - make directory
			printf("md ");
			if (scanf(" %s", directoryName) != 1) return SCANF_ERROR;

			DirectoryPosition newDirectory = createDirectory(directoryName);
			if (newDirectory == NULL) return MALLOC_ERROR;
			addDirectory(headLevelStack.next->directoryLevel, newDirectory);
			printf("Created directory %s\n", directoryName);

			break;
		case '2':
			// cd <dir> - change directory
			printf("cd ");
			if (scanf(" %s", directoryName) != 1) return SCANF_ERROR;
			changeDirectory(&headLevelStack, directoryName);
			break;
		case '3':
			// cd.. - go to parent directory
			if (pop(&headLevelStack)) printf("Already in root directory.\n");
			else printf("You are now in parent directory\n");
			break;
		case '4':
			// dir - view directory content
			printf("Printing content of %s:\n", headLevelStack.next->directoryLevel->name);
			printDirectory(headLevelStack.next->directoryLevel, 0);
			break;
		case '5':
			// end of program
			printf("\n*End of program.*\n");
			break;
		default:
			printf("Invalid input.\n");
			break;
		}

	} while (input != '5');


	deleteDirectory(&headDirectory);
	deleteStack(&headLevelStack);

	return 0;
}

DirectoryPosition createDirectory(char* directoryName) {

	DirectoryPosition newDirectory = NULL;
	newDirectory = (DirectoryPosition)malloc(sizeof(Directory));

	if (newDirectory == NULL) return NULL;

	strcpy(newDirectory->name, directoryName);
	newDirectory->subdirectoryPosition = NULL;
	newDirectory->next = NULL;

	return newDirectory;
}

int push(LevelStackPosition headLevelStack, DirectoryPosition currentDirectory) {

	LevelStackPosition newLevelStack = NULL;
	newLevelStack = (LevelStackPosition)malloc(sizeof(LevelStack));

	if (newLevelStack == NULL) return MALLOC_ERROR;

	newLevelStack->directoryLevel = currentDirectory;

	newLevelStack->next = headLevelStack->next;
	headLevelStack->next = newLevelStack;

	return EXIT_SUCCESS;
}

int pop(LevelStackPosition headLevelStack) {

	LevelStackPosition temp;

	if (headLevelStack->next == NULL) return EMPTY_STACK; // Stack is empty

	temp = headLevelStack->next;
	headLevelStack->next = headLevelStack->next->next;
	free(temp);

	return 0;
}

int addDirectory(DirectoryPosition superdirectory, DirectoryPosition subdirectory) {

	subdirectory->next = superdirectory->subdirectoryPosition;
	superdirectory->subdirectoryPosition = subdirectory;

	return EXIT_SUCCESS;
}

// Goes to child directory with directoryName
int changeDirectory(LevelStackPosition headLevelStack, char* directoryName) {

	DirectoryPosition current = headLevelStack->next->directoryLevel->subdirectoryPosition;

	while (current != NULL && strcmp(current->name, directoryName) != 0) current = current->next;

	if (current != NULL) {
		printf("Changed directory to %s\n", directoryName);
		push(headLevelStack, current);
	}
	else printf("Directory not found.\n");

	return EXIT_SUCCESS;
}

int printDirectory(DirectoryPosition current, int depth) {

	int i = 0;
	DirectoryPosition currentSubdirectory = NULL;

	if (current->subdirectoryPosition != NULL) {
		currentSubdirectory = current->subdirectoryPosition;
		while (currentSubdirectory != NULL) {
			for (i = 0; i < depth; i++)
				printf(" ");
			printf("%s\n", currentSubdirectory->name);
			printDirectory(currentSubdirectory, depth + 1);
			currentSubdirectory = currentSubdirectory->next;
		}
	}

	return 0;
}

int deleteDirectory(DirectoryPosition headDirectory) {

	DirectoryPosition temp = NULL;

	while (headDirectory->next != NULL) {
		temp = headDirectory->next;
		headDirectory->next = temp->next;
		free(temp);
	}

	return EXIT_SUCCESS;
}

int deleteStack(LevelStackPosition headLevelStack) {

	LevelStackPosition temp = NULL;

	while (headLevelStack->next != NULL) {
		temp = headLevelStack->next;
		headLevelStack->next = temp->next;
		free(temp);
	}

	return EXIT_SUCCESS;
}