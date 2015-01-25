//	Projeto 1 - Estrutura de Dados II
//	Alunos: Bruno Fouz Valente e Pedro Ivo Monteiro Privatto

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//================================================================
//===========================DECLARAÇÕES==========================
//================================================================

struct ap1Struct {
	char nomeVacina[20];
	char respAplic[10];
	char dataVacina[10];
	int codControle, codCachorro;
} ap1;

struct ap2Struct {
	char raca[15], nomeCachorro[30];
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

//================================================================
//===========================PROTOTYPES===========================
//================================================================

void menu();
void cadVacina();
void altVacina();
void remVacina();
void cadCachorro();
//void altCachorro();
//void remCachorro();
void buscaCachorro(int);
void geraIndice();
void leIndice(struct indexStruct* listaTemp);
void gravaIndice(struct indexStruct* lista);
void imprimeIndice(struct indexStruct* lista);

//================================================================
//=============================MAIN===============================
//================================================================

int main() {

	struct indexStruct* lista = malloc(sizeof(indice1));
    srand(time(NULL));
	geraIndice(lista);
	imprimeIndice(lista);

	menu();

	printf("\nGravoooou2\n");
	gravaIndice(lista);
	printf("\nGravoooou3\n");

	return 0;
}

//================================================================
//===========================FUNCTIONS============================
//================================================================

void menu() {

	char opt;

	printf("(1) Cadastrar nova vacina\n");
	printf("(2) Alterar dados de vacina existente\n");
	printf("(3) Remover dados de vacina\n");
	printf("(4) Cadastrar novo cachorro\n");
//	printf("(5) Alterar dados de cachorro existente\n");
//	printf("(6) Remover dados de cachorro\n\n");
	printf("(7) Sair\n\n");
	printf("Escolha uma opcao: ");

	scanf("%s", &opt);

	switch(opt) {
		case '1':
			cadVacina();
			menu();
			break;
		case '2':
//			altVacina();
			break;
		case '3':
//			remVacina();
			break;
		case '4':
			cadCachorro();
			menu();
			break;
		case '5':
			break;
		case '6':
			break;
		case '7':
			system("cls");
			printf("Fechando o programa...\n");
			break;
		default:
			printf("Opcao invalida!");
			menu();
			break;
	}
}

void cadVacina() {

	struct ap1Struct aux, aux2;
	int opt;
	char buffer[60];

	FILE *arq1 = fopen("Arquivo1.bin","r+b");

	if (arq1 == NULL) { //Verificação de existência do arquivo
        arq1 = fopen("Arquivo1.bin","w+b"); //Criação do arquivo,caso não exista
	}

	system("cls");
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

void cadCachorro() {

	struct ap2Struct aux,aux2;
	int opt;

	FILE *arq2 = fopen("Arquivo2.bin","r+b");

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
			printf("\nNome: %s",aux2.nomeCachorro);                       
			printf("\nRaca: %s",aux2.raca);                                  
			break;
	}

	fclose(arq2);
}

//Function que busca um cachorro pelo codigo fornecido.

void buscaCachorro(int cod) {

	struct ap2Struct aux;
	int pos,tamArq;

	FILE *buscaCao = fopen("Arquivo2.bin","r");

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

//Function que gera indíces em memória principal.

void geraIndice(struct indexStruct* lista) {
      
	struct indexStruct* listaAux = lista;
    
	FILE *arq1 = fopen("Arquivo1.bin", "r+b");

	if(arq1 == NULL) {	//Verificação de existência do arquivo
		arq1 = fopen("Arquivo1.bin", "w+b");	//Criação do arquivo,caso não exista 
	}

	indexArq1 = fopen("indexArq1.bin", "r+b");
	if(indexArq1 == NULL) {	//Verificação de existência do arquivo
		indexArq1 = fopen("indexArq1.bin", "w+b");	//Criação do arquivo,caso não exista
    
        int tam,cod,pos = 0;
        char flag;

        fseek(arq1,0,2);
        int tamArq = ftell(arq1);	//Le o tamanho do arquivo.
        fseek(arq1,0,0);

        do {
            fread(&tam, sizeof(int), 1, arq1); 		//Le o primeiro inteiro, que é o tamanho do registo.
            fread(&flag, sizeof(char), 1, arq1);	//Le o char que represanta se é valido ou não (* ou !).

            if (flag == '*') {	//Registro válido.
                fread(&cod, sizeof(int), 1, arq1);	//Le o CodControle do Arquivo para uma variavel.
                listaAux->codControle = cod;	//Atribui o CodControle a um campo da lista.
                listaAux->inicio = pos;	//Atribui a posição a um campo da lista.
                listaAux->prox = malloc(sizeof(indice1));	//Aloca memória para o próximo da lista.
                listaAux = listaAux->prox;	//"Anda" com a lista.
                fseek(arq1,(tam-sizeof(char)-sizeof(int)),1);
            }
            else {	//Registro inválido.
                fseek(arq1,(tam-sizeof(char)),1);
            }

            pos += tam + sizeof(int);

        } while (pos < tamArq);
		
  		listaAux->prox = NULL;
  		
    } else leIndice(lista);	//Caso indexArq1.bin exista.

    fclose(arq1);

}

//	Function que le os índices gerados anteriormente.

void leIndice(struct indexStruct* listaTemp) {

    struct indexStruct* lista = listaTemp;
    int cod,pos,cont = 0;

    fseek(indexArq1,0,2);
    int tamArq = (ftell(indexArq1)/(sizeof(int)*2));	//Le o número de indices (tamanho total/tamanho registro).
    fseek(indexArq1,0,0);

    while(cont < tamArq) {
        fread(&cod,sizeof(int),1,indexArq1);
        fread(&pos,sizeof(int),1,indexArq1);
        lista->codControle = cod;
        lista->inicio = pos;
        lista->prox = malloc(sizeof(indice1));
        lista = lista->prox;
        cont++;
    }

    lista->prox = NULL;
}

//	Function que grava em disco os índices gerados anteriormente.

void gravaIndice(struct indexStruct* lista) {

    while (lista->prox != NULL) {
		  printf("\nGravoooou\n");
        fwrite(&lista->codControle, sizeof(int), 1, indexArq1);
        fwrite(&lista->inicio, sizeof(int), 1, indexArq1);
        lista = lista->prox;	//"Anda" com a lista.
    }

    fclose(indexArq1);
}

void imprimeIndice(struct indexStruct* lista) {
    while (lista->prox != NULL) {
        printf("%d ", lista->codControle);
        printf("%d \n", lista->inicio);
        lista = lista->prox;
    }
}
