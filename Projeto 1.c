//	Projeto 1 - Estrutura de Dados II
//	Nome: Bruno Fouz Valente

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

//==========================================================
//==========================MAIN============================
//==========================================================

int main() {

	struct ap1Struct {
		char* nomeVacina, dataVacina, respAplic;
		int codControle, codCachorro;
	} ap1;

	struct ap2Struct {
		char raca[20], nomeCachorro[30];
		int codCachorro;
        struct ap2Struct *prox;
	} ap2;

	menu();

	return 0;	
}

//==========================================================
//========================FUNCTIONS=========================
//==========================================================

void menu()
{
     int opt;

	 printf("(1) Cadastrar nova vacina\n");
	 printf("(2) Alterar dados de vacina existente\n");
	 printf("(3) Remover dados de vacina\n\n");
	 printf("(4) Cadastrar novo cachorro\n");
	 printf("(5) Alterar dados de cachorro existente\n");
	 printf("(6) Remover dados de cachorro\n\n");
	 printf("Escolha uma opcao: ");

	 while(1)
		switch(opt)
        {
		case 1:
			cadVacina();
		case 2:
			altVacina();
		case 3:
			remVacina();
		case 4:
			cadCachorro();
		case 5:
			altCachorro();
		case 6:
			remCachorro();
		case 'q':
			break;
        }
}

void cadVacina() {

	FILE *arq1;
	struct ap1Sruct aux;
	int opt;

	arq1 = fopen("Arquivo1","r+");
	if ( arq1 == NULL ) //VERIFICAÇÃO NA ABERTURA DE ARQUIVO PARA LEITURA 
		arq1 = fopen("Arquivo1","w+"); //Criação do arquivo

		printf("CADASTRO DE NOVA VACINACAO\n");

         do
         {
           printf("\nO animal já é cadastrado?(1=Sim 0=Nao)");
           scanf("%d",&opt);
         }while ((opt != 1)||(opt!=0));
     
         switch (opt)
         {
                case 0:
                     cadCachorro();
                     break;
                case 1:
                     printf("\nQual o codigo do cachorro? ");
                     scanf("%d",&aux.codCachorro);

                     //buscaCachorro();

                     printf("\nQual o nome da vacina? ");
                     scanf("%c",&aux.nomeVacina);
                     printf("\nQual a data da vacinacao? ");
                     scanf("%c",&aux.dataVacina);
                     printf("\nQual o nome do responsavel pela aplicacao? ");
                     scanf("%c",&aux.respAplic);
                     break;
         } 
}

/*
void altVacina()
{
     FILE *arq1;
     
     arq1 = fopen("Arquivo1","r+");
     if ( arq1 == NULL ) //VERIFICAÇÃO NA ABERTURA DE ARQUIVO PARA LEITURA
     {
          printf("ARQUIVO DE ENTRADA INEXISTENTE!!!");
     }
     
}*/
/*
void remVacina()
{
     printf("REMOCAO DE VACINACAO\n\n");
     printf("Qual o codigo da vacinacao?\n");
}*/

void cadCachorro() {

	FILE *arq2;
     	struct ap2Struct aux,aux2;
     	int opt;

	arq2 = fopen("Arquivo1","r+");

	if (arq2 == NULL) //VERIFICAÇÃO NA ABERTURA DE ARQUIVO PARA LEITURA 
		arq2 = fopen("Arquivo1","w+"); //Criação do arquivo

	while (1) {
		printf("CADASTRO DE CACHORRO\n");
     
     		printf("\nQual a raca do cachorro? ");
     		scanf("%s",&aux.nomeCachorro);
     		printf("\nQual o nome do cachorro? ");
     		scanf("%s",&aux.raca);
     
     		printf("Confirma? (1=Sim 0=Nao)");
     		scanf("%d",&opt);
     		switch (opt){
                  	case 0:
                       		break;
                	case 1:
                       		fwrite(&aux.nomeCachorro, sizeof(aux.nomeCachorro), 1, arq2);
                       		fwrite(&aux.raca, sizeof(aux.raca), 1, arq2);
                       		fseek(arq2,0,0);
		                fread(&aux2.nomeCachorro, sizeof(aux.nomeCachorro), 1 ,arq2);
                       		fread(&aux2.raca, sizeof(aux.raca), 1 ,arq2); 
                       		printf("Raca: %s\n",aux2.raca);                        
                       		printf("Nome: %s\n",aux2.nomeCachorro); 
                       		break;
                  }
}
