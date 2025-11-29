#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#define CAP_PARAFINA 50
#define CAP_LEASH 25
#define CAP_QUILHA 10
#define CAP_DECK 5

enum { TYPE_PARAFINA = 0, TYPE_LEASH = 1, TYPE_QUILHA = 2, TYPE_DECK = 3, NUM_TYPES = 4 };
static const char* PRODUCT_TYPE_NAMES[NUM_TYPES] = { "Parafina", "Leash", "Quilha", "Deck" };

typedef struct Produto {
    int codigo;
    char tipo_produto[32];
    char descricao[64];
    float preco;
    struct Produto* next;
    struct Produto* prev;
} ListaProdutos;

ListaProdutos* headP = NULL;
ListaProdutos* tailP = NULL;
int tamP = 0;

typedef struct LC {
    int codigo;
    struct LC* next;
} ListaCodigos;

typedef struct caixa {
    int codigo;
    int quantP;
    ListaCodigos* lista_codigos;
    struct caixa* below;
} Caixa;

Caixa* topoParafina = NULL;
Caixa* topoLeash = NULL;
Caixa* topoQuilha = NULL;
Caixa* topoDeck = NULL;

typedef struct fila {
    char nomeCliente[64];
    char cpfCliente[16];
    int cepCliente;
    char ruaCliente[64];
    int numeroCasaCliente;
    char complementoCasaCliente[64];
    ListaProdutos* ProdutoVendido;
    struct fila* next;
} FilaEntrega;

FilaEntrega* headFE = NULL;
FilaEntrega* tailFE = NULL;
int tamFE = 0;

void AddPilhaCaixa(int, const char*);
void AdicionarLPV(int, const char*, const char*, float);
void AdicionarFE(const char*, const char*, int, const char*, int, const char*, ListaProdutos*);
void Imprimir(int);
void ImprimirPilha(const char*, Caixa*, int);
void ImprimirPilhas();
void ImprimirVendaC(const char*);
void ImprimirVendaP(float, float);
void ImprimirProdutoNode(const ListaProdutos* );
void ImprimirEntregaNode(const FilaEntrega*);
void LiberarCodigos(ListaCodigos*);
void LiberarCaixas(Caixa**);
void LiberarProdutos();
void LiberarTudo();
int PopCodigo(ListaCodigos**);
void PushCodigo(ListaCodigos**, int);
void PushCaixa(Caixa**, Caixa*);
void RemoverProdutoDaPilha(int, const char*);
ListaProdutos* VendaProduto(int);
void ComfirmacaoSE();
void limpar_buffer_stdin();
int tipo_index_from_string(const char*);
bool CodigoExiste(int);

static void handle_scanf_result(int);

static int strings_equal_ci(const char*, const char*);

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
        int r = scanf_s("%d", &escolha);
        handle_scanf_result(r);
        if (r != 1) {
            printf("Entrada invalida. Tente novamente.\n");
            limpar_buffer_stdin();
            escolha = -1;
            continue;
        }
        limpar_buffer_stdin();

        switch (escolha) {

        case 1: {
            int codigo = 0;
            char tipo_produto[32];
            char descricao[64];
            float preco = 0;

            printf("Digite o codigo do produto: ");
            r = scanf_s("%d", &codigo);
            handle_scanf_result(r);
            if (r != 1) {
                printf("Entrada invalida.\n");
                limpar_buffer_stdin();
                break;
            }
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
            r = scanf_s("%f", &preco);
            handle_scanf_result(r);
            if (r != 1) {
                printf("Entrada invalida.\n");
                limpar_buffer_stdin();
                break;
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
                printf("4 - Comfirmacao de Saida Para Entrega\n");
                printf("0 - Sair do Menu de Vendas\n");
                printf("Escolha: ");
                r = scanf_s("%d", &opcao);
                handle_scanf_result(r);
                if (r != 1) {
                    printf("Entrada invalida. Tente novamente.\n");
                    limpar_buffer_stdin();
                    opcao = -1;
                    continue;
                }
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
                    r = scanf_s("%f", &valorI);
                    handle_scanf_result(r);
                    if (r != 1) {
                        printf("Entrada invalida.\n");
                        limpar_buffer_stdin();
                        break;
                    }
                    limpar_buffer_stdin();
                    printf("Digite o Valor Final: ");
                    r = scanf_s("%f", &valorF);
                    handle_scanf_result(r);
                    if (r != 1) {
                        printf("Entrada invalida.\n");
                        limpar_buffer_stdin();
                        break;
                    }
                    limpar_buffer_stdin();
                    ImprimirVendaP(valorI, valorF);
                    break;
                }
                case 3: {
                    int codigoVenda = -1;
                    printf("Digite o codigo do produto para venda: ");
                    r = scanf_s("%d", &codigoVenda);
                    handle_scanf_result(r);
                    if (r != 1) {
                        printf("Entrada invalida. Tente novamente.\n");
                        limpar_buffer_stdin();
                        codigoVenda = -1;
                        break;
                    }
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
                    r = scanf_s("%d", &cepCliente);
                    handle_scanf_result(r);
                    if (r != 1) {
                        printf("Entrada invalida. Tente novamente.\n");
                        limpar_buffer_stdin();
                        cepCliente = 0;
                        break;
                    }
                    limpar_buffer_stdin();

                    printf("Digite o Nome da Rua do Cliente: ");
                    if (fgets(ruaCliente, sizeof(ruaCliente), stdin) == NULL) {
                        printf("Entrada invalida. Saindo.\n");
                        LiberarTudo();
                        return 0;
                    }
                    ruaCliente[strcspn(ruaCliente, "\n")] = 0;

                    printf("Digite o Numero da Casa do Cliente: ");
                    r = scanf_s("%d", &numeroCasaCliente);
                    handle_scanf_result(r);
                    if (r != 1) {
                        printf("Entrada invalida. Tente novamente.\n");
                        limpar_buffer_stdin();
                        numeroCasaCliente = 0;
                        break;
                    }
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
                    ComfirmacaoSE();
                    printf("-----------------------------------------------------------\n");
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
            r = scanf_s("%d", &pos);
            handle_scanf_result(r);
            if (r != 1) {
                printf("Entrada invalida.\n");
                limpar_buffer_stdin();
                break;
            }
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

                int tipo_idx = tipo_index_from_string(nomePilha);
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

int tipo_index_from_string(const char* s) {
    if (s == NULL) return -1;
    char buf[64];
    size_t i = 0;
    while (*s != '\0' && isspace((unsigned char)*s)) s++;
    while (*s != '\0' && i + 1 < sizeof(buf)) {
        buf[i++] = *s++;
    }
    while (i > 0 && isspace((unsigned char)buf[i - 1])) i--;
    buf[i] = '\0';

    for (int j = 0; j < NUM_TYPES; j++) {
        if (strings_equal_ci(buf, PRODUCT_TYPE_NAMES[j])) return j;
    }
    return -1;
}

void AddPilhaCaixa(int codigoProduto, const char* tipo_produto) {
    Caixa** pTopo = NULL;
    int capacidadeMax = 0;

    int tipo_idx = tipo_index_from_string(tipo_produto);
    if (tipo_idx < 0 || tipo_idx >= NUM_TYPES) {
        printf("ERRO ESTOQUE: Tipo de produto '%s' desconhecido.\n", tipo_produto);
        return;
    }
    Caixa** pilha_ptrs[NUM_TYPES] = { &topoParafina, &topoLeash, &topoQuilha, &topoDeck };
    int capacities[NUM_TYPES] = { CAP_PARAFINA, CAP_LEASH, CAP_QUILHA, CAP_DECK };

    pTopo = pilha_ptrs[tipo_idx];
    capacidadeMax = capacities[tipo_idx];

    Caixa* topoAtual = *pTopo;
    if (topoAtual == NULL || topoAtual->quantP >= capacidadeMax) {
        Caixa* novaCaixa = (Caixa*)malloc(sizeof(Caixa));
        if (novaCaixa == NULL) {
            perror("malloc falhou para nova caixa");
            return;
        }

        static int global_caixa_id = 1;
        novaCaixa->codigo = global_caixa_id++;

        novaCaixa->quantP = 0;
        novaCaixa->lista_codigos = NULL;
        novaCaixa->below = topoAtual;
        *pTopo = novaCaixa;
        topoAtual = novaCaixa;
    }

    ListaCodigos* novoCodigo = (ListaCodigos*)malloc(sizeof(ListaCodigos));
    if (novoCodigo == NULL) {
        perror("malloc falhou para novo codigo");
        return;
    }

    novoCodigo->codigo = codigoProduto;
    novoCodigo->next = topoAtual->lista_codigos;
    topoAtual->lista_codigos = novoCodigo;
    topoAtual->quantP++;
    return;
}

void ImprimirPilha(const char* nome, Caixa* topo, int capacidade) {
    printf("\n--- Pilha de %s --- (Capacidade por caixa: %d)\n", nome, capacidade);
    Caixa* aux = topo;
    if (aux == NULL) {
        printf("  (Pilha Vazia)\n");
        return;
    }

    int nivel = 0;
    while (aux != NULL) {
        if (nivel == 0) {
            printf("  Nivel %d (Topo): [Caixa Cod: %d] Qtd: %d/%d\n", nivel, aux->codigo, aux->quantP, capacidade);
        }
        else {
            printf("  Nivel %d: [Caixa Cod: %d] Qtd: %d/%d\n", nivel, aux->codigo, aux->quantP, capacidade);
        }

        ListaCodigos* cod = aux->lista_codigos;
        printf("    Codigos na Caixa: ");
        if (cod == NULL) {
            printf("(vazio)");
        }
        while (cod != NULL) {
            printf("| %d |", cod->codigo);
            cod = cod->next;
        }
        printf("\n");

        aux = aux->below;
        nivel++;
    }
}

void ImprimirPilhas() {
    printf("\n============================================\n");
    printf("     ESTADO ATUAL DO ESTOQUE     \n");
    printf("============================================\n");
    ImprimirPilha("Parafina", topoParafina, CAP_PARAFINA);
    ImprimirPilha("Leash", topoLeash, CAP_LEASH);
    ImprimirPilha("Quilha", topoQuilha, CAP_QUILHA);
    ImprimirPilha("Deck", topoDeck, CAP_DECK);
    printf("============================================\n");
}

void limpar_buffer_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void AdicionarLPV(int codigo, const char* tipo_produto, const char* descricao, float preco) {
    if (CodigoExiste(codigo)) {
        printf("Erro: Codigo %d ja cadastrado. Operacao cancelada.\n", codigo);
        return;
    }

    ListaProdutos* novo = (ListaProdutos*)malloc(sizeof(ListaProdutos));
    if (novo == NULL) {
        perror("malloc falhou");
        return;
    }

    novo->codigo = codigo;
    {
        const char* tp = tipo_produto ? tipo_produto : "";
        size_t len = strlen(tp);
        if (len >= sizeof(novo->tipo_produto)) len = sizeof(novo->tipo_produto) - 1;
        memcpy(novo->tipo_produto, tp, len);
        novo->tipo_produto[len] = '\0';
    }
    {
        const char* d = descricao ? descricao : "";
        size_t len = strlen(d);
        if (len >= sizeof(novo->descricao)) len = sizeof(novo->descricao) - 1;
        memcpy(novo->descricao, d, len);
        novo->descricao[len] = '\0';
    }

    novo->preco = preco;
    novo->next = novo->prev = NULL;

    if (headP == NULL) {
        headP = novo;
        tailP = novo;
    }
    else {
        if (novo->preco < headP->preco) {
            novo->next = headP;
            headP->prev = novo;
            headP = novo;
        }
        else if (novo->preco >= tailP->preco) {
            tailP->next = novo;
            novo->prev = tailP;
            tailP = novo;
        }
        else {
            ListaProdutos* aux = headP->next;
            while (aux != NULL) {
                if (novo->preco < aux->preco) {
                    novo->next = aux;
                    novo->prev = aux->prev;
                    aux->prev->next = novo;
                    aux->prev = novo;
                    break;
                }
                aux = aux->next;
            }
        }
    }
    tamP++;
    AddPilhaCaixa(codigo, tipo_produto);
}

void AdicionarFE(const char* nomeCliente, const char* cpfCliente, int cepCliente, const char* ruaCliente, int numeroCasaCliente, const char* complementoCasaCliente, ListaProdutos* produtoVendido) {
    FilaEntrega* NovaEntrega = (FilaEntrega*)malloc(sizeof(FilaEntrega));
    if (NovaEntrega == NULL) {
        perror("malloc falhou");
        return;
    }
    {
        const char* nc = nomeCliente ? nomeCliente : "";
        size_t len = strlen(nc);
        if (len >= sizeof(NovaEntrega->nomeCliente)) len = sizeof(NovaEntrega->nomeCliente) - 1;
        memcpy(NovaEntrega->nomeCliente, nc, len);
        NovaEntrega->nomeCliente[len] = '\0';
    }
    {
        const char* cpfc = cpfCliente ? cpfCliente : "";
        size_t len = strlen(cpfc);
        if (len >= sizeof(NovaEntrega->cpfCliente)) len = sizeof(NovaEntrega->cpfCliente) - 1;
        memcpy(NovaEntrega->cpfCliente, cpfc, len);
        NovaEntrega->cpfCliente[len] = '\0';
    }
    NovaEntrega->cepCliente = cepCliente;
    {
        const char* rc = ruaCliente ? ruaCliente : "";
        size_t len = strlen(rc);
        if (len >= sizeof(NovaEntrega->ruaCliente)) len = sizeof(NovaEntrega->ruaCliente) - 1;
        memcpy(NovaEntrega->ruaCliente, rc, len);
        NovaEntrega->ruaCliente[len] = '\0';
    }
    NovaEntrega->numeroCasaCliente = numeroCasaCliente;
    {
        const char* ccc = complementoCasaCliente ? complementoCasaCliente : "";
        size_t len = strlen(ccc);
        if (len >= sizeof(NovaEntrega->complementoCasaCliente)) len = sizeof(NovaEntrega->complementoCasaCliente) - 1;
        memcpy(NovaEntrega->complementoCasaCliente, ccc, len);
        NovaEntrega->complementoCasaCliente[len] = '\0';
    }
    NovaEntrega->ProdutoVendido = produtoVendido;
    NovaEntrega->next = NULL;
    if (headFE == NULL) {
        headFE = NovaEntrega;
        tailFE = NovaEntrega;
    }
    else if(tamFE==1) {
        headFE->next = NovaEntrega;
        tailFE = NovaEntrega;
    }
    else {
        tailFE->next = NovaEntrega;
        tailFE = NovaEntrega;
    }
    tamFE++;
    printf("-----------------------------------------------------------");
    printf("\nProduto Adicionado a Fila de Entrega");
    return;
}

bool CodigoExiste(int codigo) {
    ListaProdutos* cur = headP;
    while (cur != NULL) {
        if (cur->codigo == codigo) return true;
        cur = cur->next;
    }
    return false;
}

void Imprimir(int pos) {
    if (headP == NULL) {
        printf("\nLista esta vazia.\n");
        return;
    }

    if (pos == -1) {
        ListaProdutos* aux = headP;
        printf("\n--- Imprimindo Lista Completa (%d elementos) ---\n", tamP);
        while (aux != NULL) {
            ImprimirProdutoNode(aux);
            aux = aux->next;
        }
        printf("---------------------------------------------\n");
        return;
    }
    else {
        if (pos < 0 || pos >= tamP) {
            printf("Erro: Posicao %d eh invalida. (Tamanho atual: %d)\n", pos, tamP);
            return;
        }
        ListaProdutos* aux;
        if (pos <= tamP / 2) {
            aux = headP;
            for (int i = 0; i < pos; i++) aux = aux->next;
        }
        else {
            aux = tailP;
            for (int i = tamP - 1; i > pos; i--) aux = aux->prev;
        }
        printf("\n--- Imprimindo Indice [%d] ---\n", pos);
        printf("  Codigo: %d\n", aux->codigo);
        printf("  Tipo: %s\n", aux->tipo_produto);
        printf("  Descricao: %s\n", aux->descricao);
        printf("  Preco: R$ %.2f\n", aux->preco);
        printf("----------------------------------\n");
    }
}

void ImprimirVendaC(const char* tipo_produto) {
    if (headP == NULL) {
        printf("Nao tem produtos cadastrados!");
        return;
    }
    ListaProdutos* aux = headP;
    while (aux != NULL) {
        if (strings_equal_ci(aux->tipo_produto, tipo_produto)) {
            ImprimirProdutoNode(aux);
        }
        aux = aux->next;
    }
    return;
}

void ImprimirVendaP(float valorI, float valorF) {
    if (headP == NULL) {
        printf("Nao tem produtos cadastrados!");
        return;
    }
    ListaProdutos* aux = headP;
    while (aux != NULL) {
        if (aux->preco >= valorI && aux->preco <= valorF) {
            ImprimirProdutoNode(aux);
        }
        aux = aux->next;
    }
    return;
}

void RemoverProdutoDaPilha(int codigoProduto, const char* tipo_produto) {
    int tipo_idx = tipo_index_from_string(tipo_produto);
    if (tipo_idx < 0 || tipo_idx >= NUM_TYPES) {
        printf("ERRO ESTOQUE: Tipo de produto '%s' desconhecido.\n", tipo_produto);
        return;
    }
    
    Caixa** pilha_ptrs[NUM_TYPES] = { &topoParafina, &topoLeash, &topoQuilha, &topoDeck };
    int capacities[NUM_TYPES] = { CAP_PARAFINA, CAP_LEASH, CAP_QUILHA, CAP_DECK };

    Caixa** pTopo = pilha_ptrs[tipo_idx];
    int capacidadeMax = capacities[tipo_idx];

    Caixa* pilhaAux = NULL;
    Caixa* caixaEncontrada = NULL;
    while (*pTopo != NULL) {
        Caixa* caixaAtual = *pTopo;
        *pTopo = (*pTopo)->below;
        caixaAtual->below = NULL;

        ListaCodigos* cod = caixaAtual->lista_codigos;
        ListaCodigos* prev = NULL;
        bool found = false;
        while (cod != NULL) {
            if (cod->codigo == codigoProduto) {
                if (prev == NULL) {
                    caixaAtual->lista_codigos = cod->next;
                }
                else {
                    prev->next = cod->next;
                }
                free(cod);
                caixaAtual->quantP--;
                caixaEncontrada = caixaAtual;
                found = true;
                break;
            }
            prev = cod;
            cod = cod->next;
        }

        if (found) {
            break;
        }
        else {
            PushCaixa(&pilhaAux, caixaAtual);
        }
    }

    if (caixaEncontrada == NULL) {
        while (pilhaAux != NULL) {
            Caixa* temp = pilhaAux;
            pilhaAux = pilhaAux->below;
            PushCaixa(pTopo, temp);
        }
        printf("Venda Falhou: Nao foi encontrado o codigo %d na pilha %s.\n", codigoProduto, tipo_produto);
        return;
    }

    Caixa* caixaEquilibrio = NULL;
    if (pilhaAux != NULL) {
        caixaEquilibrio = pilhaAux;
        pilhaAux = pilhaAux->below;
        caixaEquilibrio->below = NULL;
    }

    while (pilhaAux != NULL) {
        Caixa* temp = pilhaAux;
        pilhaAux = pilhaAux->below;
        PushCaixa(pTopo, temp);
    }
    if (caixaEquilibrio != NULL) {
        if (caixaEncontrada->quantP < capacidadeMax) {
            int codEquilibrio = PopCodigo(&(caixaEquilibrio->lista_codigos));
            if (codEquilibrio != -1) {
                PushCodigo(&(caixaEncontrada->lista_codigos), codEquilibrio);
                caixaEquilibrio->quantP--;
                caixaEncontrada->quantP++;
            }
        }
    }

    PushCaixa(pTopo, caixaEncontrada);

    if (caixaEquilibrio != NULL) {
        if (caixaEquilibrio->quantP == 0) {
            printf("Info Estoque: Caixa %d ficou vazia e foi descartada.\n", caixaEquilibrio->codigo);
            free(caixaEquilibrio);
        }
        else {
            PushCaixa(pTopo, caixaEquilibrio);
        }
    }
}

ListaProdutos* VendaProduto(int codigo) {
    ListaProdutos* aux = headP;
    while (aux != NULL && aux->codigo != codigo) aux = aux->next;
    if (aux == NULL) {
        printf("Venda Falhou: Nao foi encontrado nenhum produto com o codigo %d\n", codigo);
        return NULL;
    }
    RemoverProdutoDaPilha(aux->codigo, aux->tipo_produto);

    if (aux == headP) {
        headP = aux->next;
        if (headP != NULL) headP->prev = NULL;
        else tailP = NULL;
    }
    else if (aux == tailP) {
        tailP = aux->prev;
        tailP->next = NULL;
    }
    else {
        aux->prev->next = aux->next;
        aux->next->prev = aux->prev;
    }

    tamP--;
    aux->next = NULL;
    aux->prev = NULL;

    printf("Produto %s (Cod: %d) removido da lista e do estoque.\n", aux->tipo_produto, aux->codigo);
    return aux;
}

void ComfirmacaoSE() {
    FilaEntrega* aux = headFE;
    if (aux == NULL) {
        printf("Nao tem Produtos na Fila de entrega");
        return;
    }
    else {
        ImprimirEntregaNode(aux);
        headFE = headFE->next;
        free(aux);
    }
    return;
}

void PushCaixa(Caixa** pTopo, Caixa* caixa) {
    if (caixa == NULL) return;
    caixa->below = *pTopo;
    *pTopo = caixa;
}

int PopCodigo(ListaCodigos** head) {
    if (head == NULL || *head == NULL) return -1;
    ListaCodigos* node = *head;
    int codigo = node->codigo;
    *head = node->next;
    free(node);
    return codigo;
}

void PushCodigo(ListaCodigos** head, int codigo) {
    ListaCodigos* node = (ListaCodigos*)malloc(sizeof(ListaCodigos));
    if (node == NULL) {
        perror("malloc falhou para PushCodigo");
        return;
    }
    node->codigo = codigo;
    node->next = *head;
    *head = node;
}
void LiberarCodigos(ListaCodigos* head) {
    ListaCodigos* cur = head;
    while (cur != NULL) {
        ListaCodigos* prox = cur->next;
        free(cur);
        cur = prox;
    }
}
void LiberarCaixas(Caixa** topo_ptr) {
    if (topo_ptr == NULL) return;
    Caixa* caixa = *topo_ptr;
    while (caixa != NULL) {
        Caixa* abaixo = caixa->below;
        LiberarCodigos(caixa->lista_codigos);
        free(caixa);
        caixa = abaixo;
    }
    *topo_ptr = NULL;
}

void LiberarProdutos(void) {
    ListaProdutos* cur = headP;
    while (cur != NULL) {
        ListaProdutos* prox = cur->next;
        free(cur);
        cur = prox;
    }
    headP = NULL;
    tailP = NULL;
    tamP = 0;
}
void LiberarTudo(void) {
    LiberarProdutos();
    LiberarCaixas(&topoParafina);
    LiberarCaixas(&topoLeash);
    LiberarCaixas(&topoQuilha);
    LiberarCaixas(&topoDeck);
}

void ImprimirProdutoNode(const ListaProdutos* node) {
    if (node == NULL) return;
    printf("---------------------------------------------\n");
    printf("Produto:\n");
    printf("  Codigo: %d\n", node->codigo);
    printf("  Tipo: %s\n", node->tipo_produto);
    printf("  Descricao: %s\n", node->descricao);
    printf("  Preco: R$ %.2f\n", node->preco);
    return;
}

void ImprimirEntregaNode(const FilaEntrega* node) {
    if (node == NULL) return;
    printf("---------------------------------------------\n");
    printf("Nome do Cliente: %s\n", node->nomeCliente);
    printf("CPF do Cliente: %s\n", node->cpfCliente);
    printf("CEP do Cliente: %d\n", node->cepCliente);
    printf("Rua do Cliente: %s\n", node->ruaCliente);
    printf("Numero da Casa do Cliente: %d\n", node->numeroCasaCliente);
    printf("Complemento da Casa do Cliente: %s\n", node->complementoCasaCliente);
    ImprimirProdutoNode(node->ProdutoVendido);
    return;
}

static void handle_scanf_result(int res) {
    if (res == EOF) {
        printf("EOF detectado. Saindo.\n");
        LiberarTudo();
        exit(0);
    }
}

static int strings_equal_ci(const char* a, const char* b) {
    if (a == NULL || b == NULL) return 0;
    while (*a != '\0' && *b != '\0') {
        char ca = tolower((unsigned char)*a);
        char cb = tolower((unsigned char)*b);
        if (ca != cb) return 0;
        a++; b++;
    }
    return *a == *b;
}