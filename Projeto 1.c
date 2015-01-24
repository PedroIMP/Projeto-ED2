//	Projeto 1 - Estrutura de Dados II
//	Nome: Bruno Fouz Valente

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

         struct ap1Struct {
		        char nomeVacina[20], dataVacina[9], respAplic[10];
		        int codControle, codCachorro;
         } ap1;

	     struct ap2Struct {
		        char raca[15], nomeCachorro[30];
		        int codCachorro;
                //struct ap2Struct *prox;
         } ap2;

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

	

	
	menu();

printf("\n");	
system("pause");	
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
	 printf("(3) Remover dados de vacina\n");
	 printf("(4) Cadastrar novo cachorro\n");
	 //printf("(5) Alterar dados de cachorro existente\n");
	 //printf("(6) Remover dados de cachorro\n\n");
	 printf("Escolha uma opcao: ");
	 scanf("%d",&opt);

		switch(opt)
        {
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
        }

}

void cadVacina()
{
     FILE *arq1;
     struct ap1Struct aux,aux2;
     int opt;
     
     arq1 = fopen("Arquivo1.txt","r+");
     if ( arq1 == NULL ) //VERIFICAÇÃO NA ABERTURA DE ARQUIVO PARA LEITURA 
        arq1 = fopen("Arquivo1.txt","w+"); //Criação do arquivo
     
         printf("CADASTRO DE NOVA VACINACAO\n");
         
         //do
         //{
         //  printf("\nO animal já é cadastrado?(1=Sim 0=Nao)");
         //  scanf("%d",&opt);
         //}while ((opt != 1)||(opt!=0));*/
     
        // switch (opt)
        // {
               // case 0:
                     //cadCachorro();
                    // break;
               // case 1:
                     printf("\nQual o codigo do cachorro? ");
                     scanf("%d",&aux.codCachorro); 
                                                             
                     printf("\nQual o nome da vacina? ");
                     fflush(stdin);                    
                     scanf("%s",aux.nomeVacina);                     
                     
                     printf("\nQual a data da vacinacao? ");
                     fflush(stdin);
                     scanf("%s",&aux.dataVacina);
                     printf("\nQual o nome do responsavel pela aplicacao? ");
                     fflush(stdin);
                     scanf("%s",aux.respAplic);
                     
                     int tamReg = (int)(log10(aux.codControle)+1) + (int)(log10(aux.codCachorro)+1) +           //Soma dos tamanhos dos campos do registro
                     strlen(aux.nomeVacina) + strlen(aux.dataVacina) + strlen(aux.respAplic + (6));             //6 caracteres especiais(| e *)
                     
                     fprintf(arq1, "%d*%d|%d|%s|%s|%s|",&tamReg, &aux.codControle, &aux.codCachorro,
                     &aux.nomeVacina, &aux.dataVacina, &aux.respAplic);
                     
                     fclose(arq1);
                     
                     //break;
         //} 
}


void altVacina()
{
     FILE *arq1;
     
     arq1 = fopen("Arquivo1","r+b");
     if ( arq1 == NULL ) //VERIFICAÇÃO NA ABERTURA DE ARQUIVO PARA LEITURA
     {
          printf("ARQUIVO DE ENTRADA INEXISTENTE!!!");
     }
     
}

void remVacina()
{
     printf("REMOCAO DE VACINACAO\n\n");
     printf("Qual o codigo da vacinacao?\n");
}

void cadCachorro()
{
     FILE *arq2;
     struct ap2Struct aux,aux2;
     int opt;
     
     
     arq2 = fopen("Arquivo2.bin","r+b");
     if ( arq2 == NULL ) //VERIFICAÇÃO NA ABERTURA DE ARQUIVO PARA LEITURA 
        {
          arq2 = fopen("Arquivo2.bin","w+b"); //Criação do arquivo         
        }
        fseek(arq2,0,2);
        
        
     printf("\nCADASTRO DE CACHORRO\n");
     
     aux.codCachorro = ftell(arq2)/sizeof(ap2);
     printf("\nCodigo do cachorro: %d",aux.codCachorro);
     printf("\nQual a raca do cachorro? ");
     scanf("%s",&aux.raca);
     printf("\nQual o nome do cachorro? ");
     scanf("%s",&aux.nomeCachorro);
     
     printf("Confirma? (1=Sim 0=Nao) ");
     scanf("%d",&opt);
     switch (opt){
                  case 0:
                       break;
                  case 1:                                             
                       fwrite(&aux, sizeof(ap2), 1, arq2);            //Salva os campos do registro aux no arquivo
                       fseek(arq2, -sizeof(ap2),2);                   //Posiciona o arquivo 1 registro para trás
                       fread(&aux2, sizeof(ap2), 1 ,arq2);            //Recupera o ultimo registro para aux2
                       printf("\nCodigo do cachorro: %d",aux2.codCachorro);                                         
                       printf("\nNome: %s",aux2.nomeCachorro);                       
                       printf("\nRaca: %s",aux2.raca);                                  
                       break;
                  }
     fclose(arq2);
}
