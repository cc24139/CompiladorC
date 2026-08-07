#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TabelaSimbolos.h"
#include "AnalisadorLexico.h"

static TabelaSimbolos _AumentarTabela(TabelaSimbolos *tabela);

void inicializarTabela(TabelaSimbolos *tabela){
    tabela->tamanhoLogico = 0;
    tabela->tamanho = 100;
    tabela->ScopoAtual = 0;
    tabela->tabela = malloc(tabela->tamanho * sizeof(Simbolo));
}

Simbolo GerarSimbolo (char *nome, Token tipo,TabelaSimbolos *tabela) {
    Simbolo simbolo;
    strcpy(simbolo.nome, nome);
    simbolo.token = tipo;
    if(simbolo.token == funcao || simbolo.token == procedimento) {
        tabela->ScopoAtual++;
        simbolo.escopo = tabela->ScopoAtual;
    }else{
        simbolo.escopo = tabela->ScopoAtual;
    }
    return simbolo;
}



bool InserirSimbolo(TabelaSimbolos *tabela, Simbolo simboloASerInseriddo){
    if(tabela->tamanhoLogico >= tabela->tamanho) {
        *tabela = _AumentarTabela(tabela);
    }
        tabela->tabela[tabela->tamanhoLogico] = simboloASerInseriddo;
        tabela->tamanhoLogico++;
        return true;
    }
    return false;
}

bool RemoverUltimoSimbolo(TabelaSimbolos *tabela){
    if (tabela->tamanhoLogico <= 0)
        return false;
    return RemoverSimbolo(tabela, tabela->tabela[tabela->tamanhoLogico - 1].nome);
}

bool BuscarSimbolo(TabelaSimbolos *tabela, char *nome) {
    for (int i = 0; i < tabela->tamanhoLogico; i++) {
        if (strcmp(tabela->tabela[i].nome, nome) == 0) {
            return true;
        }
    }
    return false;
}

bool RemoverSimbolo(TabelaSimbolos *tabela, char *nome) {
    for (int i = 0; i < tabela->tamanhoLogico; i++) {
        if (strcmp(tabela->tabela[i].nome, nome) == 0) {
            for (int j = i; j < tabela->tamanhoLogico - 1; j++) {
                tabela->tabela[j] = tabela->tabela[j + 1];
            }
            tabela->tamanhoLogico--;
            return true;
        }
    }
    return false;
}

TabelaSimbolos _AumentarTabela(TabelaSimbolos *tabela){
    tabela->tamanho *= 2;
    TabelaSimbolos novaTabela;
    novaTabela.tamanho = tabela->tamanho;
    novaTabela.tamanhoLogico = tabela->tamanhoLogico;
    novaTabela.tabela = malloc(novaTabela.tamanho * sizeof(Simbolo));
    for (int i = 0; i < tabela->tamanhoLogico; i++) {
        novaTabela.tabela[i] = tabela->tabela[i];
    }
    free(tabela->tabela);
    return novaTabela;
}

void ImprimirTabela(TabelaSimbolos *tabela) {
    printf("Tabela de Simbolos:\n");
    printf("Nome\tTipo\tEscopo\n");
    for (int i = 0; i < tabela->tamanhoLogico; i++) {
        printf("%s\t%s\t%d ", tabela->tabela[i].nome, tabela->tabela[i].tipo, tabela->tabela[i].escopo);
        tabela->tabela[i].mostrarValor(tabela->tabela[i].valor);
    }
}

void mostrarValorInt(void *valor) {
    printf("%d\n", *(int *)valor);
}

void LiberarTabela(TabelaSimbolos *tabela) {
    for (int i = 0; i < tabela->tamanhoLogico; i++) {
        free(tabela->tabela[i].valor);
    }
    free(tabela->tabela);
}

