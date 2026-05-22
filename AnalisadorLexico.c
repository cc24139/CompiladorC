#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "headers/basics.h"
#include "headers/AnalisadorLexico.h"
/*
	Integrantes:
Nome     RA
Daniel 	24123
Luis 	24139
*/

unsigned int linha = 1;
FILE *arquivo;
char palavraAtual[100];

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


bool retornarProximaPalavra(FILE *arquivo) {
	int charact;
	int i =0;

	while ((charact = fgetc(arquivo)) != EOF && isspace(charact)){
		if(charact == '\n') {
			linha++;
		}
	};

	if (charact == EOF) {
		strcpy(palavraAtual, "EOF");
		return false;
	}

	if(ispunct(charact)) {
		palavraAtual[0] = charact;
		palavraAtual[1] = '\0';
		return true;
	}

	palavraAtual[i++] = charact;

	while(retornarDelimitador(charact = fgetc(arquivo) )==0) {
		if(charact == '\n') {
			linha++;
		}
		palavraAtual[i++] = charact;
	}

	palavraAtual[i] = '\0';

	//Para no delimitador, mas não perde o caractere
	if (charact != EOF) {
		ungetc(charact, arquivo);
	}

	return true;
}

Token Analex()
{
	palavraAtual[0] = '\0';
	if(!(retornarProximaPalavra(arquivo))){
	    return finalDeArquivo;
	}
	if(strcmp(palavraAtual, "EOF") == 0) {
		return finalDeArquivo;
	}
	for (int i = 0; i < NUM_PALAVRAS; i++)
	{
		if (strcmp(palavraAtual, palavras[i]) == 0)
		{
			return i;
		}
	}

	if (!(palavraAtual[0] >= '0' && palavraAtual[0] <= '9'))
	{
		if(!(isalnum(palavraAtual[0])) && strlen(palavraAtual) == 1)
		{
		    return invalido;
		}
		else
		{
		    for (int i = 1; i < strlen(palavraAtual); i++)
    		{
    			if (!(isalnum(palavraAtual[i]) || palavraAtual[i] == '_'))
    			{
    				return invalido;
    			}
    		}
    		return identificador;
		}
	}
	else
	{
		for (int i = 1; i < strlen(palavraAtual); i++)
		{
			if (!(palavraAtual[i] >= '0' && palavraAtual[i] <= '9'))
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
}*/
