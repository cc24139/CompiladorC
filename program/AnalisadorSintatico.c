#include "basics.h"
#include "AnalisadorLexico.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

Token token;
//assinatura de atribuir bloco para procedimento usar
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

void parametrosFormais(Token tokenComparativo) {
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
			token = Analex();
			while(token == virgula) {
				token = Analex();
				if(token != identificador) {
					printf("Erro: Esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
					exit(-1);
				}
				token = Analex();
			}
		}
		if(token == funcao) {
			token = Analex();
			if(token != identificador) {
				printf("Erro: Esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
				exit(-1);
			}
			token = Analex();
			while(token == virgula) {
				token = Analex();
				if(token != identificador) {
					printf("Erro: Esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
					exit(-1);
				}
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
			token = Analex();
		}
		if(token == procedimento) {
			token = Analex();
			if(token != identificador) {
				printf("Erro: Esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
				exit(-1);
			}
			token = Analex();
			while (token == virgula)
			{
				token = Analex();
				if(token != identificador) {
					printf("Erro: Esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
					exit(-1);
				}
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

void AtribuirParametosFormais(Token tokenComparativo) {
	parametrosFormais(tokenComparativo);
	token = Analex(); // consome ')'
	if(token != tokenComparativo) {
		printf("Erro: esperava-se um ponto e virgula. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
}

// Protocolo: AtribuirFuncao() recebe token == funcao já carregado.
// Chama Analex() ao entrar para avançar. Ao sair, token está no próximo símbolo.
void AtribuirFuncao() {
	token = Analex(); // consome 'function'
	if(token != identificador) {
		printf("Erro: esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	token = Analex(); // consome o nome da função
	if(token != abreparenteses && token != doispontos) {
		printf("Erro: esperava-se um parametro formal ou um dois pontos. Linha: %u, função: %s()\n", linha, __func__);
		printf("Token encontrado: %s\n", tokenString[token]);
		exit(1);
	}
	if(token == abreparenteses) {
		// parametrosFormais termina em ')'; AtribuirParametosFormais avança e verifica ':'
		AtribuirParametosFormais(doispontos);
	}
	// agora token deve ser ':'
	if(token != doispontos) {
		printf("Erro: esperava-se dois pontos (tipo de retorno). Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	token = Analex(); // consome ':'
	if(token != identificador) {
		printf("Erro: esperava-se um identificador (tipo de retorno). Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	token = Analex(); // consome o tipo de retorno
	if(token != pontoevirgula) {
		printf("Erro: esperava-se um ponto e virgula. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	token = Analex(); // consome ';'
	if(!EhBloco()) {
		printf("Erro: esperava-se um bloco. Linha: %u, função: %s()\n", linha, __func__);
		printf("Token encontrado: %s\n", tokenString[token]);
		exit(1);
	}
	verificaBloco(); // ao sair, token está no próximo símbolo após o bloco
	// verificaBloco termina após processar o 'begin..end' e avança o token
	if(token != pontoevirgula) {
		printf("Erro: esperava-se um ponto e virgula após o bloco da função. Linha: %u, função: %s()\n", linha, __func__);
		printf("Token encontrado: %s\n", tokenString[token]);
		exit(1);
	}
	token = Analex(); // consome ';' final
}

// Protocolo: AtribuirProcedimento() recebe token == procedimento já carregado.
// Chama Analex() ao entrar para avançar. Ao sair, token está no próximo símbolo.
void AtribuirProcedimento() {
	token = Analex(); // consome 'procedure'
	if(token != identificador) {
		printf("Erro: esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	token = Analex(); // consome o nome do procedimento
	if(token == abreparenteses) {
		parametrosFormais(pontoevirgula); // termina consumindo ')'
		token = Analex(); // consome ')'
	}
	if(token != pontoevirgula) {
		printf("Erro: esperava-se um ponto e virgula. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	token = Analex(); // consome ';'
	if(!EhBloco()) {
		printf("Erro: esperava-se um bloco. Linha: %u, função: %s()\n", linha, __func__);
		printf("Token encontrado: %s\n", tokenString[token]);
		exit(1);
	}
	verificaBloco(); // ao sair, token está no próximo símbolo após o bloco
	if(token != pontoevirgula) {
		printf("Erro: esperava-se um ponto e virgula. Linha: %u, função: %s()\n", linha, __func__);
		printf("Token encontrado: %s\n", tokenString[token]);
		exit(1);
	}
	token = Analex(); // consome ';' final
}

void AtribuirTipoImplicito() {
	if(token != variavel) {
		printf("Erro: esperava-se a uma variavel. Linha: %u, função: %s()\n", linha, __func__);
		printf("Token encontrado: %s\n", tokenString[token]);
		exit(1);
	}
	token = Analex(); // consome 'var'
	if(token != identificador) {
		printf("esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
		printf("Token encontrado: %s\n", tokenString[token]);
		exit(1);
	}
	token = Analex(); // consome o identificador
	while(token == virgula) {
		token = Analex(); // consome ','
		if(token != identificador) {
			printf("Erro: esperava-se um identificador. Linha: %u, função: %s()\n", linha, __func__);
			printf("Token encontrado: %s\n", tokenString[token]);
			exit(1);
		}
		token = Analex(); // consome o próximo identificador
	}
	if(token != doispontos) {
		printf("Erro: esperava-se dois pontos. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	token = Analex(); // consome ':'
	if(verificaType(token) == false) {
		printf("Erro: esperava-se um tipo. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	token = Analex(); // consome o tipo
	if(token != pontoevirgula) {
		printf("Erro: esperava-se um ponto e virgula. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
}

void AtribuirVariavel() {
	token = Analex(); // consome o token de tipo (integer/double/char)
	if(token != identificador) {
		printf("Erro: Esperava um identificador. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	token = Analex(); // consome o identificador
	if(token != doispontos) {
		printf("Erro: Esperava um dois pontos. Linha: %u, função: %s()\n", linha, __func__);
		exit(-1);
	}
	token = Analex(); // consome ':'
	if(verificaType(token) == false) {
		printf("Erro: Esperava um tipo. Linha: %u, função: %s()\n", linha, __func__);
		exit(-1);
	}
	token = Analex(); // consome o tipo
	if(token != pontoevirgula) {
		printf("Erro: Esperava um pontoEVirgula. Linha: %u, função: %s()\n", linha, __func__);
		exit(-1);
	}
}

void Atribuirrotulo() {
	token = Analex(); // consome 'label'
	if (token != numero) {
		printf("Erro: esperava a palavra numero!. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	token = Analex(); // consome o número
	if (token != virgula && token != pontoevirgula) {
		printf("Erro: esperava a palavra virgula ou pontoEVirgula. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	printf("Token encontrado: %s\n", tokenString[token]);
	while(token == virgula) {
		token = Analex(); // consome ','
		if(token != numero) {
			printf("Erro: esperava a palavra numero. Linha: %u, função: %s()\n", linha, __func__);
			exit(1);
		}
		token = Analex(); // consome o número
		if (token != virgula && token != pontoevirgula) {
			printf("Erro: esperava a palavra virgula ou pontoEVirgula. Linha: %u, função: %s()\n", linha, __func__);
			exit(1);
		}
	}
	// ao sair: token == pontoevirgula
}

// Protocolo: verificaBloco() usa o token já carregado (token == primeiro token do bloco).
// Ao sair, token está no próximo símbolo após o bloco completo.
void verificaBloco() {
	printf("Token encontrado: %s\n", tokenString[token]);

	// seção de rótulos: label num, num; ...
	while(token == rotulo) {
		Atribuirrotulo(); // termina com token == pontoevirgula
		token = Analex(); // avança para o próximo token do bloco
	}

	// seção de tipo: type ...
	while(token == tipo) {
		// (tipo explícito — implementação futura)
		token = Analex();
	}

	// seção de variáveis simples (integer/double/char ident : tipo;)
	while(verificaType(token) == true) {
		AtribuirVariavel(); // termina com token == pontoevirgula
		token = Analex();   // avança para o próximo token do bloco
	}

	// seção de variáveis implícitas (var ident : tipo;)
	while(token == variavel) {
		AtribuirTipoImplicito(); // termina com token == pontoevirgula
		token = Analex();        // avança para o próximo token do bloco
	}

	// seção de procedimentos
	while (token == procedimento) {
		AtribuirProcedimento(); // ao sair, token já está no próximo símbolo
	}

	// seção de funções
	while (token == funcao) {
		AtribuirFuncao(); // ao sair, token já está no próximo símbolo
	}

	// corpo principal: begin ... end
	// token já é 'inicio' — usamos VerificaComandoSemRotulo que trata o begin..end
	// completo (consome o 'end') e ao sair deixa o próximo token carregado.
	if(token == inicio) {
		VerificaComandoSemRotulo(); // ao sair, token é o símbolo após o 'end'
	}
}

void verificaProgam() {
	if (token != programa) {
		printf("Erro: esperava a palavra program!. Linha: %u, função: %s()\n", linha, __func__);
		printf("Token encontrado: %s\n", tokenString[token]);
		exit(1);
	}
	token = Analex(); // consome 'program'
	if (token != identificador) {
		printf("Erro: esperava a palavra identificador!. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	token = Analex(); // consome o nome do programa
	if (token != abreparenteses) {
		printf("Erro: esperava a palavra abreParenteses!. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	// lista de identificadores: (id, id, ..., id)
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
	token = Analex(); // consome ')'
	if (token != pontoevirgula) {
		printf("Erro: esperava a palavra virgula ou pontoEVirgula!. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
	token = Analex(); // consome ';'
	verificaBloco();
	// após verificaBloco(), token está no próximo símbolo
	if (token != ponto) {
		printf("Erro: esperava a palavra ponto!. Linha: %u, função: %s()\n", linha, __func__);
		exit(1);
	}
}

int main() {
	arquivo = fopen("arq.txt", "r");
	token = Analex();
	verificaProgam();
	printf("Programa sintaticamente correto!\n");
}