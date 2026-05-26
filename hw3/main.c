#include <stdio.h>
#include <string.h>

#define TOKEN_NUM 1
#define TOKEN_PLUS 2
#define TOKEN_LPAREN 3
#define TOKEN_RPAREN 4
#define TOKEN_EOF 5
#define TOKEN_INVALID 6

char lexeme[1024];
int token;
int error_flag = 0;
int indent = 0;

int is_digit_char(char c) {
    return (c >= '0' && c <= '9');
}

int is_whitespace(char c) {
    return (c == ' ' || c == '\n' || c == '\t' || c == '\r');
}

void print_indent() {
    int i;
    for (i = 0; i < indent; i++) {
        printf("  ");
    }
}

void print_line(const char *s) {
    print_indent();
    printf("%s\n", s);
}

void next_token() {
    int c;
    int len = 0;

    while (1) {
        c = fgetc(stdin);
        if (c == EOF) {
            token = TOKEN_EOF;
            lexeme[0] = '\0';
            return;
        }
        if (!is_whitespace((char)c)) {
            break;
        }
    }

    if (is_digit_char((char)c)) {
        lexeme[len++] = (char)c;
        while (1) {
            c = fgetc(stdin);
            if (c == EOF) {
                break;
            }
            if (is_digit_char((char)c)) {
                if (len < 1023) {
                    lexeme[len++] = (char)c;
                }
            } else {
                ungetc(c, stdin);
                break;
            }
        }
        lexeme[len] = '\0';
        token = TOKEN_NUM;
        return;
    }

    if (c == '+') {
        lexeme[0] = '+';
        lexeme[1] = '\0';
        token = TOKEN_PLUS;
        return;
    }

    if (c == '(') {
        lexeme[0] = '(';
        lexeme[1] = '\0';
        token = TOKEN_LPAREN;
        return;
    }

    if (c == ')') {
        lexeme[0] = ')';
        lexeme[1] = '\0';
        token = TOKEN_RPAREN;
        return;
    }

    lexeme[0] = (char)c;
    lexeme[1] = '\0';
    token = TOKEN_INVALID;
}

void parse_error() {
    error_flag = 1;
}

void parse_S();
void parse_S_prime();
void parse_E();

void parse_S() {
    if (error_flag) return;

    if (token == TOKEN_NUM || token == TOKEN_LPAREN) {
        print_line("S -> E S'");
        indent++;
        parse_E();
        parse_S_prime();
        indent--;
        return;
    }

    parse_error();
}

void parse_S_prime() {
    if (error_flag) return;

    if (token == TOKEN_PLUS) {
        print_line("S' -> + S");
        next_token();
        indent++;
        parse_S();
        indent--;
        return;
    }

    if (token == TOKEN_RPAREN || token == TOKEN_EOF) {
        print_line("S' -> ε");
        return;
    }

    parse_error();
}

void parse_E() {
    if (error_flag) return;

    if (token == TOKEN_NUM) {
        print_line(lexeme);
        next_token();
        return;
    }

    if (token == TOKEN_LPAREN) {
        print_line("E -> ( S )");
        next_token();
        indent++;
        parse_S();
        indent--;
        if (error_flag) return;
        if (token != TOKEN_RPAREN) {
            parse_error();
            return;
        }
        next_token();
        return;
    }

    parse_error();
}

int main() {
    next_token();
    parse_S();

    if (error_flag || token != TOKEN_EOF) {
        printf("Parse Error\n");
    }

    return 0;
}