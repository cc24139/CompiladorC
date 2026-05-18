#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "headers/basics.h"
#include "headers/AnalisadorLexico.h"


unsigned int linha = 1;
FILE *arquivo;

char *palavras[] = {
	 "program",
	 "label",
	 "type",
	 "var",
	 "procedure",
	 "function",
	 "begin",
	 "end",
	 ":=",
	 "if",
	 "then",
	 "else",
	 "while",
	 "do",
	 "goto",
	 "+",
	 "-",
	 "*",
	 "div",
	 "=",
	 "<>",
	 "<",
	 "<=",
	 ">",
	 ">=",
	 "and",
	 "or",
	 "not",
	 "(",
	 ")",
	 "[",
	 "]",
	 ",",
	 ";",
	 ":",
	 ".",
	 "EOF"
};

char *tokenString[] = {
	 "programa",
	 "rotulo",
	 "tipo",
	 "variavel",
	 "procedimento",
	 "funcao",
	 "inicio",
	 "fim",
	 "atribuicao",
	 "se",
	 "entao",
	 "senao",
	 "enquanto",
	 "faca",
	 "vapara",
	 "mais",
	 "menos",
	 "vezes",
	 "dividir",
	 "igual",
	 "diferente",
	 "menor",
	 "menorouigual",
	 "maior",
	 "maiorouigual",
	 "e",
	 "ou",
	 "nao",
	 "abreparenteses",
	 "fechaparenteses",
	 "abrecolchetes",
	 "fechacolchetes",
	 "virgula",
	 "pontoevirgula",
	 "doispontos",
	 "ponto",
	 "numero",
	 "identificador",
	 "fimDeArquivo"
};
int retornarDelimitador(char letra) {
	if(isspace(letra) || ispunct(letra)) {
		return true;
	}
	return false;
}

int retornarProximaPalavra(FILE *arquivo,char *palavra) {
	int charact;
	int i =0;

	while ((charact = fgetc(arquivo)) != EOF && isspace(charact)){
		if(charact == '\n') {
			linha++;
		}
	};

	if (charact == EOF) {
		strcpy(palavra, "EOF");
		return false;
	}

	if(ispunct(charact)) {
		palavra[0] = charact;
		palavra[1] = '\0';
		return true;
	}

	palavra[i++] = charact;

	while(retornarDelimitador(charact = fgetc(arquivo))==0) {
		if(charact == '\n') {
			linha++;
		}
		palavra[i++] = charact;
	}

	palavra[i] = '\0';

	if (charact != EOF) {
		ungetc(charact, arquivo);
	}

	return true;
}


Token Analex()
{
	char palavra[100];
	if(!(retornarProximaPalavra(arquivo,palavra))){
	    return finalDeArquivo;
	}
	if(strcmp(palavra, "EOF") == 0) {
		return finalDeArquivo;
	}
	for (int i = 0; i < NUM_PALAVRAS; i++)
	{
		if (strcmp(palavra, palavras[i]) == 0)
		{
			return i;
		}
	}

	if (!(palavra[0] >= '0' && palavra[0] <= '9'))
	{
		if(!(isalnum(palavra[0])) && strlen(palavra) == 1)
		{
		    return invalido;
		}
		else
		{
		    for (int i = 1; i < strlen(palavra); i++)
    		{
    			if (!(isalnum(palavra[i]) || palavra[i] == '_'))
    			{
    				return invalido;
    			}
    		}
    		return identificador;
		}
	}
	else
	{
		for (int i = 1; i < strlen(palavra); i++)
		{
			if (!(palavra[i] >= '0' && palavra[i] <= '9'))
			{
				return invalido;
			}
		}
		return numero;
	}
	return invalido;
}





/*
int main()
{
	arquivo = fopen("./arq.txt", "r");
	printf("Analisador Léxico\n");
	Token token = Analex();
	while(token != finalDeArquivo) {
	    if(token == invalido){
		    printf("token inválido! linha: %d", linha);
		    return 1;
	    }
		printf("%s \n",tokenString[token]);
		token = Analex();

	}
		printf("%s \n",tokenString[token]);
}
*/