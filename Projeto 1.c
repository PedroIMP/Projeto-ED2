//
//	Estrutura de Dados II
//	Projeto 1
//
//	Alunos: Bruno Fouz Valente e Pedro Ivo Monteiro Privatto
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

//	DeclaraÃ§Ã£o de estruturas e ponteiros de arquivos globais

struct ap1Struct {
    int codControle, codCachorro;
    char nomeVacina[40];
    char dataVacina[10];
    char respAplic[50];
};

struct ap2Struct {
    int codCachorro;
    char raca[20];
    char nomeCachorro[30];
};

struct indice1Struct {
    int codControle, offset1, offset2;
};

struct indice2Struct {
	int offset2;
    char nomeVacina[40];
};

typedef struct ap1Struct AP1;
typedef struct ap2Struct AP2;
typedef struct indice1Struct indice1;
typedef struct indice2Struct indice2;

FILE *dadosCachorro;
FILE *dadosVacina;
FILE *arqIndex;
FILE *arqIndex2;
FILE *arqIndex2Offsets;
FILE *dadosVacinaCompact;

indice1 index1[10000];
indice2 index2[10000];
int numVacina, numCachorro, numIndex1, numIndex2;

//	ProtÃ³tipos

void menu();
void menuCadastro();
void menuRemocao();
void menuAlteracao();
void menuConsulta();
void setInativo();
void setOffset();
void compactarDadosVacina();
void carregarIndice();
void carregarListaIndice();
void cadastroCachorro();
void inicializaVetores();
void verificaArquivos();
void imprimirIndex1();
void imprimirIndex2();
void salvarArquivos();
void cadastroVacina();
void sortIndex2();

//	Function principal (main) do programa

int main() {
    inicializaVetores();
    verificaArquivos();
    menu();

    // DEBUG ////
    imprimirIndex1();

    printf("\n");

    imprimirIndex2();
    ///////////

    salvarArquivos();
	return 0;
}

void menu() {
	bool fim = false;
	do {
		char opt;
		printf("Cadastro de vacinas\n\n");
		printf("[1] Cadastrar nova vacina\n");
		printf("[2] Alterar dados de vacina existente\n");
		printf("[3] Remover dados de vacina\n");
		printf("[4] Cadastrar novo cachorro\n");
		printf("[5] Remover dados de cachorro\n");
		printf("[6] Sair\n\n");
		printf("Escolha uma opcao: ");

		scanf("%s", &opt);
		getchar();

		switch(opt) {
			case '1':
				cadastroVacina();
				break;
			case '2':
				menuAlteracao();
				break;
			case '3':
				menuRemocao();
				break;
			case '4':
				cadastroCachorro();
				break;
			case '5':
				menuRemocao();
				break;
			case '6':
				printf("Fechando o programa...\n");
				fim = true;
				break;
			default:
				printf("Opcao invalida!");
				menu();
				break;
		}
	} while (!fim);
}

void sortIndex2() {
    int count, count2, temp;
    char strTemp[40];
    count = 0;
    while (count <= numIndex2) {
        count2 = 0;
        while (count2 <= numIndex2) {
            if (strcmp(index2[count].nomeVacina,index2[count2].nomeVacina) < 0) {

                strcpy(strTemp,index2[count].nomeVacina);
                strcpy(index2[count].nomeVacina,index2[count2].nomeVacina);
                strcpy(index2[count2].nomeVacina,strTemp);

                temp = index2[count].offset2;
                index2[count].offset2 = index2[count2].offset2;
                index2[count2].offset2 = temp;
            }
            count2++;
        }
        count++;
    }

}

void verificaArquivos() {
    dadosCachorro = fopen("cachorros.bin", "r+b");
    if (dadosCachorro == NULL) {
        dadosCachorro = fopen("cachorros.bin", "w+b");
        dadosVacina = fopen("vacinas.bin", "w+b");
        int n = -1;
        fwrite(&n,sizeof(int),1,dadosVacina);
        fwrite(&n,sizeof(int),1,dadosVacina);
        arqIndex = fopen("indicePrim.bin", "w+b");
        arqIndex2 = fopen("indiceSec.bin", "w+b");
        arqIndex2Offsets = fopen("indiceSec2.bin", "w+b");
    }
    else {
        dadosVacina = fopen("vacinas.bin", "r+b");
        if (dadosVacina == NULL) {
            dadosVacina = fopen("vacinas.bin", "w+b");
            int n = -1;
            fwrite(&n,sizeof(int),1,dadosVacina);
            fwrite(&n,sizeof(int),1,dadosVacina);
            arqIndex = fopen("indicePrim.bin", "w+b");
            arqIndex2 = fopen("indiceSec.bin", "w+b");
            arqIndex2Offsets = fopen("indiceSec2.bin", "w+b");
        }
        else {
                fseek(dadosVacina,0,2);
                if ( (ftell(dadosVacina)) > 4) {
                    arqIndex = fopen("indicePrim.bin", "r+b");
                    arqIndex2 = fopen("indiceSec.bin", "r+b");
                    arqIndex2Offsets = fopen("indiceSec2.bin", "r+b");
                    if ( (arqIndex == NULL) || (arqIndex2 == NULL) || (arqIndex2Offsets == NULL)){
                        arqIndex = fopen("indicePrim.bin", "w+b");
                        arqIndex2 = fopen("indiceSec.bin", "w+b");
                        arqIndex2Offsets = fopen("indiceSec2.bin", "w+b");
                        carregarListaIndice();
                        sortIndex2();
                    }
                    else {
                        char flag;
                        fread(&flag,1,1,arqIndex);
                        if (&flag == "!") {
                            arqIndex = fopen("indicePrim.bin", "w+b");
                            arqIndex2 = fopen("indiceSec.bin", "r+b");
                            arqIndex2Offsets = fopen("indiceSec2.bin", "r+b");
                            carregarListaIndice();
                            sortIndex2();
                        }
                        else {
                            carregarIndice();
                        }
                    }
                }
                else {
                    dadosVacina = fopen("vacinas.bin", "w+b");
                    arqIndex = fopen("indicePrim.bin", "w+b");
                    arqIndex2 = fopen("indiceSec.bin", "w+b");
                    arqIndex2Offsets = fopen("indiceSec2.bin", "w+b");
                }
                arqIndex2 = fopen("indiceSec.bin", "r+b");
                arqIndex2Offsets = fopen("indiceSec2.bin", "r+b");
        }
    }
}

int obterCodigo() {
    int codigo;
    fseek(dadosVacina,0,0);
    fread(&codigo,sizeof(int),1,dadosVacina);
    codigo++;
    fseek(dadosVacina,-sizeof(int),1);
    fwrite(&codigo,sizeof(int),1,dadosVacina);
    return codigo;
}

int obterOffset(int tam) {
    int regTam;
    int offset1 = 4;
    int offsetAux = -1;
    int offsetReturn;
    fseek(dadosVacina,offset1,0);
    fread(&offset1,sizeof(int),1,dadosVacina);
    if (offset1 != -1) {
        do {
            fseek(dadosVacina,offset1,0);
            fread(&regTam,sizeof(int),1,dadosVacina);
            if (tam <= regTam) {
                fseek(dadosVacina,1,1);
                offsetReturn = offset1;
                fread(&offset1,sizeof(int),1,dadosVacina);
                fseek(dadosVacina,offsetAux + 5,0);
                fwrite(&offset1,sizeof(int),1,dadosVacina);
                 
                return offsetReturn;
            }
            fseek(dadosVacina,1,1);
            offsetAux = offset1;
            fread(&offset1,sizeof(int),1,dadosVacina);
        } while (offset1 != -1);
    }
    return offset1;
}

void adicionaIndice(AP1 aux, int offset1) {
    numIndex1++;
    index1[numIndex1].codControle = aux.codControle;
    index1[numIndex1].offset1 = offset1;
    index1[numIndex1].offset2 = -1;

    int count = 0;
    bool achou = false;
    while ( (count <= numIndex2) && (!achou) ){
        if (strcmp(index2[count].nomeVacina,aux.nomeVacina) == 0) {
            achou = true;
            if (index2[count].offset2 == -1)
                index2[numIndex2].offset2 = numIndex1;
            else {
                int aux = 0;
                int offset2 = index2[count].offset2;
                do {
                    aux = offset2;
                    offset2 = index1[offset2].offset2;
                } while (offset2 != -1) ;
                index1[aux].offset2 = numIndex1;
            }
        }
        else
            count++;
    }

    if (!achou) {
        numIndex2++;
        strcpy(index2[numIndex2].nomeVacina,aux.nomeVacina);
        index2[numIndex2].offset2 = numIndex1;
    }

}

void mudaOffset(int codigo, int offset1) {
    bool achou = false;
    int count = 0;

    while ((count <= numIndex1) && (!achou)) {
        if (index1[count].codControle == codigo) {
            achou = true;
            setInativo(index1[count].offset1);
            setOffset(index1[count].offset1);
            index1[count].offset1 = offset1;
        }
        count++;
    }
}

void adicionarVacina(AP1 aux, bool atualizando) {
    char buffer[105];
    sprintf(buffer,"*%d|%d|%s|%s|%s|", aux.codControle, aux.codCachorro, &aux.nomeVacina,&aux.dataVacina,&aux.respAplic);
    int tamanho = strlen(buffer);
    int offset1 = obterOffset(tamanho);
    if (offset1 == -1) {
            fseek(dadosVacina,0,2);
            offset1 = ftell(dadosVacina);
            fwrite(&tamanho, sizeof(int), 1, dadosVacina);
    }
    else {
        fseek(dadosVacina,offset1 + sizeof(int),0);
    }
    fwrite(buffer, sizeof(char), strlen(buffer), dadosVacina);
    printf("\n%d\n", tamanho);
    if (!atualizando) {
        adicionaIndice(aux,offset1);
        sortIndex2();
    }
    else
        mudaOffset(aux.codControle,offset1);
}

int buscaCachorro(int codigo) {
    fseek(dadosCachorro,0,2);
    int tamArq = ftell(dadosCachorro)/12;
    fseek(dadosCachorro,0,0);

    AP2 aux;
    int count = 0;

    bool achou = false;

    while ( (count < tamArq) && (!achou) ) {
        fread(&aux.codCachorro,sizeof(aux),1,dadosCachorro);
        if (aux.codCachorro == codigo)  {
                achou = true;
        }
        count++;
    }

    if (achou)
        return aux.codCachorro;
    else
        return -1;

}

void cadastroVacina() {
    AP1 aux;
    AP1 aux2;
    char buffer[105];
    aux.codControle = obterCodigo();
    do {
        system("cls");
        printf("Menu de Cadastro de Vacinas\n\n");
        printf("%d", aux.codControle);
        printf("Codigo Cachorro: ");
        scanf("%d", &aux.codCachorro);
        if (buscaCachorro(aux.codCachorro) == -1) {
            printf("\nCachorro nao encontrado!\n\n");
            bool sair = false;
            do {
                char opt;
                fflush(stdin);
                printf("Deseja cadastrar um cachorro? [y/n]  ");
                scanf("%c", &opt);
                switch (opt) {
                    case 'y':
                        cadastroCachorro();
                        sair = true;
                        break;
                    case 'n':
                        sair = true;
                        break;
                }
            } while (!sair);
            fflush(stdin);
        }
    } while (buscaCachorro(aux.codCachorro) == -1);
    printf("\nNome Vacina: ");
    scanf("%s", &aux.nomeVacina);
    printf("\nData: ");
    scanf("%s", &aux.dataVacina);
    printf("\nResponsavel pela Aplicacao: ");
    scanf("%s", &aux.respAplic);
    fseek(arqIndex,0,0);
    fwrite("!",1,1,arqIndex);
    adicionarVacina(aux, false);
}

void cadastroCachorro() {
    system("cls");
    AP2 aux;
    AP2 aux2;
    printf("Menu de Cadastro de Cachorros\n\n");
    fseek(dadosCachorro,0,2);
    aux.codCachorro = ftell(dadosCachorro)/sizeof(AP2);
    printf("%d\n", aux.codCachorro);
    printf("Nome: ");
    scanf("%s", aux.nomeCachorro);
    printf("\nRaca: ");
    scanf("%s", aux.raca);
    fseek(dadosCachorro,0,2);
    fwrite(&aux, sizeof(AP2), 1, dadosCachorro);
    fseek(dadosCachorro, -sizeof(AP2),2);
    fread(&aux2, sizeof(AP2), 1, dadosCachorro);
    printf("Codigo: %d\n",aux2.codCachorro);
    printf("Raca: %s\n",aux2.raca);
    printf("Nome: %s\n",aux2.nomeCachorro);
     
}

void setOffset(int newOffset) {
    bool fim = false;
    int offset1;
    int offsetAux = -1;
    fseek(dadosVacina,sizeof(int),0);
    fread(&offset1,sizeof(int),1,dadosVacina);
    while (!(fim)) {
        if ((offset1 == -1) || (newOffset < offset1)) {
            fim = true;
            fseek(dadosVacina,(offsetAux + sizeof(int) + sizeof(char)),0);
            fwrite(&newOffset,sizeof(int),1,dadosVacina);
            fseek(dadosVacina,(newOffset + sizeof(int) + sizeof(char)),0);
            fwrite(&offset1,sizeof(int),1,dadosVacina);
            printf("%d\n", newOffset);
             
        }
        fseek(dadosVacina,(offset1 + sizeof(int) + sizeof(char)),0);
        offsetAux = offset1;
        fread(&offset1,sizeof(int),1,dadosVacina);
    }
}

void setInativo(int pos) {
    fseek(dadosVacina,(pos + 4),0);
    fwrite("!", sizeof(char), 1, dadosVacina);
}

void removeDoIndex2(int pos, int codigo) {
    char nome[100];
    int tam;
    int soma = 0;

    fseek(dadosVacina,pos,0);

    fread(&tam,sizeof(int),1,dadosVacina);

    fseek(dadosVacina,1,1);

    fread(&nome,tam - 1,1,dadosVacina);

    strtok(nome,"|");

    soma += strlen(nome) + 1;

    fseek(dadosVacina,pos + 5 + soma,0);

    fread(&nome,tam - 1 - soma,1,dadosVacina);

    strtok(nome,"|");

    soma += strlen(nome) + 1;

    fseek(dadosVacina,pos + 5 + soma,0);

    fread(&nome,tam - 1 - soma,1,dadosVacina);

    strtok(nome,"|");

    int count = 0;
    bool achou = false;
    bool achou2 = false;
    while (!achou) {
        if (strcmp(index2[count].nomeVacina, nome) == 0) {
            achou = true;
            int offset1 = index2[count].offset2;
            if (index1[offset1].codControle == codigo) {
                    index2[count].offset2 = index1[offset1].offset2;
            }
            else {
                int offsetAnt;
                do {
                    offsetAnt = offset1;
                    offset1 = index1[offset1].offset2;
                    if (index1[offset1].codControle == codigo) {
                        achou2 = true;
                        index1[offsetAnt].offset2 = index1[offset1].offset2;
                    }
                } while (!achou2);
            }
        }
        else
            count++;
    }
}

void removeVacina(int codigo) {
    bool achou = false;
    int count = 0;

    while ((count <= numIndex1) && (!achou)) {
        if (index1[count].codControle == codigo) {
            achou = true;
            removeDoIndex2(index1[count].offset1, codigo);
            setInativo(index1[count].offset1);
            setOffset(index1[count].offset1);
            index1[count].codControle = -1;
        }
        count++;
    }

    if (!achou)
        printf("Nao ha nenhuma vacina de codigo %d cadastrada!\n", codigo);
    else
        printf("Vacina %d removida com sucesso!\n", codigo);

     
}

void menuRemocao() {
    system("cls");
    int codigo;
    printf("Menu de Remocao de Vacinas\n\n");
    printf("Digite o codigo da vacina a ser removida: (-1 para voltar)\n");
    scanf("%d", &codigo);
    if (codigo != -1) {
        fseek(arqIndex,0,0);
        fwrite("*",1,1,arqIndex);
        removeVacina(codigo);
    }
     
}

AP1 encontraRegistro(int pos) {
    AP1 aux;
    int tam, tamNew, temp, sum;
    char str[40];

    fseek(dadosVacina,pos,0);

    fread(&tam,sizeof(int),1,dadosVacina);

    //* maldito
    fseek(dadosVacina,sizeof(char),1);

    fread(&str,tam-1,1,dadosVacina);
    strtok(str,"|");
    sum = strlen(str) + 1;
    temp = atoi(str);
    aux.codControle = temp;

    fseek(dadosVacina,pos+4+1+sum,0);
    fread(&str,tam-sum-1,1,dadosVacina);
    strtok(str,"|");
    sum += strlen(str) + 1;
    temp = atoi(str);
    aux.codCachorro = temp;

    fseek(dadosVacina,pos+4+1+sum,0);
    fread(&str,tam-sum-1,1,dadosVacina);
    strtok(str,"|");
    sum += strlen(str) + 1;
    strcpy(aux.nomeVacina,str);

    fseek(dadosVacina,pos+4+1+sum,0);
    fread(&str,tam-sum-1,1,dadosVacina);
    strtok(str,"|");
    sum += strlen(str) + 1;
    strcpy(aux.dataVacina,str);

    fseek(dadosVacina,pos+4+1+sum,0);
    fread(&str,tam-sum-1,1,dadosVacina);
    strtok(str,"|");
    sum += strlen(str) + 1;
    strcpy(aux.respAplic,str);

    return aux;

}

int buscaVacina(int codigo) {
    int count = 0;

    while (count <= numIndex1) {
        if (index1[count].codControle == codigo) {
            return index1[count].offset1;
        }
        count++;
    }
    return -1;
}

int tamanhoRegistro(AP1 aux) {
    char buffer[105];
    sprintf(buffer,"*%d|%d|%s|%s|%s|", aux.codControle, aux.codCachorro, &aux.nomeVacina,&aux.dataVacina,&aux.respAplic);
    return strlen(buffer);
}

void reescreveVacina(AP1 aux, int offset1) {
    char buffer[105];
    sprintf(buffer,"*%d|%d|%s|%s|%s|", aux.codControle, aux.codCachorro, &aux.nomeVacina,&aux.dataVacina,&aux.respAplic);
    int tamanho = strlen(buffer);
    fseek(dadosVacina,offset1 + sizeof(int),0);
    fwrite(buffer, sizeof(char), strlen(buffer), dadosVacina);
}

void atualizaIndex2(char *nomeAnt, char *nome, int codigo) {
    int count = 0;
    bool achou = false;
    bool achou2 = false;
    int ref;
    while (!achou) {
        if (strcmp(index2[count].nomeVacina, nomeAnt) == 0) {
            achou = true;
            int offset1 = index2[count].offset2;
            if (index1[offset1].codControle == codigo) {
                    index2[count].offset2 = index1[offset1].offset2;
                    ref = offset1;
            }
            else {
                int offsetAnt;
                do {
                    offsetAnt = offset1;
                    offset1 = index1[offset1].offset2;
                    if (index1[offset1].codControle == codigo) {
                        achou2 = true;
                        index1[offsetAnt].offset2 = index1[offset1].offset2;
                        ref = offset1;
                    }
                } while (!achou2);
            }
        }
        else
            count++;
    }

    count = 0;
    achou = false;
    while ( (count <= numIndex2) && (!achou) ){
        if (strcmp(index2[count].nomeVacina, nome) == 0) {
            achou = true;
            if (index2[count].offset2 == -1)
                index2[numIndex2].offset2 = ref;
            else {
                int aux = 0;
                int offset2 = index2[count].offset2;
                do {
                    aux = offset2;
                    offset2 = index1[offset2].offset2;
                } while (offset2 != -1) ;
                index1[aux].offset2 = ref;
            }
        }
        else
            count++;
    }

    if (!achou) {
        numIndex2++;
        strcpy(index2[numIndex2].nomeVacina,nome);
        index2[numIndex2].offset2 = ref;
    }

    index1[ref].offset2 = -1;

}

void menuAlteracao() {
    system("cls");
    int menu, codigo, posAlteracao, tam;
    AP1 aux;
    printf("Menu de Alteracao de dados de Vacinacao\n\n");
    do {
        printf("Digite o codigo da vacina a ter dados alterado: \n");
        scanf("%d", &codigo);
        posAlteracao = buscaVacina(codigo);
        if (posAlteracao == -1)
            printf("\nNao ha uma vacina cadastrada disponivel com esse codigo!\n\n");
    } while (posAlteracao == -1);
    system("cls");
    printf("Menu de Alteracao de dados de Vacinacao\n\n");
    printf("1. Codigo do Cachorro\n");
    printf("2. Nome da Vacina\n");
    printf("3. Data da Vacinacao\n");
    printf("4. respAplic pela Aplicacao\n");
    printf("Escolha um campo para alterar: ");
    scanf("%d", &menu);
    aux = encontraRegistro(posAlteracao);
    char nomeAnt[40];
    fseek(arqIndex,0,0);
    fwrite("!",1,1,arqIndex);
    switch (menu) {
        case 1:
                printf("\nCodigo do Cachorro: ");
                scanf("%d", &aux.codCachorro);
                break;
        case 2:
                strcpy(nomeAnt,aux.nomeVacina);
                printf("\nNome Vacina: ");
                scanf("%s", &aux.nomeVacina);
                atualizaIndex2(nomeAnt, aux.nomeVacina, aux.codControle);
                sortIndex2();
                break;
        case 3:
                printf("\nData: ");
                scanf("%s", &aux.dataVacina);
                break;
        case 4:
                printf("\nResponsavel pela Aplicacao: ");
                scanf("%s", &aux.respAplic);
                break;
    }
    fseek(dadosVacina,posAlteracao,0);
    fread(&tam,sizeof(int),1,dadosVacina);
    if ((tamanhoRegistro(aux)) <= (tam)) {
        reescreveVacina(aux,posAlteracao);
    }
    else {
        adicionarVacina(aux, true);
    }
    printf("Codigo Controle: %d\n", aux.codControle);
    printf("Codigo Cachorro: %d\n", aux.codCachorro);
    printf("Vacina: %s\n", aux.nomeVacina);
    printf("Data: %s\n", aux.dataVacina);
    printf("respAplic: %s\n", aux.respAplic);
     
}

void menuConsulta() {

}

void inicializaVetores() {
    numIndex1 = -1;
    numIndex2 = -1;

    int i;
    for (i = 0; i <= 10000; i++) {
        index1[i].codControle = -1;
    }
}

void imprimirIndex1() {
    int count = 0;
    while (count <= numIndex1) {
        if (index1[count].codControle != -1)
                printf("%d %d \n", index1[count].codControle, index1[count].offset1);
        count++;
    }
     
}

void imprimirIndex2() {
    int count = 0;
    while (count <= numIndex2) {
        printf("%s \n", index2[count].nomeVacina);
        int offset1 = index2[count].offset2;
        while (offset1 != -1) {
            printf("        %d %d \n", index1[offset1].codControle, index1[offset1].offset1);
            offset1 = index1[offset1].offset2;
        }
        count++;
    }
}

void gravarIndex() {
    fseek(arqIndex,0,0);
    fwrite("*",1,1,arqIndex);

    int count = 0;
    while (count <= numIndex1) {
        if (index1[count].codControle != -1) {
                fwrite(&index1[count].codControle,sizeof(int),1,arqIndex);
                fwrite(&index1[count].offset1,sizeof(int),1,arqIndex);
        }
        count++;
    }
     
}

void gravarIndex2() {
    fseek(arqIndex2,0,0);
    fseek(arqIndex2Offsets,0,0);
    int count = 0;
    indice2 aux;
    while (count <= numIndex2) {
        strcpy(aux.nomeVacina,index2[count].nomeVacina);
        aux.offset2 =  index2[count].offset2;
        fwrite(&aux,sizeof(aux),1,arqIndex2);
        int offset1 = index2[count].offset2;
        while (offset1 != -1) {
            fwrite(&index1[offset1].codControle,sizeof(int),1,arqIndex2Offsets);
            fwrite(&index1[offset1].offset2,sizeof(int),1,arqIndex2Offsets);
            offset1 = index1[offset1].offset2;
        }
        count++;
    }
}

void lerIndex2() {
    fseek(arqIndex2,0,2);
    int tamArq = ftell(arqIndex2)/44;
    fseek(arqIndex2,0,0);
    indice2 aux;

    int count = 0;

    while (count < tamArq) {
        fread(&aux,sizeof(aux),1,arqIndex2);
         
        count++;
    }

    fseek(arqIndex2Offsets,0,2);
    tamArq = ftell(arqIndex2Offsets)/8;
    fseek(arqIndex2Offsets,0,0);

    count = 0;
    int codigo, offset1;
    while (count < tamArq) {
        fread(&codigo,sizeof(int),1,arqIndex2Offsets);
        fread(&offset1,sizeof(int),1,arqIndex2Offsets);
        count++;
    }

}

void carregarIndice() {
    fseek(arqIndex2,0,2);
    int tamArq = ftell(arqIndex2)/44;
    fseek(arqIndex2,0,0);

    indice2 aux;

    int codigo, offset2;
    int count = 0;
    while (count < tamArq) {
        fread(&aux,sizeof(aux),1,arqIndex2);
        numIndex2++;
        strcpy(index2[numIndex2].nomeVacina, aux.nomeVacina);
        index2[numIndex2].offset2 = aux.offset2;
        int offset1 = aux.offset2;
        while (offset1 != -1) {
            fread(&codigo,sizeof(int),1,arqIndex2Offsets);
            fread(&offset2,sizeof(int),1,arqIndex2Offsets);
            index1[offset1].codControle = codigo;
            index1[offset1].offset1 = 777;
            index1[offset1].offset2 = offset2;
            if (offset1 > numIndex1)
                numIndex1 = offset1;
            offset1 = offset2;
        }
        count++;
    }

    fseek(arqIndex,1,0);
    count = 0;
    int offset1;
    while (count <= numIndex1) {
        if (index1[count].codControle != -1) {
                fseek(arqIndex,4,1);
                fread(&offset1,sizeof(int),1,arqIndex);
                index1[count].offset1 = offset1;

        }
        count++;
    }

}

void carregarListaIndice() {
    int pos = 8;
    int tam = 0;
    char flag;
    char str[100];
    int codigo = 0;
    int ref, soma, count;
    bool achou;
    numIndex1 = -1;
    numIndex2 = -1;

    fseek(dadosVacina,0,2);
    int tamArq = ftell(dadosVacina);
    fseek(dadosVacina,8,0);

    do {
        fread(&tam, sizeof(int), 1, dadosVacina);
        fread(&flag, sizeof(char), 1, dadosVacina);

        if (flag == '*') {
            ref = ftell(dadosVacina);
            fread(&str, tam - 1, 1, dadosVacina);
            strtok(str,"|");
            codigo = atoi(str);
            soma = strlen(str)+1;

            numIndex1++;
            index1[numIndex1].codControle = codigo;
            index1[numIndex1].offset1 = pos;
            index1[numIndex1].offset2 = -1;

            fseek(dadosVacina,ref + soma,0);
            fread(&str, tam - 1 - soma , 1, dadosVacina);
            strtok(str,"|");
            soma += strlen(str) + 1;
            fseek(dadosVacina,ref + soma,0);
            fread(&str, tam - 1 - soma , 1, dadosVacina);
            strtok(str,"|");

            achou = false;
            count = 0;
            while ((count <= numIndex2) && (!achou)) {
                if (strcmp(index2[count].nomeVacina,str) == 0) {
                    achou = true;
                    if (index2[count].offset2 == -1)
                        index2[numIndex2].offset2 = numIndex1;
                    else {
                        int aux = 0;
                        int offset2 = index2[count].offset2;
                    do {
                        aux = offset2;
                        offset2 = index1[offset2].offset2;
                    } while (offset2 != -1) ;
                    index1[aux].offset2 = numIndex1;
                    }
                }
                else
                    count++;
            }

            if (!achou) {
                numIndex2++;
                strcpy(index2[numIndex2].nomeVacina,str);
                index2[numIndex2].offset2 = numIndex1;
            }



        }
        else {
            fseek(dadosVacina,(tam-sizeof(char)),1);
        }

        pos += tam + sizeof(int);

    } while (pos < tamArq);
}

void compactarDadosVacina() {
    dadosVacinaCompact = fopen("dadosVacinaCompact.bin", "w+b");

    fseek(arqIndex,0,0);
    fwrite("!",1,1,arqIndex);

    AP1 aux;
    int pos = 8;
    int tam = 0;
    char flag;
    char str[50];
    int sum;
    int n;

    fseek(dadosVacina,0,2);
    int tamArq = ftell(dadosVacina);
    fseek(dadosVacina,0,0);
    fread(&n,sizeof(int),1,dadosVacina);
    fwrite(&n,sizeof(int),1,dadosVacinaCompact);
    n = -1;
    fwrite(&n,sizeof(int),1,dadosVacinaCompact);
    fseek(dadosVacina,8,0);

    do {
        fread(&tam, sizeof(int), 1, dadosVacina);
        fread(&flag, sizeof(char), 1, dadosVacina);

        if (flag == '*') {
            fwrite(&tam,sizeof(int),1,dadosVacinaCompact);
            fwrite("*",sizeof(char),1,dadosVacinaCompact);

            fread(&str,tam-1,1,dadosVacina);
            strtok(str,"|");
            sum = strlen(str) + 1;
            fwrite(&str,strlen(str),1,dadosVacinaCompact);

            fseek(dadosVacina,pos+4+1+sum,0);
            fread(&str,tam-sum-1,1,dadosVacina);
            strtok(str,"|");
            sum += strlen(str) + 1;
            fwrite("|",1,1,dadosVacinaCompact);
            fwrite(&str,strlen(str),1,dadosVacinaCompact);

            fseek(dadosVacina,pos+4+1+sum,0);
            fread(&str,tam-sum-1,1,dadosVacina);
            strtok(str,"|");
            sum += strlen(str) + 1;
            fwrite("|",1,1,dadosVacinaCompact);
            fwrite(&str,strlen(str),1,dadosVacinaCompact);

            fseek(dadosVacina,pos+4+1+sum,0);
            fread(&str,tam-sum-1,1,dadosVacina);
            strtok(str,"|");
            sum += strlen(str) + 1;
            fwrite("|",1,1,dadosVacinaCompact);
            fwrite(&str,strlen(str),1,dadosVacinaCompact);

            fseek(dadosVacina,pos+4+1+sum,0);
            fread(&str,tam-sum-1,1,dadosVacina);
            strtok(str,"|");
            sum += strlen(str) + 1;
            fwrite("|",1,1,dadosVacinaCompact);
            fwrite(&str,strlen(str),1,dadosVacinaCompact);
            fwrite("|",1,1,dadosVacinaCompact);
            sum++;
            if (sum < tam) {
                fseek(dadosVacinaCompact,pos,0);
                fwrite(&sum,sizeof(int),1,dadosVacinaCompact);
                fseek(dadosVacinaCompact,0,2);
            }
        }
        else {
            fseek(dadosVacina,(tam-sizeof(char)),1);
        }

        pos += tam + sizeof(int);

    } while (pos < tamArq);

    fclose(dadosVacinaCompact);
    fclose(dadosVacina);
    remove("vacinas.bin");
    rename("dadosVacinaCompact.bin","vacinas.bin");
    dadosVacina = fopen("vacinas.bin", "r+b");

    carregarListaIndice();
    sortIndex2();
    fclose(arqIndex);
    fclose(arqIndex2);
    fclose(arqIndex2Offsets);
    arqIndex = fopen("indicePrim.bin", "w+b");
    arqIndex2 = fopen("indiceSec.bin", "w+b");
    arqIndex2Offsets = fopen("indiceSec2.bin", "w+b");
    gravarIndex();
    gravarIndex2();

    fseek(arqIndex,0,0);
    fwrite("*",1,1,arqIndex);
}

void salvarArquivos() {
    gravarIndex();
    gravarIndex2();
    fclose(dadosCachorro);
    fclose(dadosVacina);
    fclose(arqIndex);
    fclose(arqIndex2);
    fclose(arqIndex2Offsets);
}
