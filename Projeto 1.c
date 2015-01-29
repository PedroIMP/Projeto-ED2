//	Projeto 1 - Estrutura de Dados II
//	Nome: Bruno Fouz Valente
//	Nome: Pedro Ivo Monteiro Privatto

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>
#include <stdbool.h>

//================================================================
//===========================DECLARAÇÕES==========================
//================================================================

struct ap1Struct {
	char nomeVacina[20];
	char respAplic[10];
	char dataVacina[10];
	int codControle;
	int codCachorro;
} ap1;

struct ap2Struct {
	char raca[15];
	char nomeCachorro[30];
	int codCachorro;
} ap2;
         
struct indexStruct{
	int codControle;
	int inicio;
	struct indexStruct *prox;
} indice1;



FILE* arq1;
FILE* arq2;
FILE* indexArq1;

struct indexStruct* lista;

//================================================================
//===========================PROTOTYPES===========================
//================================================================

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



//================================================================
//=============================MAIN===============================
//================================================================

int main() {
	
    srand(time(NULL));

	
	menu();
	
	
	imprimeIndice(lista);

    salvaIndice(lista);
	getch();
	system("pause");
	return 0;
}

//================================================================
//===========================FUNCTIONS============================
//================================================================

void menu() {
	int opt;

	do{
	   system("cls");
	   printf("==========================================\n");
	   printf("(1) Cadastrar nova vacina                 |\n");
	   printf("==========================================\n");
	   printf("(2) Alterar dados de vacina existente     |\n");
	   printf("==========================================\n");
	   printf("(3) Remover dados de vacina               |\n");
	   printf("==========================================\n");
	   printf("(4) Cadastrar novo cachorro               |\n");
	   printf("==========================================\n");
	   //printf("(5) Alterar dados de cachorro existente\n");
	   //printf("(6) Remover dados de cachorro\n\n");
	   printf("(7) Sair                                  |\n");
	   printf("==========================================\n");
	   printf("Escolha uma opcao: ");
	   scanf("%d",&opt);
    //}while (opt !=7);
	switch(opt) {

		case 1:
			cadVacina();
			menu();			
			break;
		case 2:
			//altVacina();
			break;
		case 3:
			//remVacina();
			break;
		case 4:
			cadCachorro();
			menu();	
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			system("cls");
			printf("Fechando o programa...\n");
			break;
	} //Fim do switch
	}while (opt !=7);
}

void cadVacina() {
	FILE *arq1;
	struct ap1Struct aux;
	struct ap1Struct aux2;
	int opt;
	char buffer[100];

	arq1 = fopen("Arquivo1.bin","r+b");

	if (arq1 == NULL) { //Verificação de existência do arquivo
        arq1 = fopen("Arquivo1.bin","w+b"); //Criação do arquivo,caso não exista
	}

	system("cls");
	printf("CADASTRO DE NOVA VACINACAO\n");

//	printf("\nO animal ja e cadastrado? (1=Sim 0=Nao)");
//	scanf("%d",&opt);

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

			sprintf(buffer,"*%d|%d|%s|%s|%s|", aux.codControle, aux.codCachorro, &aux.nomeVacina,&aux.dataVacina,&aux.respAplic);
			int tamReg = strlen(buffer);
			int offset = obterOffset(tamReg);

    		if (offset == -1) {
                fseek(arq1,0,2);
				offset = ftell(arq1);
            	fwrite(&tamReg, sizeof(int), 1, arq1);
 			}
    		else 
        	    fseek(arq1,offset + sizeof(int),0);
									
			fwrite(buffer,sizeof(char),strlen(buffer),arq1);  
			atualizaIndice(&lista,aux,offset);
			fclose(arq1);
                     
//			break;
//		}
    printf("\nCadastro realizado com sucesso!!!");
    getch();
}

void cadCachorro() {
//Function para cadastrar um novo cachorro.
	FILE *arq2;
	struct ap2Struct aux;
	struct ap2Struct aux2;
	int opt;

	arq2 = fopen("Arquivo2.bin","r+b");

	if (arq2 == NULL) {	//Verificação de existência do arquivo
		arq2 = fopen("Arquivo2.bin","w+b");	//Criação do arquivo,caso não exista        
	}
	
	fseek(arq2,0,2);

	system("cls");   
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
			fwrite(&aux, sizeof(ap2), 1, arq2);	//Salva os campos do registro aux no arquivo
			
			
			fseek(arq2, -sizeof(ap2),2);	//Posiciona o arquivo 1 registro para trás
			fread(&aux2, sizeof(ap2), 1 ,arq2);	//Recupera o ultimo registro para aux2
			printf("\nCodigo do cachorro: %d",aux2.codCachorro);        
			printf("\nRaca: %s",aux2.raca);                                   
			printf("\nNome: %s",aux2.nomeCachorro);                       			
			getch();                                
			break;
	}

	fclose(arq2);
}

void buscaCachorro(int cod) {
//Function que busca um cachorro pelo codigo fornecido. 
	FILE *buscaCao;
	struct ap2Struct aux;
	int pos,tamArq;

	buscaCao = fopen("Arquivo2.bin","r");
	if (buscaCao == NULL) {		//Verificação de existência do arquivo
		printf("Arquivo de cachorros nao existe!!!");         
	}	//Fim do primeiro if 

	else {
		pos = (cod)*52; //Código do cachorro multiplicado pelo tamanho de cada registro

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
