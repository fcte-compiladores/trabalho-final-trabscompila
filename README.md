# Título: PARSER DE PDDL
#### Integrante: PEDRO AUGUSTO DOURADO IZARIAS - 200062620 - T03

## Introdução: 
Este projeto implementa um parser completo para a linguagem PDDL (Planning Domain Definition Language), uma linguagem especializada para descrição de problemas de planejamento automatizado em inteligência artificial. O parser realiza análise lexical e sintática de arquivos PDDL, validando tanto domínios quanto problemas de planejamento.

## Estratégias e Algoritmos Implementados:

#### Análise Lexical
- Tokenização: Implementação de lexer que reconhece tokens PDDL (parênteses, identificadores, keywords, números, operadores)
- Tratamento de comentários: Ignora linhas iniciadas com `;`
- Controle de linha: Rastreamento preciso de números de linha para relatório de erros
- Reconhecimento de operadores: Suporte a operadores matemáticos (`=`, `<`, `>`, `<=`, `>=`)

#### Análise Sintática  
- Implementação de parser que segue a gramática hierárquica do PDDL
- Verificação de parênteses: Validação de estruturas aninhadas

#### Extensões PDDL Suportadas
- PDDL básico: `:strips`, `:typing`
- Extensões numéricas: `:fluents`, funções numéricas, operadores aritméticos
- Extensões avançadas: `:adl`, `:conditional-effects`, `:durative-actions`

## Sintaxe e Semântica PDDL:

PDDL é uma linguagem declarativa baseada em lógica de primeira ordem. A linguagem separa a descrição do domínio (regras gerais) do problema específico.

#### Exemplo de Domínio
```pddl
(define (domain logistics)
    (:requirements :strips :typing)
    (:types location package vehicle)
    (:predicates 
    (at ?obj - object ?loc - location)
    (in ?pkg - package ?vehicle - vehicle))
    (:action load
    :parameters (?pkg - package ?truck - vehicle ?loc - location)
    :precondition (and (at ?pkg ?loc) (at ?truck ?loc))
    :effect (and (in ?pkg ?truck) (not (at ?pkg ?loc))))
)
```

#### Exemplo de Problema
```pddl
(define (problem delivery-task)
    (:domain logistics)
    (:objects truck1 - vehicle package1 - package depot warehouse - location)
    (:init (at truck1 depot) (at package1 depot))
    (:goal (at package1 warehouse))
)
```

#### Semântica
- Domínios definem tipos, predicados e ações disponíveis
- Problemas especificam objetos, estado inicial e objetivos
- Ações têm parâmetros, precondições e efeitos
- Precondições definem quando uma ação pode ser executada
- Efeitos especificam como uma ação modifica o estado do mundo

## Instalação: 

#### Pré-requisitos
- GCC ou outro compilador C
- Sistema Unix/Linux/macOS (testado no macOS)
- Git para clonar o repositório

#### Instalação Automatizada
```bash
# Clonar o repositório
git clone https://github.com/fcte-compiladores/trabalho-final-trabscompila.git
cd trabalho-final-trabscompila

# Executar script de instalação
./install.sh
```

## Uso:

#### Exemplos
- ./pddl_parser "Exemplos/Delivery/domain.pddl" "Exemplos/Delivery/problem.pddl"
- ./pddl_parser "Exemplos/Tracking/domain.pddl" "Exemplos/Tracking/problem.pddl"

#### Rodar todos os exemplos
- `make test`

#### Saídas
- `Accepted`: Arquivos PDDL são válidos sintaticamente
- `Rejected: Line X: [erro]`: Erro sintático com localização

## Exemplos:
O projeto contém uma pasta `Exemplos/` com arquivos PDDL organizados em graus variáveis de complexidade:

#### 1. Hello-World
- Características: Predicados básicos, uma ação, sem tipos

#### 2. Sequence
- Características: `:typing`, tipos hierárquicos, precondições múltiplas

#### 3. Logistics
- Características: Múltiplas ações interconectadas, efeitos condicionais, tipos especializados

#### 4. Factory
- Características: `:fluents`, `:adl`, funções numéricas, quantificadores existenciais

#### 5. Delivery, Tracking, World of Tanks
- Exemplos de domínios mais elaborados
- Características: Combinam múltiplas extensões PDDL avançadas

## Referências: 
- McDermott, Drew et al. (1998) - "PDDL - The Planning Domain Definition Language" - Especificação original da sintaxe PDDL
- [Gramática BNF do PDDL 3](https://artificial-intelligence.unibs.it/gerevini/papers/tmp/AIJ-3828final/appendix/appendix-bnfpddl30.pdf) 
- Enunciado do exercício Analisador Sintático: PDDL, desenvolvido pelo Prof. Bruno Ribas que inspirou a escolha desse tema para o trabalho: Preâmbulo
O objetivo deste exercício é desenvolver um analisador sintático para um subconjunto
da linguagem de descrição Planning Domain Definition Language (PDDL). Seu
programa deve receber como argumento dois arquivos: um contendo
uma descrição de domínio e outro contendo uma descrição de problema, ambos em
PDDL. O programa deve analisar o conteúdo e responder se os códigos-fonte from
aceitos ou não sitaticamente, caso não, deverá também apresentar a linha do
arquivo na qual o primeiro erro foi encontrado.


## Estrutura do código: 
- Arquivo principal: `pddl_parser_novo.c` - implementação monolítica com todas as funcionalidades
- Estruturas de dados: `enum TokenType` (tipos de tokens), `struct Token` (token atual), arrays para buffers de strings
- Análise Lexical: função `advance_token()` - tokenização, tratamento de comentários, rastreamento de linha
- Análise Sintática: funções `parse_domain()` e `parse_problem()` - parsing recursivo descendente seguindo gramática PDDL
- Tratamento de erros: função `expect_token()` - validação de tokens esperados com localização precisa de erros
- Validação semântica básica: verificação de balanceamento de parênteses e estruturas obrigatórias integrada ao parser
- Sistema de build: `Makefile` com targets para compilação, teste e instalação



## Bugs/Limitações/problemas conhecidos: 
- Extensões PDDL: suporte parcial a `:fluents` e `:adl`, faltam `:derived-predicates` e validação matemática completa
- Portabilidade: testado apenas em macOS/Unix, pode ter problemas em outros sistemas
- Melhorias incrementais futuras: adicionar validação de escopo de variáveis, recuperação de erros, modo verbose para debugging, suporte completo a todas extensões PDDL
