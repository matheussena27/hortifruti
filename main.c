#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>


#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"

#define MAX_PRODUTOS 100
#define MAX_CARRINHO 100

typedef struct {
    int codigo;
    char nome[50];
    float preco;
    float quantidade;
    char tipo[3];
} Produto;

Produto produtos[MAX_PRODUTOS];
int totalProdutos = 0;

// Carrinho de compras
typedef struct {
    int codigo;
    char nome[50];
    float preco;
    float quantidade;
} ItemCarrinho;

ItemCarrinho carrinho[MAX_CARRINHO];
int totalCarrinho = 0;

// Função para salvar produtos no arquivo
void salvarProdutosNoArquivo() {
    FILE *arquivo = fopen("produtos.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar.\n");
        return;
    }
    for (int i = 0; i < totalProdutos; i++) {
        fprintf(arquivo, "%d %s %s %.2f %2f\n", produtos[i].codigo, produtos[i].nome, produtos[i].tipo, produtos[i].preco, produtos[i].quantidade);
    }
    fclose(arquivo);
}

// Função para carregar produtos do arquivo
void carregarProdutosDoArquivo() {
    FILE *arquivo = fopen("produtos.txt", "r");
    if (arquivo == NULL) {
        printf("Nenhum arquivo de produtos encontrado.\n");
        return;
    }
    totalProdutos = 0;
    while (fscanf(arquivo, "%d %s %s %f %f", &produtos[totalProdutos].codigo, produtos[totalProdutos].nome, produtos[totalProdutos].tipo, &produtos[totalProdutos].preco, &produtos[totalProdutos].quantidade) == 5) {
        totalProdutos++;
    }
    fclose(arquivo);
}

// Função para cadastrar um novo produto
void cadastrarProduto() {
    if (totalProdutos < MAX_PRODUTOS) {
        Produto novoProduto;
        int produtoExistente = 0;  // Flag para verificar se o produto já existe
        system("cls");
        printf("===============================================\n");
        printf("               CADASTRO DE PRODUTOS           \n");
        printf("===============================================\n");

        printf("Digite o código do produto: ");
        scanf("%d", &novoProduto.codigo);

        // Verificar se o código do produto já existe
        for (int i = 0; i < totalProdutos; i++) {
            if (produtos[i].codigo == novoProduto.codigo) {
                produtoExistente = 1;  // Produto já existe
                break;
            }
        }

        if (produtoExistente) {
            printf(RED"\nErro: Já existe um produto cadastrado com o código %d.\n"RESET, novoProduto.codigo);
            while (getchar() != '\n');
            getchar();
            return;  // Retorna sem cadastrar o novo produto
        }

        printf("Digite o nome do produto: ");
        scanf("%s", novoProduto.nome);

        printf("Digite o tipo do produto[und/kg]: ");
        scanf("%s", novoProduto.tipo);

        printf("Digite o preço do produto: ");
        scanf("%f", &novoProduto.preco);

        printf("Digite a quantidade do produto: ");
        scanf("%f", &novoProduto.quantidade);

        produtos[totalProdutos++] = novoProduto;
        salvarProdutosNoArquivo();  // Salva o produto cadastrado no arquivo
        printf(GREEN"Produto cadastrado com sucesso!\n"RESET);
        while (getchar() != '\n');
        getchar();
    } else {
        printf("Limite de produtos atingido.\n");
    }
}

// Função para editar a quantidade de um produto
void editarQuantidadeProduto() {
    int codigo, encontrado = 0;
    float novaQuantidade;

    system("cls");
    printf("===============================================\n");
    printf("           EDITAR QUANTIDADE PRODUTO           \n");
    printf("===============================================\n");

    printf("Digite o código do produto que deseja editar a quantidade: ");
    scanf("%d", &codigo);

    // Busca o produto pelo código
    for (int i = 0; i < totalProdutos; i++) {
        if (produtos[i].codigo == codigo) {
            encontrado = 1;
            printf("Produto encontrado!\n");
            printf(" Nome: %-20s | Preço: %-8.2f | Estoque: %.1f%s\n", produtos[i].nome, produtos[i].preco, produtos[i].quantidade, produtos[i].tipo);

            // Solicita a nova quantidade
            printf("Digite a nova quantidade para o produto: ");
            scanf("%f", &novaQuantidade);

            // Atualiza a quantidade
            produtos[i].quantidade = novaQuantidade;

            // Salva as alterações no arquivo
            salvarProdutosNoArquivo();
            printf(GREEN"Quantidade atualizada com sucesso!\n"RESET);
            break;
        }
    }

    if (!encontrado) {
        printf(RED"\nProduto com código %d não encontrado.\n"RESET, codigo);
    }
}

// Função para listar todos os produtos cadastrados
void listarProdutos() {
    system("cls");

    // Cabeçalho da tabela
    printf("===============================================\n");
    printf("               Listagem de Produtos           \n");
    printf("===============================================\n");
    printf("| %-10s | %-20s | %-6s | %-10s | %-12s |\n", "Código", "Nome", "Tipo", "Preço", "Quantidade");
    printf("===============================================\n");

    // Imprime as informações de cada produto em formato de tabela
    for (int i = 0; i < totalProdutos; i++) {
        // Imprimindo a quantidade com 2 casas decimais
        printf("| %-10d | %-20s | %-6s | %-10.2f | %-12.2f |\n",
               produtos[i].codigo,
               produtos[i].nome,
               produtos[i].tipo,
               produtos[i].preco,
               produtos[i].quantidade);
    }

    printf("===============================================\n");
    printf("\nAperte alguma tecla para continuar...");

    // Espera o usuário pressionar uma tecla
    while (getchar() != '\n');  // Consumir o '\n' residual
    getchar();
}

// Função para apagar um produto pelo código
void apagarProduto() {
    int codigo, encontrado = 0;

    system("cls");
    printf("===============================================\n");
    printf("               APAGAR PRODUTO           \n");
    printf("===============================================\n");

    printf("Digite o código do produto que deseja apagar: ");
    scanf("%d", &codigo);

    for (int i = 0; i < totalProdutos; i++) {
        if (produtos[i].codigo == codigo) {
            encontrado = 1;
            // Shift dos produtos para "remover" o produto apagado
            for (int j = i; j < totalProdutos - 1; j++) {
                produtos[j] = produtos[j + 1];
            }
            totalProdutos--;
            salvarProdutosNoArquivo();  // Atualiza o arquivo após a remoção
            printf(GREEN"Produto apagado com sucesso.\n"RESET);
            while (getchar() != '\n');
            getchar();
            break;
        }
    }

    if (!encontrado) {
        printf("Produto com código %d não encontrado.\n", codigo);
    }
}

// Função para registrar a venda no arquivo
void salvarVenda(ItemCarrinho item) {
    FILE *arquivo = fopen("vendas.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de vendas.\n");
        return;
    }
    fprintf(arquivo, "%d %s %.2f %.2f\n", item.codigo, item.nome, item.preco, item.quantidade);
    fclose(arquivo);
}

// Função para vender produtos
void venderProduto() {
    int codigo, encontrado, itemNoCarrinho;
    float quantidade;
    char continuar, confirmarCompra, opcao;

    do {
        system("cls");
        encontrado = 0;
        printf("=========================\n");
        printf("  Sistema de Vendas\n");
        printf("=========================\n");

        printf("Escolha uma opção:\n");
        printf("1. Adicionar produto ao carrinho\n");
        printf("2. Visualizar Carrinho\n");
        printf("3. Finalizar compra\n");
        printf("Digite sua opção: ");
        scanf(" %c", &opcao);

        switch (opcao) {
            case '1':
                continuar = 's';
                printf("Digite o código do produto que deseja comprar: ");
                scanf("%d", &codigo);

                for (int i = 0; i < totalProdutos; i++) {
                    if (produtos[i].codigo == codigo) {
                        encontrado = 1;
                        printf("Produto encontrado!\n");
                        printf(" Nome: %-20s | Preço: %-8.2f | Estoque: %.1f%s\n", produtos[i].nome, produtos[i].preco, produtos[i].quantidade, produtos[i].tipo);

                        printf("Digite a quantidade que deseja comprar: ");
                        scanf("%f", &quantidade);

                        if (quantidade <= produtos[i].quantidade) {
                            itemNoCarrinho = -1;
                            for (int j = 0; j < totalCarrinho; j++) {
                                if (carrinho[j].codigo == produtos[i].codigo) {
                                    itemNoCarrinho = j;
                                    break;
                                }
                            }

                            if (itemNoCarrinho == -1) {
                                carrinho[totalCarrinho].codigo = produtos[i].codigo;
                                strcpy(carrinho[totalCarrinho].nome, produtos[i].nome);
                                carrinho[totalCarrinho].preco = produtos[i].preco;
                                carrinho[totalCarrinho].quantidade = quantidade;
                                totalCarrinho++;
                            } else {
                                carrinho[itemNoCarrinho].quantidade += quantidade;
                            }
                            produtos[i].quantidade -= quantidade;
                            printf(GREEN"\nProduto(s) adicionado(s) ao carrinho.\n"RESET);
                        } else {
                            printf(YELLOW"\nQuantidade indisponível. Temos apenas %.2f unidades no estoque.\n"RESET, produtos[i].quantidade);
                        }
                        break;
                    }
                }

                if (!encontrado) {
                    printf(RED"\nProduto com código %d não encontrado.\n"RESET, codigo);
                }
                break;

            case '2':
                    // Mostrar o conteúdo do carrinho
                    printf("\n=============================\n");
                    printf("Itens no Carrinho de Compras\n");
                    printf("=============================\n");

                    float totalCarrinhoCompra = 0;
                    for (int i = 0; i < totalCarrinho; i++) {
                        printf(" %-20s | Quantidade: %.2f | Preço unitário: %.2f | Total: %.2f\n", carrinho[i].nome, carrinho[i].quantidade, carrinho[i].preco, carrinho[i].quantidade * carrinho[i].preco);
                        totalCarrinhoCompra += carrinho[i].quantidade * carrinho[i].preco;
                    }

                    printf("=============================\n");
                    printf("Total do Carrinho: %.2f\n", totalCarrinhoCompra);
                    printf("=============================\n");
                    while (getchar() != '\n');
                    getchar();
                break;

            case '3':
                // Finalizar compra
                printf("\nDeseja finalizar a compra e confirmar o pagamento? (s/n): ");
                scanf(" %c", &confirmarCompra);

                if (confirmarCompra == 's' || confirmarCompra == 'S') {
                    salvarProdutosNoArquivo(); // Salva atualização no estoque
                    for (int i = 0; i < totalCarrinho; i++) {
                        salvarVenda(carrinho[i]);// Registrar todas as vendas no arquivo
                    }
                    printf(GREEN"\nCompra realizada com sucesso!\n"RESET);
                } else {
                    printf(RED"\nCompra cancelada. Carrinho vazio.\n"RESET);
                }

                totalCarrinho = 0;  // Limpa o carrinho
                continuar = 'n';
                break;

            default:
                printf(RED"\nOpção inválida. Tente novamente.\n"RESET);
                break;
        }

    } while (continuar == 's' || continuar == 'S');
}

// Função para exibir o relatório de vendas
void exibirRelatorioVendas() {
    FILE *arquivo = fopen("vendas.txt", "r");
    if (arquivo == NULL) {
        printf("Nenhuma venda registrada.\n");
        return;
    }
    printf("===============================================\n");
    printf("               Historico de Vendas             \n");
    printf("===============================================\n");
    printf("| %-10s | %-20s | %-10s | %-12s |\n", "Código", "Nome", "Preço", "Quantidade");
    printf("===============================================\n");

    int codigo;
    char nome[50];
    float preco, quantidade;

    while (fscanf(arquivo, "%d %s %f %f", &codigo, nome, &preco, &quantidade) == 4) {
        printf("| %-10d | %-20s | %-10.2f | %-12.2f |\n", codigo, nome, preco, quantidade);
    }

    printf("===============================================\n");
    fclose(arquivo);

    printf("\nAperte alguma tecla para continuar...");
    while (getchar() != '\n');
    getchar();
}

//função indicadores
void exibirIndicadores() {
    int maisVendidoIdx = -1, menosVendidoIdx = -1;
    int maiorEstoqueIdx = -1, menorEstoqueIdx = -1;
    float maisVendido = 0, menosVendido = __FLT_MAX__;
    float maiorEstoque = 0, menorEstoque = __FLT_MAX__;

    for (int i = 0; i < totalProdutos; i++) {
        float vendasTotais = 0;
        FILE *arquivo = fopen("vendas.txt", "r");
        if (arquivo != NULL) {
            int codigo;
            char nome[50];
            float preco, quantidade;

            while (fscanf(arquivo, "%d %s %f %f", &codigo, nome, &preco, &quantidade) == 4) {
                if (produtos[i].codigo == codigo) {
                    vendasTotais += quantidade;
                }
            }
            fclose(arquivo);
        }

        if (vendasTotais > maisVendido) {
            maisVendido = vendasTotais;
            maisVendidoIdx = i;
        }
        if (vendasTotais < menosVendido) {
            menosVendido = vendasTotais;
            menosVendidoIdx = i;
        }

        if (produtos[i].quantidade > maiorEstoque) {
            maiorEstoque = produtos[i].quantidade;
            maiorEstoqueIdx = i;
        }
        if (produtos[i].quantidade < menorEstoque) {
            menorEstoque = produtos[i].quantidade;
            menorEstoqueIdx = i;
        }
    }

    printf("===============================================\n");
    printf("               Indicadores de Estoque          \n");
    printf("===============================================\n");

    if (maisVendidoIdx != -1)
        printf("Produto mais vendido: %s com %.2f und/kg\n", produtos[maisVendidoIdx].nome, maisVendido);
    if (menosVendidoIdx != -1)
        printf("Produto menos vendido: %s com %.2f und/kg\n", produtos[menosVendidoIdx].nome, menosVendido);
    if (maiorEstoqueIdx != -1)
        printf("Produto com maior estoque: %s com %.2f und/kg\n", produtos[maiorEstoqueIdx].nome, maiorEstoque);
    if (menorEstoqueIdx != -1)
        printf("Produto com menor estoque: %s com %.2f und/kg\n", produtos[menorEstoqueIdx].nome, menorEstoque);

    printf("\nAperte alguma tecla para continuar...");
    while (getchar() != '\n');
    getchar();
}


// Submenu de relatórios
void menuRelatorios() {
    int opcaoRelatorio;
    do {
        system("cls");
        printf("=== Submenu de Relatórios ===\n");
        printf("1. Exibir Relatório de Vendas\n");
        printf("2. Exibir Indicadores\n");
        printf("3. Voltar ao Menu Principal\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcaoRelatorio);

        switch (opcaoRelatorio) {
            case 1:
                exibirRelatorioVendas();
                break;
            case 2:
                exibirIndicadores();
                break;
            case 3:
                break;
            default:
                printf(RED"Opção inválida. Tente novamente.\n"RESET);
        }
    } while (opcaoRelatorio != 3);
}

// Menu principal
void menu() {
    int opcao;
    do {
        carregarProdutosDoArquivo();
        system("cls");
        printf("Sistema Hortifruti\n");
        printf("1. Cadastrar Produto\n");
        printf("2. Editar Estoque\n");
        printf("3. Listar Produtos\n");
        printf("4. Apagar Produto\n");
        printf("5. Vender Produto\n");
        printf("6. Relatorios\n");
        printf("7. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrarProduto();
                break;
            case 2:
                editarQuantidadeProduto();
                break;
            case 3:
                listarProdutos();
                break;
            case 4:
                apagarProduto();
                break;
            case 5:
                venderProduto();
                break;
            case 6:
                menuRelatorios();
                break;
            case 7:
                printf("Saindo...\n");
                break;
            default:
                printf(RED"Opção inválida. Tente novamente.\n"RESET);
        }
    } while (opcao != 7);
}

int main() {
    setlocale(LC_ALL, "");
    menu();
    return 0;
}
