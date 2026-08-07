#ifndef TABELA_SIMBOLOS_H
#define TABELA_SIMBOLOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "basics.h"

typedef struct Simbolo {
    char nome[50];
    char tipo[20];
    int escopo;
    void *valor;
    void (*mostrarValor)(void *valor);
} Simbolo;

typedef struct TabelaSimbolos {
    Simbolo *tabela;
    int tamanhoLogico;
    int tamanho;
} TabelaSimbolos;

void inicializarTabela(TabelaSimbolos *tabela);
bool InserirSimbolo(TabelaSimbolos *tabela, Simbolo simboloASerInseriddo);
bool RemoverUltimoSimbolo(TabelaSimbolos *tabela);
bool BuscarSimbolo(TabelaSimbolos *tabela, char *nome);
bool RemoverSimbolo(TabelaSimbolos *tabela, char *nome);
void ImprimirTabela(TabelaSimbolos *tabela);
void mostrarValorInt(void *valor);
void LiberarTabela(TabelaSimbolos *tabela);

#endif


