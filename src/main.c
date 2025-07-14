
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REGISTROS 1000
#define NOME_ARQUIVO "triangulos.bin"
#define ARQ_ID "ids.txt"

char input_c(const char *mensagem);
void input_s(const char *mensagem, char *x, int tamanho);
int input_d(const char *mensagem);
float  input_f(const char *mensagem);
long int input_ld(const char *mensagem);
double input_lf(const char *mensagem);
long double input_Lf(const char *mensagem);

typedef struct {
    int id, lado_1, lado_2, lado_3;
    char tipo[12];
} Triangulo;

Triangulo triangulos[MAX_REGISTROS];
const long TAM_REGISTRO = sizeof(Triangulo);
long int total_triangulos = 0;

void clear() {
    #ifdef _linux_
        system("clear");
    #elif _WIN32
        system("cls");
    #endif
}

int carregar_registros() {
    FILE *arquivo = fopen(NOME_ARQUIVO, "rb");
    if (arquivo == NULL) return 0;
    total_triangulos = fread(triangulos, TAM_REGISTRO, MAX_REGISTROS, arquivo);
    fclose(arquivo);
    return 1;
}

int gerar_id() {
    unsigned int id = 1;
    FILE *arquivo = fopen(ARQ_ID, "r+");
    if (arquivo == NULL) {
        arquivo = fopen(ARQ_ID, "w");
        if (arquivo == NULL) {
            printf("\nERRO GRAVE!!!\n");
        } else {
            fprintf(arquivo, "%u", id);
            fclose(arquivo);
        }
    } else {
        fscanf(arquivo, "%u", &id);
        id++;
        rewind(arquivo);
        fprintf(arquivo, "%u", id);
        fclose(arquivo);
    }
    return id;
}

void verificar_triangulo(Triangulo *t) {
    unsigned int l1 = t->lado_1, l2 = t->lado_2, l3 = t->lado_3;
    if (l1 + l2 <= l3 || l1 + l3 <= l2 || l2 + l3 <= l1) {
        strcpy(t->tipo, "nao existe");
    } else if (l1 == l2 && l2 == l3) {
        strcpy(t->tipo, "equilatero");
    } else if (l1 == l2 || l2 == l3 || l1 == l3) {
        strcpy(t->tipo, "isosceles");
    } else {
        strcpy(t->tipo, "escaleno");
    }
}

int gravar_registro(const Triangulo *t, unsigned int *alterado) {
    if (alterado == NULL) {
        FILE *arquivo = fopen(NOME_ARQUIVO, "ab");
        if (arquivo == NULL) 
			return 0;
        fwrite(t, TAM_REGISTRO, 1, arquivo);
        fclose(arquivo);
        return 1;
    } else {
        FILE *arquivo = fopen(NOME_ARQUIVO, "rb+");
        if (arquivo == NULL) 
			return 0;
        fseek(arquivo, (*alterado) * TAM_REGISTRO, SEEK_SET);
        fwrite(&triangulos[*alterado], TAM_REGISTRO, 1, arquivo);
        fclose(arquivo);
        return 1;
    }
}

int Cadastrar_e_Alterar_Triangulo(unsigned int *alteracao, unsigned int *alterado) {
    unsigned int parametro;

    if (alteracao == NULL) {
        parametro = total_triangulos;
        if (total_triangulos >= MAX_REGISTROS) {
            printf("Maximo de registros atingido!\n");
            return 0;
        }
        triangulos[parametro].lado_1 = 0;
        triangulos[parametro].lado_2 = 0;
        triangulos[parametro].lado_3 = 0;
        printf("CADASTRANDO NOVO TRIANGULO...\n"
			   "(Digite apenas numeros interiros e positivos)\n");
    }else 
        parametro = *alterado;

    do {
        if(triangulos[parametro].lado_1 <= 0){
        	triangulos[parametro].lado_1 = input_d("Digite o valor do lado 1: ");
        	continue;
		}else if(triangulos[parametro].lado_2 <= 0){
			triangulos[parametro].lado_2 = input_d("Digite o valor do lado 2: ");
			continue;
		}else if(triangulos[parametro].lado_3 <= 0) {
			triangulos[parametro].lado_3 = input_d("Digite o valor do lado 3: "); 
			continue;
		}else
			break;

    } while (1);

    verificar_triangulo(&triangulos[parametro]);

    if (alteracao == NULL) {
        triangulos[parametro].id = gerar_id();
        
		if (gravar_registro(&triangulos[parametro], NULL)) {
            printf("\nRegistro realizado com sucesso!");
            total_triangulos++;
        } else {
            printf("\nErro na tentativa de registrar o triangulo!\n");
        }
    }else {
        if (gravar_registro(&triangulos[parametro], &parametro)) {
            printf("\nAlteracao realizada com sucesso!");
        } else {
            printf("\nErro na tentativa de alterar o triangulo!\n");
        }
    }

    return 1;
}

void listar_triangulos(char *c) {
    FILE *arquivo = fopen(NOME_ARQUIVO, "rb");
    unsigned int i;
    printf("\n    |=================================================|\n");
    printf("    |  ID  | Lado 1 | Lado 2 | Lado 3 |     Tipo      |\n");
    printf("    |=================================================|\n");

    for (i = 0; i < total_triangulos; i++) {
        if (c == NULL || !strcmp(triangulos[i].tipo, c)) {
            printf("    | %4u | %6u | %6u | %6u | %-14s|\n",
                   triangulos[i].id,
                   triangulos[i].lado_1,
                   triangulos[i].lado_2,
                   triangulos[i].lado_3,
                   triangulos[i].tipo);
        }
    }

    printf("    |=================================================|\n");
    fclose(arquivo);
}

void alterar_registros() {
    unsigned int opcao, id_buscado, i;
    id_buscado = input_d("Digite o ID do triangulo a ser alterado: ");
    for (i = 0; i < total_triangulos; i++) {
        if (triangulos[i].id == id_buscado) {
            clear();
            printf("Triangulo de ID (%u) encontrado!\n", id_buscado);
            printf("=========================\n");
            printf("| 1 - Alterar o lado 1 |\n");
            printf("| 2 - Alterar o lado 2 |\n");
            printf("| 3 - Alterar o lado 3 |\n");
            printf("| 4 - Alterar todos    |\n");
            printf("=========================\n");
            opcao = input_d("Escolha uma opcao[1-4]: ");

            switch (opcao) {
                case 1:
                    triangulos[i].lado_1 = 0;
                    Cadastrar_e_Alterar_Triangulo(&opcao, &i);
                    break;
                case 2:
                    triangulos[i].lado_2 = 0;
                    Cadastrar_e_Alterar_Triangulo(&opcao, &i);
                    break;
                case 3:
                    triangulos[i].lado_3 = 0;
                    Cadastrar_e_Alterar_Triangulo(&opcao, &i);
                    break;
                case 4:
                    triangulos[i].lado_1 = 0;
                    triangulos[i].lado_2 = 0;
                    triangulos[i].lado_3 = 0;
                    Cadastrar_e_Alterar_Triangulo(&opcao, &i);
                    break;
                default:
                    printf("Opcao invalida!\n");
                    break;
            }
            return;
        }
    }
    printf("Triangulo de ID (%u) nao encontrado!\n", id_buscado);
}

int main() {
    unsigned int opcao;
    if (carregar_registros())
        printf("Registros carregados com sucesso! Tamanho: (%ld) bytes\n", TAM_REGISTRO * total_triangulos);
    else
        printf("Nenhum dado encontrado! Experimente cadastrar um novo triangulo!\n");

    printf("\nPressione enter para prosseguir...");
    setbuf(stdin, NULL);
    getchar();

    do {
        clear();
        printf("    |====================================|\n");
        printf("    |  1- Cadastrar triangulo            |\n");
        printf("    |  2- Listar todos os triangulos     |\n");
        printf("    |  3- Listar somente os equilateros  |\n");
        printf("    |  4- Listar somente os isosceles    |\n");
        printf("    |  5- Listar somente os escalenos    |\n");
        printf("    |  6- Alterar dados                  |\n");
        printf("    |  7- Sair                           |\n");
        printf("    |====================================|\n");

        opcao = input_d("Escolha uma das opcoes: ");

        switch (opcao) {
            case 1:
                Cadastrar_e_Alterar_Triangulo(NULL, NULL);
                break;
            
			case 2:
            	if(!carregar_registros())
            		printf("Cadastre um triangulo primeiro!");
            	else
                	listar_triangulos(NULL);
                break;
            
			case 3:
            	if(!carregar_registros())
            		printf("Cadastre um triangulo primeiro!");
            	else
					listar_triangulos("equilatero");
                break;
            
			case 4:
            	if(!carregar_registros())
            		printf("Cadastre um triangulo primeiro!");
            	else
                	listar_triangulos("isosceles");
                break;
            
			case 5:
				if(!carregar_registros())
            		printf("Cadastre um triangulo primeiro!");
            	else
               		listar_triangulos("escaleno");
                break;
                
            case 6:
            	if(!carregar_registros())
            		printf("Cadastre um triangulo primeiro!");
            	else
                	alterar_registros();
                break;
            
			case 7:
                printf("Saindo...\n");
                break;
                
            default:
            	printf("Opcao invalida!\n");
            	break;
        }

        printf("\nPressione enter para prosseguir...");
        setbuf(stdin, NULL);
        getchar();

    }while (opcao != 7);

    return 0;
}


