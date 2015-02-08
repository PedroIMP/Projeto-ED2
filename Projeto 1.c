//////////////////////////////////////////////////////////////////////
//                                                                  //
//  Estrutura de Dados II                                           //
//  Projeto 1                                                       //
//                                                                  //
//  Alunos: Bruno Fouz Valente e Pedro Ivo Monteiro Privatto        //
//                                                                  //
//////////////////////////////////////////////////////////////////////

// Includes

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

// Declarações de estruturas e descritores de arquivos globais

struct ap1Struct {
	int codControle, codCachorro;
	char nomeVacina[40];
	char dataVacina[15];
	char respAplic[45];
}; // Estrutura para o registro do arquivo de vacinas (AP1)

struct ap2Struct {
	int codCachorro;
	char raca[20];
	char nomeCachorro[30];
}; // Estrutura para o registro (tamanho fixo) do arquivo de cachorros (AP2)

struct indice1Struct {
	int codControle, offset1, offset2;
}; // Estrutura do índice principal (Indice1)

struct indice2Struct {
	int offset2;
	char nomeVacina[40];
};

struct indice1Struct indice1[5000];
struct indice2Struct indice2[5000];
int numVacina, numCachorro, numindice1, numindice2;

FILE *arqVacinas; // Arquivo principal 1 (AP1)
FILE *arqVacinasCompactada; // Arquivo principal 1 (AP1) temporário para o processo de compactação
FILE *arqCachorros; // Arquivo principal 2 (AP2)
FILE *arqIndice1; // Arquivo do índice principal (Indice1)
FILE *arqIndice2a; // Arquivo do índice secundário A (Indice2a)
FILE *arqIndice2b; // Arquivo do índice secundário B (Indice2b)

// Protótipos

void menu();
void criarVetores();
void abrirArquivos();
void cadastrarCachorro();
void cadastrarVacina();
void alterarVacina();
void excluirVacina();
void compactarDadosVacina();
void consultarVacina();
void invalidarRegistro();
void reescreverOffset();
void completarIndice();
void completarListaIndice();
void ordenarIndices();
void adicionarIndice();
void trocarOffset();
void removerDoIndice2();
void salvarIndice1();
void salvarIndice2ab();
void reescreverVacina();
void atualizarIndice2();
void lerIndice2();
int gerarCodigo();
int obterOffset();
int procurarCachorro();
int buscarVacina();
int calcularTamanhoRegistro();

// Function principal (main) do programa

int main() {
	criarVetores();
	abrirArquivos();

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

// Function do menu principal

void menu() {
	int codigo;
	bool fim = false;
	do {
		char opcao;

		system("cls");
		printf("///////////////////////////////////////////////////\n");
		printf("//        Programa de Cadastro de Vacinas        //\n");
		printf("///////////////////////////////////////////////////\n\n");
		printf("[1] Cadastrar novo cachorro\n");
		printf("[2] Cadastrar nova vacina\n");
		printf("[3] Alterar dados de vacina existente\n");
		printf("[4] Remover dados de vacina\n");
		printf("[5] Consulta de vacina\n");
		printf("[6] Compactar arquivo de vacinas (AP1)\n");
		printf("[7] Sair\n\n");
		printf("Escolha uma opcao: ");

		fflush(stdin);
		scanf("%c", &opcao);

		switch(opcao) {
			case '1':
				cadastrarCachorro();
				break;
			case '2':
				cadastrarVacina();
				break;
			case '3':
				alterarVacina();
				break;
			case '4':
				printf("Digite o codigo da vacina a ser removida: ");

				fflush(stdin);
				scanf("%d", &codigo);

				fseek(arqIndice1, 0, 0);
				fwrite("*", 1, 1, arqIndice1);
				excluirVacina(codigo);

				break;
			case '5':
				consultarVacina();
				break;
			case '6':
				compactarDadosVacina();
				printf("Arquivo de vacinas (AP1) compactado com sucesso!\n\n");
				system("pause");
				break;
			case '7':
				printf("Fechando o programa...\n");
				fim = true;
				break;
			default:
				printf("Opcao invalida!\n\n");
				system("pause");
				menu();
				break;
		}
	} while (!fim);
}

// Function para ordenar (sort) os índices secundários (Indice2a e Indice2b)

void ordenarIndices() {
	int contador, contador2, temp;
	char strTemp[40];

	contador = 0;

	while (contador <= numindice2) {
		contador2 = 0;
		while (contador2 <= numindice2) {
			if (strcmp(indice2[contador].nomeVacina,indice2[contador2].nomeVacina) < 0) {
				strcpy(strTemp,indice2[contador].nomeVacina);
				strcpy(indice2[contador].nomeVacina,indice2[contador2].nomeVacina);
				strcpy(indice2[contador2].nomeVacina,strTemp);
				temp = indice2[contador].offset2;
				indice2[contador].offset2 = indice2[contador2].offset2;
				indice2[contador2].offset2 = temp;
			}
			contador2++;
		}
		contador++;
	}
}

// Function para abertura e verificação dos arquivos

void abrirArquivos() {
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
					completarListaIndice();
					ordenarIndices();
				} else {
					char indicador;
					fread(&indicador, 1, 1, arqIndice1);
					if (&indicador == "!") {
						arqIndice1 = fopen("Indice1.dat", "w+b");
						arqIndice2a = fopen("Indice2a.dat", "r+b");
						arqIndice2b = fopen("Indice2b.dat", "r+b");
						completarListaIndice();
						ordenarIndices();
					} else {
						completarIndice();
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

// Function para gerar o código da vacina quando realizado o cadastramento

int gerarCodigo() {
    int codigo;

    fseek(arqVacinas, 0, 0);
    fread(&codigo, sizeof(int), 1, arqVacinas);
    codigo++;
    fseek(arqVacinas, -sizeof(int), 1);
    fwrite(&codigo, sizeof(int), 1, arqVacinas);

    return codigo;
}

// Function para obter o offset

int obterOffset(int tam) {
    int regTam, offset;
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
                offset = offset1;
                fread(&offset1, sizeof(int), 1, arqVacinas);
                fseek(arqVacinas, offsetAux + 5, 0);
                fwrite(&offset1, sizeof(int), 1, arqVacinas);
                 
                return offset;
            }

            fseek(arqVacinas, 1, 1);
            offsetAux = offset1;
            fread(&offset1, sizeof(int), 1, arqVacinas);
        } while (offset1 != -1);
    }

    return offset1;
}

void adicionarIndice(struct ap1Struct aux, int offset1) {
    numindice1++;
    indice1[numindice1].codControle = aux.codControle;
    indice1[numindice1].offset1 = offset1;
    indice1[numindice1].offset2 = -1;

    int contador = 0;
    bool achou = false;

    while ( (contador <= numindice2) && (!achou) ) {
        if (strcmp(indice2[contador].nomeVacina, aux.nomeVacina) == 0) {
            achou = true;
            if (indice2[contador].offset2 == -1) {
                indice2[numindice2].offset2 = numindice1;
            } else {
                int aux = 0;
                int offset2 = indice2[contador].offset2;
                do {
                    aux = offset2;
                    offset2 = indice1[offset2].offset2;
                } while (offset2 != -1) ;
                indice1[aux].offset2 = numindice1;
            }
        } else {
            contador++;
		}
    }

    if (!achou) {
        numindice2++;
        strcpy(indice2[numindice2].nomeVacina, aux.nomeVacina);
        indice2[numindice2].offset2 = numindice1;
    }
}

void trocarOffset(int codigo, int offset1) {
    bool achou = false;
    int contador = 0;

    while ((contador <= numindice1) && (!achou)) {
        if (indice1[contador].codControle == codigo) {
            achou = true;
            invalidarRegistro(indice1[contador].offset1);
            reescreverOffset(indice1[contador].offset1);
            indice1[contador].offset1 = offset1;
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
        adicionarIndice(aux,offset1);
        ordenarIndices();
    } else {
        trocarOffset(aux.codControle,offset1);
	}
}

int procurarCachorro(int codigo) {
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

void cadastrarVacina() {
    struct ap1Struct temporario;
	struct ap2Struct tempCachorro;
    char buffer[105];

    temporario.codControle = gerarCodigo();

    do {
	   	system("cls");
        printf("Cadastro de Vacina\n\n");
        printf("Digite o codigo do cachorro: ");
        scanf("%d", &temporario.codCachorro);

        if (procurarCachorro(temporario.codCachorro) == -1) {
            printf("\nCachorro nao encontrado! ");
            bool sair = false;
            do {
                char opcao;

				printf("Deseja cadastrar um novo cachorro? (1=Sim, 0=Nao):  ");

                fflush(stdin);
                scanf("%c", &opcao);

                switch (opcao) {
                    case '1':
                        cadastrarCachorro();
                        sair = true;
                        break;
                    case '0':
                        sair = true;
                        break;
					default:
						printf("Opcao invalida!\n\n");
						system("pause");
						break;
                }
            } while (!sair);
        }
    } while (procurarCachorro(temporario.codCachorro) == -1);

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
    system("pause");
}

void cadastrarCachorro() {
    struct ap2Struct temporario;

	system("cls");
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
    system("pause");
}

void reescreverOffset(int newOffset) {
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

void invalidarRegistro(int pos) {
    fseek(arqVacinas, (pos + 4), 0);
    fwrite("!", sizeof(char), 1, arqVacinas);
}

void removerDoIndice2(int pos, int codigo) {
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
        if (strcmp(indice2[contador].nomeVacina, nome) == 0) {
            achou = true;
            int offset1 = indice2[contador].offset2;
            if (indice1[offset1].codControle == codigo) {
                    indice2[contador].offset2 = indice1[offset1].offset2;
            } else {
                int offsetAnt;
                do {
                    offsetAnt = offset1;
                    offset1 = indice1[offset1].offset2;
                    if (indice1[offset1].codControle == codigo) {
                        achou2 = true;
                        indice1[offsetAnt].offset2 = indice1[offset1].offset2;
                    }
                } while (!achou2);
            }
        } else {
            contador++;
		}
    }
}

void excluirVacina(int codigo) {
    bool achou = false;
    int contador = 0;

    while ((contador <= numindice1) && (!achou)) {
        if (indice1[contador].codControle == codigo) {
            achou = true;
            removerDoIndice2(indice1[contador].offset1, codigo);
            invalidarRegistro(indice1[contador].offset1);
            reescreverOffset(indice1[contador].offset1);
            indice1[contador].codControle = -1;
        }
        contador++;
    }

    if (!achou) {
        printf("Nao ha nenhuma vacina de codigo %d cadastrada!\n", codigo);
    } else {
        printf("Vacina referente ao codigo %d removida!\n", codigo);
	}
	system("pause");
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

int buscarVacina(int codigo) {
    int contador = 0;

    while (contador <= numindice1) {
        if (indice1[contador].codControle == codigo) {
            return indice1[contador].offset1;
        }
        contador++;
    }

    return -1;
}

int calcularTamanhoRegistro(struct ap1Struct temporario) {
    char buffer[105];

    sprintf(buffer, "*%d|%d|%s|%s|%s|", temporario.codControle, temporario.codCachorro, &temporario.nomeVacina, &temporario.dataVacina, &temporario.respAplic);

    return strlen(buffer);
}

void reescreverVacina(struct ap1Struct temporario, int offset1) {
    char buffer[105];
    sprintf(buffer, "*%d|%d|%s|%s|%s|", temporario.codControle, temporario.codCachorro, &temporario.nomeVacina, &temporario.dataVacina, &temporario.respAplic);
    int tamanho = strlen(buffer);

    fseek(arqVacinas, offset1 + sizeof(int), 0);
    fwrite(buffer, sizeof(char), strlen(buffer), arqVacinas);
}

void atualizarIndice2(char *nomeAnt, char *nome, int codigo) {
    int contador = 0;
	int ref;
    bool achou = false;
    bool achou2 = false;

    while (!achou) {
        if (strcmp(indice2[contador].nomeVacina, nomeAnt) == 0) {
            achou = true;
            int offset1 = indice2[contador].offset2;
            if (indice1[offset1].codControle == codigo) {
                    indice2[contador].offset2 = indice1[offset1].offset2;
                    ref = offset1;
            } else {
                int offsetAnt;
                do {
                    offsetAnt = offset1;
                    offset1 = indice1[offset1].offset2;
                    if (indice1[offset1].codControle == codigo) {
                        achou2 = true;
                        indice1[offsetAnt].offset2 = indice1[offset1].offset2;
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

    while ( (contador <= numindice2) && (!achou) ){
        if (strcmp(indice2[contador].nomeVacina, nome) == 0) {
            achou = true;
            if (indice2[contador].offset2 == -1) {
                indice2[numindice2].offset2 = ref;
            } else {
                int temporario = 0;
                int offset2 = indice2[contador].offset2;
                do {
                    temporario = offset2;
                    offset2 = indice1[offset2].offset2;
                } while (offset2 != -1) ;
                indice1[temporario].offset2 = ref;
            }
        } else {
            contador++;
		}
    }

    if (!achou) {
        numindice2++;
        strcpy(indice2[numindice2].nomeVacina, nome);
        indice2[numindice2].offset2 = ref;
    }

    indice1[ref].offset2 = -1;
}

void alterarVacina() {
    int codigo, posAlteracao, tam;
	char opcao;
    struct ap1Struct temporario;

	system("cls");
    printf("Alterar Dados de Vacina\n\n");

    do {
        printf("Digite o codigo da vacina: ");
        scanf("%d", &codigo);
        posAlteracao = buscarVacina(codigo);
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

	fflush(stdin);
    scanf("%c", &opcao);

    temporario = encontraRegistro(posAlteracao);
    char nomeAnt[40];
    fseek(arqIndice1, 0, 0);
    fwrite("!", 1, 1, arqIndice1);

    switch (opcao) {
        case '1':
                printf("\nCodigo do cachorro: ");
                scanf("%d", &temporario.codCachorro);
                break;
        case '2':
                strcpy(nomeAnt, temporario.nomeVacina);
                printf("\nNome da vacina: ");
                scanf("%s", &temporario.nomeVacina);
                atualizarIndice2(nomeAnt, temporario.nomeVacina, temporario.codControle);
                ordenarIndices();
                break;
        case '3':
                printf("\nData da vacinacao: ");
                scanf("%s", &temporario.dataVacina);
                break;
        case '4':
                printf("\nResponsavel pela aplicacao: ");
                scanf("%s", &temporario.respAplic);
                break;
		default:
				printf("Opcao invalida!\n\n");
				system("pause");
				break;
    }

    fseek(arqVacinas, posAlteracao, 0);
    fread(&tam, sizeof(int), 1, arqVacinas);

    if ((calcularTamanhoRegistro(temporario)) <= (tam)) {
        reescreverVacina(temporario, posAlteracao);
    } else {
        adicionarVacina(temporario, true);
    }

    system("pause");
}

void consultarVacina() {
	int codProcurado;
	int RRN = 0, tam, offsetAP1, soma;
	bool achou = 0;
	char str[100];

	system("cls");
	printf("Digite o codigo da vacina a ser procurada: ");
	scanf("%d", &codProcurado);

	while ((indice1[RRN].codControle != -1) && (!achou)) {
		if (indice1[RRN].codControle == codProcurado) {
			achou = true;
			offsetAP1 = indice1[RRN].offset1;
			printf("\nOffset do registro: %d\n",offsetAP1);
			fseek(arqVacinas, offsetAP1, 0);
			fread(&tam, sizeof(int), 1, arqVacinas);

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

	system("pause");
}

void criarVetores() {
    numindice1 = -1;
    numindice2 = -1;

    int i;

    for (i = 0; i <= 5000; i++) {
        indice1[i].codControle = -1;
    }
}


void salvarIndice1() {
    fseek(arqIndice1, 0, 0);
    fwrite("*", 1, 1, arqIndice1);

    int contador = 0;

    while (contador <= numindice1) {
        if (indice1[contador].codControle != -1) {
                fwrite(&indice1[contador].codControle, sizeof(int), 1, arqIndice1);
                fwrite(&indice1[contador].offset1, sizeof(int), 1, arqIndice1);
        }
        contador++;
    } 
}

void salvarIndice2ab() {
    fseek(arqIndice2a, 0, 0);
    fseek(arqIndice2b, 0, 0);

    int contador = 0;

    struct indice2Struct temporario;
    while (contador <= numindice2) {
        strcpy(temporario.nomeVacina, indice2[contador].nomeVacina);
        temporario.offset2 =  indice2[contador].offset2;
        fwrite(&temporario, sizeof(temporario), 1, arqIndice2a);
        int offset1 = indice2[contador].offset2;
        while (offset1 != -1) {
            fwrite(&indice1[offset1].codControle, sizeof(int), 1, arqIndice2b);
            fwrite(&indice1[offset1].offset2, sizeof(int), 1, arqIndice2b);
            offset1 = indice1[offset1].offset2;
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

void completarIndice() {
    fseek(arqIndice2a, 0, 2);
    int tamArq = ftell(arqIndice2a) / 44;
    fseek(arqIndice2a, 0, 0);

    struct indice2Struct temporario;

    int codigo, offset2;
    int contador = 0;

    while (contador < tamArq) {
        fread(&temporario, sizeof(temporario), 1, arqIndice2a);
        numindice2++;
        strcpy(indice2[numindice2].nomeVacina, temporario.nomeVacina);
        indice2[numindice2].offset2 = temporario.offset2;
        int offset1 = temporario.offset2;
        while (offset1 != -1) {
            fread(&codigo, sizeof(int), 1, arqIndice2b);
            fread(&offset2, sizeof(int), 1, arqIndice2b);
            indice1[offset1].codControle = codigo;
            indice1[offset1].offset1 = 777;
            indice1[offset1].offset2 = offset2;
            if (offset1 > numindice1) {
                numindice1 = offset1;
			}
            offset1 = offset2;
        }
        contador++;
    }

    fseek(arqIndice1, 1, 0);
    contador = 0;

    int offset1;

    while (contador <= numindice1) {
        if (indice1[contador].codControle != -1) {
                fseek(arqIndice1, 4, 1);
                fread(&offset1, sizeof(int), 1, arqIndice1);
                indice1[contador].offset1 = offset1;
        }
        contador++;
    }
}

void completarListaIndice() {
    int pos = 8;
    int tam = 0;
    char indicador;
    char str[100];
    int codigo = 0;
    int ref, soma, contador;
    bool achou;
    numindice1 = -1;
    numindice2 = -1;

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

            numindice1++;
            indice1[numindice1].codControle = codigo;
            indice1[numindice1].offset1 = pos;
            indice1[numindice1].offset2 = -1;

            fseek(arqVacinas, ref + soma, 0);
            fread(&str, tam - 1 - soma , 1, arqVacinas);
            strtok(str, "|");
            soma += strlen(str) + 1;
            fseek(arqVacinas, ref + soma, 0);
            fread(&str, tam - 1 - soma , 1, arqVacinas);
            strtok(str, "|");

            achou = false;
            contador = 0;

            while ((contador <= numindice2) && (!achou)) {
                if (strcmp(indice2[contador].nomeVacina, str) == 0) {
                    achou = true;
                    if (indice2[contador].offset2 == -1) {
                        indice2[numindice2].offset2 = numindice1;
                    } else {
                        int temporario = 0;
                        int offset2 = indice2[contador].offset2;
                    	do {
                        	temporario = offset2;
                        	offset2 = indice1[offset2].offset2;
                    	} while (offset2 != -1) ;
                    	indice1[temporario].offset2 = numindice1;
                    }
                } else {
                    contador++;
				}
            }

            if (!achou) {
                numindice2++;
                strcpy(indice2[numindice2].nomeVacina, str);
                indice2[numindice2].offset2 = numindice1;
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

    completarListaIndice();
    ordenarIndices();

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
