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

//	Declaração de estruturas e ponteiros de arquivos globais

typedef struct ap1Struct {
	int codControle, codCachorro;
	char nomeVacina[20], respAplic[10], dataVacina[10];
} ap1;

typedef struct ap2Struct {
	int codCachorro;
	char raca[15], nomeCachorro[30];
} ap2;

typedef struct indexStruct {
	int codControle, inicio;
	struct indexStruct *prox;
} indice1;

FILE* arq1;
FILE* arq2;
FILE* indexArq1;
FILE *arq1Compact;

indice1 *lista;

//	Protótipos

void menu();
void cadVacina();
void altVacina();
void remVacina();
void cadCachorro();
void buscaCachorro(int);
void verificaIndice();
void carregaIndice(struct indexStruct **lista);
void salvaIndice(struct indexStruct *lista);
void leIndice(struct indexStruct **lista);
void atualizaIndice(struct indexStruct **lista, struct ap1Struct aux, int tamanho);
void imprimeIndice(struct indexStruct *lista);
int obterOffset(int tam);
void setInativo(int pos);
void setOffset(int newOffset);

//	Function principal (main) do programa

int main() {
	verificaIndice();

    srand(time(NULL));

	menu();

	imprimeIndice(lista);
    salvaIndice(lista);
	return 0;
}

void menu() { //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	char opt;

	printf("Cadastro de vacinas\n\n");
	printf("[1] Cadastrar nova vacina\n");
	printf("[2] Alterar dados de vacina existente\n");
	printf("[3] Remover dados de vacina\n");
	printf("[4] Cadastrar novo cachorro\n");
	printf("[5] Alterar dados de cachorro existente\n");
	printf("[6] Remover dados de cachorro\n");
	printf("[7] Sair\n\n");
	printf("Escolha uma opcao: ");

	scanf("%s", &opt);
	getchar();

	switch(opt) {
		case '1':
			cadVacina();
			menu();
			break;
		case '2':
			menuAlterarVacina();
			menu();
			break;
		case '3':
//			remVacina();
			menu();
			break;
		case '4':
			cadCachorro();
			menu();
			break;
		case '5':
			menu();
			break;
		case '6':
			menu();
			break;
		case '7':
			printf("Fechando o programa...\n");
			break;
		default:
			printf("Opcao invalida!");
			menu();
			break;
	}
}

void menuAlterarVacina() { /////////////////////////////////////////////////////////////////////////////////////////////////////////
	char optAlt;
	
	printf("Qual dado voc� quer alterar?\n\n");
	printf("[1] Codigo do Cachorro\n");
	printf("[2] Nome da Vacina\n");
	printf("[3] Data de Vacinacao\n");
	printf("[4] Responsavel pela Alteracao\n");
	printf("[5] Sair\n\n");
	printf("Escolha uma opcao: ");
	
	scanf("%s", &optAlt);
	getchar();
	
	switch(optAlt) {
		case '1':
			menuAlterarVacina();
			//;
		case '2':
			menuAlterarVacina();
			//;
		case '3':
			menuAlterarVacina();
			//;
		case '4':
			menuAlterarVacina();
			//;
		case '5':
			menu();
			break;
		default:
			printf("Opcao invalida!");
			menuAlterarVacina();
			break;
	}
}

//	Function para o cadastro de vacinas
void cadVacina() { ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct ap1Struct aux, aux2;
	int opt;
	char buffer[60];

	FILE *arq1 = fopen("Arquivo1.bin","r+b");

	if (arq1 == NULL) { //Verificação de existência do arquivo
        arq1 = fopen("Arquivo1.bin","w+b"); //Criação do arquivo,caso não exista
	}

	printf("CADASTRO DE NOVA VACINACAO\n");

//	printf("\nO animal ja e cadastrado? (1=Sim 0=Nao)");
//	scanf("%d", &opt);

//	switch(opt) {
//		case 0:
//			cadCachorro();
//			break;
//		case 1:
			printf("\nQual o codigo do cachorro? ");
			scanf("%d",&aux.codCachorro);

//			buscaCachorro(aux.codCachorro); 

			printf("\nQual o nome da vacina? ");
			fflush(stdin);                    
			scanf("%s", &aux.nomeVacina);                   

			printf("\nQual a data da vacinacao? ");
			fflush(stdin);
			scanf("%s", &aux.dataVacina);
			
			printf("\nQual o nome do responsavel pela aplicacao? ");
			fflush(stdin);
			scanf("%s", &aux.respAplic);
			aux.codControle = rand() % 10000 + 1;                             
			
			fseek(arq1,0,2);

			sprintf(buffer,"|%s|%s|%s|",&aux.nomeVacina,&aux.dataVacina,&aux.respAplic);
			int tamReg = (strlen(buffer) + (2*sizeof (int)) + (2*sizeof(char)));


			fwrite(&tamReg, sizeof(int), 1, arq1);
			fwrite("*", sizeof(char), 1, arq1);
			fwrite(&aux.codControle, sizeof(int), 1, arq1);
			fwrite("|", sizeof(char), 1, arq1);
			fwrite(&aux.codCachorro, sizeof(int), 1, arq1);
									
			fwrite(buffer,sizeof(char),strlen(buffer),arq1);  

			fclose(arq1);
                     
//			break;
//		}
    printf("\nCadastro realizado com sucesso!!!");
}

//Function para cadastrar um novo cachorro.
void cadCachorro() { /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	struct ap2Struct aux, aux2;
	int opt;

	FILE *arq2 = fopen("Arquivo2.bin","r+b");

	if (arq2 == NULL) {	//Verificação de existência do arquivo
		arq2 = fopen("Arquivo2.bin","w+b");	//Criação do arquivo,caso não exista        
	}
	
	fseek(arq2,0,2);

	printf("CADASTRO DE CACHORRO\n");
	aux.codCachorro = ftell(arq2)/sizeof(ap2);
	printf("\nCodigo do cachorro: %d",aux.codCachorro);
	printf("\nQual a raca do cachorro? ");
	fflush(stdin);
	scanf("%s",&aux.raca);
	printf("\nQual o nome do cachorro? ");
	fflush(stdin);
	scanf("%s",&aux.nomeCachorro);

	printf("Confirma? (1=Sim 0=Nao) ");
	scanf("%d",&opt);
	switch (opt) {
		case 0:
			break;
		case 1:                                             
			fwrite(&aux, sizeof(ap2), 1, arq2);	// Salva os campos do registro aux no arquivo
			fseek(arq2, -sizeof(ap2), 2);	// Posiciona o arquivo 1 registro para trás
			fread(&aux2, sizeof(ap2), 1 ,arq2);	// Recupera o ultimo registro para aux2
			printf("\nCodigo do cachorro: %d",aux2.codCachorro);                                         
			printf("\nNome: %s",aux2.nomeCachorro);                       
			printf("\nRaca: %s",aux2.raca);                                  
			break;
	}

	fclose(arq2);
}

//Function que busca um cachorro pelo codigo fornecido.
void buscaCachorro(int cod) { ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	struct ap2Struct aux;
	int pos,tamArq;

	FILE *buscaCao = fopen("Arquivo2.bin","r");

	if (buscaCao == NULL) {		// Verificação de existência do arquivo
		printf("Arquivo de cachorros nao existe!!!");         
	}	//Fim do primeiro if 

	else {
		pos = (cod)*52; // Código do cachorro multiplicado pelo tamanho de cada registro

		fseek(buscaCao,0,2);
		tamArq = (int) ftell(buscaCao);

		if (pos<=tamArq) {
			fseek(buscaCao,pos,0);
			fread(&aux, sizeof(ap2), 1, buscaCao);
			printf("\nInformacoes do cachorro:");                                         
			printf("\nNome: %s",aux.nomeCachorro);                       
			printf("\nRaca: %s\n",aux.raca);                     
		} //Fim do segundo if

		else {
			printf("Codigo invalido!!!");
		} //Fim do segundo else
	} //Fim do primeiro else
}

void verificaIndice() {
    lista = NULL;
    arq1 = fopen("Arquivo1.bin", "r+b");
    if (arq1 == NULL) {
        arq1 = fopen("Arquivo1.bin", "w+b");
        int n = -1;
        fwrite(&n,sizeof(int),1,arq1);
        fwrite(&n,sizeof(int),1,arq1);
        fclose(arq1);
        indexArq1 = fopen("Indice1.bin", "w+b");
    }
    else {
            fseek(arq1,0,2);
            if (ftell(arq1) != 4) {
                indexArq1 = fopen("Indice1.bin", "r+b");
                if (indexArq1 == NULL) {
                    indexArq1 = fopen("Indice1.bin", "w+b");
                    carregaIndice(&lista);
                }
                else
                    fseek(arq1,0,2);
                    if ( (ftell(arq1)) != 0)
                       leIndice(&lista);
                    else
                        lista = NULL;
            }
    }
}

//certo
void carregaIndice(struct indexStruct **lista) {
    arq1 = fopen("Arquivo1.bin", "r+b");

    *lista = NULL;

    int pos = 8;
    int tam = 0;
    int cod = 0;
    char flag;
    char codStr[20];

    fseek(arq1,0,2);
    int tamArq = ftell(arq1);
    fseek(arq1,8,0);

    do {
        fread(&tam, sizeof(int), 1, arq1);
        fread(&flag, sizeof(char), 1, arq1);

        if (flag == '*') {
            fread(&codStr, tam - 2, 1, arq1);
            strtok(&codStr,"|");
            cod = atoi(&codStr);
            struct indexStruct *listaAux = malloc(sizeof(indice1));
            listaAux->prox = *lista;
            *lista = listaAux;
            (*lista)->codControle = cod;
            (*lista)->inicio = pos;
            fseek(arq1,1,1);
        }
        else {
            fseek(arq1,(tam-sizeof(char)),1);
        }

        pos += tam + sizeof(int);

    } while (pos < tamArq);

    fclose(arq1);
}

//certo
void salvaIndice(struct indexStruct *lista) {
    indexArq1 = fopen("Indice1.bin", "w+b");

    while (lista != NULL) {
        fwrite(&lista->codControle, sizeof(int), 1, indexArq1);
        fwrite(&lista->inicio, sizeof(int), 1, indexArq1);
        lista = lista->prox;
    }

    fclose(indexArq1);
}

//certo
void leIndice(struct indexStruct **lista) {
    indexArq1 = fopen("Indice1.bin", "r+b");

    *lista = malloc(sizeof(indice1));
    struct indexStruct *listaAux = *lista;

    int cod;
    int pos;
    int cont = 1;

    fseek(indexArq1,0,2);
    int tamArq = (ftell(indexArq1)/(sizeof(int)*2));
    fseek(indexArq1,0,0);

    fread(&cod,sizeof(int),1,indexArq1);
    fread(&pos,sizeof(int),1,indexArq1);
    listaAux->codControle = cod;
    listaAux->inicio = pos;
    while(cont < tamArq) {
        fread(&cod,sizeof(int),1,indexArq1);
        fread(&pos,sizeof(int),1,indexArq1);
        listaAux->prox = malloc(sizeof(indice1));
        listaAux = listaAux->prox;
        listaAux->codControle = cod;
        listaAux->inicio = pos;
        cont++;
    }
    listaAux->prox = NULL;

    fclose(indexArq1);
}

//certo
void atualizaIndice(struct indexStruct **lista, struct ap1Struct aux, int tamanho) {
    struct indexStruct *listaAux = malloc(sizeof(indice1));

    listaAux->inicio = tamanho;
    listaAux->codControle = aux.codControle;
    listaAux->prox = *lista;
    *lista = listaAux;
}

//certo
void imprimeIndice(struct indexStruct *lista) {
    while (lista != NULL) {
        printf("%d ", lista->codControle);
        printf("%d \n", lista->inicio);
        lista = lista->prox;
    }
    getch();
}

//certa mudar nome e printf
void removeVacina(int codigo) {
    struct indexStruct *listaTemp = lista;
    struct indexStruct *listaAux = lista;
    bool achou = false;

	if (listaTemp != NULL) {
        if (listaTemp->codControle == codigo) {
            achou = true;
        	setInativo(listaTemp->inicio);
        	setOffset(listaTemp->inicio);
        	lista = lista->prox;
        	free(listaTemp);
        }
    	listaAux = listaTemp;
    	listaTemp = listaTemp->prox;
    	while (listaTemp != NULL) {
            if (listaTemp->codControle == codigo) {
     		    achou = true;
                setInativo(listaTemp->inicio);
                setOffset(listaTemp->inicio);
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



//certo
int obterCodigo() {
    arq1 = fopen("Arquivo1.bin", "r+b");
    int cod;
    
    fread(&cod,sizeof(int),1,arq1);
    cod++;
    fseek(arq1,-sizeof(int),1);
    fwrite(&cod,sizeof(int),1,arq1);
    fclose(arq1);
    return cod;
}

//certo
int obterPos() {
    arq1 = fopen("Arquivo1.bin", "r+b");
    fseek(arq1,0,2);
    int pos = ftell(arq1);
    fclose(arq1);
    return pos;
}

//debug
void percorreListaRemocao() {
    arq1 = fopen("Arquivo1.bin", "r+b");
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

// certo trocar nome
void setOffset(int newOffset) {
    arq1 = fopen("Arquivo1.bin", "r+b");
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

// certo trocar nome
void setInativo(int pos) {
    arq1 = fopen("Arquivo1.bin", "r+b");
    fseek(arq1,(pos + 4),0);
    fwrite("!", sizeof(char), 1, arq1);
    fclose(arq1);
}


//certo trocar nome
int obterOffset(int tam) {
    arq1 = fopen("Arquivo1.bin", "r+b");
    int regTam;
    int offset = 4;
    int offsetAux = -1;
    int offsetReturn;
    fseek(arq1,offset,0);
    fread(&offset,sizeof(int),1,arq1);
    if (offset != -1) {
        do {
		   	printf("Teste 1");
            fseek(arq1,offset,0);
            fread(&regTam,sizeof(int),1,arq1);
            if (tam <= regTam) {
			   		printf("Teste 3");
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
        printf("Teste 4");
    }
    fclose(arq1);
    return offset;
}
