#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define numPalavras 36

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
    "."
};

char *tokens[38] = {
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
    "identificador"
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
    identificador
} palavrasReservadas;



palavrasReservadas RetornaPalavraCorrespondente(char* palavra)
{
    for (int i = 0; i < numPalavras; i++)
    {
        if (strcmp(palavra, palavras[i]) == 0)
        {
            return i;
        }
    }
    
    if (!(palavra[0] >= '0' && palavra[0] <= '9'))
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


char* retornarLinha(FILE *arquivo) {
    static char linha[100];

    if (fgets(linha, sizeof(linha), arquivo) != NULL) {
        return strtok(linha," ");
    }

    return NULL;
}

int main()
{
    FILE *arquivo = fopen("arq.txt", "r");
    char* linha;
    int posToken =-1;
    while (!feof(arquivo)){
        if ((linha = retornarLinha(arquivo)) !=NULL){
            while(linha != NULL){
                posToken = RetornaPalavraCorrespondente(linha);
                if(posToken == -1){
                    printf("Erro na leitura de %s",linha);
                    break;
                }
                printf("%s ",tokens[posToken]);
                linha = strtok(NULL, " ");
            }
        }
    }
    
