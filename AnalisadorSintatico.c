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
}

void verificaBloco() {
	Token token = anaLex();
	if (token != label) {
		printf("Erro: esperava a palavra label!");
		exit(1);
	}
	token = anaLex();
	if (token != numero) {
		printf("Erro: esperava a palavra numero!");
		exit(1);
	}
	token = anaLex();
	if (token != virgula || token != pontoEVirgula) {
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
		if (token != virgula || token != pontoEVirgula) {
			printf("Erro: esperava a palavra virgula ou pontoEVirgula");
			exit(1);
		}
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
    if(token != pontoEVirgula){
        printf("Erro: Esperava um pontoEVirgula");
        exit(-1);
    }
}


