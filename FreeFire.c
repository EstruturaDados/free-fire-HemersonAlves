#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// ============================================================================
// Codigo da Ilha - Edicao Free Fire
// ----------------------------------------------------------------------------
// [Nivel Novato - apenas em COMENTARIOS]
// - Struct Item (nome, tipo, quantidade).
// - Mochila com vetor estatico de ate 10 itens.
// - Inserir, remover, listar e buscar (sequencial) no vetor.
//
// [Nivel Aventureiro - apenas em COMENTARIOS]
// - Comparar vetor (lista sequencial) com lista encadeada.
// - Implementar as mesmas operacoes nas duas estruturas:
//   inserir, remover, listar, buscar sequencial.
// - Ordenar o vetor por nome (Selection Sort).
// - Aplicar busca binaria no vetor ordenado.
// - Contar comparacoes nas buscas sequenciais e binaria.
// ============================================================================

#define CAPACIDADE_MOCHILA 10

// ----------------------------------------------------------------------------
// Structs
// ----------------------------------------------------------------------------

// Struct Item:
// Representa um item da mochila, com nome, tipo e quantidade.
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

// Struct No:
// Usada na lista encadeada. Contem um Item e ponteiro para o proximo.
typedef struct No {
    Item dados;
    struct No *proximo;
} No;

// ----------------------------------------------------------------------------
// Dados globais
// ----------------------------------------------------------------------------

// Mochila com vetor (lista sequencial)
Item mochilaVetor[CAPACIDADE_MOCHILA];
int numItensVetor = 0;
bool vetorOrdenadoPorNome = false;

// Mochila com lista encadeada
No *mochilaLista = NULL;

// Contadores de comparacoes
long comparacoesSeqVetor = 0;
long comparacoesBinVetor = 0;
long comparacoesSeqLista = 0;

// ----------------------------------------------------------------------------
// Prototipos gerais
// ----------------------------------------------------------------------------
void limparBufferEntrada(void);
void limparTela(void);
void exibirMenu(void);

// Vetor - operacoes
void inserirItemVetor(void);
void removerItemVetor(void);
void listarItensVetor(void);
int  buscarSequencialVetor(const char *nome, long *comparacoes);
void ordenarVetorPorNome(void);
int  buscarBinariaVetor(const char *nome, long *comparacoes);

// Lista encadeada - operacoes
void inserirItemLista(void);
void removerItemLista(void);
void listarItensLista(void);
No* buscarSequencialLista(const char *nome, long *comparacoes);

// Liberacao de memoria
void liberarLista(No **inicio);

// ============================================================================
// Funcao principal
// ============================================================================
int main(void) {
    int opcao;

    do {
        exibirMenu();
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida.\n");
            limparBufferEntrada();
            continue;
        }
        limparBufferEntrada();

        switch (opcao) {
            // Vetor
            case 1:
                inserirItemVetor();
                listarItensVetor();
                break;
            case 2:
                removerItemVetor();
                listarItensVetor();
                break;
            case 3:
                listarItensVetor();
                break;
            case 4: {
                char nome[30];
                printf("Digite o NOME para busca sequencial (vetor): ");
                if (scanf(" %29[^\n]", nome) != 1) {
                    printf("Entrada invalida.\n");
                    limparBufferEntrada();
                    break;
                }
                limparBufferEntrada();
                comparacoesSeqVetor = 0;
                int idx = buscarSequencialVetor(nome, &comparacoesSeqVetor);
                if (idx == -1) {
                    printf("Item '%s' nao encontrado (vetor).\n", nome);
                } else {
                    Item *it = &mochilaVetor[idx];
                    printf("Item encontrado (vetor):\n");
                    printf("  Nome      : %s\n", it->nome);
                    printf("  Tipo      : %s\n", it->tipo);
                    printf("  Quantidade: %d\n", it->quantidade);
                }
                printf("Comparacoes na busca sequencial (vetor): %ld\n", comparacoesSeqVetor);
                break;
            }
            case 5:
                ordenarVetorPorNome();
                listarItensVetor();
                break;
            case 6: {
                char nome[30];
                printf("Digite o NOME para busca binaria (vetor): ");
                if (scanf(" %29[^\n]", nome) != 1) {
                    printf("Entrada invalida.\n");
                    limparBufferEntrada();
                    break;
                }
                limparBufferEntrada();

                if (!vetorOrdenadoPorNome) {
                    printf("O vetor ainda nao esta ordenado por nome. Ordene primeiro (opcao 5).\n");
                    break;
                }

                comparacoesBinVetor = 0;
                int idx = buscarBinariaVetor(nome, &comparacoesBinVetor);
                if (idx == -1) {
                    printf("Item '%s' nao encontrado na busca binaria.\n", nome);
                } else {
                    Item *it = &mochilaVetor[idx];
                    printf("Item encontrado na busca binaria:\n");
                    printf("  Nome      : %s\n", it->nome);
                    printf("  Tipo      : %s\n", it->tipo);
                    printf("  Quantidade: %d\n", it->quantidade);
                }
                printf("Comparacoes na busca binaria (vetor): %ld\n", comparacoesBinVetor);
                break;
            }

            // Lista encadeada
            case 7:
                inserirItemLista();
                listarItensLista();
                break;
            case 8:
                removerItemLista();
                listarItensLista();
                break;
            case 9:
                listarItensLista();
                break;
            case 10: {
                char nome[30];
                printf("Digite o NOME para busca sequencial (lista): ");
                if (scanf(" %29[^\n]", nome) != 1) {
                    printf("Entrada invalida.\n");
                    limparBufferEntrada();
                    break;
                }
                limparBufferEntrada();
                comparacoesSeqLista = 0;
                No *resultado = buscarSequencialLista(nome, &comparacoesSeqLista);
                if (!resultado) {
                    printf("Item '%s' nao encontrado (lista).\n", nome);
                } else {
                    Item *it = &resultado->dados;
                    printf("Item encontrado (lista):\n");
                    printf("  Nome      : %s\n", it->nome);
                    printf("  Tipo      : %s\n", it->tipo);
                    printf("  Quantidade: %d\n", it->quantidade);
                }
                printf("Comparacoes na busca sequencial (lista): %ld\n", comparacoesSeqLista);
                break;
            }

            case 0:
                printf("Saindo do sistema de mochila da ilha...\n");
                break;

            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }

    } while (opcao != 0);

    // Libera memoria dinamica da lista antes de sair
    liberarLista(&mochilaLista);

    return 0;
}

// ============================================================================
// Implementacao - Utilitarios
// ============================================================================

// limparBufferEntrada:
// Evita problemas ao alternar entre scanf e leitura de strings.
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

// limparTela:
// Simula limpeza de tela com varias quebras de linha.
void limparTela(void) {
    for (int i = 0; i < 30; i++) {
        printf("\n");
    }
}

// exibirMenu:
// Mostra o menu principal com opcoes separadas para vetor e lista.
void exibirMenu(void) {
    printf("\n=============================================\n");
    printf("        MOCHILA DA ILHA - NIVEL AVENTUREIRO  \n");
    printf("=============================================\n");
    printf("Vetor: %d / %d itens | Ordenado por nome: %s\n",
           numItensVetor,
           CAPACIDADE_MOCHILA,
           vetorOrdenadoPorNome ? "SIM" : "NAO");
    printf("Lista encadeada: (tamanho exibido na listagem)\n\n");

    printf(" 1. Inserir item (vetor)\n");
    printf(" 2. Remover item (vetor)\n");
    printf(" 3. Listar itens (vetor)\n");
    printf(" 4. Buscar sequencial por nome (vetor)\n");
    printf(" 5. Ordenar vetor por nome (Selection Sort)\n");
    printf(" 6. Buscar binaria por nome (vetor)\n");
    printf(" 7. Inserir item (lista encadeada)\n");
    printf(" 8. Remover item (lista encadeada)\n");
    printf(" 9. Listar itens (lista encadeada)\n");
    printf("10. Buscar sequencial por nome (lista encadeada)\n");
    printf(" 0. Sair\n");
    printf("=============================================\n");
}

// ============================================================================
// Implementacao - Vetor
// ============================================================================

// inserirItemVetor:
// Adiciona um novo item ao vetor, se houver espaco.
void inserirItemVetor(void) {
    if (numItensVetor >= CAPACIDADE_MOCHILA) {
        printf("Mochila (vetor) cheia. Nao e possivel adicionar mais itens.\n");
        return;
    }

    Item novo;
    printf("Digite o NOME do item: ");
    if (scanf(" %29[^\n]", novo.nome) != 1) {
        printf("Entrada invalida.\n");
        limparBufferEntrada();
        return;
    }
    limparBufferEntrada();

    printf("Digite o TIPO do item (ex: arma, municao, cura): ");
    if (scanf(" %19[^\n]", novo.tipo) != 1) {
        printf("Entrada invalida.\n");
        limparBufferEntrada();
        return;
    }
    limparBufferEntrada();

    printf("Digite a QUANTIDADE: ");
    if (scanf("%d", &novo.quantidade) != 1) {
        printf("Entrada invalida.\n");
        limparBufferEntrada();
        return;
    }
    limparBufferEntrada();

    if (novo.quantidade < 0) {
        printf("Quantidade negativa ajustada para 0.\n");
        novo.quantidade = 0;
    }

    mochilaVetor[numItensVetor] = novo;
    numItensVetor++;
    vetorOrdenadoPorNome = false;

    printf("Item adicionado ao vetor com sucesso.\n");
}

// removerItemVetor:
// Remove um item do vetor procurando pelo nome.
void removerItemVetor(void) {
    if (numItensVetor == 0) {
        printf("A mochila (vetor) esta vazia.\n");
        return;
    }

    char nome[30];
    printf("Digite o NOME do item a remover (vetor): ");
    if (scanf(" %29[^\n]", nome) != 1) {
        printf("Entrada invalida.\n");
        limparBufferEntrada();
        return;
    }
    limparBufferEntrada();

    long comp = 0;
    int idx = buscarSequencialVetor(nome, &comp);
    if (idx == -1) {
        printf("Item '%s' nao encontrado no vetor.\n", nome);
        return;
    }

    for (int i = idx; i < numItensVetor - 1; i++) {
        mochilaVetor[i] = mochilaVetor[i + 1];
    }
    numItensVetor--;
    vetorOrdenadoPorNome = false;

    printf("Item '%s' removido do vetor.\n", nome);
}

// listarItensVetor:
// Exibe todos os itens armazenados no vetor.
void listarItensVetor(void) {
    printf("\n--- Itens na mochila (vetor) ---\n");
    if (numItensVetor == 0) {
        printf("Nenhum item cadastrado no vetor.\n");
        return;
    }

    for (int i = 0; i < numItensVetor; i++) {
        printf("[%d]\n", i);
        printf("  Nome      : %s\n", mochilaVetor[i].nome);
        printf("  Tipo      : %s\n", mochilaVetor[i].tipo);
        printf("  Quantidade: %d\n", mochilaVetor[i].quantidade);
    }
}

// buscarSequencialVetor:
// Busca sequencial pelo nome no vetor. Retorna indice ou -1.
int buscarSequencialVetor(const char *nome, long *comparacoes) {
    for (int i = 0; i < numItensVetor; i++) {
        (*comparacoes)++;
        if (strcmp(mochilaVetor[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

// ordenarVetorPorNome:
// Implementa Selection Sort por nome (ordem alfabetica).
void ordenarVetorPorNome(void) {
    if (numItensVetor <= 1) {
        printf("Poucos itens no vetor, nada para ordenar.\n");
        vetorOrdenadoPorNome = true;
        return;
    }

    long comparacoes = 0;

    for (int i = 0; i < numItensVetor - 1; i++) {
        int indiceMenor = i;
        for (int j = i + 1; j < numItensVetor; j++) {
            comparacoes++;
            if (strcmp(mochilaVetor[j].nome, mochilaVetor[indiceMenor].nome) < 0) {
                indiceMenor = j;
            }
        }
        if (indiceMenor != i) {
            Item temp = mochilaVetor[i];
            mochilaVetor[i] = mochilaVetor[indiceMenor];
            mochilaVetor[indiceMenor] = temp;
        }
    }

    vetorOrdenadoPorNome = true;
    printf("Vetor ordenado por nome (Selection Sort).\n");
    printf("Comparacoes na ordenacao: %ld\n", comparacoes);
}

// buscarBinariaVetor:
// Requer vetor ordenado por nome. Retorna indice ou -1.
int buscarBinariaVetor(const char *nome, long *comparacoes) {
    int inicio = 0;
    int fim = numItensVetor - 1;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        (*comparacoes)++;
        int cmp = strcmp(mochilaVetor[meio].nome, nome);
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
// Implementacao - Lista encadeada
// ============================================================================

// inserirItemLista:
// Insere um novo item no inicio da lista encadeada.
void inserirItemLista(void) {
    Item novo;
    printf("Digite o NOME do item (lista): ");
    if (scanf(" %29[^\n]", novo.nome) != 1) {
        printf("Entrada invalida.\n");
        limparBufferEntrada();
        return;
    }
    limparBufferEntrada();

    printf("Digite o TIPO do item (lista): ");
    if (scanf(" %19[^\n]", novo.tipo) != 1) {
        printf("Entrada invalida.\n");
        limparBufferEntrada();
        return;
    }
    limparBufferEntrada();

    printf("Digite a QUANTIDADE (lista): ");
    if (scanf("%d", &novo.quantidade) != 1) {
        printf("Entrada invalida.\n");
        limparBufferEntrada();
        return;
    }
    limparBufferEntrada();

    if (novo.quantidade < 0) {
        printf("Quantidade negativa ajustada para 0.\n");
        novo.quantidade = 0;
    }

    No *novoNo = (No *)malloc(sizeof(No));
    if (!novoNo) {
        printf("Falha ao alocar memoria para o novo no.\n");
        return;
    }
    novoNo->dados = novo;
    novoNo->proximo = mochilaLista;
    mochilaLista = novoNo;

    printf("Item adicionado na lista encadeada.\n");
}

// removerItemLista:
// Remove um item da lista encadeada pelo nome.
void removerItemLista(void) {
    if (!mochilaLista) {
        printf("A lista encadeada esta vazia.\n");
        return;
    }

    char nome[30];
    printf("Digite o NOME do item a remover (lista): ");
    if (scanf(" %29[^\n]", nome) != 1) {
        printf("Entrada invalida.\n");
        limparBufferEntrada();
        return;
    }
    limparBufferEntrada();

    No *atual = mochilaLista;
    No *anterior = NULL;

    while (atual) {
        if (strcmp(atual->dados.nome, nome) == 0) {
            if (anterior == NULL) {
                mochilaLista = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }
            free(atual);
            printf("Item '%s' removido da lista encadeada.\n", nome);
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }

    printf("Item '%s' nao encontrado na lista encadeada.\n", nome);
}

// listarItensLista:
// Percorre a lista e exibe todos os itens armazenados.
void listarItensLista(void) {
    printf("\n--- Itens na mochila (lista encadeada) ---\n");
    if (!mochilaLista) {
        printf("Nenhum item cadastrado na lista.\n");
        return;
    }

    No *atual = mochilaLista;
    int indice = 0;
    while (atual) {
        printf("[%d]\n", indice);
        printf("  Nome      : %s\n", atual->dados.nome);
        printf("  Tipo      : %s\n", atual->dados.tipo);
        printf("  Quantidade: %d\n", atual->dados.quantidade);
        atual = atual->proximo;
        indice++;
    }
}

// buscarSequencialLista:
// Busca sequencial por nome na lista encadeada.
// Retorna ponteiro para o no ou NULL se nao encontrou.
No* buscarSequencialLista(const char *nome, long *comparacoes) {
    No *atual = mochilaLista;
    while (atual) {
        (*comparacoes)++;
        if (strcmp(atual->dados.nome, nome) == 0) {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}

// liberarLista:
// Libera toda a memoria alocada na lista encadeada.
void liberarLista(No **inicio) {
    No *atual = *inicio;
    while (atual) {
        No *prox = atual->proximo;
        free(atual);
        atual = prox;
    }
    *inicio = NULL;
}
