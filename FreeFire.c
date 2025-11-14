#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ============================================================================
// Codigo da Ilha – Torre de Fuga
// Nivel: Mestre
// ----------------------------------------------------------------------------
// Este modulo representa a fase final do jogo, em que o jogador precisa
// organizar os componentes da torre de resgate.
//
// Funcionalidades principais:
// - Struct Componente (nome, tipo, prioridade).
// - Cadastro de ate 20 componentes.
// - Bubble sort por nome.
// - Insertion sort por tipo.
// - Selection sort por prioridade.
// - Busca binaria por nome (apenas apos ordenacao por nome).
// - Contagem de comparacoes e tempo de execucao de cada algoritmo.
// ============================================================================

#define MAX_COMPONENTES 20

// ---------------------------------------------------------------------------
// Struct Componente:
// Representa um elemento da torre de fuga, com nome, tipo e prioridade.
// Exemplo de nome: "chip central"
// Exemplo de tipo: "controle", "suporte", "propulsao"
// Prioridade: de 1 (baixa) a 10 (altissima)
// ---------------------------------------------------------------------------
typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade;
} Componente;

// Vetor de componentes e quantidade atual cadastrada.
Componente componentes[MAX_COMPONENTES];
int numComponentes = 0;

// Indica se o vetor esta ordenado por nome (necessario para busca binaria).
int ordenadoPorNome = 0;

// ---------------------------------------------------------------------------
// Prototipos das funcoes
// ---------------------------------------------------------------------------

// Interface
void limparTela(void);
void exibirMenu(void);
int  lerInteiro(const char *mensagem);
void lerString(const char *mensagem, char *destino, size_t tamanho);

// Cadastro e exibicao
void cadastrarComponente(void);
void mostrarComponentes(const Componente v[], int n);

// Algoritmos de ordenacao
void bubbleSortNome(Componente v[], int n, long *comparacoes);
void insertionSortTipo(Componente v[], int n, long *comparacoes);
void selectionSortPrioridade(Componente v[], int n, long *comparacoes);

// Busca binaria
int buscaBinariaPorNome(const Componente v[], int n, const char *chave, long *comparacoes);

// Medicao de tempo e desempenho
void medirTempoOrdenacao(
    void (*algoritmo)(Componente[], int, long*),
    Componente v[],
    int n,
    const char *descricao,
    int marcaOrdenadoPorNome
);

// ============================================================================
// Funcao principal
// ============================================================================
int main(void) {
    int opcao;
    srand((unsigned)time(NULL));

    do {
        exibirMenu();
        opcao = lerInteiro("Escolha uma opcao: ");

        switch (opcao) {
            case 1:
                cadastrarComponente();
                mostrarComponentes(componentes, numComponentes);
                break;

            case 2:
                medirTempoOrdenacao(bubbleSortNome, componentes, numComponentes,
                                    "Bubble Sort por NOME", 1);
                mostrarComponentes(componentes, numComponentes);
                break;

            case 3:
                medirTempoOrdenacao(insertionSortTipo, componentes, numComponentes,
                                    "Insertion Sort por TIPO", 0);
                mostrarComponentes(componentes, numComponentes);
                break;

            case 4:
                medirTempoOrdenacao(selectionSortPrioridade, componentes, numComponentes,
                                    "Selection Sort por PRIORIDADE", 0);
                mostrarComponentes(componentes, numComponentes);
                break;

            case 5: {
                if (!ordenadoPorNome) {
                    printf("\nO vetor ainda NAO esta ordenado por nome.\n");
                    printf("Use a opcao 2 (Bubble Sort por nome) antes da busca binaria.\n");
                    break;
                }
                if (numComponentes == 0) {
                    printf("\nNao ha componentes cadastrados.\n");
                    break;
                }

                char chave[30];
                lerString("Digite o NOME do componente-chave para busca binaria: ",
                          chave, sizeof(chave));

                long comparacoes = 0;
                int idx = buscaBinariaPorNome(componentes, numComponentes, chave, &comparacoes);

                if (idx == -1) {
                    printf("\nComponente-chave '%s' NAO encontrado.\n", chave);
                } else {
                    printf("\nComponente-chave encontrado:\n");
                    printf("  Nome      : %s\n", componentes[idx].nome);
                    printf("  Tipo      : %s\n", componentes[idx].tipo);
                    printf("  Prioridade: %d\n", componentes[idx].prioridade);
                }
                printf("Comparacoes da busca binaria: %ld\n", comparacoes);
                break;
            }

            case 6:
                mostrarComponentes(componentes, numComponentes);
                break;

            case 0:
                printf("\nEncerrando modulo da torre de fuga...\n");
                break;

            default:
                printf("\nOpcao invalida. Tente novamente.\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}

// ============================================================================
// Implementacao das funcoes de interface
// ============================================================================

// limparTela:
// Simula limpar a tela imprimindo varias linhas em branco.
void limparTela(void) {
    for (int i = 0; i < 30; i++) {
        printf("\n");
    }
}

// exibirMenu:
// Apresenta as opcoes disponiveis ao jogador.
void exibirMenu(void) {
    printf("\n=============================================\n");
    printf("       TORRE DE FUGA - NIVEL MESTRE          \n");
    printf("=============================================\n");
    printf("Componentes cadastrados: %d / %d\n", numComponentes, MAX_COMPONENTES);
    printf("Vetor ordenado por NOME: %s\n", ordenadoPorNome ? "SIM" : "NAO");
    printf("---------------------------------------------\n");
    printf(" 1. Cadastrar componente\n");
    printf(" 2. Ordenar por NOME (Bubble Sort)\n");
    printf(" 3. Ordenar por TIPO (Insertion Sort)\n");
    printf(" 4. Ordenar por PRIORIDADE (Selection Sort)\n");
    printf(" 5. Buscar componente-chave por NOME (Busca Binaria)\n");
    printf(" 6. Mostrar todos os componentes\n");
    printf(" 0. Sair\n");
    printf("=============================================\n");
}

// lerInteiro:
// Le um inteiro a partir de uma linha usando fgets + sscanf.
int lerInteiro(const char *mensagem) {
    char buffer[64];
    int valor = 0;

    printf("%s", mensagem);
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        return 0;
    }
    sscanf(buffer, "%d", &valor);
    return valor;
}

// lerString:
// Le uma string usando fgets, removendo a quebra de linha final, se existir.
void lerString(const char *mensagem, char *destino, size_t tamanho) {
    printf("%s", mensagem);
    if (fgets(destino, (int)tamanho, stdin) != NULL) {
        size_t len = strlen(destino);
        if (len > 0 && destino[len - 1] == '\n') {
            destino[len - 1] = '\0';
        }
    } else {
        // Em caso de erro, garante string vazia.
        if (tamanho > 0) destino[0] = '\0';
    }
}

// ============================================================================
// Cadastro e exibicao
// ============================================================================

// cadastrarComponente:
// Permite ao jogador inserir um novo componente, caso haja espaco.
void cadastrarComponente(void) {
    if (numComponentes >= MAX_COMPONENTES) {
        printf("\nCapacidade maxima de componentes atingida.\n");
        return;
    }

    Componente novo;
    lerString("\nDigite o NOME do componente: ", novo.nome, sizeof(novo.nome));
    lerString("Digite o TIPO do componente (ex: controle, suporte, propulsao): ",
              novo.tipo, sizeof(novo.tipo));
    novo.prioridade = lerInteiro("Digite a PRIORIDADE (1 a 10): ");

    if (novo.prioridade < 1) {
        printf("Prioridade ajustada para 1.\n");
        novo.prioridade = 1;
    } else if (novo.prioridade > 10) {
        printf("Prioridade ajustada para 10.\n");
        novo.prioridade = 10;
    }

    componentes[numComponentes] = novo;
    numComponentes++;
    ordenadoPorNome = 0;  // apos insercao, deixa de estar garantidamente ordenado

    printf("Componente cadastrado com sucesso.\n");
}

// mostrarComponentes:
// Exibe todos os componentes cadastrados em formato de tabela simples.
void mostrarComponentes(const Componente v[], int n) {
    printf("\n--- Componentes da Torre de Fuga ---\n");
    if (n == 0) {
        printf("Nenhum componente cadastrado.\n");
        return;
    }

    for (int i = 0; i < n; i++) {
        printf("[%d]\n", i);
        printf("  Nome      : %s\n", v[i].nome);
        printf("  Tipo      : %s\n", v[i].tipo);
        printf("  Prioridade: %d\n", v[i].prioridade);
    }
}

// ============================================================================
// Algoritmos de ordenacao
// ============================================================================

// bubbleSortNome:
// Ordena o vetor por NOME (ordem alfabetica) usando Bubble Sort.
// contabiliza o numero de comparacoes em *comparacoes.
void bubbleSortNome(Componente v[], int n, long *comparacoes) {
    *comparacoes = 0;
    if (n <= 1) return;

    int trocou;
    do {
        trocou = 0;
        for (int i = 0; i < n - 1; i++) {
            (*comparacoes)++;
            if (strcmp(v[i].nome, v[i + 1].nome) > 0) {
                Componente temp = v[i];
                v[i] = v[i + 1];
                v[i + 1] = temp;
                trocou = 1;
            }
        }
        n--;
    } while (trocou);
}

// insertionSortTipo:
// Ordena o vetor por TIPO (ordem alfabetica) usando Insertion Sort.
void insertionSortTipo(Componente v[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 1; i < n; i++) {
        Componente chave = v[i];
        int j = i - 1;

        while (j >= 0) {
            (*comparacoes)++;
            if (strcmp(v[j].tipo, chave.tipo) > 0) {
                v[j + 1] = v[j];
                j--;
            } else {
                break;
            }
        }
        v[j + 1] = chave;
    }
}

// selectionSortPrioridade:
// Ordena o vetor por PRIORIDADE usando Selection Sort.
// Por padrao, vamos ordenar da MAIOR prioridade para a MENOR.
void selectionSortPrioridade(Componente v[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < n - 1; i++) {
        int indiceMaior = i;
        for (int j = i + 1; j < n; j++) {
            (*comparacoes)++;
            if (v[j].prioridade > v[indiceMaior].prioridade) {
                indiceMaior = j;
            }
        }
        if (indiceMaior != i) {
            Componente temp = v[i];
            v[i] = v[indiceMaior];
            v[indiceMaior] = temp;
        }
    }
}

// ============================================================================
// Busca binaria
// ============================================================================

// buscaBinariaPorNome:
// Realiza busca binaria pelo nome em vetor JA ordenado por nome.
// Retorna o indice do elemento encontrado, ou -1 se nao achou.
int buscaBinariaPorNome(const Componente v[], int n, const char *chave, long *comparacoes) {
    *comparacoes = 0;
    int inicio = 0;
    int fim = n - 1;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        (*comparacoes)++;
        int cmp = strcmp(v[meio].nome, chave);

        if (cmp == 0) {
            return meio;
        } else if (cmp < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    return -1;
}

// ============================================================================
// Medicao de tempo
// ============================================================================

// medirTempoOrdenacao:
// Recebe um ponteiro para funcao de ordenacao, aplica no vetor atual,
// mede o tempo de execucao com clock() e mostra o numero de comparacoes.
//
// Parametro marcaOrdenadoPorNome:
// - 1: apos essa ordenacao, o vetor e considerado ordenado por nome.
// - 0: apos essa ordenacao, NAO garantimos ordenacao por nome.
void medirTempoOrdenacao(
    void (*algoritmo)(Componente[], int, long*),
    Componente v[],
    int n,
    const char *descricao,
    int marcaOrdenadoPorNome
) {
    if (n <= 0) {
        printf("\nNao ha componentes para ordenar.\n");
        return;
    }

    long comparacoes = 0;
    clock_t inicio = clock();
    algoritmo(v, n, &comparacoes);
    clock_t fim = clock();

    double tempoSegundos = (double)(fim - inicio) / CLOCKS_PER_SEC;

    if (marcaOrdenadoPorNome)
        ordenadoPorNome = 1;
    else
        ordenadoPorNome = 0;

    printf("\nResultado da ordenacao: %s\n", descricao);
    printf("Comparacoes: %ld\n", comparacoes);
    printf("Tempo de execucao: %.6f segundos\n", tempoSegundos);
}
