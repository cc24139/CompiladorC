#include "basics.h"
#include "AnalisadorLexico.h"

void verificaProgam() {
	Token token = anaLex();
	if (token != Programa) {
		printf("Erro: esperava a palavra program!");
		exit(1);
	}
	token = anaLex();
	if (token != identificador) {
		printf("Erro: esperava a palavra identificador!");
		exit(1);
	}
	token = anaLex();
	if (token != abreParenteses) {
		printf("Erro: esperava a palavra abreParenteses!");
		exit(1);
	}
	while (token != fechaParenteses) {
		token = anaLex();
		if (token != identificador) {
			printf("Erro: esperava a palavra identificador!");
			exit(1);
		}
		token = anaLex();
		if (token != virgula || token != fechaParenteses) {
			printf("Erro: esperava a palavra virgula ou fechaParenteses!");
			exit(1);
		}
	}
	token = anaLex();
	if (token != pontoEVirgula) {
		printf("Erro: esperava a palavra virgula ou pontoEVirgula!");
		exit(1);
	}
	verificaBloco();
	token = anaLex();
	if (token != ponto) {
		printf("Erro: esperava a palavra virgula ou ponto!");
		exit(1);
	}
}
void verificaBloco() {
	Token token = anaLex();
	if(token == label){
		AtribuirLabel();
	}

	token = anaLex();
    if (verificaType(Token) == false){
        printf("Erro: Esperava um tipo");
        exit(1);
    }
    while(verificaType(token) == true){
        AtribuirVariavel();
        token = anaLex();
    }
    if (token != implicito){
        printf("Erro: Esperava implicito");
		exit(1);
    }
	token = analex();
	if(token != identificador){
		printf("Erro: Esperava identificador");
		exit(1);
	}
	while(token != identificador ){
		AtribuirTipoImplicito();
		token = analex();
	}
	token = analex();
	if(token != procedimento || token != )
    
}

void  AtribuirLabel(){
	Token token = anaLex();
	if (token != numero) {
		printf("Erro: esperava a palavra numero!");
		exit(1);
	}
	token = anaLex();
	if (token != virgula || token != pontoevirgula) {
		printf("Erro: esperava a palavra virgula ou pontoEVirgula");
		exit(1);
	}
	while(token != virgula) {
		token = anaLex();
		if(token != numero) {
			printf("Erro: esperava a palavra numero");
			exit(1);
		}
		token = anaLex();
		if (token != virgula || token != pontoevirgula) {
			printf("Erro: esperava a palavra virgula ou pontoEVirgula");
			exit(1);
		}
	}
	
}

void AtribuirVariavel(){
    Token token = anaLex();
    if(token != identificador){
        printf("Erro:Esperava um identificador");
        exit(1);
    }
    token = anaLex();
    if(token != igual){
        printf("Erro: Esperava um =");
        exit(-1);
    }
    ///Verificar tipo
    token = anaLex();
    if(VerificarTipo(token) == false){
        printf("Erro: Esperava um tipo");
        exit(-1);
    }
    token = anaLex();
    if(token != pontoEVirgula){
        printf("Erro: Esperava um pontoEVirgula");
        exit(-1);
    }
}

void AtribuirTipoImplicito(){
	Token token;
	while( (token= analex()) != virgula ){
		token  = analex();
		if(token != identificador){
			printf("Erro: esperava-se um identificador");
			exit(1);
		}
	}
	if(token != doispontos){
		printf("Erro: esperava-se dois pontos");
		exit(1);
	}
	token = analex();
	if(token != tipo ){
		printf("Erro: esperava-se um tipo");
		exit(1);
	}
	token = analex();
	if(token != pontoevirgula){
		printf("Erro: esperava-se um ponto e virgula");
		exit(1);
	}
}

void VerificaComandoSemRotulo()
{
	token = anaLex();
	if(token == identificador)
	{
		token = anaLex();
		if (token == abrecolchetes)
		{
			verificaExpressao();
			token = anaLex();
			while(token != fechacolchetes)
			{
				if(token != virgula)
				{
					printf("Erro: Esperava-se uma virgula");
        			exit(-1);
				}
				VerificaExpressao();
			}
			token = anaLex();
			if(token == atribuicao)
			{
				VerificaExpressao();
			}
		}
		if(token == atribuicao)
		{
			VerificaExpressao();
		}
		if(token == abreparenteses)
		{
			while(token != fechaparenteses)
			{
				if(token != virgula)
				{
					printf("Erro: Esperava-se uma virgula");
        			exit(-1);
				}
				VerificaExpressao();
				token = anaLex();
			}
		}
		
	}
	else if(token == vapara)
	{
		token = anaLex();
		if (token != numero)
		{
			printf("Erro: Esperava-se um número");
        	exit(-1);
		}		
	}
	else if(token == inicio)
	{
		VerificaComando();
		token = anaLex();
		while(token != fim)
		{
			if (token != doispontos)
			{
				printf("Erro: Esperava-se um dois pontos");
        		exit(-1);
			}
			verificaComando();
			token = anaLex();
		}
	}
	else if (token == se)
	{
		VerificaExpressao();
		token = anaLex();
		if(token != entao)
		{
			printf("Erro: Esperava-se um entao");
        	exit(-1);
		}
		VerificaComandoSemRotulo();
		token = anaLex();
		if (token == senao)
		{
			VerificaComandoSemRotulo();
		}
		
	}
	else if(token == enquanto)
	{
		VerificaExpressao();
		token = anaLex();
		if (token != faca)
		{
			printf("Erro: Esperava-se um faca");
        	exit(-1);
		}
		VerificaComandoSemRotulo();
	}
	else
	{
		printf("Erro: Esperava-se um identificador, vapara, inicio, se ou enquanto");
		exit(-1);
	}
	
}

void VerificaComando()
{
	verificaComandoSemRotulo();
	token = anaLex();
	if (token != numero)
	{
		printf("Erro: Esperava-se um número ou comando sem rótulo");
		exit(-1);
	}
	token = anaLex();
	if (token != doispontos)
	{
		printf("Erro: Esperava-se dois pontos");
		exit(-1);
	}
	VerificaComandoSemRotulo();
}
