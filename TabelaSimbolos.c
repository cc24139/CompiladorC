#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "headers/basics.h"

struct TabelaSimbolos _AumentarTabela(struct TabelaSimbolos *tabela);
bool BuscarSimbolo(struct TabelaSimbolos *tabela, char* nome);
bool RemoverSimbolo(struct TabelaSimbolos *tabela, char* nome);
void ImprimirTabela(struct TabelaSimbolos *tabela);

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

void inicializarTabela(struct TabelaSimbolos *tabela){
    tabela->tamanhoLogico = 0;
    tabela->tamanho = 100;
    tabela->tabela = (struct Simbolo *)malloc(tabela->tamanho * sizeof(struct Simbolo));
}

bool InserirSimbolo(struct TabelaSimbolos *tabela, struct Simbolo simboloASerInseriddo){
    if(tabela->tamanhoLogico >= tabela->tamanho) {
        *tabela = _AumentarTabela(tabela);
    }
    if (!BuscarSimbolo(tabela, simboloASerInseriddo.nome)) {
        tabela->tabela[tabela->tamanhoLogico] = simboloASerInseriddo;
        tabela->tamanhoLogico++;
        return true;
    }
    return false;
}

bool BuscarSimbolo(struct TabelaSimbolos *tabela, char* nome) {
    for (int i = 0; i < tabela->tamanhoLogico; i++) {
        if (strcmp(tabela->tabela[i].nome, nome) == 0) {
            return true;
        }
    }
    return false;
}

bool RemoverSimbolo(struct TabelaSimbolos *tabela, char* nome) {
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

struct TabelaSimbolos _AumentarTabela(struct TabelaSimbolos *tabela){
    tabela->tamanho *= 2;
    struct TabelaSimbolos novaTabela;
    novaTabela.tamanho = tabela->tamanho;
    novaTabela.tamanhoLogico = tabela->tamanhoLogico;
    novaTabela.tabela = (struct Simbolo *)malloc(novaTabela.tamanho * sizeof(struct Simbolo));
    for (int i = 0; i < tabela->tamanhoLogico; i++) {
        novaTabela.tabela[i] = tabela->tabela[i];
    }
    free(tabela->tabela);
    return novaTabela;
}

void ImprimirTabela(struct TabelaSimbolos *tabela) {
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

void LiberarTabela(struct TabelaSimbolos *tabela) {
    for (int i = 0; i < tabela->tamanhoLogico; i++) {
        free(tabela->tabela[i].valor);
    }
    free(tabela->tabela);
}

int main() {
    struct TabelaSimbolos tabela;
    inicializarTabela(&tabela);
    void* valorInt1 = malloc(sizeof(int));
    *(int *)valorInt1 = 12;
    struct Simbolo simbolo1 = {"x", "int", 0, valorInt1, mostrarValorInt};
    struct Simbolo simbolo2 = {"y", "float", 0,  valorInt1, mostrarValorInt};
    struct Simbolo simbolo3 = {"z", "char", 0,  valorInt1, mostrarValorInt};

    InserirSimbolo(&tabela, simbolo1);
    InserirSimbolo(&tabela, simbolo2);
    InserirSimbolo(&tabela, simbolo3);

    ImprimirTabela(&tabela);

    RemoverSimbolo(&tabela, "y");

    printf("\nApós remover o símbolo 'y':\n");
    ImprimirTabela(&tabela);

    free(tabela.tabela);
    return 0;
}