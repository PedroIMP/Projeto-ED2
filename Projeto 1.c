//////////////////////////////////////////////////////////////////////
//                                                                  //
//  Estrutura de Dados II                                           //
//  Projeto 1                                                       //
//                                                                  //
//  Alunos: Bruno Fouz Valente e Pedro Ivo Monteiro Privatto        //
//                                                                  //
//////////////////////////////////////////////////////////////////////

//	Includes

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

//	DeclaraÃ§Ã£o de estruturas e descritores de arquivos globais

struct ap1Struct {
	int codControle, codCachorro;
	char nomeVacina[40];
	char dataVacina[15];
	char respAplic[45];
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

struct indice1Struct index1[5000];
struct indice2Struct index2[5000];
int numVacina, numCachorro, numIndex1, numIndex2;

FILE *arqCachorros;
FILE *arqVacinas;
FILE *arqVacinasCompactada;
FILE *arqIndice1;
FILE *arqIndice2a;
FILE *arqIndice2b;

//	ProtÃ³tipos

void menu();
void consultaVacina();
void alteraVacina();
void setInativo();
void setOffset();
void compactarDadosVacina();
void carregarIndice();
void carregarListaIndice();
void cadastroCachorro();
void inicializaVetores();
void inicializaArquivos();
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

	salvarIndice1();
	salvarIndice2ab();

	fclose(arqCachorros);
	fclose(arqVacinas);
	fclose(arqIndice1);
	fclose(arqIndice2a);
	fclose(arqIndice2b);

	return 0;
}

//	Function do menu principal

void menu() {
	int codigo;
	bool fim = false;
	do {
		char opcao;

		printf("///////////////////////////////////////////////\n");
		printf("//      Programa de Cadastro de Vacinas      //\n");
		printf("///////////////////////////////////////////////\n\n");
		printf("[1] Cadastrar novo cachorro\n");
		printf("[2] Cadastrar nova vacina\n");
		printf("[3] Alterar dados de vacina existente\n");
		printf("[4] Remover dados de vacina\n");
		printf("[5] Consulta de vacina\n");
		printf("[6] Compactar arquivo de vacinas (AP1)\n");
		printf("[7] Sair\n\n");
		printf("Escolha uma opcao: ");

		scanf("%s", &opcao);
		getchar();

		switch(opcao) {
			case '1':
				cadastroCachorro();
				break;
			case '2':
				cadastroVacina();
				break;
			case '3':
				alteraVacina();
				break;
			case '4':
				printf("Digite o codigo da vacina a ser removida: (-1 para voltar)\n");
				scanf("%d", &codigo);

				if (codigo != -1) {
					fseek(arqIndice1, 0, 0);
					fwrite("*", 1, 1, arqIndice1);
					removeVacina(codigo);
				}

				break;
			case '5':
				consultaVacina();
				break;
			case '6':
				compactarDadosVacina();
				printf("Arquivo de vacinas (AP1) compactado com sucesso!\n");
				break;
			case '7':
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
	int contador, contador2, temp;
	char strTemp[40];

	contador = 0;

	while (contador <= numIndex2) {
		contador2 = 0;
		while (contador2 <= numIndex2) {
			if (strcmp(index2[contador].nomeVacina,index2[contador2].nomeVacina) < 0) {
				strcpy(strTemp,index2[contador].nomeVacina);
				strcpy(index2[contador].nomeVacina,index2[contador2].nomeVacina);
				strcpy(index2[contador2].nomeVacina,strTemp);
				temp = index2[contador].offset2;
				index2[contador].offset2 = index2[contador2].offset2;
				index2[contador2].offset2 = temp;
			}
			contador2++;
		}
		contador++;
	}
}

void inicializaArquivos() {
	arqCachorros = fopen("AP2.dat", "r+b");
	if (arqCachorros == NULL) {
		arqCachorros = fopen("AP2.dat", "w+b");
		arqVacinas = fopen("AP1.dat", "w+b");
		int n = -1;
		fwrite(&n, sizeof(int), 1, arqVacinas);
		fwrite(&n, sizeof(int), 1, arqVacinas);
		arqIndice1 = fopen("Indice1.dat", "w+b");
		arqIndice2a = fopen("Indice2a.dat", "w+b");
		arqIndice2b = fopen("Indice2b.dat", "w+b");
	} else {
		arqVacinas = fopen("AP1.dat", "r+b");
		if (arqVacinas == NULL) {
			arqVacinas = fopen("AP1.dat", "w+b");
			int n = -1;
			fwrite(&n, sizeof(int), 1, arqVacinas);
			fwrite(&n, sizeof(int), 1, arqVacinas);
			arqIndice1 = fopen("Indice1.dat", "w+b");
			arqIndice2a = fopen("Indice2a.dat", "w+b");
			arqIndice2b = fopen("Indice2b.dat", "w+b");
		} else {
			fseek(arqVacinas,0,2);
			if ( (ftell(arqVacinas)) > 4) {
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
					char indicador;
					fread(&indicador, 1, 1, arqIndice1);
					if (&indicador == "!") {
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
				arqVacinas = fopen("AP1.dat", "w+b");
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

    fseek(arqVacinas, 0, 0);
    fread(&codigo, sizeof(int), 1, arqVacinas);
    codigo++;
    fseek(arqVacinas, -sizeof(int), 1);
    fwrite(&codigo, sizeof(int), 1, arqVacinas);

    return codigo;
}

int obterOffset(int tam) {
    int regTam, offsetReturn;
    int offset1 = 4;
    int offsetAux = -1;

    fseek(arqVacinas, offset1, 0);
    fread(&offset1, sizeof(int), 1, arqVacinas);

    if (offset1 != -1) {
        do {
            fseek(arqVacinas, offset1, 0);
            fread(&regTam, sizeof(int), 1, arqVacinas);
            if (tam <= regTam) {
                fseek(arqVacinas, 1, 1);
                offsetReturn = offset1;
                fread(&offset1, sizeof(int), 1, arqVacinas);
                fseek(arqVacinas, offsetAux + 5, 0);
                fwrite(&offset1, sizeof(int), 1, arqVacinas);
                 
                return offsetReturn;
            }
            fseek(arqVacinas, 1, 1);
            offsetAux = offset1;
            fread(&offset1, sizeof(int), 1, arqVacinas);
        } while (offset1 != -1);
    }

    return offset1;
}

void adicionaIndice(struct ap1Struct aux, int offset1) {
    numIndex1++;
    index1[numIndex1].codControle = aux.codControle;
    index1[numIndex1].offset1 = offset1;
    index1[numIndex1].offset2 = -1;

    int contador = 0;
    bool achou = false;

    while ( (contador <= numIndex2) && (!achou) ) {
        if (strcmp(index2[contador].nomeVacina, aux.nomeVacina) == 0) {
            achou = true;
            if (index2[contador].offset2 == -1) {
                index2[numIndex2].offset2 = numIndex1;
            } else {
                int aux = 0;
                int offset2 = index2[contador].offset2;
                do {
                    aux = offset2;
                    offset2 = index1[offset2].offset2;
                } while (offset2 != -1) ;
                index1[aux].offset2 = numIndex1;
            }
        } else {
            contador++;
		}
    }

    if (!achou) {
        numIndex2++;
        strcpy(index2[numIndex2].nomeVacina, aux.nomeVacina);
        index2[numIndex2].offset2 = numIndex1;
    }
}

void mudaOffset(int codigo, int offset1) {
    bool achou = false;
    int contador = 0;

    while ((contador <= numIndex1) && (!achou)) {
        if (index1[contador].codControle == codigo) {
            achou = true;
            setInativo(index1[contador].offset1);
            setOffset(index1[contador].offset1);
            index1[contador].offset1 = offset1;
        }
        contador++;
    }
}

void adicionarVacina(struct ap1Struct aux, bool atualizando) {
    char buffer[105];
    sprintf(buffer, "*%d|%d|%s|%s|%s|", aux.codControle, aux.codCachorro, &aux.nomeVacina, &aux.dataVacina, &aux.respAplic);
    int tamanho = strlen(buffer);
    int offset1 = obterOffset(tamanho);

    if (offset1 == -1) {
            fseek(arqVacinas, 0, 2);
            offset1 = ftell(arqVacinas);
            fwrite(&tamanho, sizeof(int), 1, arqVacinas);
    } else {
        fseek(arqVacinas, offset1 + sizeof(int), 0);
    }

    fwrite(buffer, sizeof(char), strlen(buffer), arqVacinas);

    if (!atualizando) {
        adicionaIndice(aux,offset1);
        ordenaIndices();
    } else {
        mudaOffset(aux.codControle,offset1);
	}
}

int buscaCachorro(int codigo) {
    fseek(arqCachorros, 0, 2);
    int tamArq = ftell(arqCachorros) / 12;
    fseek(arqCachorros, 0, 0);

    struct ap2Struct aux;
    int contador = 0;
    bool achou = false;

    while ( (contador < tamArq) && (!achou) ) {
        fread(&aux.codCachorro, sizeof(aux), 1, arqCachorros);
        if (aux.codCachorro == codigo)  {
			achou = true;
        }
        contador++;
    }

    if (achou) {
        return aux.codCachorro;
    } else {
        return -1;
	}
}

void cadastroVacina() {
    struct ap1Struct temporario;
	struct ap2Struct tempCachorro;
    char buffer[105];

    temporario.codControle = obterCodigo();

    do {
        printf("Cadastro de Vacina\n\n");
        printf("Digite o codigo do cachorro: ");
        scanf("%d", &temporario.codCachorro);
        if (buscaCachorro(temporario.codCachorro) == -1) {
            printf("\nCachorro nao encontrado! ");
            bool sair = false;
            do {
                char opcao;
                fflush(stdin);
                printf("Deseja cadastrar um novo cachorro? (1=Sim, 0=Nao):  ");
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

    printf("Nome da vacina: ");
    scanf("%s", &temporario.nomeVacina);
    printf("Data da vacinacao: ");
    scanf("%s", &temporario.dataVacina);
    printf("Responsavel pela aplicacao: ");
    scanf("%s", &temporario.respAplic);
    fseek(arqIndice1, 0, 0);
    fwrite("!", 1, 1, arqIndice1);
    adicionarVacina(temporario, false);

	printf("\n");
	printf("Dados da vacina:\n");
	printf("    Codigo da vacina: %d\n", temporario.codControle);
	printf("    Nome da vacina: %s\n", temporario.nomeVacina);
	printf("    Data da vacinacao: %s\n", temporario.dataVacina);
	printf("    Responsavel pela aplicacao: %s\n\n", temporario.respAplic);

	fseek(arqCachorros, ((temporario.codCachorro) * sizeof(tempCachorro)), 0);
	fread(&tempCachorro, sizeof(tempCachorro), 1, arqCachorros);

	printf("Dados do cachorro que recebera a vacina:\n");
	printf("    Codigo do cachorro: %d\n", tempCachorro.codCachorro);
	printf("    Nome do cachorro: %s\n", tempCachorro.nomeCachorro);
    printf("    Raca: %s\n\n", tempCachorro.raca);
}

void cadastroCachorro() {
    struct ap2Struct temporario;

    printf("Cadastro de Cachorro\n\n");
    fseek(arqCachorros, 0, 2);
    temporario.codCachorro = ftell(arqCachorros) / sizeof(struct ap2Struct);

    printf("Nome do cachorro: ");
    scanf("%s", temporario.nomeCachorro);
	printf("Raca: ");
    scanf("%s", temporario.raca);

    fseek(arqCachorros, 0, 2);
    fwrite(&temporario, sizeof(struct ap2Struct), 1, arqCachorros);

	printf("\n");
	printf("Dados do cachorro:\n");
    printf("    Codigo do cachorro: %d\n", temporario.codCachorro);
	printf("    Nome do cachorro: %s\n", temporario.nomeCachorro);
    printf("    Raca: %s\n\n", temporario.raca);
}

void setOffset(int newOffset) {
    bool fim = false;
    int offset1;
    int offsettemporario = -1;

    fseek(arqVacinas, sizeof(int), 0);
    fread(&offset1, sizeof(int), 1, arqVacinas);
    while (!(fim)) {
        if ((offset1 == -1) || (newOffset < offset1)) {
            fim = true;
            fseek(arqVacinas, (offsettemporario + sizeof(int) + sizeof(char)), 0);
            fwrite(&newOffset, sizeof(int), 1, arqVacinas);
            fseek(arqVacinas, (newOffset + sizeof(int) + sizeof(char)), 0);
            fwrite(&offset1, sizeof(int), 1, arqVacinas);
        }
        fseek(arqVacinas, (offset1 + sizeof(int) + sizeof(char)), 0);
        offsettemporario = offset1;
        fread(&offset1, sizeof(int), 1, arqVacinas);
    }
}

void setInativo(int pos) {
    fseek(arqVacinas, (pos + 4), 0);
    fwrite("!", sizeof(char), 1, arqVacinas);
}

void removeDoIndice2(int pos, int codigo) {
    char nome[100];
    int tam;
    int soma = 0;

    fseek(arqVacinas, pos, 0);
    fread(&tam, sizeof(int), 1, arqVacinas);
    fseek(arqVacinas, 1, 1);
    fread(&nome, tam - 1, 1, arqVacinas);
    strtok(nome, "|");
    soma += strlen(nome) + 1;
    fseek(arqVacinas, pos + 5 + soma, 0);
    fread(&nome, tam - 1 - soma, 1, arqVacinas);
    strtok(nome, "|");
    soma += strlen(nome) + 1;
    fseek(arqVacinas, pos + 5 + soma, 0);
    fread(&nome, tam - 1 - soma, 1, arqVacinas);
    strtok(nome, "|");

    int contador = 0;
    bool achou = false;
    bool achou2 = false;
    while (!achou) {
        if (strcmp(index2[contador].nomeVacina, nome) == 0) {
            achou = true;
            int offset1 = index2[contador].offset2;
            if (index1[offset1].codControle == codigo) {
                    index2[contador].offset2 = index1[offset1].offset2;
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
            contador++;
		}
    }
}

void removeVacina(int codigo) {
    bool achou = false;
    int contador = 0;

    while ((contador <= numIndex1) && (!achou)) {
        if (index1[contador].codControle == codigo) {
            achou = true;
            removeDoIndice2(index1[contador].offset1, codigo);
            setInativo(index1[contador].offset1);
            setOffset(index1[contador].offset1);
            index1[contador].codControle = -1;
        }
        contador++;
    }

    if (!achou) {
        printf("Nao ha nenhuma vacina de codigo %d cadastrada!\n", codigo);
    } else {
        printf("Vacina referente ao codigo %d removida!\n", codigo);
	}
}

struct ap1Struct encontraRegistro(int pos) {
    struct ap1Struct temporario;
    int tam, tam2, temp, soma;
    char str[40];

    fseek(arqVacinas, pos, 0);
    fread(&tam, sizeof(int), 1, arqVacinas);
    fseek(arqVacinas, sizeof(char), 1);

    fread(&str, tam - 1, 1, arqVacinas);
    strtok(str, "|");
    soma = strlen(str) + 1;
    temp = atoi(str);
    temporario.codControle = temp;

    fseek(arqVacinas, pos + 5 + soma, 0);
    fread(&str, tam - soma - 1, 1, arqVacinas);
    strtok(str, "|");
    soma += strlen(str) + 1;
    temp = atoi(str);
    temporario.codCachorro = temp;

    fseek(arqVacinas, pos + 5 + soma, 0);
    fread(&str, tam - soma - 1, 1, arqVacinas);
    strtok(str, "|");
    soma += strlen(str) + 1;
    strcpy(temporario.nomeVacina, str);

    fseek(arqVacinas, pos + 5 + soma, 0);
    fread(&str, tam - soma - 1, 1, arqVacinas);
    strtok(str, "|");
    soma += strlen(str) + 1;
    strcpy(temporario.dataVacina, str);

    fseek(arqVacinas, pos + 5 + soma, 0);
    fread(&str, tam - soma - 1, 1, arqVacinas);
    strtok(str, "|");
    soma += strlen(str) + 1;
    strcpy(temporario.respAplic, str);

    return temporario;
}

int buscaVacina(int codigo) {
    int contador = 0;

    while (contador <= numIndex1) {
        if (index1[contador].codControle == codigo) {
            return index1[contador].offset1;
        }
        contador++;
    }
    return -1;
}

int tamanhoRegistro(struct ap1Struct temporario) {
    char buffer[105];
    sprintf(buffer, "*%d|%d|%s|%s|%s|", temporario.codControle, temporario.codCachorro, &temporario.nomeVacina, &temporario.dataVacina, &temporario.respAplic);
    return strlen(buffer);
}

void reescreveVacina(struct ap1Struct temporario, int offset1) {
    char buffer[105];
    sprintf(buffer, "*%d|%d|%s|%s|%s|", temporario.codControle, temporario.codCachorro, &temporario.nomeVacina, &temporario.dataVacina, &temporario.respAplic);
    int tamanho = strlen(buffer);
    fseek(arqVacinas, offset1 + sizeof(int), 0);
    fwrite(buffer, sizeof(char), strlen(buffer), arqVacinas);
}

void atualizaIndex2(char *nomeAnt, char *nome, int codigo) {
    int contador = 0;
	int ref;
    bool achou = false;
    bool achou2 = false;

    while (!achou) {
        if (strcmp(index2[contador].nomeVacina, nomeAnt) == 0) {
            achou = true;
            int offset1 = index2[contador].offset2;
            if (index1[offset1].codControle == codigo) {
                    index2[contador].offset2 = index1[offset1].offset2;
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
            contador++;
		}
    }

    contador = 0;
    achou = false;
    while ( (contador <= numIndex2) && (!achou) ){
        if (strcmp(index2[contador].nomeVacina, nome) == 0) {
            achou = true;
            if (index2[contador].offset2 == -1) {
                index2[numIndex2].offset2 = ref;
            } else {
                int temporario = 0;
                int offset2 = index2[contador].offset2;
                do {
                    temporario = offset2;
                    offset2 = index1[offset2].offset2;
                } while (offset2 != -1) ;
                index1[temporario].offset2 = ref;
            }
        } else {
            contador++;
		}
    }

    if (!achou) {
        numIndex2++;
        strcpy(index2[numIndex2].nomeVacina, nome);
        index2[numIndex2].offset2 = ref;
    }

    index1[ref].offset2 = -1;
}

void alteraVacina() {
    int menu, codigo, posAlteracao, tam;
    struct ap1Struct temporario;

    printf("Alterar Dados de Vacina\n\n");
    do {
        printf("Digite o codigo da vacina: ");
        scanf("%d", &codigo);
        posAlteracao = buscaVacina(codigo);
        if (posAlteracao == -1) {
            printf("\nNao ha nenhuma vacina cadastrada com esse codigo!\n\n");
		}

    } while (posAlteracao == -1);
     
    printf("\nAlterar:\n\n");
    printf("[1] Codigo do cachorro\n");
    printf("[2] Nome da vacina\n");
    printf("[3] Data da vacinacao\n");
    printf("[4] Responsavel pela aplicacao\n\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &menu);

    temporario = encontraRegistro(posAlteracao);
    char nomeAnt[40];
    fseek(arqIndice1, 0, 0);
    fwrite("!", 1, 1, arqIndice1);

    switch (menu) {
        case 1:
                printf("\nCodigo do cachorro: ");
                scanf("%d", &temporario.codCachorro);
                break;
        case 2:
                strcpy(nomeAnt, temporario.nomeVacina);
                printf("\nNome da vacina: ");
                scanf("%s", &temporario.nomeVacina);
                atualizaIndex2(nomeAnt, temporario.nomeVacina, temporario.codControle);
                ordenaIndices();
                break;
        case 3:
                printf("\nData da vacinacao: ");
                scanf("%s", &temporario.dataVacina);
                break;
        case 4:
                printf("\nResponsavel pela aplicacao: ");
                scanf("%s", &temporario.respAplic);
                break;
    }

    fseek(arqVacinas, posAlteracao, 0);
    fread(&tam, sizeof(int), 1, arqVacinas);

    if ((tamanhoRegistro(temporario)) <= (tam)) {
        reescreveVacina(temporario, posAlteracao);
    } else {
        adicionarVacina(temporario, true);
    }  
}

void consultaVacina() {
	int codProcurado;
	int RRN = 0, tam, offsetAP1, soma;
	bool achou = 0;
	char str[100];

	printf("Digite o codigo da vacina a ser procurada: ");
	scanf("%d", &codProcurado);

	while ((index1[RRN].codControle != -1) && (!achou)) {
		if (index1[RRN].codControle == codProcurado) {
			achou = true;
			offsetAP1 = index1[RRN].offset1;
			fseek(arqVacinas, offsetAP1, 0);
			fread(&tam, sizeof(int), 1, arqVacinas);
			printf("\nO tamanho e %d\n", tam); //debug

			fseek(arqVacinas, 1, 1);
			fread(&str, tam - 1, 1, arqVacinas);
			strtok(str, "|");
			soma = strlen(str) + 1;
			printf("\nCodigo da vacina: %s", str);

			fseek(arqVacinas, offsetAP1 + 5 + soma, 0);
			fread(&str, tam - soma - 1, 1, arqVacinas);
			strtok(str, "|");
			soma += strlen(str) + 1;
			printf("\nCodigo do cachorro: %s", str);

			fseek(arqVacinas, offsetAP1 + 5 + soma, 0);
			fread(&str, tam - soma - 1, 1, arqVacinas);
			strtok(str, "|");
			soma += strlen(str) + 1;
			printf("\nNome da vacina: %s", str);

			fseek(arqVacinas, offsetAP1 + 5 + soma, 0);
			fread(&str, tam - soma - 1, 1, arqVacinas);
			strtok(str, "|");
			soma += strlen(str) + 1;
			printf("\nData da vacina: %s", str);

			fseek(arqVacinas, offsetAP1 + 5 + soma, 0);
			fread(&str, tam - soma - 1, 1, arqVacinas);
			strtok(str, "|");
			soma += strlen(str) + 1;
			printf("\nResponsavel pela aplicacao: %s\n", str);                                                    
		} else {
			RRN++;
		}
	}

	if (!achou) {
		printf("Vacina referente ao codigo %d nao encontrada.\n\n", codProcurado);
	}
}

void inicializaVetores() {
    numIndex1 = -1;
    numIndex2 = -1;

    int i;

    for (i = 0; i <= 5000; i++) {
        index1[i].codControle = -1;
    }
}

void salvarIndice1() {
    fseek(arqIndice1, 0, 0);
    fwrite("*", 1, 1, arqIndice1);

    int contador = 0;

    while (contador <= numIndex1) {
        if (index1[contador].codControle != -1) {
                fwrite(&index1[contador].codControle, sizeof(int), 1, arqIndice1);
                fwrite(&index1[contador].offset1, sizeof(int), 1, arqIndice1);
        }
        contador++;
    } 
}

void salvarIndice2ab() {
    fseek(arqIndice2a, 0, 0);
    fseek(arqIndice2b, 0, 0);

    int contador = 0;

    struct indice2Struct temporario;
    while (contador <= numIndex2) {
        strcpy(temporario.nomeVacina, index2[contador].nomeVacina);
        temporario.offset2 =  index2[contador].offset2;
        fwrite(&temporario, sizeof(temporario), 1, arqIndice2a);
        int offset1 = index2[contador].offset2;
        while (offset1 != -1) {
            fwrite(&index1[offset1].codControle, sizeof(int), 1, arqIndice2b);
            fwrite(&index1[offset1].offset2, sizeof(int), 1, arqIndice2b);
            offset1 = index1[offset1].offset2;
        }
        contador++;
    }
}

void lerIndice2() {
    fseek(arqIndice2a, 0, 2);
    int tamArq = ftell(arqIndice2a) / 44;
    fseek(arqIndice2a, 0, 0);
    struct indice2Struct temporario;

    int contador = 0;

    while (contador < tamArq) {
        fread(&temporario, sizeof(temporario), 1, arqIndice2a);
         
        contador++;
    }

    fseek(arqIndice2b, 0, 2);
    tamArq = ftell(arqIndice2b) / 8;
    fseek(arqIndice2b, 0, 0);

    contador = 0;
    int codigo, offset1;
    while (contador < tamArq) {
        fread(&codigo, sizeof(int), 1, arqIndice2b);
        fread(&offset1, sizeof(int), 1, arqIndice2b);
        contador++;
    }
}

void carregarIndice() {
    fseek(arqIndice2a, 0, 2);
    int tamArq = ftell(arqIndice2a) / 44;
    fseek(arqIndice2a, 0, 0);

    struct indice2Struct temporario;

    int codigo, offset2;
    int contador = 0;

    while (contador < tamArq) {
        fread(&temporario, sizeof(temporario), 1, arqIndice2a);
        numIndex2++;
        strcpy(index2[numIndex2].nomeVacina, temporario.nomeVacina);
        index2[numIndex2].offset2 = temporario.offset2;
        int offset1 = temporario.offset2;
        while (offset1 != -1) {
            fread(&codigo, sizeof(int), 1, arqIndice2b);
            fread(&offset2, sizeof(int), 1, arqIndice2b);
            index1[offset1].codControle = codigo;
            index1[offset1].offset1 = 777;
            index1[offset1].offset2 = offset2;
            if (offset1 > numIndex1) {
                numIndex1 = offset1;
			}
            offset1 = offset2;
        }
        contador++;
    }

    fseek(arqIndice1, 1, 0);
    contador = 0;

    int offset1;

    while (contador <= numIndex1) {
        if (index1[contador].codControle != -1) {
                fseek(arqIndice1, 4, 1);
                fread(&offset1, sizeof(int), 1, arqIndice1);
                index1[contador].offset1 = offset1;
        }
        contador++;
    }
}

void carregarListaIndice() {
    int pos = 8;
    int tam = 0;
    char indicador;
    char str[100];
    int codigo = 0;
    int ref, soma, contador;
    bool achou;
    numIndex1 = -1;
    numIndex2 = -1;

    fseek(arqVacinas, 0, 2);
    int tamArq = ftell(arqVacinas);
    fseek(arqVacinas, 8, 0);

    do {
        fread(&tam, sizeof(int), 1, arqVacinas);
        fread(&indicador, sizeof(char), 1, arqVacinas);

        if (indicador == '*') {
            ref = ftell(arqVacinas);
            fread(&str, tam - 1, 1, arqVacinas);
            strtok(str, "|");
            codigo = atoi(str);
            soma = strlen(str)+1;

            numIndex1++;
            index1[numIndex1].codControle = codigo;
            index1[numIndex1].offset1 = pos;
            index1[numIndex1].offset2 = -1;

            fseek(arqVacinas, ref + soma, 0);
            fread(&str, tam - 1 - soma , 1, arqVacinas);
            strtok(str, "|");
            soma += strlen(str) + 1;
            fseek(arqVacinas, ref + soma, 0);
            fread(&str, tam - 1 - soma , 1, arqVacinas);
            strtok(str, "|");

            achou = false;
            contador = 0;
            while ((contador <= numIndex2) && (!achou)) {
                if (strcmp(index2[contador].nomeVacina, str) == 0) {
                    achou = true;
                    if (index2[contador].offset2 == -1) {
                        index2[numIndex2].offset2 = numIndex1;
                    } else {
                        int temporario = 0;
                        int offset2 = index2[contador].offset2;
                    	do {
                        	temporario = offset2;
                        	offset2 = index1[offset2].offset2;
                    	} while (offset2 != -1) ;
                    	index1[temporario].offset2 = numIndex1;
                    }
                } else {
                    contador++;
				}
            }

            if (!achou) {
                numIndex2++;
                strcpy(index2[numIndex2].nomeVacina, str);
                index2[numIndex2].offset2 = numIndex1;
            }
        } else {
            fseek(arqVacinas, (tam - sizeof(char)), 1);
        }
        pos += tam + sizeof(int);
    } while (pos < tamArq);
}

void compactarDadosVacina() {
    arqVacinasCompactada = fopen("AP1-compactado.dat", "w+b");

    fseek(arqIndice1, 0, 0);
    fwrite("!", 1, 1, arqIndice1);

    struct ap1Struct temporario;
    int pos = 8;
    int tam = 0;
    char indicador;
    char str[50];
    int soma;
    int n;

    fseek(arqVacinas, 0, 2);
    int tamArq = ftell(arqVacinas);
    fseek(arqVacinas, 0, 0);
    fread(&n, sizeof(int), 1, arqVacinas);
    fwrite(&n, sizeof(int), 1, arqVacinasCompactada);
    n = -1;
    fwrite(&n, sizeof(int), 1, arqVacinasCompactada);
    fseek(arqVacinas, 8, 0);

    do {
        fread(&tam, sizeof(int), 1, arqVacinas);
        fread(&indicador, sizeof(char), 1, arqVacinas);

        if (indicador == '*') {
            fwrite(&tam, sizeof(int), 1, arqVacinasCompactada);
            fwrite("*", sizeof(char), 1, arqVacinasCompactada);

            fread(&str, tam - 1, 1, arqVacinas);
            strtok(str, "|");
            soma = strlen(str) + 1;
            fwrite(&str, strlen(str), 1, arqVacinasCompactada);

            fseek(arqVacinas, pos + 5 + soma, 0);
            fread(&str, tam - soma - 1, 1, arqVacinas);
            strtok(str, "|");
            soma += strlen(str) + 1;
            fwrite("|", 1, 1, arqVacinasCompactada);
            fwrite(&str, strlen(str), 1, arqVacinasCompactada);

            fseek(arqVacinas, pos + 5 + soma, 0);
            fread(&str, tam - soma - 1, 1, arqVacinas);
            strtok(str, "|");
            soma += strlen(str) + 1;
            fwrite("|", 1, 1, arqVacinasCompactada);
            fwrite(&str, strlen(str), 1, arqVacinasCompactada);

            fseek(arqVacinas, pos + 5 + soma, 0);
            fread(&str, tam - soma - 1, 1, arqVacinas);
            strtok(str, "|");
            soma += strlen(str) + 1;
            fwrite("|", 1, 1, arqVacinasCompactada);
            fwrite(&str, strlen(str), 1, arqVacinasCompactada);

            fseek(arqVacinas, pos + 5 + soma, 0);
            fread(&str, tam - soma - 1, 1, arqVacinas);
            strtok(str, "|");
            soma += strlen(str) + 1;
            fwrite("|", 1, 1, arqVacinasCompactada);
            fwrite(&str, strlen(str), 1, arqVacinasCompactada);
            fwrite("|", 1, 1, arqVacinasCompactada);
            soma++;
            if (soma < tam) {
                fseek(arqVacinasCompactada, pos, 0);
                fwrite(&soma, sizeof(int), 1, arqVacinasCompactada);
                fseek(arqVacinasCompactada, 0, 2);
            }
        } else {
            fseek(arqVacinas, (tam - sizeof(char)), 1);
        }

        pos += tam + sizeof(int);

    } while (pos < tamArq);

    fclose(arqVacinasCompactada);
    fclose(arqVacinas);

    remove("AP1.dat");
    rename("AP1-compactado.dat", "AP1.dat");
    arqVacinas = fopen("AP1.dat", "r+b");

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

    fseek(arqIndice1, 0, 0);
    fwrite("*", 1, 1, arqIndice1);
}
