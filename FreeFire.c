#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// ============================================================================
// Código da Ilha – Edição Free Fire
// Nível: Novato
// ---------------------------------------------------------------------------
// Este programa implementa o inventário inicial (mochila) do jogador usando
// APENAS lista sequencial (vetor estático).
//
// Funcionalidades do NÍVEL NOVATO:
// - Struct Item com nome, tipo e quantidade.
// - Cadastro (inserirItem) de até 10 itens.
// - Remoção de item pelo nome (removerItem).
// - Listagem dos itens (listarItens) após as operações.
// - Busca sequencial por nome (buscarItem).
//
// Níveis futuros (APENAS EM COMENTÁRIOS):
// - Ordenação com critérios (nome, tipo, prioridade).
// - Busca binária em dados ordenados.
// - Comparação de desempenho, etc.
// ============================================================================

#define CAPACIDADE_MOCHILA 10

// Struct Item:
// Representa um componente da mochila com nome, tipo e quantidade.
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

// Vetor da mochila e controle de quantidade atual.
Item mochila[CAPACIDADE_MOCHILA];
int numItens = 0;

// ---------------------------------------------------------------------------
// Protótipos das funções do nível novato
// ---------------------------------------------------------------------------
void limparBufferEntrada(void);
void limparTela(void);
void exibirMenu(void);

void inserirItem(Item mochila[], int *numItens, int capacidade);
void removerItem(Item mochila[], int *numItens);
void listarItens(const Item mochila[], int numItens);
void buscarItem(const Item mochila[], int numItens);

// Função auxiliar de busca sequencial que retorna o índice do item ou -1.
int buscarIndicePorNome(const Item mochila[], int numItens, const char *nome);

// ============================================================================
// Função principal
// ============================================================================
int main() {
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
            case 1:
                inserirItem(mochila, &numItens, CAPACIDADE_MOCHILA);
                listarItens(mochila, numItens);
                break;

            case 2:
                removerItem(mochila, &numItens);
                listarItens(mochila, numItens);
                break;

            case 3:
                listarItens(mochila, numItens);
                break;

            case 4:
                buscarItem(mochila, numItens);
                break;

            case 0:
                printf("Saindo da mochila da ilha...\n");
                break;

            default:
                printf("Opcao invalida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

// ============================================================================
// Implementação das funções
// ============================================================================

// limparBufferEntrada():
// Remove caracteres restantes no buffer de entrada (stdin),
// evitando problemas ao alternar entre scanf e leitura de strings.
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // descarta
    }
}

// limparTela():
// Simula limpeza de tela imprimindo várias quebras de linha.
void limparTela(void) {
    for (int i = 0; i < 30; i++) {
        printf("\n");
    }
}

// exibirMenu():
// Apresenta o menu principal ao jogador.
void exibirMenu(void) {
    printf("\n========================================\n");
    printf("     MOCHILA DA ILHA - NIVEL NOVATO     \n");
    printf("========================================\n");
    printf("Itens na mochila: %d / %d\n\n", numItens, CAPACIDADE_MOCHILA);
    printf("1. Adicionar um item\n");
    printf("2. Remover um item\n");
    printf("3. Listar todos os itens\n");
    printf("4. Buscar um item pelo nome (busca sequencial)\n");
    printf("0. Sair\n");
    printf("========================================\n");
}

// inserirItem():
// Adiciona um novo item à mochila se houver espaço.
// Solicita nome, tipo e quantidade.
// Usa scanf com formato para ler strings com espaços.
void inserirItem(Item mochila[], int *numItens, int capacidade) {
    if (*numItens >= capacidade) {
        printf("Mochila cheia! Nao e possivel adicionar mais itens.\n");
        return;
    }

    Item novo;

    printf("Digite o NOME do item: ");
    if (scanf(" %29[^\n]", novo.nome) != 1) {
        printf("Entrada invalida para nome.\n");
        limparBufferEntrada();
        return;
    }
    limparBufferEntrada();

    printf("Digite o TIPO do item (ex: arma, municao, cura): ");
    if (scanf(" %19[^\n]", novo.tipo) != 1) {
        printf("Entrada invalida para tipo.\n");
        limparBufferEntrada();
        return;
    }
    limparBufferEntrada();

    printf("Digite a QUANTIDADE do item: ");
    if (scanf("%d", &novo.quantidade) != 1) {
        printf("Entrada invalida para quantidade.\n");
        limparBufferEntrada();
        return;
    }
    limparBufferEntrada();

    if (novo.quantidade < 0) {
        printf("Quantidade negativa ajustada para 0.\n");
        novo.quantidade = 0;
    }

    mochila[*numItens] = novo;
    (*numItens)++;

    printf("Item adicionado com sucesso!\n");
}

// removerItem():
// Remove um item da mochila com base no nome informado pelo jogador.
// Se encontrar, desloca os elementos seguintes para fechar o "buraco".
void removerItem(Item mochila[], int *numItens) {
    if (*numItens == 0) {
        printf("A mochila esta vazia. Nao ha itens para remover.\n");
        return;
    }

    char nomeBusca[30];
    printf("Digite o NOME do item que deseja remover: ");
    if (scanf(" %29[^\n]", nomeBusca) != 1) {
        printf("Entrada invalida.\n");
        limparBufferEntrada();
        return;
    }
    limparBufferEntrada();

    int indice = buscarIndicePorNome(mochila, *numItens, nomeBusca);
    if (indice == -1) {
        printf("Item '%s' nao encontrado na mochila.\n", nomeBusca);
        return;
    }

    // Desloca todos os itens à direita uma posição para a esquerda.
    for (int i = indice; i < *numItens - 1; i++) {
        mochila[i] = mochila[i + 1];
    }
    (*numItens)--;

    printf("Item '%s' removido com sucesso.\n", nomeBusca);
}

// listarItens():
// Exibe todos os itens cadastrados na mochila em formato de tabela simples.
void listarItens(const Item mochila[], int numItens) {
    printf("\n--- Itens na mochila ---\n");

    if (numItens == 0) {
        printf("Nenhum item cadastrado.\n");
        return;
    }

    for (int i = 0; i < numItens; i++) {
        printf("[%d]\n", i);
        printf("  Nome      : %s\n", mochila[i].nome);
        printf("  Tipo      : %s\n", mochila[i].tipo);
        printf("  Quantidade: %d\n", mochila[i].quantidade);
    }
}

// buscarItem():
// Realiza uma busca sequencial por nome e mostra os dados do item se encontrado.
void buscarItem(const Item mochila[], int numItens) {
    if (numItens == 0) {
        printf("A mochila esta vazia. Nao ha itens para buscar.\n");
        return;
    }

    char nomeBusca[30];
    printf("Digite o NOME do item que deseja buscar: ");
    if (scanf(" %29[^\n]", nomeBusca) != 1) {
        printf("Entrada invalida.\n");
        limparBufferEntrada();
        return;
    }
    limparBufferEntrada();

    int indice = buscarIndicePorNome(mochila, numItens, nomeBusca);
    if (indice == -1) {
        printf("Item '%s' nao encontrado.\n", nomeBusca);
    } else {
        printf("Item encontrado:\n");
        printf("  Nome      : %s\n", mochila[indice].nome);
        printf("  Tipo      : %s\n", mochila[indice].tipo);
        printf("  Quantidade: %d\n", mochila[indice].quantidade);
    }
}

// buscarIndicePorNome():
// Implementa busca sequencial passando por todos os itens da mochila
// até encontrar um nome igual (strcmp == 0). Retorna índice ou -1.
int buscarIndicePorNome(const Item mochila[], int numItens, const char *nome) {
    for (int i = 0; i < numItens; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}
