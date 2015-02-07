////////////////////////////////////////////////////////////////////
//                                                                //
//  Estrutura de Dados II                                         //
//  Projeto 1                                                     //
//                                                                //
//  Alunos: Bruno Fouz Valente e Pedro Ivo Monteiro Privatto      //
//                                                                //
////////////////////////////////////////////////////////////////////

// Includes

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

FILE *dadosCachorro;
FILE *dadosVacina;
FILE *dadosVacinaCompact;
FILE *arqIndice1;
FILE *arqIndice2a;
FILE *arqIndice2b;

struct indice1Struct index1[10000];
struct indice2Struct index2[10000];
int numVacina, numCachorro, numIndex1, numIndex2;

//	ProtÃ³tipos

void menu();
void menuCadastro();
void menuRemocao();
void alteraVacina();
void menuConsulta();
void setInativo();
void setOffset();
void compactarDadosVacina();
void carregarIndice();
void carregarListaIndice();
void cadastroCachorro();
void inicializaVetores();
void inicializaArquivos();
void imprimirIndice1();
void imprimirIndice2();
void cadastroVacina();
void ordenaIndices();
int obterCodigo();
int obterOffset();
void adicionaIndice();
void mudaOffset();
int buscaCachorro();
void removeDoIndice2();
void removeVacina();
void salvarIndice1();
void salvarIndice2ab();

//	Function principal (main) do programa

int main() {
	inicializaVetores();
	inicializaArquivos();
	menu();

	// DEBUG ////
	imprimirIndice1();

	printf("\n");

	imprimirIndice2();

	salvarIndice1();
	salvarIndice2ab();
	fclose(dadosCachorro);
	fclose(dadosVacina);
	fclose(arqIndice1);
	fclose(arqIndice2a);
	fclose(arqIndice2b);
	return 0;
}

void menu() {
	bool fim = false;
	do {
		char opcao;

		printf("Cadastro de vacinas\n\n");
		printf("[1] Cadastrar nova vacina\n");
		printf("[2] Alterar dados de vacina existente\n");
		printf("[3] Remover dados de vacina\n");
		printf("[4] Cadastrar novo cachorro\n");
		printf("[5] Remover dados de cachorro\n");
		printf("[6] Consulta\n");
		printf("[7] Compactar AP1\n");
		printf("[8] Sair\n\n");
		printf("Escolha uma opcao: ");

		scanf("%s", &opcao);
		getchar();

		switch(opcao) {
			case '1':
				cadastroVacina();
				break;
			case '2':
				alteraVacina();
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
				menuConsulta();
				break;
			case '7':
				compactarDadosVacina();
				printf("Arquivo de vacinas compactado com sucesso!\n");
				break;
			case '8':
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

void ordenaIndices() {
    int cont, cont2, temp;
    char strTemp[40];

    cont = 0;

    while (cont <= numIndex2) {
        cont2 = 0;
        while (cont2 <= numIndex2) {
            if (strcmp(index2[cont].nomeVacina,index2[cont2].nomeVacina) < 0) {
                strcpy(strTemp,index2[cont].nomeVacina);
                strcpy(index2[cont].nomeVacina,index2[cont2].nomeVacina);
                strcpy(index2[cont2].nomeVacina,strTemp);
                temp = index2[cont].offset2;
                index2[cont].offset2 = index2[cont2].offset2;
                index2[cont2].offset2 = temp;
            }
            cont2++;
        }
        cont++;
    }
}

void inicializaArquivos() {
    dadosCachorro = fopen("AP2.dat", "r+b");
    if (dadosCachorro == NULL) {
        dadosCachorro = fopen("AP2.dat", "w+b");
        dadosVacina = fopen("AP1.dat", "w+b");
        int n = -1;
        fwrite(&n,sizeof(int),1,dadosVacina);
        fwrite(&n,sizeof(int),1,dadosVacina);
        arqIndice1 = fopen("Indice1.dat", "w+b");
        arqIndice2a = fopen("Indice2a.dat", "w+b");
        arqIndice2b = fopen("Indice2b.dat", "w+b");
    } else {
        dadosVacina = fopen("AP1.dat", "r+b");
        if (dadosVacina == NULL) {
            dadosVacina = fopen("AP1.dat", "w+b");
            int n = -1;
            fwrite(&n,sizeof(int),1,dadosVacina);
            fwrite(&n,sizeof(int),1,dadosVacina);
            arqIndice1 = fopen("Indice1.dat", "w+b");
            arqIndice2a = fopen("Indice2a.dat", "w+b");
            arqIndice2b = fopen("Indice2b.dat", "w+b");
        } else {
			fseek(dadosVacina,0,2);
			if ( (ftell(dadosVacina)) > 4) {
				arqIndice1 = fopen("Indice1.dat", "r+b");
				arqIndice2a = fopen("Indice2a.dat", "r+b");
				arqIndice2b = fopen("Indice2b.dat", "r+b");
				if ( (arqIndice1 == NULL) || (arqIndice2a == NULL) || (arqIndice2b == NULL)) {
					arqIndice1 = fopen("Indice1.dat", "w+b");
					arqIndice2a = fopen("Indice2a.dat", "w+b");
					arqIndice2b = fopen("Indice2b.dat", "w+b");
					carregarListaIndice();
					ordenaIndices();
				} else {
					char flag;
					fread(&flag,1,1,arqIndice1);
					if (&flag == "!") {
						arqIndice1 = fopen("Indice1.dat", "w+b");
						arqIndice2a = fopen("Indice2a.dat", "r+b");
						arqIndice2b = fopen("Indice2b.dat", "r+b");
						carregarListaIndice();
						ordenaIndices();
					} else {
						carregarIndice();
					}
				}
			} else {
				dadosVacina = fopen("AP1.dat", "w+b");
				arqIndice1 = fopen("Indice1.dat", "w+b");
				arqIndice2a = fopen("Indice2a.dat", "w+b");
				arqIndice2b = fopen("Indice2b.dat", "w+b");
			}

			arqIndice2a = fopen("Indice2a.dat", "r+b");
			arqIndice2b = fopen("Indice2b.dat", "r+b");
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
    int regTam, offsetReturn;
    int offset1 = 4;
    int offsetAux = -1;

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

void adicionaIndice(struct ap1Struct aux, int offset1) {
    numIndex1++;
    index1[numIndex1].codControle = aux.codControle;
    index1[numIndex1].offset1 = offset1;
    index1[numIndex1].offset2 = -1;

    int cont = 0;
    bool achou = false;

    while ( (cont <= numIndex2) && (!achou) ) {
        if (strcmp(index2[cont].nomeVacina,aux.nomeVacina) == 0) {
            achou = true;
            if (index2[cont].offset2 == -1) {
                index2[numIndex2].offset2 = numIndex1;
            } else {
                int aux = 0;
                int offset2 = index2[cont].offset2;
                do {
                    aux = offset2;
                    offset2 = index1[offset2].offset2;
                } while (offset2 != -1) ;
                index1[aux].offset2 = numIndex1;
            }
        } else {
            cont++;
		}
    }

    if (!achou) {
        numIndex2++;
        strcpy(index2[numIndex2].nomeVacina,aux.nomeVacina);
        index2[numIndex2].offset2 = numIndex1;
    }
}

void mudaOffset(int codigo, int offset1) {
    bool achou = false;
    int cont = 0;

    while ((cont <= numIndex1) && (!achou)) {
        if (index1[cont].codControle == codigo) {
            achou = true;
            setInativo(index1[cont].offset1);
            setOffset(index1[cont].offset1);
            index1[cont].offset1 = offset1;
        }
        cont++;
    }
}

void adicionarVacina(struct ap1Struct aux, bool atualizando) {
    char buffer[105];
    sprintf(buffer,"*%d|%d|%s|%s|%s|", aux.codControle, aux.codCachorro, &aux.nomeVacina,&aux.dataVacina,&aux.respAplic);
    int tamanho = strlen(buffer);
    int offset1 = obterOffset(tamanho);

    if (offset1 == -1) {
            fseek(dadosVacina,0,2);
            offset1 = ftell(dadosVacina);
            fwrite(&tamanho, sizeof(int), 1, dadosVacina);
    } else {
        fseek(dadosVacina,offset1 + sizeof(int),0);
    }

    fwrite(buffer, sizeof(char), strlen(buffer), dadosVacina);
    printf("\n%d\n", tamanho);

    if (!atualizando) {
        adicionaIndice(aux,offset1);
        ordenaIndices();
    } else {
        mudaOffset(aux.codControle,offset1);
	}
}

int buscaCachorro(int codigo) {
    fseek(dadosCachorro,0,2);
    int tamArq = ftell(dadosCachorro)/12;
    fseek(dadosCachorro,0,0);

    struct ap2Struct aux;
    int cont = 0;
    bool achou = false;

    while ( (cont < tamArq) && (!achou) ) {
        fread(&aux.codCachorro,sizeof(aux),1,dadosCachorro);
        if (aux.codCachorro == codigo)  {
			achou = true;
        }
        cont++;
    }

    if (achou) {
        return aux.codCachorro;
    } else {
        return -1;
	}
}

void cadastroVacina() {
    struct ap1Struct temporario;
    struct ap1Struct temporario2;
    char buffer[105];

    temporario.codControle = obterCodigo();
    do {
        printf("Menu de Cadastro de Vacinas\n\n");
        printf("Codigo da vacina: %d\n\n", temporario.codControle);
        printf("Codigo Cachorro: ");
        scanf("%d", &temporario.codCachorro);
        if (buscaCachorro(temporario.codCachorro) == -1) {
            printf("\nCachorro nao encontrado!\n\n");
            bool sair = false;
            do {
                char opcao;
                fflush(stdin);
                printf("Deseja cadastrar um cachorro? (1=sim, 0=nao, m=menu:  ");
                scanf("%c", &opcao);
                switch (opcao) {
                    case '1':
                        cadastroCachorro();
                        sair = true;
                        break;
                    case '0':
                        sair = true;
                        break;
					default:
						printf("Opcao invalida!");
						break;
                }
            } while (!sair);
            fflush(stdin);
        }
    } while (buscaCachorro(temporario.codCachorro) == -1);
    printf("\nNome Vacina: ");
    scanf("%s", &temporario.nomeVacina);
    printf("\nData: ");
    scanf("%s", &temporario.dataVacina);
    printf("\nResponsavel pela Aplicacao: ");
    scanf("%s", &temporario.respAplic);
    fseek(arqIndice1,0,0);
    fwrite("!",1,1,arqIndice1);
    adicionarVacina(temporario, false);
}

void cadastroCachorro() {
    struct ap2Struct temporario;
    struct ap2Struct temporario2;
    printf("Menu de Cadastro de Cachorros\n\n");
    fseek(dadosCachorro,0,2);
    temporario.codCachorro = ftell(dadosCachorro)/sizeof(struct ap2Struct);
    printf("%d\n", temporario.codCachorro);
    printf("Nome: ");
    scanf("%s", temporario.nomeCachorro);
    printf("\nRaca: ");
    scanf("%s", temporario.raca);
    fseek(dadosCachorro,0,2);
    fwrite(&temporario, sizeof(struct ap2Struct), 1, dadosCachorro);
    fseek(dadosCachorro, -sizeof(struct ap2Struct),2);
    fread(&temporario2, sizeof(struct ap2Struct), 1, dadosCachorro);
    printf("Codigo: %d\n",temporario2.codCachorro);
    printf("Raca: %s\n",temporario2.raca);
    printf("Nome: %s\n",temporario2.nomeCachorro);
}

void setOffset(int newOffset) {
    bool fim = false;
    int offset1;
    int offsettemporario = -1;

    fseek(dadosVacina,sizeof(int),0);
    fread(&offset1,sizeof(int),1,dadosVacina);
    while (!(fim)) {
        if ((offset1 == -1) || (newOffset < offset1)) {
            fim = true;
            fseek(dadosVacina,(offsettemporario + sizeof(int) + sizeof(char)),0);
            fwrite(&newOffset,sizeof(int),1,dadosVacina);
            fseek(dadosVacina,(newOffset + sizeof(int) + sizeof(char)),0);
            fwrite(&offset1,sizeof(int),1,dadosVacina);
            printf("%d\n", newOffset);
             
        }
        fseek(dadosVacina,(offset1 + sizeof(int) + sizeof(char)),0);
        offsettemporario = offset1;
        fread(&offset1,sizeof(int),1,dadosVacina);
    }
}

void setInativo(int pos) {
    fseek(dadosVacina,(pos + 4),0);
    fwrite("!", sizeof(char), 1, dadosVacina);
}

void removeDoIndice2(int pos, int codigo) {
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

    int cont = 0;
    bool achou = false;
    bool achou2 = false;
    while (!achou) {
        if (strcmp(index2[cont].nomeVacina, nome) == 0) {
            achou = true;
            int offset1 = index2[cont].offset2;
            if (index1[offset1].codControle == codigo) {
                    index2[cont].offset2 = index1[offset1].offset2;
            } else {
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
        } else {
            cont++;
		}
    }
}

void removeVacina(int codigo) {
    bool achou = false;
    int cont = 0;

    while ((cont <= numIndex1) && (!achou)) {
        if (index1[cont].codControle == codigo) {
            achou = true;
            removeDoIndice2(index1[cont].offset1, codigo);
            setInativo(index1[cont].offset1);
            setOffset(index1[cont].offset1);
            index1[cont].codControle = -1;
        }
        cont++;
    }

    if (!achou) {
        printf("Nao ha nenhuma vacina de codigo %d cadastrada!\n", codigo);
    } else {
        printf("Vacina %d removida com sucesso!\n", codigo);
	}
}

void menuRemocao() {
    int codigo;

    printf("Menu de Remocao de Vacinas\n\n");
    printf("Digite o codigo da vacina a ser removida: (-1 para voltar)\n");
    scanf("%d", &codigo);
    if (codigo != -1) {
        fseek(arqIndice1,0,0);
        fwrite("*",1,1,arqIndice1);
        removeVacina(codigo);
    }
}

struct ap1Struct encontraRegistro(int pos) {
    struct ap1Struct temporario;
    int tam, tam2, temp, soma;
    char str[40];

    fseek(dadosVacina,pos,0);
    fread(&tam,sizeof(int),1,dadosVacina);
    fseek(dadosVacina,sizeof(char),1);

    fread(&str,tam-1,1,dadosVacina);
    strtok(str, "|");
    soma = strlen(str) + 1;
    temp = atoi(str);
    temporario.codControle = temp;

    fseek(dadosVacina,pos+4+1+soma,0);
    fread(&str,tam-soma-1,1,dadosVacina);
    strtok(str, "|");
    soma += strlen(str) + 1;
    temp = atoi(str);
    temporario.codCachorro = temp;

    fseek(dadosVacina,pos+4+1+soma,0);
    fread(&str,tam-soma-1,1,dadosVacina);
    strtok(str, "|");
    soma += strlen(str) + 1;
    strcpy(temporario.nomeVacina,str);

    fseek(dadosVacina,pos+4+1+soma,0);
    fread(&str,tam-soma-1,1,dadosVacina);
    strtok(str, "|");
    soma += strlen(str) + 1;
    strcpy(temporario.dataVacina,str);

    fseek(dadosVacina,pos+4+1+soma,0);
    fread(&str,tam-soma-1,1,dadosVacina);
    strtok(str, "|");
    soma += strlen(str) + 1;
    strcpy(temporario.respAplic,str);

    return temporario;
}

int buscaVacina(int codigo) {
    int cont = 0;

    while (cont <= numIndex1) {
        if (index1[cont].codControle == codigo) {
            return index1[cont].offset1;
        }
        cont++;
    }
    return -1;
}

int tamanhoRegistro(struct ap1Struct temporario) {
    char buffer[105];
    sprintf(buffer,"*%d|%d|%s|%s|%s|", temporario.codControle, temporario.codCachorro, &temporario.nomeVacina,&temporario.dataVacina,&temporario.respAplic);
    return strlen(buffer);
}

void reescreveVacina(struct ap1Struct temporario, int offset1) {
    char buffer[105];
    sprintf(buffer,"*%d|%d|%s|%s|%s|", temporario.codControle, temporario.codCachorro, &temporario.nomeVacina,&temporario.dataVacina,&temporario.respAplic);
    int tamanho = strlen(buffer);
    fseek(dadosVacina,offset1 + sizeof(int),0);
    fwrite(buffer, sizeof(char), strlen(buffer), dadosVacina);
}

void atualizaIndex2(char *nomeAnt, char *nome, int codigo) {
    int cont = 0;
	int ref;
    bool achou = false;
    bool achou2 = false;

    while (!achou) {
        if (strcmp(index2[cont].nomeVacina, nomeAnt) == 0) {
            achou = true;
            int offset1 = index2[cont].offset2;
            if (index1[offset1].codControle == codigo) {
                    index2[cont].offset2 = index1[offset1].offset2;
                    ref = offset1;
            } else {
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
        } else {
            cont++;
		}
    }

    cont = 0;
    achou = false;
    while ( (cont <= numIndex2) && (!achou) ){
        if (strcmp(index2[cont].nomeVacina, nome) == 0) {
            achou = true;
            if (index2[cont].offset2 == -1) {
                index2[numIndex2].offset2 = ref;
            } else {
                int temporario = 0;
                int offset2 = index2[cont].offset2;
                do {
                    temporario = offset2;
                    offset2 = index1[offset2].offset2;
                } while (offset2 != -1) ;
                index1[temporario].offset2 = ref;
            }
        } else {
            cont++;
		}
    }

    if (!achou) {
        numIndex2++;
        strcpy(index2[numIndex2].nomeVacina,nome);
        index2[numIndex2].offset2 = ref;
    }

    index1[ref].offset2 = -1;
}

void alteraVacina() {
    int menu, codigo, posAlteracao, tam;

    struct ap1Struct temporario;
    printf("Menu de Alteracao de dados de Vacinacao\n\n");
    do {
        printf("Digite o codigo da vacina a ter dados alterado: \n");
        scanf("%d", &codigo);
        posAlteracao = buscaVacina(codigo);
        if (posAlteracao == -1) {
            printf("\nNao ha uma vacina cadastrada disponivel com esse codigo!\n\n");
		}

    } while (posAlteracao == -1);
     
    printf("Menu de Alteracao de dados de Vacinacao\n\n");
    printf("1. Codigo do Cachorro\n");
    printf("2. Nome da Vacina\n");
    printf("3. Data da Vacinacao\n");
    printf("4. Responsavel pela Aplicacao\n");
    printf("Escolha um campo para alterar: ");
    scanf("%d", &menu);
    temporario = encontraRegistro(posAlteracao);
    char nomeAnt[40];
    fseek(arqIndice1,0,0);
    fwrite("!",1,1,arqIndice1);
    switch (menu) {
        case 1:
                printf("\nCodigo do Cachorro: ");
                scanf("%d", &temporario.codCachorro);
                break;
        case 2:
                strcpy(nomeAnt,temporario.nomeVacina);
                printf("\nNome Vacina: ");
                scanf("%s", &temporario.nomeVacina);
                atualizaIndex2(nomeAnt, temporario.nomeVacina, temporario.codControle);
                ordenaIndices();
                break;
        case 3:
                printf("\nData: ");
                scanf("%s", &temporario.dataVacina);
                break;
        case 4:
                printf("\nResponsavel pela Aplicacao: ");
                scanf("%s", &temporario.respAplic);
                break;
    }
    fseek(dadosVacina,posAlteracao,0);
    fread(&tam,sizeof(int),1,dadosVacina);

    if ((tamanhoRegistro(temporario)) <= (tam)) {
        reescreveVacina(temporario,posAlteracao);
    } else {
        adicionarVacina(temporario, true);
    }

    printf("\nCodigo Controle: %d\n", temporario.codControle);
    printf("Codigo Cachorro: %d\n", temporario.codCachorro);
    printf("Vacina: %s\n", temporario.nomeVacina);
    printf("Data: %s\n", temporario.dataVacina);
    printf("Responsavel pela Aplicacao: %s\n", temporario.respAplic);   
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

void imprimirIndice1() {
    int cont = 0;

    while (cont <= numIndex1) {
        if (index1[cont].codControle != -1) {
                printf("%d %d \n", index1[cont].codControle, index1[cont].offset1);
		}
        cont++;
    }
}

void imprimirIndice2() {
    int cont = 0;

    while (cont <= numIndex2) {
        printf("%s \n", index2[cont].nomeVacina);
        int offset1 = index2[cont].offset2;
        while (offset1 != -1) {
            printf("        %d %d \n", index1[offset1].codControle, index1[offset1].offset1);
            offset1 = index1[offset1].offset2;
        }
        cont++;
    }
}

void salvarIndice1() {
    fseek(arqIndice1,0,0);
    fwrite("*",1,1,arqIndice1);

    int cont = 0;

    while (cont <= numIndex1) {
        if (index1[cont].codControle != -1) {
                fwrite(&index1[cont].codControle,sizeof(int),1,arqIndice1);
                fwrite(&index1[cont].offset1,sizeof(int),1,arqIndice1);
        }
        cont++;
    } 
}

void salvarIndice2ab() {
    fseek(arqIndice2a,0,0);
    fseek(arqIndice2b,0,0);

    int cont = 0;

    struct indice2Struct temporario;
    while (cont <= numIndex2) {
        strcpy(temporario.nomeVacina,index2[cont].nomeVacina);
        temporario.offset2 =  index2[cont].offset2;
        fwrite(&temporario,sizeof(temporario),1,arqIndice2a);
        int offset1 = index2[cont].offset2;
        while (offset1 != -1) {
            fwrite(&index1[offset1].codControle,sizeof(int),1,arqIndice2b);
            fwrite(&index1[offset1].offset2,sizeof(int),1,arqIndice2b);
            offset1 = index1[offset1].offset2;
        }
        cont++;
    }
}

void lerIndex2() {
    fseek(arqIndice2a,0,2);
    int tamArq = ftell(arqIndice2a)/44;
    fseek(arqIndice2a,0,0);
    struct indice2Struct temporario;

    int cont = 0;

    while (cont < tamArq) {
        fread(&temporario,sizeof(temporario),1,arqIndice2a);
         
        cont++;
    }

    fseek(arqIndice2b,0,2);
    tamArq = ftell(arqIndice2b)/8;
    fseek(arqIndice2b,0,0);

    cont = 0;
    int codigo, offset1;
    while (cont < tamArq) {
        fread(&codigo,sizeof(int),1,arqIndice2b);
        fread(&offset1,sizeof(int),1,arqIndice2b);
        cont++;
    }

}

void carregarIndice() {
    fseek(arqIndice2a,0,2);
    int tamArq = ftell(arqIndice2a)/44;
    fseek(arqIndice2a,0,0);

    struct indice2Struct temporario;

    int codigo, offset2;
    int cont = 0;

    while (cont < tamArq) {
        fread(&temporario,sizeof(temporario),1,arqIndice2a);
        numIndex2++;
        strcpy(index2[numIndex2].nomeVacina, temporario.nomeVacina);
        index2[numIndex2].offset2 = temporario.offset2;
        int offset1 = temporario.offset2;
        while (offset1 != -1) {
            fread(&codigo,sizeof(int),1,arqIndice2b);
            fread(&offset2,sizeof(int),1,arqIndice2b);
            index1[offset1].codControle = codigo;
            index1[offset1].offset1 = 777;
            index1[offset1].offset2 = offset2;
            if (offset1 > numIndex1) {
                numIndex1 = offset1;
			}
            offset1 = offset2;
        }
        cont++;
    }

    fseek(arqIndice1,1,0);
    cont = 0;

    int offset1;

    while (cont <= numIndex1) {
        if (index1[cont].codControle != -1) {
                fseek(arqIndice1,4,1);
                fread(&offset1,sizeof(int),1,arqIndice1);
                index1[cont].offset1 = offset1;
        }
        cont++;
    }
}

void carregarListaIndice() {
    int pos = 8;
    int tam = 0;
    char flag;
    char str[100];
    int codigo = 0;
    int ref, soma, cont;
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
            cont = 0;
            while ((cont <= numIndex2) && (!achou)) {
                if (strcmp(index2[cont].nomeVacina,str) == 0) {
                    achou = true;
                    if (index2[cont].offset2 == -1) {
                        index2[numIndex2].offset2 = numIndex1;
                    } else {
                        int temporario = 0;
                        int offset2 = index2[cont].offset2;
                    	do {
                        	temporario = offset2;
                        	offset2 = index1[offset2].offset2;
                    	} while (offset2 != -1) ;
                    	index1[temporario].offset2 = numIndex1;
                    }
                } else {
                    cont++;
				}
            }

            if (!achou) {
                numIndex2++;
                strcpy(index2[numIndex2].nomeVacina,str);
                index2[numIndex2].offset2 = numIndex1;
            }
        } else {
            fseek(dadosVacina,(tam-sizeof(char)),1);
        }
        pos += tam + sizeof(int);
    } while (pos < tamArq);
}

void compactarDadosVacina() {
    dadosVacinaCompact = fopen("AP1-compactado.dat", "w+b");

    fseek(arqIndice1,0,0);
    fwrite("!",1,1,arqIndice1);

    struct ap1Struct temporario;
    int pos = 8;
    int tam = 0;
    char flag;
    char str[50];
    int soma;
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
            soma = strlen(str) + 1;
            fwrite(&str,strlen(str),1,dadosVacinaCompact);

            fseek(dadosVacina,pos+4+1+soma,0);
            fread(&str,tam-soma-1,1,dadosVacina);
            strtok(str,"|");
            soma += strlen(str) + 1;
            fwrite("|",1,1,dadosVacinaCompact);
            fwrite(&str,strlen(str),1,dadosVacinaCompact);

            fseek(dadosVacina,pos+4+1+soma,0);
            fread(&str,tam-soma-1,1,dadosVacina);
            strtok(str,"|");
            soma += strlen(str) + 1;
            fwrite("|",1,1,dadosVacinaCompact);
            fwrite(&str,strlen(str),1,dadosVacinaCompact);

            fseek(dadosVacina,pos+4+1+soma,0);
            fread(&str,tam-soma-1,1,dadosVacina);
            strtok(str,"|");
            soma += strlen(str) + 1;
            fwrite("|",1,1,dadosVacinaCompact);
            fwrite(&str,strlen(str),1,dadosVacinaCompact);

            fseek(dadosVacina,pos+4+1+soma,0);
            fread(&str,tam-soma-1,1,dadosVacina);
            strtok(str,"|");
            soma += strlen(str) + 1;
            fwrite("|",1,1,dadosVacinaCompact);
            fwrite(&str,strlen(str),1,dadosVacinaCompact);
            fwrite("|",1,1,dadosVacinaCompact);
            soma++;
            if (soma < tam) {
                fseek(dadosVacinaCompact,pos,0);
                fwrite(&soma,sizeof(int),1,dadosVacinaCompact);
                fseek(dadosVacinaCompact,0,2);
            }
        } else {
            fseek(dadosVacina,(tam-sizeof(char)),1);
        }

        pos += tam + sizeof(int);

    } while (pos < tamArq);

    fclose(dadosVacinaCompact);
    fclose(dadosVacina);
    remove("AP1.dat");
    rename("AP1-compactado.dat","AP1.dat");
    dadosVacina = fopen("AP1.dat", "r+b");

    carregarListaIndice();
    ordenaIndices();
    fclose(arqIndice1);
    fclose(arqIndice2a);
    fclose(arqIndice2b);
    arqIndice1 = fopen("Indice1.dat", "w+b");
    arqIndice2a = fopen("Indice2a.dat", "w+b");
    arqIndice2b = fopen("Indice2b.dat", "w+b");
    salvarIndice1();
    salvarIndice2ab();

    fseek(arqIndice1,0,0);
    fwrite("*",1,1,arqIndice1);
}
