#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
    char *lexeme;
    char *token_name;
    struct node *next;
} node;

void print_list(node *head) {
    node *cur = head;
    while (cur != NULL) {
        printf("%s: %s\n", cur->lexeme, cur->token_name);
        cur = cur->next;
    }
}

void append_token(node **head, node **tail, const char *lexeme, const char *token_name) {
    node *newnode = (node*)malloc(sizeof(node));
    int len = (int)strlen(lexeme);
    newnode->lexeme = (char*)malloc(len + 1);
    strcpy(newnode->lexeme, lexeme);

    len = (int)strlen(token_name);
    newnode->token_name = (char*)malloc(len + 1);
    strcpy(newnode->token_name, token_name);

    newnode->next = NULL;

    if (*head == NULL) {
        *head = newnode;
        *tail = newnode;
    } else {
        (*tail)->next = newnode;
        *tail = newnode;
    }
}

int is_letter_or_underscore(char c) {
    if ((c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') ||
        c == '_') return 1;
    return 0;
}

int is_letter_digit_or_underscore(char c) {
    if ((c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') ||
        (c >= '0' && c <= '9') ||
        c == '_') return 1;
    return 0;
}

int is_digit_char(char c) {
    if (c >= '0' && c <= '9') return 1;
    return 0;
}

int is_whitespace(char c) {
    if (c == ' ' || c == '\n' || c == '\t' || c == '\r') return 1;
    return 0;
}

const char* keyword_or_id(const char *s) {
    if (strcmp(s, "int") == 0)
        return "TYPE_TOKEN";
    if (strcmp(s, "main") == 0)
        return "MAIN_TOKEN";
    if (strcmp(s, "if") == 0)
        return "IF_TOKEN";
    if (strcmp(s, "else") == 0)
        return "ELSE_TOKEN";
    if (strcmp(s, "while") == 0)
        return "WHILE_TOKEN";
    return "ID_TOKEN";
}

int main(){
    FILE *fp = stdin;

    node *head = NULL;
    node *tail = NULL;

    char buffer[1024];
    int buf_len = 0;

    int c;

    while ((c = fgetc(fp)) != EOF) {

        if (is_whitespace((char)c)) {
            continue;
        }

        if (is_letter_or_underscore((char)c)) {
            buf_len = 0;
            buffer[buf_len++] = (char)c;

            while (1) {
                int d = fgetc(fp);
                if (d == EOF) {
                    buffer[buf_len] = '\0';
                    break;
                }
                if (is_letter_digit_or_underscore((char)d)) {
                    if (buf_len < (int)sizeof(buffer) - 1) {
                        buffer[buf_len++] = (char)d;
                    }
                } else {
                    ungetc(d, fp);
                    buffer[buf_len] = '\0';
                    break;
                }
            }

            append_token(&head, &tail, buffer, keyword_or_id(buffer));
            continue;
        }

        if (is_digit_char((char)c)) {
            buf_len = 0;
            buffer[buf_len++] = (char)c;

            while (1) {
                int d = fgetc(fp);
                if (d == EOF) {
                    buffer[buf_len] = '\0';
                    break;
                }
                if (is_digit_char((char)d)) {
                    if (buf_len < (int)sizeof(buffer) - 1) {
                        buffer[buf_len++] = (char)d;
                    }
                } else {
                    ungetc(d, fp);
                    buffer[buf_len] = '\0';
                    break;
                }
            }

            append_token(&head, &tail, buffer, "LITERAL_TOKEN");
            continue;
        }

        if (c == '=') {
            int d = fgetc(fp);
            if (d == '=') {
                append_token(&head, &tail, "==", "EQUAL_TOKEN");
            } else {
                if (d != EOF) ungetc(d, fp);
                append_token(&head, &tail, "=", "ASSIGN_TOKEN");
            }
            continue;
        }

        if (c == '<') {
            int d = fgetc(fp);
            if (d == '=') {
                append_token(&head, &tail, "<=", "LESSEQUAL_TOKEN");
            } else {
                if (d != EOF) ungetc(d, fp);
                append_token(&head, &tail, "<", "LESS_TOKEN");
            }
            continue;
        }

        if (c == '>') {
            int d = fgetc(fp);
            if (d == '=') {
                append_token(&head, &tail, ">=", "GREATEREQUAL_TOKEN");
            } else {
                if (d != EOF) ungetc(d, fp);
                append_token(&head, &tail, ">", "GREATER_TOKEN");
            }
            continue;
        }

        if (c == '+') {
            append_token(&head, &tail, "+", "PLUS_TOKEN");
            continue;
        }
        if (c == '-') {
            append_token(&head, &tail, "-", "MINUS_TOKEN");
            continue;
        }
        if (c == '(') {
            append_token(&head, &tail, "(", "LEFTPAREN_TOKEN");
            continue;
        }
        if (c == ')') {
            append_token(&head, &tail, ")", "REFTPAREN_TOKEN");
            continue;
        }
        if (c == '{') {
            append_token(&head, &tail, "{", "LEFTBRACE_TOKEN");
            continue;
        }
        if (c == '}') {
            append_token(&head, &tail, "}", "REFTBRACE_TOKEN");
            continue;
        }
        if (c == ';') {
            append_token(&head, &tail, ";", "SEMICOLON_TOKEN");
            continue;
        }
    }

    print_list(head);

    node *cur = head;
    while (cur != NULL) {
        node *next = cur->next;
        free(cur->lexeme);
        free(cur->token_name);
        free(cur);
        cur = next;
    }

    return 0;
}
