#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include "FunctionsMenu.h"

int main() {
    int escolha = -1, Verificacao = -1, pos = -1;
    do {
        printf("\n--- MENU DE PRODUTOS ---\n");
        printf("1 - Adicionar Produto a Lista de Venda\n");
        printf("2 - Abrir Menu de Vendas\n");
        printf("3 - Verificar Lista de Produtos a Venda\n");
        printf("4 - Verificar Pilha de Uma Categoria\n");
        printf("5 - Verificar as Pilhas de Todas as Categorias\n");
        printf("0 - Sair do programa\n");
        printf("Escolha: ");
        Verificacao = InsercaoInt(&escolha);
        if (Verificacao == 1) {
            printf("Entrada invalida. Saindo.\n");
            LiberarTudo();
            return 0;
        }
        else if (Verificacao == 2) {
            printf("Erro: Voce nao digitou um numero valido!\n");
            continue;
        }
        else if (Verificacao == 3) {
            printf("Erro: Voce digitou caracteres invalidos! Digite apenas numeros.\n");
            continue;
        }

        switch (escolha) {

        case 1: {
            int codigo = 0;
            char tipo_produto[32];
            char descricao[64];
            float preco = 0;

            printf("Digite o codigo do produto: ");
            Verificacao = InsercaoInt(&codigo);
            if (Verificacao == 1) {
                printf("Entrada invalida. Saindo.\n");
                LiberarTudo();
                return 0;
            }
            else if (Verificacao == 2) {
                printf("Erro: Voce nao digitou um numero valido!\n");
                continue;
            }
            else if (Verificacao == 3) {
                printf("Erro: Voce digitou caracteres invalidos! Digite apenas numeros.\n");
                continue;
            }

            printf("Digite do tipo de produto: ");
            Verificacao = InsercaoString(tipo_produto, sizeof(tipo_produto));
            if (Verificacao == 1) {
                printf("Entrada invalida. Saindo.\n");
                LiberarTudo();
                return 0;
            }
            else if (Verificacao == 2) {
                printf("Erro: Voce nao digitou uma string valida!\n");
                continue;
            }

            printf("Digite a descricao o produto: ");
            Verificacao = InsercaoString(descricao, sizeof(descricao));
            if (Verificacao == 1) {
                printf("Entrada invalida. Saindo.\n");
                LiberarTudo();
                return 0;
            }
            else if (Verificacao == 2) {
                printf("Erro: Voce nao digitou uma string valida!\n");
                continue;
            }

            printf("Digite o preco do produto: ");
            Verificacao = InsercaoFloat(&preco);
            if (Verificacao == 1) {
                printf("Entrada invalida. Saindo.\n");
                LiberarTudo();
                return 0;
            }
            else if (Verificacao == 2) {
                printf("Erro: Voce nao digitou um numero valido!\n");
                continue;
            }
            else if (Verificacao == 3) {
                printf("Erro: Voce digitou caracteres invalidos! Digite apenas numeros.\n");
                continue;
            }
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
                Verificacao = InsercaoInt(&opcao);
                if (Verificacao == 1) {
                    printf("Entrada invalida. Saindo.\n");
                    LiberarTudo();
                    return 0;
                }
                else if (Verificacao == 2) {
                    printf("Erro: Voce nao digitou um numero valido!\n");
                    continue;
                }
                else if (Verificacao == 3) {
                    printf("Erro: Voce digitou caracteres invalidos! Digite apenas numeros.\n");
                    continue;
                }

                switch (opcao) {
                case 1: {
                    char categoria[64];
                    printf("Digite a Categoria que Deseja: ");
                    Verificacao = InsercaoString(categoria, sizeof(categoria));
                    if (Verificacao == 1) {
                        printf("Entrada invalida. Saindo.\n");
                        LiberarTudo();
                        return 0;
                    }
                    else if (Verificacao == 2) {
                        printf("Erro: Voce nao digitou uma string valida!\n");
                        continue;
                    }
                    ImprimirVendaC(categoria);
                    break;
                }
                case 2: {
                    float valorI, valorF;
                    printf("Digite o Valor Inicial: ");
                    Verificacao = InsercaoFloat(&valorI);
                    if (Verificacao == 1) {
                        printf("Entrada invalida. Saindo.\n");
                        LiberarTudo();
                        return 0;
                    }
                    else if (Verificacao == 2) {
                        printf("Erro: Voce nao digitou um numero valido!\n");
                        continue;
                    }
                    else if (Verificacao == 3) {
                        printf("Erro: Voce digitou caracteres invalidos! Digite apenas numeros.\n");
                        continue;
                    }
                    printf("Digite o Valor Final: ");
                    Verificacao = InsercaoFloat(&valorF);
                    if (Verificacao == 1) {
                        printf("Entrada invalida. Saindo.\n");
                        LiberarTudo();
                        return 0;
                    }
                    else if (Verificacao == 2) {
                        printf("Erro: Voce nao digitou um numero valido!\n");
                        continue;
                    }
                    else if (Verificacao == 3) {
                        printf("Erro: Voce digitou caracteres invalidos! Digite apenas numeros.\n");
                        continue;
                    }
                    ImprimirVendaP(valorI, valorF);
                    break;
                }
                case 3: {
                    int codigoVenda = -1;
                    printf("Digite o codigo do produto para venda: ");
                    Verificacao = InsercaoInt(&codigoVenda);
                    if (Verificacao == 1) {
                        printf("Entrada invalida. Saindo.\n");
                        LiberarTudo();
                        return 0;
                    }
                    else if (Verificacao == 2) {
                        printf("Erro: Voce nao digitou um numero valido!\n");
                        continue;
                    }
                    else if (Verificacao == 3) {
                        printf("Erro: Voce digitou caracteres invalidos! Digite apenas numeros.\n");
                        continue;
                    }
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
                    Verificacao = InsercaoString(nomeCliente, sizeof(nomeCliente));
                    if (Verificacao == 1) {
                        printf("Entrada invalida. Saindo.\n");
                        LiberarTudo();
                        return 0;
                    }
                    else if (Verificacao == 2) {
                        printf("Erro: Voce nao digitou uma string valida!\n");
                        continue;
                    }

                    printf("Digite o CPF do Cliente: ");
                    Verificacao = InsercaoString(cpfCliente, sizeof(cpfCliente));
                    if (Verificacao == 1) {
                        printf("Entrada invalida. Saindo.\n");
                        LiberarTudo();
                        return 0;
                    }
                    else if (Verificacao == 2) {
                        printf("Erro: Voce nao digitou uma string valida!\n");
                        continue;
                    }

                    printf("Digite o CEP (Somente numeros) do Cliente: ");
                    Verificacao = InsercaoInt(&cepCliente);
                    if (Verificacao == 1) {
                        printf("Entrada invalida. Saindo.\n");
                        LiberarTudo();
                        return 0;
                    }
                    else if (Verificacao == 2) {
                        printf("Erro: Voce nao digitou um numero valido!\n");
                        continue;
                    }
                    else if (Verificacao == 3) {
                        printf("Erro: Voce digitou caracteres invalidos! Digite apenas numeros.\n");
                        continue;
                    }

                    printf("Digite o Nome da Rua do Cliente: ");
                    Verificacao = InsercaoString(ruaCliente, sizeof(ruaCliente));
                    if (Verificacao == 1) {
                        printf("Entrada invalida. Saindo.\n");
                        LiberarTudo();
                        return 0;
                    }
                    else if (Verificacao == 2) {
                        printf("Erro: Voce nao digitou uma string valida!\n");
                        continue;
                    }

                    printf("Digite o Numero da Casa do Cliente: ");
                    Verificacao = InsercaoInt(&numeroCasaCliente);
                    if (Verificacao == 1) {
                        printf("Entrada invalida. Saindo.\n");
                        LiberarTudo();
                        return 0;
                    }
                    else if (Verificacao == 2) {
                        printf("Erro: Voce nao digitou um numero valido!\n");
                        continue;
                    }
                    else if (Verificacao == 3) {
                        printf("Erro: Voce digitou caracteres invalidos! Digite apenas numeros.\n");
                        continue;
                    }

                    printf("Digite o Complemento da Casa do Cliente: ");
                    Verificacao = InsercaoString(complementoCasaCliente, sizeof(complementoCasaCliente));
                    if (Verificacao == 1) {
                        printf("Entrada invalida. Saindo.\n");
                        LiberarTudo();
                        return 0;
                    }
                    else if (Verificacao == 2) {
                        printf("Erro: Voce nao digitou uma string valida!\n");
                        continue;
                    }
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
            Verificacao = InsercaoInt(&pos);
            if (Verificacao == 1) {
                printf("Entrada invalida. Saindo.\n");
                LiberarTudo();
                return 0;
            }
            else if (Verificacao == 2) {
                printf("Erro: Voce nao digitou um numero valido!\n");
                continue;
            }
            else if (Verificacao == 3) {
                printf("Erro: Voce digitou caracteres invalidos! Digite apenas numeros.\n");
                continue;
            }
            Imprimir(pos);
            break;
        }

        case 4: {
            while (true) {
                char nomePilha[64];
                printf("Digite o Nome da Pilha (Parafina, Leash, Quilha, Deck): ");
                Verificacao = InsercaoString(nomePilha, sizeof(nomePilha));
                if (Verificacao == 1) {
                    printf("Entrada invalida. Saindo.\n");
                    LiberarTudo();
                    return 0;
                }
                else if (Verificacao == 2) {
                    printf("Erro: Voce nao digitou uma string valida!\n");
                    continue;
                }

                int tipo_idx = IndiceParaString(nomePilha);
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