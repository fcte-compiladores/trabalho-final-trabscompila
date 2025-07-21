#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Tipos de tokens
typedef enum {
    TOKEN_LPAREN,     // (
    TOKEN_RPAREN,     // )
    TOKEN_KEYWORD,    // :action, :parameters, etc.
    TOKEN_IDENTIFIER, // nomes, variáveis
    TOKEN_DASH,       // -
    TOKEN_NUMBER,     // números
    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

// Estrutura do token
typedef struct {
    TokenType type;
    char value[256];
    int line;
} Token;

// Variáveis globais do lexer
FILE *input_file;
Token current_token;
int line_number = 1;

// Protótipos
void advance_token();
void expect_token(TokenType expected);
void skip_whitespace();
int is_keyword(const char *str);
void parse_domain();
void parse_problem();
void skip_section();
void print_token_info(); // Nova função para debug

// Implementação do lexer
void skip_whitespace() {
    int c;
    while ((c = fgetc(input_file)) != EOF) {
        if (c == '\n') {
            line_number++;
        } else if (!isspace(c)) {
            ungetc(c, input_file);
            break;
        }
    }
}

int is_keyword(const char *str) {
    const char *keywords[] = {
        // Estruturas básicas
        "define", "domain", "problem", 
        // Seções do domínio
        ":requirements", ":types", ":predicates", ":action", ":functions",
        ":constants", ":derived",
        // Seções do problema
        ":domain", ":init", ":goal", ":objects", ":length", ":serial-length",
        // Parâmetros de ação
        ":parameters", ":precondition", ":effect", ":duration", ":condition",
        // Operadores lógicos
        "and", "or", "not", "imply", "exists", "forall", "when",
        // Requirements
        ":strips", ":typing", ":negative-preconditions", ":disjunctive-preconditions",
        ":equality", ":existential-preconditions", ":universal-preconditions",
        ":quantified-preconditions", ":conditional-effects", ":fluents",
        ":adl", ":durative-actions", ":duration-inequalities", ":continuous-effects",
        ":derived-predicates", ":timed-initial-literals", ":preferences",
        ":constraints", ":action-costs",
        // Operadores numéricos
        "increase", "decrease", "assign", "scale-up", "scale-down", "=", "<", ">", "<=", ">=",
        // Temporais (removemos "at" pois pode ser nome de predicado)
        "over", "start", "end", "all"
    };
    
    int num_keywords = sizeof(keywords)/sizeof(keywords[0]);
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void advance_token() {
    skip_whitespace();
    
    int c = fgetc(input_file);
    if (c == EOF) {
        current_token.type = TOKEN_EOF;
        strcpy(current_token.value, "EOF");
        current_token.line = line_number;
        return;
    }
    
    current_token.line = line_number;
    
    switch (c) {
        case '(':
            current_token.type = TOKEN_LPAREN;
            strcpy(current_token.value, "(");
            break;
            
        case ')':
            current_token.type = TOKEN_RPAREN;
            strcpy(current_token.value, ")");
            break;
            
        case '-':
            current_token.type = TOKEN_DASH;
            strcpy(current_token.value, "-");
            break;
            
        case '=':
            current_token.type = TOKEN_KEYWORD;
            strcpy(current_token.value, "=");
            break;
            
        case '<':
            c = fgetc(input_file);
            if (c == '=') {
                current_token.type = TOKEN_KEYWORD;
                strcpy(current_token.value, "<=");
            } else {
                ungetc(c, input_file);
                current_token.type = TOKEN_KEYWORD;
                strcpy(current_token.value, "<");
            }
            break;
            
        case '>':
            c = fgetc(input_file);
            if (c == '=') {
                current_token.type = TOKEN_KEYWORD;
                strcpy(current_token.value, ">=");
            } else {
                ungetc(c, input_file);
                current_token.type = TOKEN_KEYWORD;
                strcpy(current_token.value, ">");
            }
            break;
            
        case ';':
            // Comentário - pular até o fim da linha
            while ((c = fgetc(input_file)) != EOF && c != '\n');
            if (c == '\n') line_number++;
            advance_token(); // Recursão para próximo token
            break;
            
        default:
            if (isalpha(c) || c == ':' || c == '?') {
                // Identificador ou keyword
                int i = 0;
                current_token.value[i++] = c;
                
                while ((c = fgetc(input_file)) != EOF && 
                       (isalnum(c) || c == '-' || c == '_')) {
                    if (i < 255) {
                        current_token.value[i++] = c;
                    }
                }
                ungetc(c, input_file);
                current_token.value[i] = '\0';
                
                current_token.type = is_keyword(current_token.value) ? 
                                   TOKEN_KEYWORD : TOKEN_IDENTIFIER;
            } else if (isdigit(c)) {
                // Número
                int i = 0;
                current_token.value[i++] = c;
                
                while ((c = fgetc(input_file)) != EOF && (isdigit(c) || c == '.')) {
                    if (i < 255) {
                        current_token.value[i++] = c;
                    }
                }
                ungetc(c, input_file);
                current_token.value[i] = '\0';
                current_token.type = TOKEN_NUMBER;
            } else {
                current_token.type = TOKEN_ERROR;
                current_token.value[0] = c;
                current_token.value[1] = '\0';
            }
            break;
    }
}

void expect_token(TokenType expected) {
    if (current_token.type != expected) {
        const char* type_names[] = {"(", ")", "keyword", "identifier", "-", "number", "EOF", "error"};
        fprintf(stderr, "Rejected: Line %d: Expected %s, got %s ('%s')\n", 
                current_token.line, 
                type_names[expected], 
                type_names[current_token.type], 
                current_token.value);
        exit(1);
    }
    advance_token();
}

// Função de debug para mostrar informações do token atual
void print_token_info() {
    const char* type_names[] = {"(", ")", "keyword", "identifier", "-", "number", "EOF", "error"};
    printf("DEBUG: Line %d: %s '%s'\n", 
           current_token.line, 
           type_names[current_token.type], 
           current_token.value);
}

// Pula uma seção inteira balanceando parênteses
void skip_section() {
    expect_token(TOKEN_LPAREN); // Espera o ( que abre a seção
    
    int paren_count = 1;
    
    while (paren_count > 0 && current_token.type != TOKEN_EOF) {
        if (current_token.type == TOKEN_LPAREN) {
            paren_count++;
        } else if (current_token.type == TOKEN_RPAREN) {
            paren_count--;
        }
        advance_token();
    }
}

// Parser para domínio
void parse_domain() {
    expect_token(TOKEN_LPAREN);     // (
    expect_token(TOKEN_KEYWORD);    // define
    expect_token(TOKEN_LPAREN);     // (
    expect_token(TOKEN_KEYWORD);    // domain
    expect_token(TOKEN_IDENTIFIER); // nome-do-dominio
    expect_token(TOKEN_RPAREN);     // )
    
    // Processar seções do domínio
    while (current_token.type == TOKEN_LPAREN) {
        advance_token(); // Consome (
        
        if (current_token.type == TOKEN_KEYWORD) {
            if (strcmp(current_token.value, ":requirements") == 0) {
                advance_token();
                // Pular requirements
                while (current_token.type == TOKEN_KEYWORD) {
                    advance_token();
                }
                expect_token(TOKEN_RPAREN);
                
            } else if (strcmp(current_token.value, ":types") == 0) {
                advance_token();
                // Pular types
                while (current_token.type == TOKEN_IDENTIFIER || 
                       current_token.type == TOKEN_DASH) {
                    advance_token();
                }
                expect_token(TOKEN_RPAREN);
                
            } else if (strcmp(current_token.value, ":constants") == 0) {
                advance_token();
                // Pular constants (similar aos types)
                while (current_token.type == TOKEN_IDENTIFIER || 
                       current_token.type == TOKEN_DASH) {
                    advance_token();
                }
                expect_token(TOKEN_RPAREN);
                
            } else if (strcmp(current_token.value, ":functions") == 0) {
                advance_token();
                // Processar functions (similar aos predicados)
                while (current_token.type == TOKEN_LPAREN) {
                    advance_token(); // (
                    expect_token(TOKEN_IDENTIFIER); // nome-funcao
                    
                    // Pular parâmetros e tipo de retorno
                    while (current_token.type == TOKEN_IDENTIFIER || 
                           current_token.type == TOKEN_DASH) {
                        advance_token();
                    }
                    expect_token(TOKEN_RPAREN); // )
                }
                expect_token(TOKEN_RPAREN);
                
            } else if (strcmp(current_token.value, ":derived") == 0) {
                advance_token();
                // Pular predicado derivado completo
                skip_section();
                
            } else if (strcmp(current_token.value, ":predicates") == 0) {
                advance_token();
                // Processar predicados
                while (current_token.type == TOKEN_LPAREN) {
                    advance_token(); // (
                    expect_token(TOKEN_IDENTIFIER); // nome-predicado
                    
                    // Pular parâmetros
                    while (current_token.type == TOKEN_IDENTIFIER || 
                           current_token.type == TOKEN_DASH) {
                        advance_token();
                    }
                    expect_token(TOKEN_RPAREN); // )
                }
                expect_token(TOKEN_RPAREN);
                
            } else if (strcmp(current_token.value, ":action") == 0) {
                advance_token();
                expect_token(TOKEN_IDENTIFIER); // nome-acao
                
                // Processar seções da ação
                while (current_token.type == TOKEN_KEYWORD) {
                    if (strcmp(current_token.value, ":parameters") == 0) {
                        advance_token();
                        expect_token(TOKEN_LPAREN);
                        // Pular parâmetros
                        while (current_token.type != TOKEN_RPAREN && 
                               current_token.type != TOKEN_EOF) {
                            advance_token();
                        }
                        expect_token(TOKEN_RPAREN);
                        
                    } else if (strcmp(current_token.value, ":precondition") == 0) {
                        advance_token();
                        skip_section(); // Pula toda a precondição
                        
                    } else if (strcmp(current_token.value, ":effect") == 0) {
                        advance_token();
                        skip_section(); // Pula todo o efeito
                        
                    } else {
                        // Seção desconhecida
                        advance_token();
                        if (current_token.type == TOKEN_LPAREN) {
                            skip_section();
                        }
                    }
                }
                expect_token(TOKEN_RPAREN); // ) da ação
                
            } else {
                // Seção desconhecida
                skip_section();
            }
        } else {
            fprintf(stderr, "Rejected: Line %d: Expected keyword in domain section\n", 
                    current_token.line);
            exit(1);
        }
    }
    
    expect_token(TOKEN_RPAREN); // ) final do domínio
}

// Parser para problema
void parse_problem() {
    expect_token(TOKEN_LPAREN);     // (
    expect_token(TOKEN_KEYWORD);    // define
    expect_token(TOKEN_LPAREN);     // (
    expect_token(TOKEN_KEYWORD);    // problem
    expect_token(TOKEN_IDENTIFIER); // nome-do-problema
    expect_token(TOKEN_RPAREN);     // )
    
    // Processar seções do problema
    while (current_token.type == TOKEN_LPAREN) {
        advance_token(); // Consome (
        
        if (current_token.type == TOKEN_KEYWORD) {
            if (strcmp(current_token.value, ":domain") == 0) {
                advance_token();
                expect_token(TOKEN_IDENTIFIER); // nome-dominio
                expect_token(TOKEN_RPAREN);
                
            } else if (strcmp(current_token.value, ":objects") == 0) {
                advance_token();
                // Pular objects (similar aos types/constants)
                while (current_token.type == TOKEN_IDENTIFIER || 
                       current_token.type == TOKEN_DASH) {
                    advance_token();
                }
                expect_token(TOKEN_RPAREN);
                
            } else if (strcmp(current_token.value, ":init") == 0) {
                advance_token();
                // Processar estado inicial
                while (current_token.type == TOKEN_LPAREN) {
                    advance_token(); // (
                    
                    // Pode ser um predicado ou uma atribuição de função
                    if (current_token.type == TOKEN_KEYWORD && strcmp(current_token.value, "=") == 0) {
                        // Atribuição de função: (= (function obj) value)
                        advance_token(); // =
                        expect_token(TOKEN_LPAREN); // (
                        expect_token(TOKEN_IDENTIFIER); // nome da função
                        
                        // Pular argumentos
                        while (current_token.type == TOKEN_IDENTIFIER) {
                            advance_token();
                        }
                        expect_token(TOKEN_RPAREN); // ) da função
                        
                        // Pular valor (pode ser número ou identificador)
                        if (current_token.type == TOKEN_NUMBER || current_token.type == TOKEN_IDENTIFIER) {
                            advance_token();
                        }
                    } else {
                        // Predicado normal
                        expect_token(TOKEN_IDENTIFIER); // predicado
                        
                        // Pular objetos
                        while (current_token.type == TOKEN_IDENTIFIER) {
                            advance_token();
                        }
                    }
                    expect_token(TOKEN_RPAREN); // )
                }
                expect_token(TOKEN_RPAREN);
                
            } else if (strcmp(current_token.value, ":goal") == 0) {
                advance_token();
                expect_token(TOKEN_LPAREN);
                // Pular todo o goal balanceando parênteses
                int paren_count = 1;
                while (paren_count > 0 && current_token.type != TOKEN_EOF) {
                    if (current_token.type == TOKEN_LPAREN) {
                        paren_count++;
                    } else if (current_token.type == TOKEN_RPAREN) {
                        paren_count--;
                    }
                    advance_token();
                }
                expect_token(TOKEN_RPAREN); // ) da seção :goal
                
            } else {
                // Seção desconhecida
                skip_section();
            }
        } else {
            fprintf(stderr, "Rejected: Line %d: Expected keyword in problem section\n", 
                    current_token.line);
            exit(1);
        }
    }
    
    expect_token(TOKEN_RPAREN); // ) final do problema
}

// Função principal
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <arquivo_dominio> <arquivo_problema>\n", argv[0]);
        return 1;
    }
    
    // Processar domínio
    input_file = fopen(argv[1], "r");
    if (!input_file) {
        perror("Erro ao abrir arquivo de domínio");
        return 1;
    }
    
    line_number = 1;
    advance_token(); // Lê primeiro token
    
    parse_domain();
    
    if (current_token.type != TOKEN_EOF) {
        fprintf(stderr, "Rejected: Line %d: Extra content after domain\n", 
                current_token.line);
        fclose(input_file);
        return 1;
    }
    
    fclose(input_file);
    
    // Processar problema
    input_file = fopen(argv[2], "r");
    if (!input_file) {
        perror("Erro ao abrir arquivo de problema");
        return 1;
    }
    
    line_number = 1;
    advance_token(); // Lê primeiro token
    
    parse_problem();
    
    if (current_token.type != TOKEN_EOF) {
        fprintf(stderr, "Rejected: Line %d: Extra content after problem\n", 
                current_token.line);
        fclose(input_file);
        return 1;
    }
    
    fclose(input_file);
    
    printf("Accepted\n");
    return 0;
}
