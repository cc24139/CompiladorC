
#include "basics.h"
#include "AnalisadorLexico.h"
#include "TabelaSimbolos.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
/* 
	Integrantes
Nomes	RAs
Daniel	24123
Luis	24139
*/

Token token;
TabelaSimbolos tabela;
//assinaturas de metodos caso ocorra recursão indireta
void verificaBloco();
void VerificaComando();
void VerificaExpressao();

bool verificaType(Token token) {
	if(strcmp(palavraAtual, "integer") == 0)
		return true;

	if(strcmp(palavraAtual, "double") == 0)
		return true;

	if(strcmp(palavraAtual, "char") == 0)
		return true;

	return false;
}

void Fator() {
	if(token == numero) {
		token = Analex();
		return;
	}
	if (token == nao) {
		token = Analex(); 
		Fator();          
		return;
	}
	if(token == abreparenteses) {
		token = Analex(); 
		VerificaExpressao();
		if(token != fechaparenteses) {
			printf("Erro: Esperava-se um fechaparenteses. Linha: %u, função: %s()\n", linha, __func__);
			exit(-1);
		}
		token = Analex();
		return;
	}
	if(token == identificador) {
		token = Analex(); 
		if(token == abreparenteses) {
			token = Analex(); 
			if(token != fechaparenteses) {
				VerificaExpressao();
				while(token == virgula) {
					token = Analex();
					VerificaExpressao();
				}
			}
			if(token != fechaparenteses) {
				printf("Erro: Esperava-se um fechaparenteses. Linha: %u, função: %s()\n", linha, __func__);
				exit(-1);
			}
			token = Analex(); 
			return;
		}
		if (token == abrecolchetes) {
			token = Analex(); 
			VerificaExpressao();
			while(token == virgula) {
				token = Analex(); 
				VerificaExpressao();
			}
			if(token != fechacolchetes) {
				printf("Erro: Esperava-se um fechacolchetes. Linha: %u, função: %s()\n", linha, __func__);
				exit(-1);
			}
			token = Analex();
			return;
		}
		return;
	}
	printf("Erro: Esperava-se um identificador, número, abreparenteses ou nao. Linha: %u, função: %s()\n", linha, __func__);
	exit(-1);
}

void Termo() {
	if(token != identificador && token != numero && token != nao && token != abreparenteses) {
		printf("Erro: Esperava-se um fator. Linha: %u, função: %s()\n", linha, __func__);
		exit(-1);
	}
	Fator();
	while(token == vezes || token == dividir || token == e) {
		token = Analex(); 
		if(token != identificador && token != numero && token != nao && token != abreparenteses) {
			printf("Erro: Esperava-se um fator. Linha: %u, função: %s()\n", linha, __func__);
			exit(-1);
		}
		Fator();
	}
}


void ExpressaoSimples() {
	if(token == mais || token == menos) {
		token = Analex();
	}
	if(token != identificador && token != numero && token != nao && token != abreparenteses) {
		printf("Erro: Esperava-se um Termo. Linha: %u, função: %s()\n", linha, __func__);
		exit(-1);
	}
	Termo(); 
	while(token == mais || token == menos || token == ou) {
		token = Analex();
		if(token != identificador && token != numero && token != nao && token != abreparenteses) {
			printf("Erro: Esperava-se um Termo. Linha: %u, função: %s()\n", linha, __func__);
			exit(-1);
		}
		Termo();
	}
}


void VerificaExpressao() {
	ExpressaoSimples();
	if(token == igual || token == diferente || token == menor ||
	   token == menorouigual || token == maior || token == maiorouigual) {
		token = Analex(); 
		ExpressaoSimples();
	}
}

void VerificaComandoSemRotulo()
{
	if(token == identificador)
	{
		token = Analex();
		if (token == abrecolchetes)
		{
			token = Analex();
			VerificaExpressao();
			while(token == virgula)
			{
				token = Analex(); 
				VerificaExpressao();
			}
			if(token != fechacolchetes)
			{
				printf("Erro: Esperava-se um fechacolchetes. Linha: %u, função: %s()\n", linha, __func__);
				exit(-1);
			}
			token = Analex();
		}
		if(token == atribuicao)
		{
			token = Analex();
			VerificaExpressao();
		}
		else if(token == abreparenteses)
		{
			token = Analex(); 
			if(token != fechaparenteses) {
				VerificaExpressao();
				while(token == virgula)
				{
					token = Analex();
					VerificaExpressao();
				}
			}
			if(token != fechaparenteses)
			{
				printf("Erro: Esperava-se um fechaparenteses. Linha: %u, função: %s()\n", linha, __func__);
				exit(-1);
			}
			token = Analex(); 
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
		token = Analex(); 
	}
	else if(token == inicio)
	{
	
		VerificaComando();       
		while(token != fim)
		{
			if (token != pontoevirgula)
			{
				printf("Erro: Esperava-se um ponto e virgula. Linha: %u, função: %s()\n", linha, __func__);
				exit(-1);
			}
			VerificaComando();   
		}
		token = Analex();
	}
	else if (token == se)
	{
		token = Analex(); 
		VerificaExpressao();
		if(token != entao)
		{
			printf("Erro: Esperava-se um entao. Linha: %u, função: %s()\n", linha, __func__);
			exit(-1);
		}
		token = Analex(); 
		VerificaComandoSemRotulo();
		if (token == senao)
		{
			token = Analex();
			VerificaComandoSemRotulo();
		}
	}
	else if(token == enquanto)
	{
		token = Analex();
		VerificaExpressao();
		if (token != faca)
		{
			printf("Erro: Esperava-se um faca. Linha: %u, função: %s()\n", linha, __func__);
			exit(-1);
		}
		token = Analex(); 
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
	token = Analex(); 
	if(token != numero && token != identificador && token != vapara &&
	   token != inicio && token != se && token != enquanto) {
		printf("Erro: Esperava-se um número ou comando sem rótulo. Linha: %u, função: %s()\n", linha, __func__);
		exit(-1);
	}
	
	if(token == numero) {
		token = Analex();
		if (token != doispontos)
		{
			printf("Erro: Esperava-se dois pontos após rótulo. Linha: %u, função: %s()\n", linha, __func__);
			exit(-1);
		}
		token = Analex();
	}
	VerificaComandoSemRotulo();
}

bool EhBloco() {
	bool Ehrotulo = token == rotulo;
	bool EhTipo = token == tipo;
	bool EhImplicito = token == variavel;
	bool EhProcedimento = token == procedimento;
	bool EhFuncao = token == funcao;
	bool EhBegin = token == inicio;

	return Ehrotulo || EhTipo || EhImplicito || EhProcedimento || EhFuncao || EhBegin;
}

void parametrosFormais() {
	while (token == pontoevirgula || token == abreparenteses) {
		token = Analex();
		if(token != identificador && token != variavel && token != funcao && token != procedimento) {
			printf("Erro: Esperava-se um identificador, variavel, funcao ou procedimento. Linha: %u, função: %s()\n", linha, __func__);
			exit(-1);
		}
		if(token == variavel) {
			token = Analex();
			if(token != identificador) {
				printf("Erro: Esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
				exit(-1);
			}
		}
		if (token == identificador) {
			int primeiro = tabela.tamanhoLogico;
			InserirSimbolo(&tabela, GerarSimbolo(palavraAtual, variavel, &tabela, NULL));
			token = Analex();
			if(token != doispontos) {
				printf("Erro: Esperava-se um dois pontos. Linha: %u, função: %s()\n", linha, __func__);
				exit(-1);
			}
			token = Analex();
			if(verificaType(token) == false) {
				printf("Erro: Esperava-se um tipo. Linha: %u, função: %s()\n", linha, __func__);
				exit(-1);
			}
			tabela.tabela[primeiro].valor = malloc(strlen(palavraAtual) + 1);
			if(tabela.tabela[primeiro].valor == NULL)
				exit(EXIT_FAILURE);
			strcpy(tabela.tabela[primeiro].valor, palavraAtual);
			token = Analex();
			while(token == virgula) {
				token = Analex();
				if(token != identificador) {
					printf("Erro: Esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
					exit(-1);
				}
				InserirSimbolo(&tabela, GerarSimbolo(palavraAtual, variavel, &tabela, tabela.tabela[primeiro].valor));
				token = Analex();
			}
		}
		if(token == funcao) {
			int primeiro = tabela.tamanhoLogico;
			token = Analex();
			if(token != identificador) {
				printf("Erro: Esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
				exit(-1);
			}
			InserirSimbolo(&tabela, GerarSimbolo(palavraAtual, funcao, &tabela, NULL));
			token = Analex();
			while(token == virgula) {
				token = Analex();
				if(token != identificador) {
					printf("Erro: Esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
					exit(-1);
				}
				InserirSimbolo(&tabela, GerarSimbolo(palavraAtual, funcao, &tabela, NULL));
				token = Analex();
			}
			if(token != doispontos) {
				printf("Erro: Esperava-se um dois pontos. Linha: %u, função: %s()\n", linha, __func__);
				exit(-1);
			}
			token = Analex();
			if(token != identificador) {
				printf("Erro: Esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
				exit(-1);
			}
			for(int i = primeiro; i < tabela.tamanhoLogico; i++) {
				tabela.tabela[i].valor = malloc(strlen(palavraAtual) + 1);
				if(tabela.tabela[i].valor == NULL)
					exit(EXIT_FAILURE);
				strcpy(tabela.tabela[i].valor, palavraAtual);
			}
			token = Analex();
		}
		if(token == procedimento) {
			token = Analex();
			if(token != identificador) {
				printf("Erro: Esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
				exit(-1);
			}
			InserirSimbolo(&tabela, GerarSimbolo(palavraAtual, procedimento, &tabela, NULL));
			token = Analex();
			while (token == virgula)
			{
				token = Analex();
				if(token != identificador) {
					printf("Erro: Esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
					exit(-1);
				}
				InserirSimbolo(&tabela, GerarSimbolo(palavraAtual, procedimento, &tabela, NULL));
				token = Analex();
			}
		}
	}
	if(token != fechaparenteses) {
		printf("Erro: Esperava-se um fechaparenteses. Linha: %u, função: %s()\n", linha, __func__);
		printf("Token encontrado: %s\n", tokenString[token]);
		exit(-1);
	}
}

void AtribuirParametosFormais() {
	parametrosFormais();
	token = Analex();
	
}


void AtribuirFuncao() {
	token = Analex();
	if(token != identificador) {
		printf("Erro: esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	int indiceFuncao = tabela.tamanhoLogico;
	Simbolo simbFunc = GerarSimbolo(palavraAtual, funcao, &tabela, NULL);
	InserirSimbolo(&tabela, simbFunc);
	tabela.ScopoAtual++;
	token = Analex(); 
	if(token != abreparenteses && token != doispontos) {
		printf("Erro: esperava-se um parametro formal ou um dois pontos. Linha: %u, função: %s()\n", linha, __func__);
		printf("Token encontrado: %s\n", tokenString[token]);
		exit(1);
	}
	if(token == abreparenteses) {
	
		AtribuirParametosFormais();
	}
	if(token != doispontos) {
		printf("Erro: esperava-se dois pontos (tipo de retorno). Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	token = Analex(); 
	if(token != identificador) {
		printf("Erro: esperava-se um identificador (tipo de retorno). Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	tabela.tabela[indiceFuncao].valor = malloc(strlen(palavraAtual) + 1);
	if(tabela.tabela[indiceFuncao].valor == NULL)
		exit(EXIT_FAILURE);
	strcpy(tabela.tabela[indiceFuncao].valor, palavraAtual);
	token = Analex(); 
	if(token != pontoevirgula) {
		printf("Erro: esperava-se um ponto e virgula. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	token = Analex(); 
	if(!EhBloco()) {
		printf("Erro: esperava-se um bloco. Linha: %u, função: %s()\n", linha, __func__);
		printf("Token encontrado: %s\n", tokenString[token]);
		exit(1);
	}
	verificaBloco(); 
	
	if(token != pontoevirgula) {
		printf("Erro: esperava-se um ponto e virgula após o bloco da função. Linha: %u, função: %s()\n", linha, __func__);
		printf("Token encontrado: %s\n", tokenString[token]);
		exit(1);
	}
	token = Analex(); 
	RemoverScopo(&tabela, tabela.ScopoAtual);
}


void AtribuirProcedimento() {
	token = Analex(); 
	if(token != identificador) {
		printf("Erro: esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	Simbolo simbProc = GerarSimbolo(palavraAtual, procedimento, &tabela, NULL);
	InserirSimbolo(&tabela, simbProc);
	tabela.ScopoAtual++;
	token = Analex();
	if(token == abreparenteses) {
		AtribuirParametosFormais();
	}
	if(token != pontoevirgula) {
		printf("Erro: esperava-se um ponto e virgula. Linha: %u, função: %s()\n", linha, __func__);
		printf("Token encontrado: %s\n", tokenString[token]);
		exit(1);
	}
	token = Analex();
	if(!EhBloco()) {
		printf("Erro: esperava-se um bloco. Linha: %u, função: %s()\n", linha, __func__);
		printf("Token encontrado: %s\n", tokenString[token]);
		exit(1);
	}
	verificaBloco(); 
	if(token != pontoevirgula) {
		printf("Erro: esperava-se um ponto e virgula. Linha: %u, função: %s()\n", linha, __func__);
		printf("Token encontrado: %s\n", tokenString[token]);
		exit(1);
	}
	token = Analex(); 
	RemoverScopo(&tabela, tabela.ScopoAtual);
}

void AtribuirTipoImplicito() {
	if(token != variavel) {
		printf("Erro: esperava-se a uma variavel. Linha: %u, função: %s()\n", linha, __func__);
		printf("Token encontrado: %s\n", tokenString[token]);
		exit(1);
	}
	token = Analex(); 
	if(token != identificador) {
		printf("esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
		printf("Token encontrado: %s\n", tokenString[token]);
		exit(1);
	}
	int primeiro = tabela.tamanhoLogico;
	InserirSimbolo(&tabela, GerarSimbolo(palavraAtual, variavel, &tabela, NULL));
	token = Analex(); 
	while(token == virgula) {
		token = Analex(); 
		if(token != identificador) {
			printf("Erro: esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
			printf("Token encontrado: %s\n", tokenString[token]);
			exit(1);
		}
		InserirSimbolo(&tabela, GerarSimbolo(palavraAtual, variavel, &tabela, NULL));
		token = Analex(); 
	}
	if(token != doispontos) {
		printf("Erro: esperava-se dois pontos. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	token = Analex(); 
	if(verificaType(token) == false) {
		printf("Erro: esperava-se um tipo. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	for(int i = primeiro; i < tabela.tamanhoLogico; i++) {
		tabela.tabela[i].valor = malloc(strlen(palavraAtual) + 1);
		if(tabela.tabela[i].valor == NULL)
			exit(EXIT_FAILURE);
		strcpy(tabela.tabela[i].valor, palavraAtual);
	}
	token = Analex();
	if(token != pontoevirgula) {
		printf("Erro: esperava-se um ponto e virgula. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
}

void AtribuirVariavel() {
	token = Analex(); 
	if(token != identificador) {
		printf("Erro: Esperava um identificador. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	int primeiro = tabela.tamanhoLogico;
	InserirSimbolo(&tabela, GerarSimbolo(palavraAtual, variavel, &tabela, NULL));
	token = Analex();
	if(token != doispontos) {
		printf("Erro: Esperava um dois pontos. Linha: %u, função: %s()\n", linha, __func__);
		exit(-1);
	}
	token = Analex(); 
	if(verificaType(token) == false) {
		printf("Erro: Esperava um tipo. Linha: %u, função: %s()\n", linha, __func__);
		exit(-1);
	}
	tabela.tabela[primeiro].valor = malloc(strlen(palavraAtual) + 1);
	if(tabela.tabela[primeiro].valor == NULL)
		exit(EXIT_FAILURE);
	strcpy(tabela.tabela[primeiro].valor, palavraAtual);
	token = Analex(); 
	if(token != pontoevirgula) {
		printf("Erro: Esperava um pontoEVirgula. Linha: %u, função: %s()\n", linha, __func__);
		exit(-1);
	}
}

void Atribuirrotulo() {
	token = Analex(); 
	if (token != numero) {
		printf("Erro: esperava a palavra numero!. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	Simbolo simbolo = GerarSimbolo(palavraAtual,rotulo,&tabela,NULL);
	InserirSimbolo(&tabela,simbolo);
	token = Analex(); 
	if (token != virgula && token != pontoevirgula) {
		printf("Erro: esperava a palavra virgula ou pontoEVirgula. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	printf("Token encontrado: %s\n", tokenString[token]);
	while(token == virgula) {
		token = Analex(); 
		if(token != numero) {
			printf("Erro: esperava a palavra numero. Linha: %u, função: %s()\n", linha, __func__);
			exit(1);
		}
		Simbolo simbolo = GerarSimbolo(palavraAtual,rotulo,&tabela,NULL);
		InserirSimbolo(&tabela,simbolo);
		token = Analex(); 
		if (token != virgula && token != pontoevirgula) {
			printf("Erro: esperava a palavra virgula ou pontoEVirgula. Linha: %u, função: %s()\n", linha, __func__);
			exit(1);
		}

	}

}


void verificaBloco() {
	while(EhBloco()){
		while(token == rotulo) {
			Atribuirrotulo(); 
			token = Analex();
		}

		while(verificaType(token) == true) {
			AtribuirVariavel(); 
			token = Analex();   
		}


		while(token == variavel) {
			AtribuirTipoImplicito(); 
			token = Analex();       
		}


		while (token == procedimento) {
			AtribuirProcedimento(); 
		}


		while (token == funcao) {
			AtribuirFuncao(); 
		}


		if(token == inicio) {
			VerificaComandoSemRotulo();
		}
	}
	//RemoverUltimoSimbolo(&tabela);
}

void verificaProgam() {
	if (token != programa) {
		printf("Erro: esperava a palavra program!. Linha: %u, função: %s()\n", linha, __func__);
		printf("Token encontrado: %s\n", tokenString[token]);
		exit(1);
	}
	token = Analex();
	if (token != identificador) {
		printf("Erro: esperava a palavra identificador!. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	InserirSimbolo(&tabela, GerarSimbolo(palavraAtual, programa, &tabela, NULL));
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
		InserirSimbolo(&tabela, GerarSimbolo(palavraAtual, identificador, &tabela, NULL));
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
	token = Analex(); 
	if(!EhBloco()) {
		printf("Erro: esperava-se um bloco. Linha: %u, função: %s()\n", linha, __func__);
		printf("Token encontrado: %s\n", tokenString[token]);
		exit(1);
	}
	verificaBloco();
	if (token != ponto) {
		printf("Erro: esperava a palavra ponto!. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
}

int main() {
	arquivo = fopen("arq.txt", "r");
	token = Analex();
	inicializarTabela(&tabela);
	verificaProgam();
	ImprimirTabela(&tabela);
	RemoverScopo(&tabela, tabela.ScopoAtual);
	ImprimirTabela(&tabela);
	LiberarTabela(&tabela);
	fclose(arquivo);
	printf("Programa sintaticamente correto!\n");
}
