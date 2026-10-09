#include "basics.h"
#include "AnalisadorLexico.h"
#include "TabelaSimbolos.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/*
    Integrantes
    Nomes   RAs
    Daniel  24123
    Luis    24139
*/

Token token;
TabelaSimbolos tabela;

// Assinaturas de métodos caso ocorra recursão indireta.
void verificaBloco();
void VerificaComando();
TipoSemantico VerificaExpressao();


static Simbolo *BuscarRotuloVisivel(char *nome)
{
    for(int i = tabela.tamanhoLogico - 1; i >= 0; i--)
    {
        if(tabela.tabela[i].token == rotulo &&
           strcmp(tabela.tabela[i].nome, nome) == 0)
        {
            return &tabela.tabela[i];
        }
    }
    return NULL;
}

static void InserirSimboloOuErro(Simbolo simbolo)
{
    if(!InserirSimbolo(&tabela, simbolo))
    {
        printf(
            "Erro: identificador '%s' já declarado neste escopo. "
            "Linha: %u, função: %s()\n",
            simbolo.nome, linha, __func__
        );
        exit(1);
    }
}

static TipoSemantico ObterTipo(const char *nome)
{
    if(strcmp(nome, "integer") == 0)
        return tipoInteger;

    if(strcmp(nome, "double") == 0)
        return tipoDouble;

    if(strcmp(nome, "char") == 0)
        return tipoChar;

    return tipoIndefinido;
}

static const char *NomeTipo(TipoSemantico tipoAtual)
{
    switch(tipoAtual)
    {
        case tipoInteger: return "integer";
        case tipoDouble: return "double";
        case tipoChar: return "char";
        case tipoBooleano: return "booleano";
        default: return "indefinido";
    }
}

static void CompararTipos(TipoSemantico tipoAtual, const Simbolo *simbolo)
{
    if(tipoAtual == tipoIndefinido || tipoAtual != simbolo->tipo)
    {
        printf(
            "Erro semântico: tipos incompatíveis para '%s': "
            "esperado %s, recebido %s. Linha: %u, função: %s()\n",
            simbolo->nome, NomeTipo(simbolo->tipo), NomeTipo(tipoAtual),
            linha, __func__
        );
        exit(1);
    }
}

bool verificaType(Token token)
{
    return token == identificador && ObterTipo(palavraAtual) != tipoIndefinido;
}

TipoSemantico Fator()
{
    if(token == numero)
    {
        token = Analex();
        return tipoInteger;
    }

    if(token == caractere)
    {
        token = Analex();
        return tipoChar;
    }

    if(token == nao)
    {
        token = Analex();
        Fator();
        return tipoBooleano;
    }

    if(token == abreparenteses)
    {
        token = Analex();
        TipoSemantico tipoAtual = VerificaExpressao();

        if(token != fechaparenteses)
        {
            printf(
                "Erro: Esperava-se um fechaparenteses. "
                "Linha: %u, função: %s()\n",
                linha, __func__
            );
            exit(-1);
        }

        token = Analex();
        return tipoAtual;
    }

    if(token == identificador)
    {
        Simbolo *simbolo = BuscarSimbolo(&tabela,palavraAtual);

        if(simbolo == NULL)
        {
            printf(
                "Erro: identificador '%s' não declarado. "
                "Linha: %u, função: %s()\n",
                palavraAtual, linha, __func__
            );
            exit(-1);
        }

        token = Analex();

        if(token == abreparenteses)
        {
            if(simbolo->token != funcao)
            {
                printf(
                    "Erro: identificador '%s' não é uma função. "
                    "Linha: %u, função: %s()\n",
                    simbolo->nome, linha, __func__
                );
                exit(-1);
            }

            token = Analex();

            if(token != fechaparenteses)
            {
                VerificaExpressao();

                while(token == virgula)
                {
                    token = Analex();
                    VerificaExpressao();
                }
            }

            if(token != fechaparenteses)
            {
                printf(
                    "Erro: Esperava-se um fechaparenteses. "
                    "Linha: %u, função: %s()\n",
                    linha, __func__
                );
                exit(-1);
            }

            token = Analex();
            return simbolo->tipo;
        }

        if(token == abrecolchetes)
        {
            if(simbolo->token != variavel)
            {
                printf(
                    "Erro: identificador '%s' não é uma variável. "
                    "Linha: %u, função: %s()\n",
                    simbolo->nome, linha, __func__
                );
                exit(-1);
            }

            token = Analex();
            VerificaExpressao();

            while(token == virgula)
            {
                token = Analex();
                VerificaExpressao();
            }

            if(token != fechacolchetes)
            {
                printf(
                    "Erro: Esperava-se um fechacolchetes. "
                    "Linha: %u, função: %s()\n",
                    linha, __func__
                );
                exit(-1);
            }

            token = Analex();
            return simbolo->tipo;
        }

        if(simbolo->token == procedimento ||
           simbolo->token == rotulo ||
           simbolo->token == programa)
        {
            printf(
                "Erro: identificador '%s' não pode ser usado em uma expressão. "
                "Linha: %u, função: %s()\n",
                simbolo->nome, linha, __func__
            );
            exit(-1);
        }

        return simbolo->tipo;
    }

    printf(
        "Erro: Esperava-se um identificador, número, caractere, abreparenteses ou nao. "
        "Linha: %u, função: %s()\n",
        linha, __func__
    );
    exit(-1);
}

TipoSemantico Termo()
{
    if(token != identificador &&
       token != numero &&
       token != caractere &&
       token != nao &&
       token != abreparenteses)
    {
        printf(
            "Erro: Esperava-se um fator. Linha: %u, função: %s()\n",
            linha, __func__
        );
        exit(-1);
    }

    TipoSemantico tipoAtual = Fator();

    while(token == vezes || token == dividir || token == e)
    {
        Token operador = token;
        token = Analex();

        if(token != identificador &&
           token != numero &&
           token != caractere &&
           token != nao &&
           token != abreparenteses)
        {
            printf(
                "Erro: Esperava-se um fator. Linha: %u, função: %s()\n",
                linha, __func__
            );
            exit(-1);
        }

        TipoSemantico tipoFator = Fator();
        if(operador == e)
            tipoAtual = tipoBooleano;
        else if(tipoAtual != tipoFator)
            tipoAtual = tipoIndefinido;
    }

    return tipoAtual;
}

TipoSemantico ExpressaoSimples()
{
    if(token == mais || token == menos)
    {
        token = Analex();
    }

    if(token != identificador &&
       token != numero &&
       token != caractere &&
       token != nao &&
       token != abreparenteses)
    {
        printf(
            "Erro: Esperava-se um Termo. Linha: %u, função: %s()\n",
            linha, __func__
        );
        exit(-1);
    }

    TipoSemantico tipoAtual = Termo();

    while(token == mais || token == menos || token == ou)
    {
        Token operador = token;
        token = Analex();

        if(token != identificador &&
           token != numero &&
           token != caractere &&
           token != nao &&
           token != abreparenteses)
        {
            printf(
                "Erro: Esperava-se um Termo. Linha: %u, função: %s()\n",
                linha, __func__
            );
            exit(-1);
        }

        TipoSemantico tipoTermo = Termo();
        if(operador == ou)
            tipoAtual = tipoBooleano;
        else if(tipoAtual != tipoTermo)
            tipoAtual = tipoIndefinido;
    }

    return tipoAtual;
}

static bool EhOperadorRelacional(Token token)
{
    return token == igual ||
           token == diferente ||
           token == menor ||
           token == menorouigual ||
           token == maior ||
           token == maiorouigual;
}

TipoSemantico VerificaExpressao()
{
    TipoSemantico tipoAtual = ExpressaoSimples();

    if(EhOperadorRelacional(token))
    {
        token = Analex();
        ExpressaoSimples();
        return tipoBooleano;
    }

    return tipoAtual;
}


void VerificaCondicao()
{
    ExpressaoSimples();

    if(!EhOperadorRelacional(token))
    {
        printf(
            "Erro: esperava-se um operador relacional na condição. "
            "Linha: %u, função: %s()\n",
            linha, __func__
        );
        exit(-1);
    }

    token = Analex();
    ExpressaoSimples();
}

void VerificaComandoSemRotulo()
{
    if(token == identificador)
    {
        Simbolo *simbolo = BuscarSimbolo(&tabela,palavraAtual);

        if(simbolo == NULL)
        {
            printf(
                "Erro: identificador '%s' não declarado. "
                "Linha: %u, função: %s()\n",
                palavraAtual, linha, __func__
            );
            exit(-1);
        }
        Simbolo *simboloAtual = BuscarSimbolo(&tabela,palavraAtual);
        token = Analex();
     
        if(token == abrecolchetes)
        {
            if(simbolo->token != variavel)
            {
                printf(
                    "Erro: identificador '%s' não é uma variável. "
                    "Linha: %u, função: %s()\n",
                    simbolo->nome, linha, __func__
                );
                exit(-1);
            }
            
            token = Analex();
           
            VerificaExpressao();
                if(EhOperadorRelacional(token)){
                printf("Erro: não usar operador relacional em atribuição"
                "linha: %u, função %s()\n",linha,__func__);
                exit(-1);
            }
        
            while(token == virgula)
            {
                token = Analex();
                VerificaExpressao();
            }

            if(token != fechacolchetes)
            {
                printf(
                    "Erro: Esperava-se um fechacolchetes. "
                    "Linha: %u, função: %s()\n",
                    linha, __func__
                );
                exit(-1);
            }

            token = Analex();
        }
      
        if(token == atribuicao)
        {
           
            if(simbolo->token != variavel && simbolo->token != funcao)
            {
                printf(
                    "Erro: identificador '%s' não pode receber atribuição. "
                    "Linha: %u, função: %s()\n",
                    simbolo->nome, linha, __func__
                );
                exit(-1);
            }

            token = Analex();
            
          
            TipoSemantico tipoAtual = ExpressaoSimples();
            if(EhOperadorRelacional(token)){
                printf("Erro: não usar operador relacional em atribuição"
                "linha: %u, função %s()\n",linha,__func__);
                exit(-1);
            }

            CompararTipos(tipoAtual, simboloAtual);
            //atualizar
            Simbolo atualizado = *simboloAtual;
            //atualizado.valor = palavraAtual;
            AtualizarSimbolo(&tabela, atualizado);
        }
        else if(token == abreparenteses)
        {
            if(simbolo->token != procedimento)
            {
                printf(
                    "Erro: identificador '%s' não é um procedimento. "
                    "Linha: %u, função: %s()\n",
                    simbolo->nome, linha, __func__
                );
                exit(-1);
            }

            token = Analex();

            if(token != fechaparenteses)
            {
                VerificaExpressao();

                while(token == virgula)
                {
                    token = Analex();
                    VerificaExpressao();
                }
            }

            if(token != fechaparenteses)
            {
                printf(
                    "Erro: Esperava-se um fechaparenteses. "
                    "Linha: %u, função: %s()\n",
                    linha, __func__
                );
                exit(-1);
            }

            token = Analex();
        }
    }
    else if(token == vapara)
    {
        token = Analex();

        if(token != numero)
        {
            printf(
                "Erro: Esperava-se um número. Linha: %u, função: %s()\n",
                linha, __func__
            );
            exit(-1);
        }

        if(BuscarRotuloVisivel(palavraAtual) == NULL)
        {
            printf(
                "Erro: rótulo '%s' não declarado. "
                "Linha: %u, função: %s()\n",
                palavraAtual, linha, __func__
            );
            exit(-1);
        }

        token = Analex();
    }
    else if(token == inicio)
    {
        VerificaComando();

        while(token != fim)
        {
            if(token != pontoevirgula)
            {
                printf(
                    
                    "token: %s"
                    "Erro: Esperava-se um ponto e virgula. "
                    "Linha: %u, função: %s()\n",
                    tokenString[token],linha, __func__
                );
                exit(-1);
            }

            VerificaComando();
        }

        token = Analex();
    }
    else if(token == se)
    {
        token = Analex();
        VerificaCondicao();

        if(token != entao)
        {
            printf(
                "Erro: Esperava-se um entao. Linha: %u, função: %s()\n",
                linha, __func__
            );
            exit(-1);
        }

        token = Analex();
        VerificaComandoSemRotulo();

        if(token == senao)
        {
            token = Analex();
            VerificaComandoSemRotulo();
        }
    }
    else if(token == enquanto)
    {
        token = Analex();
        VerificaCondicao();

        if(token != faca)
        {
            printf(
                "Erro: Esperava-se um faca. Linha: %u, função: %s()\n",
                linha, __func__
            );
            exit(-1);
        }

        token = Analex();
        VerificaComandoSemRotulo();
    }
    else
    {
        printf(
            "Erro: Esperava-se um identificador, vapara, inicio, se ou enquanto. "
            "Linha: %u, função: %s()\n",
            linha, __func__
        );
        exit(-1);
    }
}

void VerificaComando()
{
    token = Analex();

    if(token == fim)
    {
        return;
    }

    if(token != numero &&
       token != identificador &&
       token != vapara &&
       token != inicio &&
       token != se &&
       token != enquanto)
    {
        printf(
            "Erro: Esperava-se um número ou comando sem rótulo. "
            "Linha: %u, função: %s()\n",
            linha, __func__
        );
        exit(-1);
    }

    if(token == numero)
    {
        if(BuscarRotuloVisivel(palavraAtual) == NULL)
        {
            printf(
                "Erro: rótulo '%s' não declarado. "
                "Linha: %u, função: %s()\n",
                palavraAtual, linha, __func__
            );
            exit(-1);
        }

        token = Analex();

        if(token != doispontos)
        {
            printf(
                "Erro: Esperava-se dois pontos após rótulo. "
                "Linha: %u, função: %s()\n",
                linha, __func__
            );
            exit(-1);
        }

        token = Analex();
    }

    VerificaComandoSemRotulo();
}

bool EhBloco()
{
    bool Ehrotulo = token == rotulo;
    bool EhTipo = token == tipo || verificaType(token);
    bool EhImplicito = token == variavel;
    bool EhProcedimento = token == procedimento;
    bool EhFuncao = token == funcao;
    bool EhBegin = token == inicio;

    return Ehrotulo ||
           EhTipo ||
           EhImplicito ||
           EhProcedimento ||
           EhFuncao ||
           EhBegin;
}

void parametrosFormais()
{
    while(token == pontoevirgula || token == abreparenteses)
    {
        token = Analex();

        if(token != identificador &&
           token != variavel &&
           token != funcao &&
           token != procedimento)
        {
            printf(
                "Erro: Esperava-se um identificador, variavel, funcao ou procedimento. "
                "Linha: %u, função: %s()\n",
                linha, __func__
            );
            exit(-1);
        }

        if(token == variavel)
        {
            token = Analex();

            if(token != identificador)
            {
                printf(
                    "Erro: Esperava-se um identificador. "
                    "Linha: %u, função: %s()\n",
                    linha, __func__
                );
                exit(-1);
            }
        }

        if(token == identificador)
        {
            int primeiro = tabela.tamanhoLogico;

            InserirSimboloOuErro(
                GerarSimbolo(palavraAtual, variavel, &tabela, NULL)
            );

            token = Analex();

            if(token != doispontos)
            {
                printf(
                    "Erro: Esperava-se um dois pontos. "
                    "Linha: %u, função: %s()\n",
                    linha, __func__
                );
                exit(-1);
            }

            token = Analex();

            if(verificaType(token) == false)
            {
                printf(
                    "Erro: Esperava-se um tipo. Linha: %u, função: %s()\n",
                    linha, __func__
                );
                exit(-1);
            }

            tabela.tabela[primeiro].valor = malloc(strlen(palavraAtual) + 1);

            if(tabela.tabela[primeiro].valor == NULL)
                exit(-1);

            strcpy(tabela.tabela[primeiro].valor, palavraAtual);
            tabela.tabela[primeiro].tipo = ObterTipo(palavraAtual);
            token = Analex();

            while(token == virgula)
            {
                token = Analex();

                if(token != identificador)
                {
                    printf(
                        "Erro: Esperava-se um identificador. "
                        "Linha: %u, função: %s()\n",
                        linha, __func__
                    );
                    exit(-1);
                }

                InserirSimboloOuErro(
                    GerarSimbolo(
                        palavraAtual,
                        variavel,
                        &tabela,
                        tabela.tabela[primeiro].valor
                    )
                );
                tabela.tabela[tabela.tamanhoLogico - 1].tipo =
                    tabela.tabela[primeiro].tipo;

                token = Analex();
            }
        }

        if(token == funcao)
        {
            int primeiro = tabela.tamanhoLogico;
            token = Analex();

            if(token != identificador)
            {
                printf(
                    "Erro: Esperava-se um identificador. "
                    "Linha: %u, função: %s()\n",
                    linha, __func__
                );
                exit(-1);
            }

            InserirSimboloOuErro(
                GerarSimbolo(palavraAtual, funcao, &tabela, NULL)
            );

            token = Analex();

            while(token == virgula)
            {
                token = Analex();

                if(token != identificador)
                {
                    printf(
                        "Erro: Esperava-se um identificador. "
                        "Linha: %u, função: %s()\n",
                        linha, __func__
                    );
                    exit(-1);
                }

                InserirSimboloOuErro(
                    GerarSimbolo(palavraAtual, funcao, &tabela, NULL)
                );

                token = Analex();
            }

            if(token != doispontos)
            {
                printf(
                    "Erro: Esperava-se um dois pontos. "
                    "Linha: %u, função: %s()\n",
                    linha, __func__
                );
                exit(-1);
            }

            token = Analex();

            if(verificaType(token) == false)
            {
                printf(
                    "Erro: Esperava-se um tipo de retorno. "
                    "Linha: %u, função: %s()\n",
                    linha, __func__
                );
                exit(-1);
            }

            for(int i = primeiro; i < tabela.tamanhoLogico; i++)
            {
                tabela.tabela[i].valor = malloc(strlen(palavraAtual) + 1);

                if(tabela.tabela[i].valor == NULL)
                    exit(-1);

                strcpy(tabela.tabela[i].valor, palavraAtual);
                tabela.tabela[i].tipo = ObterTipo(palavraAtual);
            }

            token = Analex();
        }

        if(token == procedimento)
        {
            token = Analex();

            if(token != identificador)
            {
                printf(
                    "Erro: Esperava-se um identificador. "
                    "Linha: %u, função: %s()\n",
                    linha, __func__
                );
                exit(-1);
            }

            InserirSimboloOuErro(
                GerarSimbolo(palavraAtual, procedimento, &tabela, NULL)
            );

            token = Analex();

            while(token == virgula)
            {
                token = Analex();

                if(token != identificador)
                {
                    printf(
                        "Erro: Esperava-se um identificador. "
                        "Linha: %u, função: %s()\n",
                        linha, __func__
                    );
                    exit(-1);
                }

                InserirSimboloOuErro(
                    GerarSimbolo(palavraAtual, procedimento, &tabela, NULL)
                );

                token = Analex();
            }
        }
    }

    if(token != fechaparenteses)
    {
        printf(
            "Erro: Esperava-se um fechaparenteses. "
            "Linha: %u, função: %s()\n",
            linha, __func__
        );
        printf("Token encontrado: %s\n", tokenString[token]);
        exit(-1);
    }
}

void AtribuirParametosFormais()
{
    parametrosFormais();
    token = Analex();
}

void AtribuirFuncao()
{
    token = Analex();

    if(token != identificador)
    {
        printf(
            "Erro: esperava-se um identificador. "
            "Linha: %u, função: %s()\n",
            linha, __func__
        );
        exit(1);
    }

    int indiceFuncao = tabela.tamanhoLogico;
    Simbolo simbFunc = GerarSimbolo(palavraAtual, funcao, &tabela, NULL);

    InserirSimboloOuErro(simbFunc);
    tabela.ScopoAtual++;
    //ImprimirTabela(&tabela);
    token = Analex();

    if(token != abreparenteses && token != doispontos)
    {
        printf(
            "Erro: esperava-se um parametro formal ou um dois pontos. "
            "Linha: %u, função: %s()\n",
            linha, __func__
        );
        printf("Token encontrado: %s\n", tokenString[token]);
        exit(1);
    }

    if(token == abreparenteses)
    {
        AtribuirParametosFormais();
    }

    if(token != doispontos)
    {
        printf(
            "Erro: esperava-se dois pontos (tipo de retorno). "
            "Linha: %u, função: %s()\n",
            linha, __func__
        );
        exit(1);
    }

    token = Analex();

    if(verificaType(token) == false)
    {
        printf(
            "Erro: esperava-se um tipo de retorno. "
            "Linha: %u, função: %s()\n",
            linha, __func__
        );
        exit(1);
    }

    tabela.tabela[indiceFuncao].valor = malloc(strlen(palavraAtual) + 1);

    if(tabela.tabela[indiceFuncao].valor == NULL)
        exit(-1);

    strcpy(tabela.tabela[indiceFuncao].valor, palavraAtual);
    tabela.tabela[indiceFuncao].tipo = ObterTipo(palavraAtual);
    token = Analex();

    if(token != pontoevirgula)
    {
        printf(
            "Erro: esperava-se um ponto e virgula. "
            "Linha: %u, função: %s()\n",
            linha, __func__
        );
        exit(1);
    }

    token = Analex();

    if(!EhBloco())
    {
        printf(
            "Erro: esperava-se um bloco. Linha: %u, função: %s()\n",
            linha, __func__
        );
        printf("Token encontrado: %s\n", tokenString[token]);
        exit(1);
    }

    verificaBloco();

    if(token != pontoevirgula)
    {
        printf(
            "Erro: esperava-se um ponto e virgula após o bloco da função. "
            "Linha: %u, função: %s()\n",
            linha, __func__
        );
        printf("Token encontrado: %s\n", tokenString[token]);
        exit(1);
    }

    token = Analex();
    RemoverScopo(&tabela, tabela.ScopoAtual);
}

void AtribuirProcedimento()
{
    token = Analex();

    if(token != identificador)
    {
        printf(
            "Erro: esperava-se um identificador. "
            "Linha: %u, função: %s()\n",
            linha, __func__
        );
        exit(1);
    }

    Simbolo simbProc = GerarSimbolo(
        palavraAtual, procedimento, &tabela, NULL
    );

    InserirSimboloOuErro(simbProc);
    tabela.ScopoAtual++;
    token = Analex();

    if(token == abreparenteses)
    {
        AtribuirParametosFormais();
    }

    if(token != pontoevirgula)
    {
        printf(
            "Erro: esperava-se um ponto e virgula. "
            "Linha: %u, função: %s()\n",
            linha, __func__
        );
        printf("Token encontrado: %s\n", tokenString[token]);
        exit(1);
    }

    token = Analex();

    if(!EhBloco())
    {
        printf(
            "Erro: esperava-se um bloco. Linha: %u, função: %s()\n",
            linha, __func__
        );
        printf("Token encontrado: %s\n", tokenString[token]);
        exit(1);
    }

    verificaBloco();

    if(token != pontoevirgula)
    {
        printf(
            "Erro: esperava-se um ponto e virgula. "
            "Linha: %u, função: %s()\n",
            linha, __func__
        );
        printf("Token encontrado: %s\n", tokenString[token]);
        exit(-1);
    }

    token = Analex();
    RemoverScopo(&tabela, tabela.ScopoAtual);
}

void AtribuirTipoImplicito()
{
    if(token != variavel)
    {
        printf(
            "Erro: esperava-se a uma variavel. Linha: %u, função: %s()\n",
            linha, __func__
        );
        printf("Token encontrado: %s\n", tokenString[token]);
        exit(-1);
    }

    token = Analex();

    if(token != identificador)
    {
        printf(
            "Erro: esperava-se um identificador. "
            "Linha: %u, função: %s()\n",
            linha, __func__
        );
        printf("Token encontrado: %s\n", tokenString[token]);
        exit(-1);
    }

    int primeiro = tabela.tamanhoLogico;

    InserirSimboloOuErro(
        GerarSimbolo(palavraAtual, variavel, &tabela, NULL)
    );

    token = Analex();

    while(token == virgula)
    {
        token = Analex();

        if(token != identificador)
        {
            printf(
                "Erro: esperava-se um identificador. "
                "Linha: %u, função: %s()\n",
                linha, __func__
            );
            printf("Token encontrado: %s\n", tokenString[token]);
            exit(-1);
        }

        InserirSimboloOuErro(
            GerarSimbolo(palavraAtual, variavel, &tabela, NULL)
        );

        token = Analex();
    }

    if(token != doispontos)
    {
        printf(
            "Erro: esperava-se dois pontos. Linha: %u, função: %s()\n",
            linha, __func__
        );
        exit(1);
    }

    token = Analex();

    if(verificaType(token) == false)
    {
        printf(
            "Erro: esperava-se um tipo. Linha: %u, função: %s()\n",
            linha, __func__
        );
        exit(1);
    }

    for(int i = primeiro; i < tabela.tamanhoLogico; i++)
    {
        tabela.tabela[i].valor = malloc(strlen(palavraAtual) + 1);

        if(tabela.tabela[i].valor == NULL)
            exit(-1);

        strcpy(tabela.tabela[i].valor, palavraAtual);
        tabela.tabela[i].tipo = ObterTipo(palavraAtual);
    }

    token = Analex();

    if(token != pontoevirgula)
    {
        printf(
            "Erro: esperava-se um ponto e virgula. "
            "Linha: %u, função: %s()\n",
            linha, __func__
        );
        exit(1);
    }
}


void AtribuirVariavel()
{
    TipoSemantico tipoAtual = ObterTipo(palavraAtual);
    token = Analex();

    if(token != identificador)
    {
        printf(
            "Erro: Esperava um identificador. Linha: %u, função: %s()\n",
            linha, __func__
        );
        exit(1);
    }

    int primeiro = tabela.tamanhoLogico;

    InserirSimboloOuErro(
        GerarSimbolo(palavraAtual, variavel, &tabela, NULL)
    );

    token = Analex();

    if(token != doispontos)
    {
        printf(
            "Erro: Esperava um dois pontos. Linha: %u, função: %s()\n",
            linha, __func__
        );
        exit(-1);
    }

    token = Analex();

    if(verificaType(token) == false)
    {
        printf(
            "Erro: Esperava um tipo. Linha: %u, função: %s()\n",
            linha, __func__
        );
        exit(-1);
    }
    tabela.tabela[primeiro].valor = malloc(strlen(palavraAtual) + 1);

    if(tabela.tabela[primeiro].valor == NULL)
        exit(-1);

    strcpy(tabela.tabela[primeiro].valor, palavraAtual);
    tabela.tabela[primeiro].tipo = ObterTipo(palavraAtual);
    CompararTipos(tipoAtual, &tabela.tabela[primeiro]);
    token = Analex();

    if(token != pontoevirgula)
    {
        printf(
            "Erro: Esperava um pontoEVirgula. Linha: %u, função: %s()\n",
            linha, __func__
        );
        exit(-1);
    }
}

void Atribuirrotulo()
{
    token = Analex();

    if(token != numero)
    {
        printf(
            "Erro: esperava a palavra numero! Linha: %u, função: %s()\n",
            linha, __func__
        );
        exit(1);
    }

    Simbolo simbolo = GerarSimbolo(palavraAtual, rotulo, &tabela, NULL);
    InserirSimboloOuErro(simbolo);
    token = Analex();

    if(token != virgula && token != pontoevirgula)
    {
        printf(
            "Erro: esperava a palavra virgula ou pontoEVirgula. "
            "Linha: %u, função: %s()\n",
            linha, __func__
        );
        exit(1);
    }



    while(token == virgula)
    {
        token = Analex();

        if(token != numero)
        {
            printf(
                "Erro: esperava a palavra numero. "
                "Linha: %u, função: %s()\n",
                linha, __func__
            );
            exit(1);
        }

        Simbolo simbolo = GerarSimbolo(
            palavraAtual, rotulo, &tabela, NULL
        );

        InserirSimboloOuErro(simbolo);
        token = Analex();

        if(token != virgula && token != pontoevirgula)
        {
            printf(
                "Erro: esperava a palavra virgula ou pontoEVirgula. "
                "Linha: %u, função: %s()\n",
                linha, __func__
            );
            exit(1);
        }
    }
}

void verificaBloco()
{
    while(EhBloco())
    {
        while(token == rotulo)
        {
            Atribuirrotulo();
            token = Analex();
        }

        while(verificaType(token) == true)
        {
            AtribuirVariavel();
            token = Analex();
        }

        while(token == variavel)
        {
            AtribuirTipoImplicito();
            token = Analex();
        }

        while(token == procedimento)
        {
            AtribuirProcedimento();
        }

        while(token == funcao)
        {
            AtribuirFuncao();
        }

        if(token == inicio)
        {
            VerificaComandoSemRotulo();
        }
    }
}

void verificaProgam()
{
    if(token != programa)
    {
        printf(
            "Erro: esperava a palavra program! Linha: %u, função: %s()\n",
            linha, __func__
        );
        printf("Token encontrado: %s\n", tokenString[token]);
        exit(-1);
    }

    token = Analex();

    if(token != identificador)
    {
        printf(
            "Erro: esperava a palavra identificador! "
            "Linha: %u, função: %s()\n",
            linha, __func__
        );
        exit(1);
    }

    InserirSimboloOuErro(
        GerarSimbolo(palavraAtual, programa, &tabela, NULL)
    );

    token = Analex();

    if(token != abreparenteses)
    {
        printf(
            "Erro: esperava a palavra abreParenteses! "
            "Linha: %u, função: %s()\n",
            linha, __func__
        );
        exit(1);
    }

    while(token != fechaparenteses)
    {
        token = Analex();

        if(token != identificador)
        {
            printf(
                "Erro: esperava a palavra identificador! "
                "Linha: %u, função: %s()\n",
                linha, __func__
            );
            exit(1);
        }

        InserirSimboloOuErro(
            GerarSimbolo(palavraAtual, identificador, &tabela, NULL)
        );

        token = Analex();

        if(token != virgula && token != fechaparenteses)
        {
            printf(
                "Erro: esperava a palavra virgula ou fechaparenteses! "
                "Linha: %u, função: %s()\n",
                linha, __func__
            );
            exit(1);
        }
    }

    token = Analex();

    if(token != pontoevirgula)
    {
        printf(
            "Erro: esperava a palavra virgula ou pontoEVirgula! "
            "Linha: %u, função: %s()\n",
            linha, __func__
        );
        exit(1);
    }

    token = Analex();

    if(!EhBloco())
    {
        printf(
            "Erro: esperava-se um bloco. Linha: %u, função: %s()\n",
            linha, __func__
        );
        printf("Token encontrado: %s\n", tokenString[token]);
        exit(-1);
    }

    verificaBloco();

    if(token != ponto)
    {
        printf(
            "Erro: esperava a palavra ponto! Linha: %u, função: %s()\n",
            linha, __func__
        );
        exit(1);
    }
}

int main()
{
    arquivo = fopen("arq.txt", "r");
    token = Analex();

    inicializarTabela(&tabela);
    verificaProgam();

    ImprimirTabela(&tabela);
    RemoverScopo(&tabela, tabela.ScopoAtual);
    ImprimirTabela(&tabela);
    LiberarTabela(&tabela);

    fclose(arquivo);

    printf("Programa sintaticamente e SEMANTICAMENTE correto!\n");

    return 0;
}
