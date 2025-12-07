#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include "headfunctions.h"

static bool strings_equal_ci(const char* a, const char* b) {
    if (a == NULL || b == NULL) return false;
    while (*a != '\0' && *b != '\0') {
        char ca = tolower((unsigned char)*a);
        char cb = tolower((unsigned char)*b);
        if (ca != cb) return false;
        a++; b++;
    }
    return *a == *b;
}

int tipo_index_para_string(const char* s) {
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

bool AddPilhaCaixa(int codigoProduto, const char* tipo_produto) {
    int tipo_idx = tipo_index_para_string(tipo_produto);
    if (tipo_idx < 0 || tipo_idx >= NUM_TYPES) {
        printf("ERRO ESTOQUE: Tipo de produto '%s' desconhecido.\n", tipo_produto);
        return true;
    }

    Caixa** pTopo = NULL;
    int capacidadeMax = 0;
    Caixa** pilha_ptrs[NUM_TYPES] = { &topoParafina, &topoLeash, &topoQuilha, &topoDeck };
    int capacities[NUM_TYPES] = { CAP_PARAFINA, CAP_LEASH, CAP_QUILHA, CAP_DECK };

    pTopo = pilha_ptrs[tipo_idx];
    capacidadeMax = capacities[tipo_idx];

    Caixa* topoAtual = NULL;
    if (*pTopo == NULL) {
        Caixa* novaCaixa = (Caixa*)malloc(sizeof(Caixa));
        if (novaCaixa == NULL) {
            perror("malloc falhou para nova caixa");
            return true;
        }
        novaCaixa->codigo = caixa_id_counters[tipo_idx]++;
        novaCaixa->quantP = 0;
        novaCaixa->lista_codigos = NULL;
        novaCaixa->below = NULL;

        *pTopo = novaCaixa;
        topoAtual = novaCaixa;
    }
    else {
        Caixa* cur = *pTopo;
        while (cur != NULL) {
            if (cur->quantP < capacidadeMax) {
                topoAtual = cur;
            }
            break;
        }
        if (topoAtual == NULL) {
            Caixa* novaCaixa = (Caixa*)malloc(sizeof(Caixa));
            if (novaCaixa == NULL) {
                perror("malloc falhou para nova caixa extra");
                return true;
            }
            novaCaixa->codigo = caixa_id_counters[tipo_idx]++;
            novaCaixa->quantP = 0;
            novaCaixa->lista_codigos = NULL;

            novaCaixa->below = *pTopo;
            *pTopo = novaCaixa;
            topoAtual = novaCaixa;
        }
    }
    ListaCodigos* novoCodigo = (ListaCodigos*)malloc(sizeof(ListaCodigos));
    if (novoCodigo == NULL) {
        perror("malloc falhou para novo codigo");
        return true;
    }

    novoCodigo->codigo = codigoProduto;
    novoCodigo->next = topoAtual->lista_codigos;
    topoAtual->lista_codigos = novoCodigo;

    topoAtual->quantP++;

    return false;
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

    if (AddPilhaCaixa(codigo, tipo_produto)) {
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
    else if (tamFE == 1) {
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

bool RemoverProdutoDaPilha(int codigoProduto, const char* tipo_produto) {
    int tipo_idx = tipo_index_para_string(tipo_produto);
    if (tipo_idx < 0 || tipo_idx >= NUM_TYPES) {
        printf("ERRO ESTOQUE: Tipo de produto '%s' desconhecido.\n", tipo_produto);
        return true;
    }

    Caixa* pilha_ptrs[NUM_TYPES] = { topoParafina, topoLeash, topoQuilha, topoDeck };
    int capacities[NUM_TYPES] = { CAP_PARAFINA, CAP_LEASH, CAP_QUILHA, CAP_DECK };

    Caixa* pTopo = pilha_ptrs[tipo_idx];
    int capacidadeMax = capacities[tipo_idx];

    Caixa* topoPilhaAux = NULL;
    Caixa* enderecoCaixaAbaixo = NULL;
    Caixa* caixaAntiga = NULL;
    int contadorPush = 0;

    while (pTopo != NULL) {
        ListaCodigos* posCerta = pTopo->lista_codigos;
        for (int i=0; i<pTopo->quantP; i++){
            if (posCerta->codigo == codigoProduto) {
                break;
            }
            posCerta = posCerta->next;
        }
        if (posCerta != NULL) {
            if (contadorPush == 0) {
                pTopo->quantP--;
                pTopo->lista_codigos = posCerta->next;
                free(posCerta);
                return false;
            }
            else {
                for (int i = 0; i < contadorPush; i++) {
                    if (i == contadorPush - 1) {
                        ListaCodigos* ultimaPos = topoPilhaAux->lista_codigos;
                        ListaCodigos* penultimaPos = NULL;
                        for (int i = 0; i < topoPilhaAux->quantP; i++) {
                            if (ultimaPos->next == NULL) {
                                posCerta->codigo = ultimaPos->codigo;
                                penultimaPos->next = NULL;
                                free(ultimaPos);
                                topoPilhaAux->quantP--;
                                break;
                            }
                            penultimaPos = ultimaPos;
                            ultimaPos = ultimaPos->next;
                        }
                    }
                    enderecoCaixaAbaixo = topoPilhaAux->below;
                    topoPilhaAux->below = pTopo;
                    pTopo = topoPilhaAux;
                    topoPilhaAux = enderecoCaixaAbaixo;
                }
            }
            return false;
        }
        else {
            caixaAntiga = topoPilhaAux;
            topoPilhaAux = pTopo;
            pTopo = pTopo->below;
            topoPilhaAux->below = caixaAntiga;
            contadorPush++;
        }
    }
    return true;
}

ListaProdutos* VendaProduto(int codigoProduto) {
    ListaProdutos* aux = headP;
    while (aux != NULL && aux->codigo != codigoProduto) aux = aux->next;
    if (aux == NULL) {
        printf("Venda Falhou: Nao foi encontrado nenhum produto com o codigo %d\n", codigoProduto);
        return NULL;
    }
    if (RemoverProdutoDaPilha(aux->codigo, aux->tipo_produto)) {
        printf("O Codigo: %d nao foi encontrado!", codigoProduto);
        return NULL;
    }

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
