#include "basics.h"
#include "AnalisadorLexico.h"

static Token token;

void verificaProgam() {
	 token = anaLex();
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
	token = anaLex();
	while(token == rotulo){
		Atribuirrotulo();
		token = analex();
	}

    while(verificaType(token) == true){
        AtribuirVariavel();
        token = anaLex();
    }

	while(token == variavel ){
		AtribuirTipoImplicito();
		token = analex();
	}
	while (token == procedimento){
		AtribuirProcedimento();
		token = analex();
	}
	while (token == funcao){
		AtribuirFuncao();
		token = analex();
	}
	while (token == inicio || token == fim){
		while(token == numero){
			VerificaComando();
			token = anaLex();
		}
	}
    
}


void  Atribuirrotulo(){
	token = anaLex();
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
    token = anaLex();
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
    if(token != pontoevirgula){
        printf("Erro: Esperava um pontoEVirgula");
        exit(-1);
    }
}

void AtribuirTipoImplicito(){
	token;
	token = analex();
	if(token != variavel){
		printf("Erro: esperava-se a uma variavel");
		exit(1);
	}
	token = analex();
	if(token != identificador){
		printf("esperava-se um identificador");
		exit(1);
	}
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

void AtribuirProcedimento(){
	token = analex();
	if(token != abreparenteses || token != EhBloco()){
		printf("Esperava-se um parametro formal ou um bloco!");
		exit(1);
	}
	if(token == abreparenteses){
		parametrosFormais(pontoevirgula);
		}
	
	token = analex();
	if(token != EhBloco()){
		printf("Esperava-se um bloco!");
		exit(1);
	}
	AtribuirBloco(); //recursão indireta em bloco
	token = analex();
	if(token != pontoevirgula){
		printf("Erro: esperava-se um ponto e virgula");
		exit(1);
	}
}

void AtribuirParametosFormais(Token tokenComparativo){
	parametrosFormais();
	token = analex();
	if(token != tokenComparativo){
		printf("Erro: esperava-se um ponto e virgula");
		exit(1);
	}
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

void AtribuirFuncao(){
	token = analex();
	if(token != identificador){
		printf("Erro: esperava-se um identificador");
		exit(1);
	}
	token = analex();
	if(token != abreparenteses || token != doispontos){
		printf("Erro: esperava-se um parametro formal ou um dois pontos");
		exit(1);
	}
	AtribuirParametosFormais(doispontos);
	token = analex();
	if(token != identificador){
		printf("Erro: esperava-se um identificador");
		exit(1);
	}
	token = analex();
	if(token != EhBloco()){
		printf("Erro: esperava-se um bloco");
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

void Expressao(){
	ExpressaoSimples();
	token = anaLex();
	if(token == igual || token == diferente || token == menor || token == menorouigual || token == maior || token == maiorouigual){
		token = anaLex();
		if(token != identificador){
			printf("Erro: Esperava-se um identificador");
			exit(-1);
		}
		ExpressaoSimples();
	}
}

void ExpressaoSimples(){
	token = anaLex();
	if(token != mais && token != menos && token != identificador){
		printf("Erro: Esperava-se um operador ou um termo");
		exit(-1);
	}
	if(token == mais || token == menos){
		token = anaLex();
		if(token != identificador){
			printf("Erro: Esperava-se um termo");
			exit(-1);
		}
	}
	termo();
	token = analex();
	while(token == mais || token == menos || token == ou){
		token = analex();
		if(token != identificador){
			printf("Erro: Esperava-se um termo");
			exit(-1);
		}
		termo();
		token = analex();
	}
}

void Termo(){
	if(token != identificador){ // pão durismo, o termo sempre vai começar com um fator que sempre vai começar com um identificador
		printf("Erro: Esperava-se um identificador");
		exit(-1);
	}
	token = analex();
	while(token == vezes || token == dividir || token == e){
		token = analex();
		if(token != identificador){
			printf("Erro: Esperava-se um identificador");
			exit(-1);
			fator();
	}
	
}