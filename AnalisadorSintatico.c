#include "headers/basics.h"
#include "headers/AnalisadorLexico.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

static Token token;
//assinatura de atribuir bloco para procedimento usar
void verificaBloco();
void VerificaComando();
void VerificaExpressao();

bool verificaType(Token token){
	if(token != tipo){
		printf("Erro: Esperava-se um tipo. Linha: %u, função: %s()\n", linha, __func__);
		exit(-1);
	}
	return true;
}

void Fator(){
	if(token != identificador || token != numero || token != abreparenteses || token != nao){
		printf("Erro: Esperava-se um identificador, número, abreparenteses ou nao. Linha: %u, função: %s()\n", linha, __func__);
		exit(-1);
	}
	if(token == numero){
		return;
	}
	if (token == nao){
		token = Analex();
		Fator();
		return;
	}
	if(token == abreparenteses){
		token = Analex();
		VerificaExpressao();
		token = Analex();
		if(token != fechaparenteses){
			printf("Erro: Esperava-se um fechaparenteses. Linha: %u, função: %s()\n", linha, __func__);
			exit(-1);
		}
		return;
	}
	token = Analex();
	if(token == abreparenteses){
		VerificaExpressao();
		token = Analex();
		while(token == virgula){
			VerificaExpressao();
			token = Analex();
		}
		if(token != fechaparenteses){
			printf("Erro: Esperava-se um fechaparenteses. Linha: %u, função: %s()\n", linha, __func__);
			exit(-1);
		}
		return;
	}
	if (token == abrecolchetes){
		VerificaExpressao();
		token = Analex();
		while(token == virgula){
			VerificaExpressao();
			token = Analex();
		}
		if(token != fechacolchetes){
			printf("Erro: Esperava-se um fechacolchetes. Linha: %u, função: %s()\n", linha, __func__);
			exit(-1);
		}
		return;
	}
}

void Termo(){
	if(token != identificador){
		printf("Erro: Esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
		exit(-1);
	}
	token = Analex();
	while(token == vezes || token == dividir || token == e){
		token = Analex();
		if(token != identificador){
			printf("Erro: Esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
			exit(-1);
			Fator();
		}
	}
}

void ExpressaoSimples(){
	token = Analex();
	if(token != mais && token != menos && token != identificador){
		printf("Erro: Esperava-se um operador ou um Termo. Linha: %u, função: %s()\n", linha, __func__);
		exit(-1);
	}
	if(token == mais || token == menos){
		token = Analex();
		if(token != identificador){
			printf("Erro: Esperava-se um Termo. Linha: %u, função: %s()\n", linha, __func__);
			exit(-1);
		}
	}
	Termo();
	token = Analex();
	while(token == mais || token == menos || token == ou){
		token = Analex();
		if(token != identificador){
			printf("Erro: Esperava-se um Termo. Linha: %u, função: %s()\n", linha, __func__);
			exit(-1);
		}
		Termo();
		token = Analex();
	}
}

void VerificaExpressao(){
	ExpressaoSimples();
	token = Analex();
	if(token == igual || token == diferente || token == menor || token == menorouigual || token == maior || token == maiorouigual){
		token = Analex();
		if(token != identificador){
			printf("Erro: Esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
			exit(-1);
		}
		ExpressaoSimples();
	}
}

void VerificaComandoSemRotulo()
{
	token = Analex();
	if(token == identificador)
	{
		token = Analex();
		if (token == abrecolchetes)
		{
			VerificaExpressao();
			token = Analex();
			while(token != fechacolchetes)
			{
				if(token != virgula)
				{
					printf("Erro: Esperava-se uma virgula. Linha: %u, função: %s()\n", linha, __func__);
					exit(-1);
				}
				VerificaExpressao();
			}
			token = Analex();
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
					printf("Erro: Esperava-se uma virgula. Linha: %u, função: %s()\n", linha, __func__);
					exit(-1);
				}
				VerificaExpressao();
				token = Analex();
			}
		}
	}
	else if(token == vapara)
	{
		token = Analex();
		if (token != numero)
		{
			printf("Erro: Esperava-se um número. Linha: %u, função: %s()\n", linha, __func__);
			exit(-1);
		}		
	}
	else if(token == inicio)
	{
		VerificaComando();
		token = Analex();
		while(token != fim)
		{
			if (token != doispontos)
			{
				printf("Erro: Esperava-se um dois pontos. Linha: %u, função: %s()\n", linha, __func__);
				exit(-1);
			}
			VerificaComando();
			token = Analex();
		}
	}
	else if (token == se)
	{
		VerificaExpressao();
		token = Analex();
		if(token != entao)
		{
			printf("Erro: Esperava-se um entao. Linha: %u, função: %s()\n", linha, __func__);
			exit(-1);
		}
		VerificaComandoSemRotulo();
		token = Analex();
		if (token == senao)
		{
			VerificaComandoSemRotulo();
		}
	}
	else if(token == enquanto)
	{
		VerificaExpressao();
		token = Analex();
		if (token != faca)
		{
			printf("Erro: Esperava-se um faca. Linha: %u, função: %s()\n", linha, __func__);
			exit(-1);
		}
		VerificaComandoSemRotulo();
	}
	else
	{
		printf("Erro: Esperava-se um identificador, vapara, inicio, se ou enquanto. Linha: %u, função: %s()\n", linha, __func__);
		exit(-1);
	}
}
void VerificaComando()
{
	VerificaComandoSemRotulo();
	token = Analex();
	if (token != numero)
	{
		printf("Erro: Esperava-se um número ou comando sem rótulo. Linha: %u, função: %s()\n", linha, __func__);
		exit(-1);
	}
	token = Analex();
	if (token != doispontos)
	{
		printf("Erro: Esperava-se dois pontos. Linha: %u, função: %s()\n", linha, __func__);
		exit(-1);
	}
	VerificaComandoSemRotulo();
}

bool EhBloco(){
	bool Ehrotulo = token == rotulo;
	bool EhTipo = token == tipo;
	bool EhImplicito = token == EhImplicito;
	bool EhProcedimento = token == procedimento;
	bool EhFuncao = token == funcao;
	bool EhBegin = token == inicio;
	return Ehrotulo || EhTipo ||  EhImplicito||EhProcedimento || EhFuncao || EhBegin;
}

void parametrosFormais(Token tokenComparativo){
	while (token == pontoevirgula){
		token = Analex();
		if(token != identificador  && token != variavel && token != funcao && token != procedimento){
			printf("Erro: Esperava-se um identificador, variavel, funcao ou procedimento. Linha: %u, função: %s()\n", linha, __func__);
			exit(-1);
		}
		if(token == variavel){
			token = Analex();
			if(token != identificador){
				printf("Erro: Esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
				exit(-1);
			}
		}
		if (token == identificador){
			token = Analex();
			while(token == virgula){
				token = Analex();
				if(token != identificador){
					printf("Erro: Esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
					exit(-1);
				}
				token = Analex();
			}
			if(token != doispontos){
				printf("Erro: Esperava-se um dois pontos. Linha: %u, função: %s()\n", linha, __func__);
				exit(-1);
			}
			token = Analex();
		}
		if(token == funcao){
			token = Analex();
			if(token != identificador){
				printf("Erro: Esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
				exit(-1);
			}
			token = Analex();
			while(token == virgula){
				token = Analex();
				if(token != identificador){
					printf("Erro: Esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
					exit(-1);
				}
				token = Analex();
			}
			if(token != doispontos){
				printf("Erro: Esperava-se um dois pontos. Linha: %u, função: %s()\n", linha, __func__);
				exit(-1);
			}
			token = Analex();
			if(token != identificador){
				printf("Erro: Esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
				exit(-1);
			}
			token = Analex();
		}
		if(token == procedimento){
			token = Analex();
			if(token != identificador){
				printf("Erro: Esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
				exit(-1);
			}
			token = Analex();
			while (token == virgula)
			{
				token = Analex();
				if(token != identificador){
					printf("Erro: Esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
					exit(-1);
				}
				token = Analex();
			}

			
	}
	}
	if(token != fechaparenteses){
		printf("Erro: Esperava-se um fechaparenteses. Linha: %u, função: %s()\n", linha, __func__);
		exit(-1);
	}
}

void AtribuirParametosFormais(Token tokenComparativo){
	parametrosFormais(tokenComparativo);
	token = Analex();
	if(token != tokenComparativo){
		printf("Erro: esperava-se um ponto e virgula. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
}

void AtribuirFuncao(){
	token = Analex();
	if(token != identificador){
		printf("Erro: esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	token = Analex();
	if(token != abreparenteses || token != doispontos){
		printf("Erro: esperava-se um parametro formal ou um dois pontos. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	AtribuirParametosFormais(doispontos);
	token = Analex();
	if(token != identificador){
		printf("Erro: esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	token = Analex();
	if(token != EhBloco()){
		printf("Erro: esperava-se um bloco. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
}

void AtribuirProcedimento(){
	token = Analex();
	if(token != abreparenteses || token != EhBloco()){
		printf("Esperava-se um parametro formal ou um bloco!");
		exit(1);
	}
	if(token == abreparenteses){
		parametrosFormais(pontoevirgula);
	}
	token = Analex();
	if(token != EhBloco()){
		printf("Esperava-se um bloco!");
		exit(1);
	}
	verificaBloco();
	token = Analex();
	if(token != pontoevirgula){
		printf("Erro: esperava-se um ponto e virgula. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
}

void AtribuirTipoImplicito(){
	token;
	token = Analex();
	if(token != variavel){
		printf("Erro: esperava-se a uma variavel. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	token = Analex();
	if(token != identificador){
		printf("esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	while( (token= Analex()) != virgula ){
		token  = Analex();
		if(token != identificador){
			printf("Erro: esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
			exit(1);
		}
	}
	if(token != doispontos){
		printf("Erro: esperava-se dois pontos. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	token = Analex();
	if(token != tipo ){
		printf("Erro: esperava-se um tipo. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	token = Analex();
	if(token != pontoevirgula){
		printf("Erro: esperava-se um ponto e virgula. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
}

void AtribuirVariavel(){
	token = Analex();
	if(token != identificador){
		printf("Erro: Esperava um identificador. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	token = Analex();
	if(token != igual){
		printf("Erro: Esperava um =. Linha: %u, função: %s()\n", linha, __func__);
		exit(-1);
	}
	token = Analex();
	if(verificaType(token) == false){
		printf("Erro: Esperava um tipo. Linha: %u, função: %s()\n", linha, __func__);
		exit(-1);
	}
	token = Analex();
	if(token != pontoevirgula){
		printf("Erro: Esperava um pontoEVirgula. Linha: %u, função: %s()\n", linha, __func__);
		exit(-1);
	}
}

void  Atribuirrotulo(){
	token = Analex();
	if (token != numero) {
		printf("Erro: esperava a palavra numero!. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	token = Analex();
	if (token != virgula || token != pontoevirgula) {
		printf("Erro: esperava a palavra virgula ou pontoEVirgula. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	while(token != virgula) {
		token = Analex();
		if(token != numero) {
			printf("Erro: esperava a palavra numero. Linha: %u, função: %s()\n", linha, __func__);
			exit(1);
		}
		token = Analex();
		if (token != virgula || token != pontoevirgula) {
			printf("Erro: esperava a palavra virgula ou pontoEVirgula. Linha: %u, função: %s()\n", linha, __func__);
			exit(1);
		}
	}
}

void verificaBloco() {
	token = Analex();
	while(token == rotulo){
		Atribuirrotulo();
		token = Analex();
	}
	while(verificaType(token) == true){
		AtribuirVariavel();
		token = Analex();
	}
	while(token == variavel ){
		AtribuirTipoImplicito();
		token = Analex();
	}
	while (token == procedimento){
		AtribuirProcedimento();
		token = Analex();
	}
	while (token == funcao){
		AtribuirFuncao();
		token = Analex();
	}
	while (token == inicio || token == fim){
		while(token == numero){
			VerificaComando();
			token = Analex();
		}
	}
}

void verificaProgam() {
	token = Analex();
	if (token != programa) {
		printf("Erro: esperava a palavra program!. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	token = Analex();
	if (token != identificador) {
		printf("Erro: esperava a palavra identificador!. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	token = Analex();
	if (token != abreparenteses) {
		printf("Erro: esperava a palavra abreParenteses!. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	while (token != fechaparenteses) {
		token = Analex();
		if (token != identificador) {
			printf("Erro: esperava a palavra identificador!. Linha: %u, função: %s()\n", linha, __func__);
			exit(1);
		}
		token = Analex();
		if (token != virgula && token != fechaparenteses) {
			printf("Erro: esperava a palavra virgula ou fechaparenteses!. Linha: %u, função: %s()\n", linha, __func__);
			exit(1);
		}
	}
	token = Analex();
	if (token != pontoevirgula) {
		printf("Erro: esperava a palavra virgula ou pontoEVirgula!. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	verificaBloco();
	token = Analex();
	if (token != ponto) {
		printf("Erro: esperava a palavra virgula ou ponto!. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
}

int main(){
	arquivo = fopen("arq.txt", "r");
	verificaProgam();
	printf("Programa sintaticamente correto!\n");
}
