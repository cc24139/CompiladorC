#ifndef ANALEX_H
#define ANALEX_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define NUM_PALAVRAS 37

 extern char *palavras[];
 extern char *tokenString[];
typedef enum {
    programa, rotulo,
	 tipo, variavel, procedimento, funcao,
    inicio, fim, atribuicao, se, entao, senao, enquanto, faca,
    vapara, mais, menos, vezes, dividir, igual, diferente,
    menor, menorouigual, maior, maiorouigual, e, ou, nao,
    abreparenteses, fechaparenteses, abrecolchetes,
    fechacolchetes, virgula, pontoevirgula, doispontos,
    ponto, numero, identificador, finalDeArquivo, invalido
} Token;

int retornarDelimitador(char letra);
int retornarProximaPalavra(FILE *arquivo, char *palavra);
Token Analex();

extern unsigned int linha;
extern FILE *arquivo;

#endif