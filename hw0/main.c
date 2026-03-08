#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct node
{
    char cha;
    int count;
    struct node *next;
}node;

void print_list(node *head) {
    node *cur = head;
    while (cur != NULL) {

        if(cur->cha == '\n')
            printf("\\n : %d\n", cur->count);
        else if(cur->cha == ' ')
            printf("space : %d\n", cur->count);
        else if(cur->cha == '\t')
            printf("\\t : %d\n", cur->count);
        else
            printf("%c : %d\n", cur->cha, cur->count);

        cur = cur->next;
    }
}

int main(){
    FILE *fp = fopen(__FILE__,"r");
    if(!fp){
        printf("file not exist");
        return 0;
    }

    node *head = NULL;
    node *tail = NULL;

    node *table[256];
    memset(table, 0, sizeof(table));

    char c;

    while ((c = fgetc(fp)) != EOF){
        unsigned char index = (unsigned char)c;

        if (table[index] != NULL){
            table[index]->count++;
        }
        else{
            node *newnode = (node*)malloc(sizeof(node));

            newnode->cha = c;
            newnode->count = 1;
            newnode->next = NULL;

            table[index] = newnode;

            if(head == NULL){
                head = newnode;
                tail = newnode;
            }
            else{
                tail->next = newnode;
                tail = newnode;
            }
        }
    }

    fclose(fp);

    print_list(head);

    return 0;
}