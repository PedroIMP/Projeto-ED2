//	Projeto 1 - Estrutura de Dados II
//	Nome: Bruno Fouz Valente

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>

struct ap1Struct {
	char* nomeVacina;
	char* respAplic;
	char* dataVacina;
	int codControle, codCachorro;
} ap1;

struct ap2Struct {
	char raca[15], nomeCachorro[30];
	int codCachorro;
	//struct ap2Struct *prox;
} ap2;
         
struct indexStruct{
	int codControle;
	int inicio;
	struct indexStruct *prox;
} indice1;

//==========================================================
//========================PROTOTYPES========================
//==========================================================

void menu();
void cadVacina();
void altVacina();
void remVacina();
void cadCachorro();
void altCachorro();
void remCachorro();
void buscaCachorro(int);

//==========================================================
//==========================MAIN============================
//==========================================================

int main() {
    srand(time(NULL));
	menu();

	printf("\n");
	system("pause");
	return 0;
}

//==========================================================
//========================FUNCTIONS=========================
//==========================================================

void menu() {
	int opt;

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

	switch(opt) {

		case 1:
			cadVacina();
			break;
		case 2:
			//altVacina();
			break;
		case 3:
			//remVacina();
			break;
		case 4:
			cadCachorro();
			break;
		case 5:
			//altCachorro();
			break;
		case 6:
			//remCachorro();
			break;
		case 7:
			system("cls");
			printf("Fechando o programa...");
			system("close");
			break;
	} //Fim do switch
}

void cadVacina() {
	FILE *arq1;
	struct ap1Struct aux,aux2;
	int opt;
	char buffer[60];

	arq1 = fopen("Arquivo1.bin","r+b");

	if (arq1 == NULL) { //VERIFICAÇÃO NA ABERTURA DE ARQUIVO PARA LEITURA 
        arq1 = fopen("Arquivo1.bin","w+b"); //Criação do arquivo
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

			sprintf(buffer,"|%s|%s|%s|",&aux.nomeVacina,&aux.dataVacina,&aux.respAplic);
			int tamReg = (strlen(buffer) + (2*sizeof (int)) + (2*sizeof(char)));

			printf("\ntamanho %d\n",tamReg);
			fwrite(&tamReg, sizeof(int), 1, arq1);
			fwrite("*", sizeof(char), 1, arq1);
			fwrite(&aux.codControle, sizeof(int), 1, arq1);
			fwrite("|", sizeof(char), 1, arq1);
			fwrite(&aux.codCachorro, sizeof(int), 1, arq1);
			fwrite("|", sizeof(char), 1, arq1);
			
			//ERRO COMEÇA
			fwrite(aux.nomeVacina, strlen(aux.nomeVacina), 1, arq1);
			fwrite("|", sizeof(char), 1, arq1);
			fwrite(aux.dataVacina,strlen(aux.dataVacina), 1, arq1);
			fwrite("|", sizeof(char), 1, arq1);
			fwrite(aux.respAplic, strlen(aux.respAplic), 1, arq1);
			fwrite("|", sizeof(char), 1, arq1);
			//ERRO TERMINA
			
			
			//fwrite(buffer,sizeof(char),strlen(buffer),arq1);  

			fclose(arq1);
                     
//			break;
//		}
}

void cadCachorro() {
	FILE *arq2;
	struct ap2Struct aux,aux2;
	int opt;

	arq2 = fopen("Arquivo2.bin","r+b");

	if (arq2 == NULL) {	//VERIFICAÇÃO NA ABERTURA DE ARQUIVO PARA LEITURA
		arq2 = fopen("Arquivo2.bin","w+b"); //Criação do arquivo         
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

void buscaCachorro(int cod) {
	FILE *arqBusca;
	struct ap2Struct aux;
	int pos,tamArq;

	arqBusca = fopen("Arquivo2.bin","r");
	if (arqBusca == NULL) {	//VERIFICAÇÃO NA ABERTURA DE ARQUIVO PARA LEITURA
		printf("Arquivo de cachorros nao existe!!!");         
	}	//Fim do primeiro if 

	else {
		pos = (cod)*52; //Código do cachorro multiplicado pelo tamanho de cada registro

		fseek(arqBusca,0,2);
		tamArq = (int) ftell(arqBusca);

		if (pos<=tamArq) {
			fseek(arqBusca,pos,0);
			fread(&aux, sizeof(ap2), 1, arqBusca);
			printf("\nInformacoes do cachorro:");                                         
			printf("\nNome: %s",aux.nomeCachorro);                       
			printf("\nRaca: %s\n",aux.raca);                     
		} //Fim do segundo if

		else {
			printf("Codigo invalido!!!");
		} //Fim do segundo else
	} //Fim do primeiro else
}
