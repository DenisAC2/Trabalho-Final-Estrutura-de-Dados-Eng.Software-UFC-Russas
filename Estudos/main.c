#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include "headfunctions.h"

int main() {
    int escolha = -1;
    do {
        escolha = -1;
        static int pos = -1;
        printf("\n--- MENU DE PRODUTOS ---\n");
        printf("1 - Adicionar Produto a Lista de Venda\n");
        printf("2 - Abrir Menu de Vendas\n");
        printf("3 - Verificar Lista de Produtos a Venda\n");
        printf("4 - Verificar Pilha de Uma Categoria\n");
        printf("5 - Verificar as Pilhas de Todas as Categorias\n");
        printf("0 - Sair do programa\n");
        printf("Escolha: ");
        scanf_s("%d", &escolha);
        limpar_buffer_stdin();

        switch (escolha) {

        case 1: {
            int codigo = 0;
            char tipo_produto[32];
            char descricao[64];
            float preco = 0;

            printf("Digite o codigo do produto: ");
            scanf_s("%d", &codigo);
            limpar_buffer_stdin();

            printf("Digite do tipo de produto: ");
            if (fgets(tipo_produto, sizeof(tipo_produto), stdin) == NULL) {
                printf("Entrada invalida. Saindo.\n");
                LiberarTudo();
                return 0;
            }
            tipo_produto[strcspn(tipo_produto, "\n")] = 0;

            printf("Digite a descricao o produto: ");
            if (fgets(descricao, sizeof(descricao), stdin) == NULL) {
                printf("Entrada invalida. Saindo.\n");
                LiberarTudo();
                return 0;
            }
            descricao[strcspn(descricao, "\n")] = 0;

            printf("Digite o preco do produto: ");
            scanf_s("%f", &preco);
            AdicionarLPV(codigo, tipo_produto, descricao, preco);
            break;
        }

        case 2: {
            int opcao = -1;
            ListaProdutos* ProdutoVendido = NULL;
            do {
                printf("\n--- MENU DE VENDAS ---\n");
                printf("1 - Ver Produtos Por Categoria\n");
                printf("2 - Ver Produtos Por Faixa de Preco\n");
                printf("3 - Vender Produto\n");
                printf("4 - Confirmacao de Saida Para Entrega\n");
                printf("0 - Sair do Menu de Vendas\n");
                printf("Escolha: ");
                scanf_s("%d", &opcao);
                limpar_buffer_stdin();

                switch (opcao) {
                case 1: {
                    char categoria[64];
                    printf("Digite a Categoria que Deseja: ");
                    if (fgets(categoria, sizeof(categoria), stdin) == NULL) {
                        printf("Entrada invalida. Saindo.\n");
                        LiberarTudo();
                        return 0;
                    }
                    categoria[strcspn(categoria, "\n")] = 0;
                    ImprimirVendaC(categoria);
                    break;
                }
                case 2: {
                    float valorI, valorF;
                    printf("Digite o Valor Inicial: ");
                    scanf_s("%f", &valorI);
                    limpar_buffer_stdin();
                    printf("Digite o Valor Final: ");
                    scanf_s("%f", &valorF);
                    limpar_buffer_stdin();
                    ImprimirVendaP(valorI, valorF);
                    break;
                }
                case 3: {
                    int codigoVenda = -1;
                    printf("Digite o codigo do produto para venda: ");
                    scanf_s("%d", &codigoVenda);
                    limpar_buffer_stdin();
                    printf("-----------------------------------------------------------\n");
                    ProdutoVendido = VendaProduto(codigoVenda);
                    printf("-----------------------------------------------------------\n");
                    if (ProdutoVendido == NULL) {
                        break;
                    }
                    char nomeCliente[64];
                    char cpfCliente[16];
                    int cepCliente = 0;
                    char ruaCliente[64];
                    int numeroCasaCliente = 0;
                    char complementoCasaCliente[64];

                    printf("Produto Vendido, Informe os Dados da Entrega:\n");
                    printf("-----------------------------------------------------------\n");
                    printf("Digite o Nome do Cliente: ");
                    if (fgets(nomeCliente, sizeof(nomeCliente), stdin) == NULL) {
                        printf("Entrada invalida. Saindo.\n");
                        LiberarTudo();
                        return 0;
                    }
                    nomeCliente[strcspn(nomeCliente, "\n")] = 0;

                    printf("Digite o CPF do Cliente: ");
                    if (fgets(cpfCliente, sizeof(cpfCliente), stdin) == NULL) {
                        printf("Entrada invalida. Saindo.\n");
                        LiberarTudo();
                        return 0;
                    }
                    cpfCliente[strcspn(cpfCliente, "\n")] = 0;

                    printf("Digite o CEP (Somente numeros) do Cliente: ");
                    scanf_s("%d", &cepCliente);
                    limpar_buffer_stdin();

                    printf("Digite o Nome da Rua do Cliente: ");
                    if (fgets(ruaCliente, sizeof(ruaCliente), stdin) == NULL) {
                        printf("Entrada invalida. Saindo.\n");
                        LiberarTudo();
                        return 0;
                    }
                    ruaCliente[strcspn(ruaCliente, "\n")] = 0;

                    printf("Digite o Numero da Casa do Cliente: ");
                    scanf_s("%d", &numeroCasaCliente);
                    limpar_buffer_stdin();

                    printf("Digite o Complemento da Casa do Cliente: ");
                    if (fgets(complementoCasaCliente, sizeof(complementoCasaCliente), stdin) == NULL) {
                        printf("Entrada invalida. Saindo.\n");
                        LiberarTudo();
                        return 0;
                    }
                    complementoCasaCliente[strcspn(complementoCasaCliente, "\n")] = 0;
                    AdicionarFE(nomeCliente, cpfCliente, cepCliente, ruaCliente, numeroCasaCliente, complementoCasaCliente, ProdutoVendido);
                    printf("\n-----------------------------------------------------------\n");
                    break;
                }
                case 4: {
                    printf("\n-----------------------------------------------------------\n");
                    printf("Saindo Para Entrega: \n");
                    ConfirmacaoSE();
                    printf("\n-----------------------------------------------------------\n");
                    break;
                }
                case 0: {
                    printf("Saindo do Menu de Vendas...\n");
                    break;
                }
                default: {
                    printf("Opcao invalida. Tente novamente.\n");
                    break;
                }
                }
            } while (opcao != 0);
            break;
        }

        case 3: {
            printf("Digite a posicao desejada de 0 a %d (-1 para todos): ", tamP - 1);
            scanf_s("%d", &pos);
            Imprimir(pos);
            break;
        }

        case 4: {
            while (true) {
                char nomePilha[64];
                printf("Digite o Nome da Pilha (Parafina, Leash, Quilha, Deck): ");
                if (fgets(nomePilha, sizeof(nomePilha), stdin) == NULL) {
                    printf("Entrada invalida. Saindo.\n");
                    LiberarTudo();
                    return 0;
                }
                nomePilha[strcspn(nomePilha, "\n")] = 0;

                int tipo_idx = tipo_index_para_string(nomePilha);
                if (tipo_idx == -1) {
                    printf("ERRO ESTOQUE: Tipo de produto '%s' desconhecido. Tente novamente.\n", nomePilha);
                    continue;
                }

                Caixa* pilhas[NUM_TYPES] = { topoParafina, topoLeash, topoQuilha, topoDeck };
                int capacities[NUM_TYPES] = { CAP_PARAFINA, CAP_LEASH, CAP_QUILHA, CAP_DECK };

                if (tipo_idx >= 0 && tipo_idx < NUM_TYPES) {
                    ImprimirPilha(PRODUCT_TYPE_NAMES[tipo_idx], pilhas[tipo_idx], capacities[tipo_idx]);
                }
                else {
                    printf("ERRO ESTOQUE: Tipo de produto '%s' desconhecido.\n", nomePilha);
                }
                break;
            }
            break;
        }

        case 5:
            ImprimirPilhas();
            break;

        case 0:
            printf("Obrigado! Ate mais.\n");
            break;

        default:
            printf("Opcao invalida. Tente novamente.\n");
            break;
        }
    } while (escolha != 0);

    LiberarTudo();
    return 0;
}