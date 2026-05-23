#ifndef TabelaSimbolos_H
#define TabelaSimbolos_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct Simbolo {
    char nome[50];
    char tipo[20];
    int escopo;
    void *valor;
    void (*mostrarValor)(void *valor);
};

struct TabelaSimbolos {
    struct Simbolo *tabela;
    int tamanhoLogico;
    int tamanho;
};

struct TabelaSimbolos _AumentarTabela(struct TabelaSimbolos *tabela);
bool BuscarSimbolo(struct TabelaSimbolos *tabela, char* nome);
bool RemoverSimbolo(struct TabelaSimbolos *tabela, char* nome);
void ImprimirTabela(struct TabelaSimbolos *tabela);
void inicializarTabela(struct TabelaSimbolos *tabela)
bool InserirSimbolo(struct TabelaSimbolos *tabela, struct Simbolo simboloASerInseriddo);
bool BuscarSimbolo(struct TabelaSimbolos *tabela, char* nome) 
bool RemoverSimbolo(struct TabelaSimbolos *tabela, char* nome)
struct TabelaSimbolos _AumentarTabela(struct TabelaSimbolos *tabela)
void ImprimirTabela(struct TabelaSimbolos *tabela) 