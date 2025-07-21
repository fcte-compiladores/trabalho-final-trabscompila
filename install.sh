#!/bin/bash

echo "=== Parser PDDL - Script de Instalação ==="

# Verificar se GCC está instalado
if ! command -v gcc &> /dev/null; then
    echo "Erro: GCC não está instalado. Por favor, instale GCC primeiro."
    exit 1
fi

echo "✓ GCC encontrado: $(gcc --version | head -n1)"

# Compilar o parser
echo "Compilando parser..."
make clean
make

if [ $? -eq 0 ]; then
    echo "✓ Compilação bem-sucedida!"
    
    # Executar testes
    echo "Executando testes..."
    make test
    
    if [ $? -eq 0 ]; then
        echo "✓ Todos os testes passaram!"
        echo ""
        echo "=== Instalação Concluída ==="
        echo "Para usar o parser:"
        echo "  ./pddl_parser <dominio.pddl> <problema.pddl>"
        echo ""
        echo "Exemplos incluídos em 'Exemplos/'"
    else
        echo "✗ Alguns testes falharam!"
        exit 1
    fi
else
    echo "✗ Erro na compilação!"
    exit 1
fi
