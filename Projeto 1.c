#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <conio.h>

typedef struct tipoAP1 {
    int codigoControle;
    int codigoCachorro;
    char nomeVacina[40];
    char dataVacina[10];
    char responsavel[50];
} AP1;

typedef struct tipoAP2 {
    int codigoCachorro;
    char raca[20];
    char nomeCachorro[30];
} AP2;

typedef struct tipoIndice {
    int codigoControle;
    int pos;
    struct tipoIndice *prox;
} indice;

FILE *arq1;
FILE *arq2;
FILE *indexArq1;
indice *lista;

void menu();
void menuCadastro();
void cadastroCachorro();
void cadastroVacina();
void menuAlteracao();
void imprimirIndice();
void gravarIndice();
void lerIndice();
void carregarListaIndice();
void atualizaIndice();
void verificaArqIndex();
void menuRemocao();
int  obterCodigo();
void percorreListaRemocao();


int main() {
    verificaArqIndex();

    percorreListaRemocao();

    menu();

    imprimirIndice(lista);

    gravarIndice(lista);

    return 0;
}

//Verifica a existência do arquivo de indíces e toma as devidas providências
void verificaArqIndex() {
    lista = NULL;
    arq1 = fopen("arq1.bin", "r+b");
    if (arq1 == NULL) {
        arq1 = fopen("arq1.bin", "w+b");
        int n = -1;
        fwrite(&n,sizeof(int),1,arq1);
        fwrite(&n,sizeof(int),1,arq1);
        fclose(arq1);
        indexArq1 = fopen("indexArq1.bin", "w+b");
    }
    else {
            fseek(arq1,0,2);
            if ( (ftell(arq1)) != 4) {
                indexArq1 = fopen("indexArq1.bin", "r+b");
                if (indexArq1 == NULL) {
                    indexArq1 = fopen("indexArq1.bin", "w+b");
                    carregarListaIndice(&lista);
                }
                else {
                    fseek(indexArq1,0,2);
                    if ( (ftell(indexArq1)) != 0)
                        lerIndice(&lista);
                    else
                        lista = NULL;
                }
            }
    }
}

//Existindo o arquivo de cadastro de vacinas e não o de índices, carrega a lista de índices
void carregarListaIndice(indice **lista) {
    arq1 = fopen("arq1.bin", "r+b");

    *lista = NULL;

    int pos = 8;
    int tam = 0;
    char flag;
    char strCodigo[20];
    int codigo = 0;

    fseek(arq1,0,2);
    int tamArq = ftell(arq1);
    fseek(arq1,8,0);

    do {
        fread(&tam, sizeof(int), 1, arq1);
        fread(&flag, sizeof(char), 1, arq1);

        if (flag == '*') {
            fread(&strCodigo, tam - 1, 1, arq1);
            strtok(&strCodigo,"|");
            codigo = atoi(&strCodigo);
            indice *listaAux = malloc(sizeof(indice));
            listaAux->prox = *lista;
            *lista = listaAux;
            (*lista)->codigoControle = codigo;
            (*lista)->pos = pos;
        }
        else {
            fseek(arq1,(tam-sizeof(char)),1);
        }

        pos += tam + sizeof(int);

    } while (pos < tamArq);

    fclose(arq1);
}

//Grava o arquivo de índices
void gravarIndice(indice *lista) {
    indexArq1 = fopen("indexArq1.bin", "w+b");

    while (lista != NULL) {
        fwrite(&lista->codigoControle, sizeof(int), 1, indexArq1);
        fwrite(&lista->pos, sizeof(int), 1, indexArq1);
        lista = lista->prox;
    }

    fclose(indexArq1);
}

//Existindo o arquivo de índices, carrega a lista
void lerIndice(indice **lista) {
    indexArq1 = fopen("indexArq1.bin", "r+b");

    *lista = malloc(sizeof(indice));
    indice *listaAux = *lista;

    int codigo, pos;
    int count = 1;

    fseek(indexArq1,0,2);
    int tamArq = (ftell(indexArq1)/(sizeof(int)*2));
    fseek(indexArq1,0,0);

    fread(&codigo,sizeof(int),1,indexArq1);
    fread(&pos,sizeof(int),1,indexArq1);
    listaAux->codigoControle = codigo;
    listaAux->pos = pos;
    while(count < tamArq) {
        fread(&codigo,sizeof(int),1,indexArq1);
        fread(&pos,sizeof(int),1,indexArq1);
        listaAux->prox = malloc(sizeof(indice));
        listaAux = listaAux->prox;
        listaAux->codigoControle = codigo;
        listaAux->pos = pos;
        count++;
    }
    listaAux->prox = NULL;

    fclose(indexArq1);
}

int obterCodigo() {
    arq1 = fopen("arq1.bin", "r+b");
    int codigo;
    fread(&codigo,sizeof(int),1,arq1);
    codigo++;
    fseek(arq1,-sizeof(int),1);
    fwrite(&codigo,sizeof(int),1,arq1);
    fclose(arq1);
    return codigo;
}

int obterPos() {
    arq1 = fopen("arq1.bin", "r+b");
    fseek(arq1,0,2);
    int pos = ftell(arq1);
    fclose(arq1);
    return pos;
}

void atualizaIndice(indice **lista, AP1 aux, int pos) {
    indice *listaAux = malloc(sizeof(indice));
    listaAux->pos = pos;
    listaAux->codigoControle = aux.codigoControle;
    listaAux->prox = *lista;
    *lista = listaAux;
}

void imprimirIndice(indice *lista) {
    while (lista != NULL) {
        printf("%d ", lista->codigoControle);
        printf("%d \n", lista->pos);
        lista = lista->prox;
    }
    getch();
}

void percorreListaRemocao() {
    arq1 = fopen("arq1.bin", "r+b");
    int offset = 4;
    fseek(arq1,offset,0);
    fread(&offset,sizeof(int),1,arq1);
    printf("%d\n", offset);
    fseek(arq1,offset + sizeof(char) + sizeof(int),0);
    fread(&offset,sizeof(int),1,arq1);
    printf("%d\n", offset);
    fseek(arq1,offset + sizeof(char) + sizeof(int),0);
    fread(&offset,sizeof(int),1,arq1);
    printf("%d\n", offset);
    fseek(arq1,offset + sizeof(char) + sizeof(int),0);
    fread(&offset,sizeof(int),1,arq1);
    printf("%d\n", offset);
    fclose(arq1);
    getch();
}

void setOffset(int newOffset) {
    arq1 = fopen("arq1.bin", "r+b");
    bool fim = false;
    int offset;
    int offsetAux = -1;
    fseek(arq1,sizeof(int),0);
    fread(&offset,sizeof(int),1,arq1);
    while (!(fim)) {
        if ((offset == -1) || (newOffset < offset)) {
            fim = true;
            fseek(arq1,(offsetAux + sizeof(int) + sizeof(char)),0);
            fwrite(&newOffset,sizeof(int),1,arq1);
            fseek(arq1,(newOffset + sizeof(int) + sizeof(char)),0);
            fwrite(&offset,sizeof(int),1,arq1);
            printf("%d\n", newOffset);
            getch();
        }
        fseek(arq1,(offset + sizeof(int) + sizeof(char)),0);
        offsetAux = offset;
        fread(&offset,sizeof(int),1,arq1);
    }
    fclose(arq1);
}

void setInativo(int pos) {
    arq1 = fopen("arq1.bin", "r+b");
    fseek(arq1,(pos + 4),0);
    fwrite("!", sizeof(char), 1, arq1);
    fclose(arq1);
}

void removeVacina(int codigo) {
    indice *listaTemp = lista;
    indice *listaAux = lista;
    bool achou = false;

    if (listaTemp != NULL) {
        if (listaTemp->codigoControle == codigo) {
            achou = true;
            setInativo(listaTemp->pos);
            setOffset(listaTemp->pos);
            lista = lista->prox;
            free(listaTemp);
        }
        listaAux = listaTemp;
        listaTemp = listaTemp->prox;
        while (listaTemp != NULL) {
            if (listaTemp->codigoControle == codigo) {
                achou = true;
                setInativo(listaTemp->pos);
                setOffset(listaTemp->pos);
                listaAux->prox = listaTemp->prox;
                free(listaTemp);

            }
            listaAux = listaTemp;
            listaTemp = listaTemp->prox;
        }
    }

    if (!(achou))
        printf("Nao ha nenhuma vacina de codigo %d cadastrada!\n", codigo);
    else
        printf("Vacina %d removida com sucesso!\n", codigo);

    getch();
}

//Retorna o offset para escrita seguindo a regra do "first-fit" e removendo o elemento da lista de remoções. Caso retorne -1, o novo registro deve ser escrito no final do arquivo.
int obterOffset(int tam) {
    arq1 = fopen("arq1.bin", "r+b");
    int regTam;
    int offset = 4;
    int offsetAux = -1;
    int offsetReturn;
    fseek(arq1,offset,0);
    fread(&offset,sizeof(int),1,arq1);
    if (offset != -1) {
        do {
            fseek(arq1,offset,0);
            fread(&regTam,sizeof(int),1,arq1);
            if (tam <= regTam) {
                fseek(arq1,1,1);
                offsetReturn = offset;
                fread(&offset,sizeof(int),1,arq1);
                fseek(arq1,offsetAux + 5,0);
                fwrite(&offset,sizeof(int),1,arq1);
                getch();
                fclose(arq1);
                return offsetReturn;
            }
            fseek(arq1,1,1);
            offsetAux = offset;
            fread(&offset,sizeof(int),1,arq1);
        } while (offset != -1);
    }
    fclose(arq1);
    return offset;
}

void adicionarVacina(AP1 aux) {
    char buffer[105];
    sprintf(buffer,"*%d|%d|%s|%s|%s|", aux.codigoControle, aux.codigoCachorro, &aux.nomeVacina,&aux.dataVacina,&aux.responsavel);
    int tamanho = strlen(buffer);
    int offset = obterOffset(tamanho);
    arq1 = fopen("arq1.bin", "r+b");
    if (offset == -1) {
            fseek(arq1,0,2);
            offset = ftell(arq1);
            fwrite(&tamanho, sizeof(int), 1, arq1);
    }
    else {
        fseek(arq1,offset + sizeof(int),0);
    }
    fwrite(buffer, sizeof(char), strlen(buffer), arq1);
    printf("\n%d\n", tamanho);
    atualizaIndice(&lista,aux,offset);
    fclose(arq1);
}

void reescreveVacina(AP1 aux, int offset) {
    char buffer[105];
    sprintf(buffer,"*%d|%d|%s|%s|%s|", aux.codigoControle, aux.codigoCachorro, &aux.nomeVacina,&aux.dataVacina,&aux.responsavel);
    int tamanho = strlen(buffer);
    arq1 = fopen("arq1.bin", "r+b");
    fseek(arq1,offset + sizeof(int),0);
    fwrite(buffer, sizeof(char), strlen(buffer), arq1);
    //printf("\n%d\n", tamanho);
    //atualizaIndice(&lista,aux,offset);
    fclose(arq1);
}

void menu() {
    bool fim = false;
    do {
        system("cls");
        int menu;
        printf("Programa de Cadastro de Vacinas\n\n");
        printf("1. Cadastro\n");
        printf("2. Alteracao de Vacina\n");
        printf("3. Remocao\n");
        printf("4. Sair\n");
        printf("Escolha uma acao: ");
        scanf("%d", &menu);
        switch (menu) {
            case 1:
                menuCadastro();
                break;
            case 2:
                menuAlteracao();
                break;
            case 3:
                menuRemocao();
                break;
            case 4:
                fim = true;
                break;
        }
    } while (!fim);
}

void menuCadastro() {
    system("cls");
    int menu;
    printf("Menu de Cadastro\n\n");
    printf("1. Cadastrar cachorro\n");
    printf("2. Cadastrar vacina\n");
    printf("Escolha uma acao: ");
    scanf("%d", &menu);
    switch (menu) {
        case 1:
                cadastroCachorro();
                break;
        case 2:
                cadastroVacina();
                break;
    }
}

void menuRemocao() {
    system("cls");
    int codigo;
    printf("Menu de Remocao de Vacinas\n\n");
    printf("Digite o codigo da vacina a ser removida: (-1 para voltar)\n");
    scanf("%d", &codigo);
    if (codigo != -1)
        removeVacina(codigo);
    getch();
}


int buscaVacina(int codigo) {
    while (lista != NULL) {
        if (lista->codigoControle == codigo) {
            return lista->pos;
        }
        lista = lista->prox;
    }
    return -1;
}

int tamanhoRegistro(AP1 aux) {
    char buffer[105];
    sprintf(buffer,"*%d|%d|%s|%s|%s|", aux.codigoControle, aux.codigoCachorro, &aux.nomeVacina,&aux.dataVacina,&aux.responsavel);
    return strlen(buffer);
}

AP1 encontraRegistro(int pos) {
    arq1 = fopen("arq1.bin", "r+b");
    AP1 aux;
    int tam, tamNew, temp, sum;
    char str[40];

    fseek(arq1,pos,0);

    fread(&tam,sizeof(int),1,arq1);

    //* maldito
    fseek(arq1,sizeof(char),1);

    fread(&str,tam-1,1,arq1);
    strtok(str,"|");
    sum = strlen(str) + 1;
    temp = atoi(str);
    aux.codigoControle = temp;

    fseek(arq1,pos+4+1+sum,0);
    fread(&str,tam-sum-1,1,arq1);
    strtok(str,"|");
    sum += strlen(str) + 1;
    temp = atoi(str);
    aux.codigoCachorro = temp;

    fseek(arq1,pos+4+1+sum,0);
    fread(&str,tam-sum-1,1,arq1);
    strtok(str,"|");
    sum += strlen(str) + 1;
    strcpy(aux.nomeVacina,str);

    fseek(arq1,pos+4+1+sum,0);
    fread(&str,tam-sum-1,1,arq1);
    strtok(str,"|");
    sum += strlen(str) + 1;
    strcpy(aux.dataVacina,str);

    fseek(arq1,pos+4+1+sum,0);
    fread(&str,tam-sum-1,1,arq1);
    strtok(str,"|");
    sum += strlen(str) + 1;
    strcpy(aux.responsavel,str);

    fclose(arq1);

    return aux;

}

void menuAlteracao() {
    system("cls");
    int menu, codigo, posAlteracao, tam;
    AP1 aux;
    printf("Menu de Alteracao de dados de Vacinacao\n\n");
    printf("Digite o codigo da vacina a ter dados alterado: \n");
    do {
        scanf("%d", &codigo);
        posAlteracao = buscaVacina(codigo);
        if (posAlteracao == -1) {
            printf("Nao ha uma vacina cadastrada disponivel com esse codigo!\n");
            getch();
        }
    } while (posAlteracao == -1);
    system("cls");
    printf("Menu de Alteracao de dados de Vacinacao\n\n");
    printf("1. Codigo do Cachorro\n");
    printf("2. Nome da Vacina\n");
    printf("3. Data da Vacinacao\n");
    printf("4. Responsavel pela Aplicacao\n");
    printf("Escolha um campo para alterar: ");
    scanf("%d", &menu);
    aux = encontraRegistro(posAlteracao);
    switch (menu) {
        case 1:
                printf("\nCodigo do Cachorro: ");
                scanf("%d", &aux.codigoCachorro);
                break;
        case 2:
                printf("\nNome Vacina: ");
                scanf("%s", &aux.nomeVacina);
                break;
        case 3:
                printf("\nData: ");
                scanf("%s", &aux.dataVacina);
                break;
        case 4:
                printf("\nResponsavel pela Aplicacao: ");
                scanf("%s", &aux.responsavel);
                break;
    }
    arq1 = fopen("arq1.bin", "r+b");
    fseek(arq1,posAlteracao,0);
    fread(&tam,sizeof(int),1,arq1);
    fclose(arq1);
    if ((tamanhoRegistro(aux)) <= (tam))
        reescreveVacina(aux,posAlteracao);
    else {
        removeVacina(codigo);
        adicionarVacina(aux);
    }
    printf("Codigo Controle: %d\n", aux.codigoControle);
    printf("Codigo Cachorro: %d\n", aux.codigoCachorro);
    printf("Vacina: %s\n", aux.nomeVacina);
    printf("Data: %s\n", aux.dataVacina);
    printf("Responsavel: %s\n", aux.responsavel);
    getch();
}

void cadastroCachorro() {
    system("cls");
    AP2 aux;
    AP2 aux2;
    printf("Menu de Cadastro de Cachorros\n\n");
    arq2 = fopen("arq2.bin", "r+b");
    if (arq2 == NULL)
        arq2 = fopen("arq2.bin", "w+b");
    fseek(arq2,0,2);
    aux.codigoCachorro = ftell(arq2)/sizeof(AP2);
    printf("%d\n", aux.codigoCachorro);
    printf("Nome: ");
    scanf("%s", aux.nomeCachorro);
    printf("\nRaca: ");
    scanf("%s", aux.raca);
    fseek(arq2,0,2);
    fwrite(&aux, sizeof(AP2), 1, arq2);
    fseek(arq2, -sizeof(AP2),2);
    fread(&aux2, sizeof(AP2), 1, arq2);
    printf("Codigo: %d\n",aux2.codigoCachorro);
    printf("Raca: %s\n",aux2.raca);
    printf("Nome: %s\n",aux2.nomeCachorro);
    fclose(arq2);
    getch();
}

void cadastroVacina() {
    system("cls");
    AP1 aux;
    AP1 aux2;
    char buffer[105];
    printf("Menu de Cadastro de Vacinas\n\n");
    aux.codigoControle = obterCodigo();
    printf("%d", aux.codigoControle);
    printf("Codigo Cachorro: ");
    scanf("%d", &aux.codigoCachorro);
    printf("\nNome Vacina: ");
    scanf("%s", &aux.nomeVacina);
    printf("\nData: ");
    scanf("%s", &aux.dataVacina);
    printf("\nResponsavel pela Aplicacao: ");
    scanf("%s", &aux.responsavel);
    adicionarVacina(aux);
    getch();
}
