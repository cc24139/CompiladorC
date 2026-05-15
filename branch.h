#include "basics.h"
#include "AnalisadorLexico.h"

void verificaProgam() {
	Token token = anaLex();
	if (token != programa) {
		printf("Erro: esperava a palavra program!");
		exit(1);
	}
	token = anaLex();
	if (token != identificador) {
		printf("Erro: esperava a palavra identificador!");
		exit(1);
	}
	token = anaLex();
	if (token != abreparenteses) {
		printf("Erro: esperava a palavra abreParenteses!");
		exit(1);
	}
	while (token != fechaparenteses) {
		token = anaLex();
		if (token != identificador) {
			printf("Erro: esperava a palavra identificador!");
			exit(1);
		}
		token = anaLex();
		if (token != virgula || token != fechaparenteses) {
			printf("Erro: esperava a palavra virgula ou fechaParenteses!");
			exit(1);
		}
	}
	token = anaLex();
	if (token != pontoevirgula) {
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

