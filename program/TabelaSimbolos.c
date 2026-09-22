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

Simbolo GerarSimbolo (char *nome, Token tipo,TabelaSimbolos *tabela, void *valor) {
    Simbolo simbolo;
    strcpy(simbolo.nome, nome);
    simbolo.token = tipo;
    simbolo.valor = valor;
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

//Como é adicionado em forma de pilha o escopo atual fica no final do vetor
void RemoverScopo(TabelaSimbolos *tabela, int escopo){
    for (int i = tabela->tamanhoLogico - 1; i >= 0; i--) {
        if (tabela->tabela[i].escopo == escopo) {
            RemoverSimbolo(tabela, tabela->tabela[i].nome);
        }
        else{
            //escopo desejado já foi limpo
            tabela->ScopoAtual--;
            break;
        }
    }
}

bool RemoverSimbolo(TabelaSimbolos *tabela, char *nome) {
    ImprimirTabela(tabela);
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
        printf("%s\t%s\t%d ", tabela->tabela[i].nome, tokenString[tabela->tabela[i].token], tabela->tabela[i].escopo);
        printf("%s\n", (char *)tabela->tabela[i].valor);
    }
}



void LiberarTabela(TabelaSimbolos *tabela) {
    for (int i = 0; i < tabela->tamanhoLogico; i++) {
        free(tabela->tabela[i].valor);
    }
    free(tabela->tabela);
}

