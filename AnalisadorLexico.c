#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define numPalavras 36
FILE *arquivo;
typedef enum{
    false,
    true
}bool;

const char *palavras[] = {
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

char *tokenString[39] = {
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

typedef enum {
	programa,
	rotulo,
	tipo,
	variavel,
	procedimento,
	funcao,
	inicio,
	fim,
	atribuicao,
	se,
	entao,
	senao,
	enquanto,
	faca,
	vapara,
	mais,
	menos,
	vezes,
	dividir,
	igual,
	diferente,
	menor,
	menorouigual,
	maior,
	maiorouigual,
	e,
	ou,
	nao,
	abreparenteses,
	fechaparenteses,
	abrecolchetes,
	fechacolchetes,
	virgula,
	pontoevirgula,
	doispontos,
	ponto,
	numero,
	identificador,
	finalDeArquivo
} Token;

int retornarDelimitador(char letra) {
	if(isspace(letra) || ispunct(letra)) {
		return true;
	}
	return false;
}

int retornarProximaPalavra(FILE *arquivo,char *palavra) {
	int charact;
	int i =0;

	while ((charact = fgetc(arquivo)) != EOF && isspace(charact));

	if (charact == EOF) {
		palavra = "EOF";
		return false;
	}
	if(isspace(charact) || ispunct(charact)) {
		palavra [0] = charact;
		palavra[1] = '\0';
		return true;
	}
	palavra[i++] = charact;
	while(retornarDelimitador(charact = fgetc(arquivo))==0) {
		palavra[i++] = charact;
	}
	palavra[i++] = '\0';

	if (charact != EOF) {
		ungetc(charact, arquivo);
	}
	return true;
}


Token AnalisadorLexico()
{
	char *palavra;
	if(!(retornarProximaPalavra(arquivo,palavra))){
	    return finalDeArquivo;
	}
	if(palavra == "EOF") {
		return finalDeArquivo;
	}
	for (int i = 0; i < numPalavras; i++)
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
		    return -1;
		}
		else
		{
		    for (int i = 1; i < strlen(palavra); i++)
    		{
    			if (!(isalnum(palavra[i]) || palavra[i] == '_'))
    			{
    				return -1;
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
				return -1;
			}
		}
		return numero;
	}
	return -1;
}






int main()
{
	arquivo = fopen("arq.txt", "r");
	Token token = AnalisadorLexico();
	while(token != finalDeArquivo) {
		printf("%s \n",tokenString[token]);
		token = AnalisadorLexico();
	}
		printf("%s \n",tokenString[token]);
}
