#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    char ch;
    int count;
    struct Node *next;
} Node;

Node* createNode(char ch) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newNode->ch = ch;
    newNode->count = 1;
    newNode->next = NULL;
    return newNode;
}

Node* findNode(Node *head, char ch) {
    Node *cur = head;
    while (cur != NULL) {
        if (cur->ch == ch) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

void insertOrCount(Node **head, char ch) {
    Node *found = findNode(*head, ch);

    if (found != NULL) {
        found->count++;
        return;
    }

    Node *newNode = createNode(ch);

    if (*head == NULL) {
        *head = newNode;
        return;
    }

    Node *cur = *head;
    while (cur->next != NULL) {
        cur = cur->next;
    }
    cur->next = newNode;
}

/* output */
void printList(Node *head) {
    Node *cur = head;
    int first = 1;

    while (cur != NULL) {
        if (!first)
            printf(",");

        if (cur->ch == '\n')
            printf("\\n");
        else if (cur->ch == '\t')
            printf("\\t");
        else if (cur->ch == ' ')
            printf(" ");
        else
            printf("%c", cur->ch);

        first = 0;
        cur = cur->next;
    }

    printf("\n");
}

/* release linked list */
void freeList(Node *head) {
    Node *cur = head;
    while (cur != NULL) {
        Node *temp = cur;
        cur = cur->next;
        free(temp);
    }
}

int main(void) {
    FILE *fp;
    int c;
    Node *head = NULL;

    fp = fopen("main.c", "r");
    if (fp == NULL) {
        printf("Cannot open file main.c\n");
        return 1;
    }

    /* Scanner */
    while ((c = fgetc(fp)) != EOF) {
        insertOrCount(&head, (char)c);
    }

    fclose(fp);

    printList(head);

    freeList(head);
    return 0;
}
