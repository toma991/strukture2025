#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 1024
#define EXIT_SUCCESS 0
#define FILE_OPEN_ERROR -1
#define MALLOC_ERROR -2
#define SCANF_ERROR -3

typedef struct article {
    char name[20];
    int quantity;
    int price;
    struct article* next;
} Article;

typedef struct receipt {
    char date[11];
    Article headArticle;
    struct receipt* next;
} Receipt;

int addArticle(Article* head, Article* article);
int addReceipt(Receipt* head, Receipt* receipt);
int readReceipt(char* fileName, Receipt* headReceipt);
int printArticles(Article* article);
int printReceipts(Receipt* receipt);
int getArticleInPeriod(Receipt* currentReceipt, char* article, char* startDate, char* endDate, int* income, int* count);
int getMinMaxSoldInPeriod(Receipt* headReceipt, char* startDate, char* endDate, char* minArticle, int* minQuantity, char* maxArticle, int* maxQuantity);
int getMinMaxPrice(Receipt* headReceipt, char* minArticle, int* minPrice, char* maxArticle, int* maxPrice);
int deleteArticles(Article* articleHead);
int deleteReceipts(Receipt* receiptHead);

int main()
{
    int income = 0, count = 0;
    char article[100] = { 0 };
    char startDate[20] = { 0 }, endDate[20] = { 0 };
    char currReceipt[MAX_LINE] = { 0 };

    FILE* filePointer = fopen("racuni.txt", "r");
    if (!filePointer) {
        printf("File racuni.txt could not be opened.\n");
        return FILE_OPEN_ERROR;
    }

    Receipt headReceipt = { .date = {0}, .next = NULL };
    headReceipt.headArticle.next = NULL;

    while (fgets(currReceipt, MAX_LINE, filePointer)) {
        currReceipt[strcspn(currReceipt, "\n")] = 0;
        if (strlen(currReceipt) == 0) continue;
        readReceipt(currReceipt, &headReceipt);
    }
    fclose(filePointer);

    printReceipts(headReceipt.next);

    printf("\nEnter start date (YYYY-MM-DD): ");
    if (scanf("%s", startDate) != 1) return SCANF_ERROR;
    printf("Enter end date (YYYY-MM-DD): ");
    if (scanf("%s", endDate) != 1) return SCANF_ERROR;

    char minArticle[30], maxArticle[30];
    int minQ = 0, maxQ = 0;
    getMinMaxSoldInPeriod(headReceipt.next, startDate, endDate, minArticle, &minQ, maxArticle, &maxQ);

    printf("\nMost sold article in period: %s (%d pcs)\n", maxArticle, maxQ);
    printf("Least sold article in period: %s (%d pcs)\n", minArticle, minQ);

    printf("\nEnter article name to check: ");
    if (scanf("%s", article) != 1) return SCANF_ERROR;

    income = 0;
    count = 0;

    getArticleInPeriod(headReceipt.next, article, startDate, endDate, &income, &count);

    printf("Income from %d sold '%s' items in period: %d\n", count, article, income);

    char minP[30], maxP[30];
    int minPr = 0, maxPr = 0;

    getMinMaxPrice(headReceipt.next, minP, &minPr, maxP, &maxPr);

    printf("\nCheapest article overall: %s (%d)\n", minP, minPr);
    printf("Most expensive article overall: %s (%d)\n", maxP, maxPr);

    deleteReceipts(&headReceipt);

    return 0;
}

int addArticle(Article* head, Article* article)
{
    Article* current = head;
    while (current->next != NULL && strcmp(current->next->name, article->name) < 0)
        current = current->next;

    article->next = current->next;
    current->next = article;
    return 0;
}

int addReceipt(Receipt* head, Receipt* receipt)
{
    Receipt* current = head;
    while (current->next != NULL && strcmp(current->next->date, receipt->date) < 0)
        current = current->next;

    receipt->next = current->next;
    current->next = receipt;
    return 0;
}

int readReceipt(char* fileName, Receipt* headReceipt)
{
    FILE* filePointer = fopen(fileName, "r");
    if (!filePointer) {
        printf("Could not open receipt file %s\n", fileName);
        return FILE_OPEN_ERROR;
    }

    Receipt* newReceipt = malloc(sizeof(Receipt));
    if (!newReceipt) return MALLOC_ERROR;

    newReceipt->next = NULL;
    newReceipt->headArticle.next = NULL;

    if (!fgets(newReceipt->date, 11, filePointer)) {
        free(newReceipt);
        fclose(filePointer);
        return SCANF_ERROR;
    }
    newReceipt->date[strcspn(newReceipt->date, "\n")] = 0;

    while (1) {
        Article* newArticle = malloc(sizeof(Article));
        if (!newArticle) return MALLOC_ERROR;

        int r = fscanf(filePointer, " %19[^,], %d, %d", newArticle->name, &newArticle->quantity, &newArticle->price);

        if (r != 3) {
            free(newArticle);
            break;
        }

        newArticle->next = NULL;
        addArticle(&newReceipt->headArticle, newArticle);
    }

    addReceipt(headReceipt, newReceipt);
    fclose(filePointer);
    return 0;
}

int printArticles(Article* current)
{
    while (current) {
        printf("\t%s, %d, %d\n", current->name, current->quantity, current->price);
        current = current->next;
    }
    return 0;
}

int printReceipts(Receipt* current)
{
    while (current) {
        printf("Receipt from date %s:\n", current->date);
        printArticles(current->headArticle.next);
        current = current->next;
    }
    return 0;
}

int getArticleInPeriod(Receipt* currentReceipt, char* article, char* startDate, char* endDate, int* income, int* count)
{
    while (currentReceipt && strcmp(currentReceipt->date, startDate) < 0)
        currentReceipt = currentReceipt->next;

    while (currentReceipt && strcmp(currentReceipt->date, endDate) <= 0) {
        Article* currentArticle = currentReceipt->headArticle.next;

        while (currentArticle) {
            if (strcmp(currentArticle->name, article) == 0) {
                *income += currentArticle->quantity * currentArticle->price;
                *count += currentArticle->quantity;
            }
            currentArticle = currentArticle->next;
        }
        currentReceipt = currentReceipt->next;
    }
    return EXIT_SUCCESS;
}

int getMinMaxSoldInPeriod(Receipt* headReceipt, char* startDate, char* endDate, char* minArticle, int* minQuantity, char* maxArticle, int* maxQuantity)
{
    typedef struct tempNode {
        char name[20];
        int totalQuantity;
        struct tempNode* next;
    } TempNode;

    TempNode* head = NULL;
    TempNode* currentT = NULL;
    TempNode* newNode = NULL;

    Receipt* currentReceipt = headReceipt;
    while (currentReceipt && strcmp(currentReceipt->date, startDate) < 0)
        currentReceipt = currentReceipt->next;

    while (currentReceipt && strcmp(currentReceipt->date, endDate) <= 0) {
        Article* currentArticle = currentReceipt->headArticle.next;

        while (currentArticle) {
            currentT = head;
            while (currentT != NULL && strcmp(currentT->name, currentArticle->name) != 0)
                currentT = currentT->next;

            if (currentT == NULL) {
                newNode = (TempNode*)malloc(sizeof(TempNode));
                strcpy(newNode->name, currentArticle->name);
                newNode->totalQuantity = currentArticle->quantity;
                newNode->next = head;
                head = newNode;
            }
            else {
                currentT->totalQuantity += currentArticle->quantity;
            }
            currentArticle = currentArticle->next;
        }
        currentReceipt = currentReceipt->next;
    }

    if (!head) {
        printf("No articles found in the given period.\n");
        return -1;
    }

    strcpy(minArticle, head->name);
    strcpy(maxArticle, head->name);
    *minQuantity = head->totalQuantity;
    *maxQuantity = head->totalQuantity;

    currentT = head->next;
    while (currentT) {
        if (currentT->totalQuantity < *minQuantity) {
            strcpy(minArticle, currentT->name);
            *minQuantity = currentT->totalQuantity;
        }
        if (currentT->totalQuantity > *maxQuantity) {
            strcpy(maxArticle, currentT->name);
            *maxQuantity = currentT->totalQuantity;
        }
        currentT = currentT->next;
    }

    while (head) {
        currentT = head;
        head = head->next;
        free(currentT);
    }

    return 0;
}

int getMinMaxPrice(Receipt* headReceipt, char* minArticle, int* minPrice,
    char* maxArticle, int* maxPrice)
{
    int init = 0;

    Receipt* currReceipt = headReceipt;
    while (currReceipt) {

        Article* currArt = currReceipt->headArticle.next;

        while (currArt) {

            if (!init) {
                strcpy(minArticle, currArt->name);
                strcpy(maxArticle, currArt->name);
                *minPrice = currArt->price;
                *maxPrice = currArt->price;
                init = 1;
            }
            else {
                if (currArt->price < *minPrice) {
                    *minPrice = currArt->price;
                    strcpy(minArticle, currArt->name);
                }
                if (currArt->price > *maxPrice) {
                    *maxPrice = currArt->price;
                    strcpy(maxArticle, currArt->name);
                }
            }

            currArt = currArt->next;
        }

        currReceipt = currReceipt->next;
    }

    if (!init) {
        printf("No articles found.\n");
        return -1;
    }

    return 0;
}

int deleteArticles(Article* articleHead)
{
    Article* temp;

    while (articleHead->next != NULL) {
        temp = articleHead->next;
        articleHead->next = temp->next;
        free(temp);
    }
    return 0;
}

int deleteReceipts(Receipt* receiptHead)
{
    Receipt* temp;

    while (receiptHead->next != NULL) {
        temp = receiptHead->next;
        receiptHead->next = temp->next;

        deleteArticles(&temp->headArticle);
        free(temp);
    }
    return 0;
}
