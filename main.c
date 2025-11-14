#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Para strcspn, strcmp e strncpy_s
#include <stdbool.h>
#include <ctype.h>

#define CAP_PARAFINA 50 //definição das capcidades das caixas
#define CAP_LEASH 25
#define CAP_QUILHA 10
#define CAP_DECK 5

// Mapeamento de tipos para evitar strcmp repetidos
//a versão 0.6 do codigo tava com muito strcmp que poderiam ser subistuidos, isso se manteve até
//a versão 0.9 do codigo.
//na versão 1.0 o codigo está mais limpo, mais otimizado e com a complexidade revisada.
//versão 1.1 será com o código em inglês... Working on it...

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

ListaProdutos* head = NULL;
ListaProdutos* tail = NULL;
int tam = 0;

typedef struct LC { //Lista de codigos encadeados para cada caixa
    int codigo;
    struct LC* next;
} ListaCodigos;

typedef struct caixa { //caixa generica
    int codigo;
    int quantP;
    ListaCodigos* lista_codigos;
    struct caixa* below;
} Caixa;

Caixa* topoParafina = NULL; //caixa especificas
Caixa* topoLeash = NULL;
Caixa* topoQuilha = NULL;
Caixa* topoDeck = NULL;

//Marcin, isso é chamado protótipo de função, isso é mais por estética do código.
void AddPilhaCaixa(int, const char*); //Adiciona na pilha de caixas
void AdicionarLPV(int, const char*, const char*, float); //Adiciona na Lista de Produtos para Venda
void Imprimir(int); //Imprimi uma posição ou todas as posições
void ImprimirPilha(const char* nome, Caixa* topo, int capacidade); //imprimi uma pilha de caixas
void ImprimirPilhas(); //imprimi todas as pilhas (4)
void ImprimirVendaC(const char*); //imprimi os produtos pesquisando por categoria (tipo_produto)
void ImprimirVendaP(float, float); //imprimi os produtos pesquisando por faixa de preco
void LiberarCodigos(ListaCodigos* head); //libera a lista de codigos de cada caixa
void LiberarCaixas(Caixa** topo_ptr); //libera cada caixa
void LiberarProdutos(); //libera a lista de produtos
void LiberarTudo(); //junta todas as funções de liberar em só uma
ListaProdutos* VendaProduto(int); //função principal para chamar a remoção e retornar o que foi removido
void RemoverProdutoDoEstoque(int, const char*); //função para remover do estoque
int PopCodigo(ListaCodigos**); //remove o primeiro codigo da lista e retorna o codigo removido ou -1 se vazio
void PushCodigo(ListaCodigos**, int); //a mesma coisa
void PushCaixa(Caixa**, Caixa*); //para não precisar retirar cada caixa para pesquisar
void limpar_buffer_stdin(); //Essa função é para limpar o '\n' que o scanf_s deixa no buffer, que o professor de Arq explicou.
void Remover(int pos); // assinado como void agora
void ImprimirProdutoNode(const ListaProdutos* node, int index); // imprime um nó diretamente (evita O(n^2)) mais otimização de codigo
int tipo_index_from_string(const char* s); //em vez de um monte de strcmp eu uso a função

//Todo scanf tem um valor de retorno, quando inserido o valor correto que ele espera...
//Temos o retorno 1, mas quando não...
//Temos o retorno 0, por isso limpamos e voltamos ao inicio do loop.

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
        if (scanf_s("%d", &escolha) != 1) { //Verificação de entrada, para saber se foi inserido um inteiro
            printf("Entrada invalida. Tente novamente.\n");
            limpar_buffer_stdin(); // Limpa entrada errada
            escolha = -1; // Reseta a escolha
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
            if (scanf_s("%d", &codigo) != 1) {
                printf("Entrada invalida.\n");
                limpar_buffer_stdin();
                break;
            }
            limpar_buffer_stdin();
            printf("Digite do tipo de produto: ");
            fgets(tipo_produto, sizeof(tipo_produto), stdin);
            tipo_produto[strcspn(tipo_produto, "\n")] = 0; // Remove o '\n'
            //Caso o que escrevermos chegar a 31 caracteres vai quebrar o codigo.
            //Temos somente o buffer do sizeof(tipo_produto) que é -1 do que o normal,
            //Então vai sobrar o \n para o proximo fgets. Voltando para o problema anterior.
            printf("Digite a descricao o produto: ");
            fgets(descricao, sizeof(descricao), stdin);
            descricao[strcspn(descricao, "\n")] = 0;

            printf("Digite o preco do produto: ");
            if (scanf_s("%f", &preco) != 1) {
                printf("Entrada invalida.\n");
                limpar_buffer_stdin();
                break;
            }

            AdicionarLPV(codigo, tipo_produto, descricao, preco);
            break;
        }

        case 2: {
            int opcao = -1;
            do {
                printf("\n--- MENU DE VENDAS ---\n");
                printf("1 - Ver Produtos Por Categoria\n");
                printf("2 - Ver Produtos Por Faixa de Preco\n");
                printf("3 - Vender Produto\n");
                printf("0 - Sair do Menu de Vendas\n");
                printf("Escolha: ");
                if (scanf_s("%d", &opcao) != 1) {
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
                    fgets(categoria, sizeof(categoria), stdin);
                    categoria[strcspn(categoria, "\n")] = 0;
                    ImprimirVendaC(categoria);
                    break;
                }
                case 2: {
                    float valorI, valorF;
                    printf("Digite o Valor Inicial: ");
                    if (scanf_s("%f", &valorI) != 1) {
                        printf("Entrada invalida.\n");
                        limpar_buffer_stdin();
                        break;
                    }
                    limpar_buffer_stdin();
                    printf("Digite o Valor Final: ");
                    if (scanf_s("%f", &valorF) != 1) {
                        printf("Entrada invalida.\n");
                        limpar_buffer_stdin();
                        break;
                    }
                    limpar_buffer_stdin();
                    ImprimirVendaP(valorI, valorF);
                    break;
                }
                case 3: {

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
            printf("Digite a posicao desejada de 0 a %d (-1 para todos): ", tam - 1);
            if (scanf_s("%d", &pos) != 1) {
                printf("Entrada invalida.\n");
                limpar_buffer_stdin();
                break;
            }
            Imprimir(pos);
            break;
        }
        case 4: { //Ver somente uma das pilhas (Versão Refatorada)
            char nomePilha[64];
            printf("Digite o Nome da Pilha (Parafina, Leash, Quilha, Deck): ");
            fgets(nomePilha, sizeof(nomePilha), stdin);
            nomePilha[strcspn(nomePilha, "\n")] = 0;

            //Converte a string em um índice
            int tipo_idx = tipo_index_from_string(nomePilha);

            //Cria "mapas" para os ponteiros e capacidades
            Caixa* pilhas[NUM_TYPES] = { topoParafina, topoLeash, topoQuilha, topoDeck };
            int capacities[NUM_TYPES] = { CAP_PARAFINA, CAP_LEASH, CAP_QUILHA, CAP_DECK };

            //Verifica o índice e chama a função
            if (tipo_idx >= 0 && tipo_idx < NUM_TYPES) {
                //Usa o índice para pegar os dados certos dos mapas
                ImprimirPilha(
                    PRODUCT_TYPE_NAMES[tipo_idx], // Nome: "Parafina"
                    pilhas[tipo_idx],             // Ponteiro: topoParafina
                    capacities[tipo_idx]          // Capacidade: CAP_PARAFINA
                );
            }
            else {
                //Se o índice for -1, o tipo é desconhecido
                printf("ERRO ESTOQUE: Tipo de produto '%s' desconhecido.\n", nomePilha);
            }
            break;
        }
        case 5: {
            ImprimirPilhas();
            break;
        }

        case 0: {
            printf("Obrigado! Ate mais.\n");
            break;
        }

        default: {
            printf("Opcao invalida. Tente novamente.\n");
            break;
        }
        }
    } while (escolha != 0);

    //Limpa a lista e pilhas antes de sair
    LiberarTudo();
    return 0;
}

int tipo_index_from_string(const char* s) {
    if (s == NULL) return -1;
    for (int i = 0; i < NUM_TYPES; i++) {
        if (strcmp(s, PRODUCT_TYPE_NAMES[i]) == 0) return i;
    }
    return -1;
}

void AddPilhaCaixa(int codigoProduto, const char* tipo_produto) {
    Caixa** pTopo = NULL; //um ponteiro que aponta para outro ponteiro
    int capacidadeMax = 0;

    int tipo_idx = tipo_index_from_string(tipo_produto);
    Caixa** pilha_ptrs[NUM_TYPES] = { &topoParafina, &topoLeash, &topoQuilha, &topoDeck };
    int capacities[NUM_TYPES] = { CAP_PARAFINA, CAP_LEASH, CAP_QUILHA, CAP_DECK };

    if (tipo_idx < 0 || tipo_idx >= NUM_TYPES) {
        printf("ERRO ESTOQUE: Tipo de produto '%s' desconhecido.\n", tipo_produto);
        return;
    }

    pTopo = pilha_ptrs[tipo_idx];
    capacidadeMax = capacities[tipo_idx];

    Caixa* topoAtual = *pTopo;
    if (topoAtual == NULL || topoAtual->quantP >= capacidadeMax) {

        Caixa* novaCaixa = (Caixa*)malloc(sizeof(Caixa));
        if (novaCaixa == NULL) {
            perror("malloc falhou para nova caixa");
            return;
        }

        static int global_caixa_id = 1; //Marcin, talvez você não saiba, mas static ajuda muito quando você quer definir um variavel,
        //mas não quer que ela seja reinicializada quando for utilizar de novo
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

//Função auxiliar para imprimir uma pilha
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

        aux = aux->below; // Vai para a caixa de baixo
        nivel++;
    }
}

//Função principal de impressão (chamada pelo main)
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
    ListaProdutos* novo = (ListaProdutos*)malloc(sizeof(ListaProdutos));
    if (novo == NULL) {
        perror("malloc falhou");
        return;
    }

    novo->codigo = codigo;
    //copia segura de strings para portabilidade (substitui strncpy_s), já que essa função que dá mais segurança às strings
    //só funciona no compilado MSVC do visual studio community que eu uso
    {
        size_t len = strlen(tipo_produto);
        if (len >= sizeof(novo->tipo_produto)) len = sizeof(novo->tipo_produto) - 1;
        memcpy(novo->tipo_produto, tipo_produto, len);
        novo->tipo_produto[len] = '\0';
    }
    {
        size_t len = strlen(descricao);
        if (len >= sizeof(novo->descricao)) len = sizeof(novo->descricao) - 1;
        memcpy(novo->descricao, descricao, len);
        novo->descricao[len] = '\0';
    }

    novo->preco = preco;
    novo->next = novo->prev = NULL;

    if (head == NULL) { //Lista vazia
        head = novo;
        tail = novo;
    }
    else {
        if (novo->preco < head->preco) { //Inserir no início
            novo->next = head;
            head->prev = novo;
            head = novo;
        }
        else if (novo->preco >= tail->preco) {// Inserir no fim
            tail->next = novo;
            novo->prev = tail;
            tail = novo;
        }
        else { // Inserir no meio
            ListaProdutos* aux = head->next;//Começa do segundo item
            while (aux != NULL) {
                if (novo->preco < aux->preco) {
                    novo->next = aux;
                    novo->prev = aux->prev;
                    aux->prev->next = novo;//O anterior ao aux aponta pro novo
                    aux->prev = novo;//O aux aponta pro novo
                    break;
                }
                aux = aux->next;
            }
        }
    }
    tam++;
    AddPilhaCaixa(codigo, tipo_produto);
}

void Imprimir(int pos) {
    if (head == NULL) {
        printf("\nLista esta vazia.\n");
        return;
    }

    if (pos == -1) {//Imprimir tudo
        ListaProdutos* aux = head;
        printf("\n--- Imprimindo Lista Completa (%d elementos) ---\n", tam);
        int i = 0;
        while (aux != NULL) {
            ImprimirProdutoNode(aux, i);
            aux = aux->next;
            i++;
        }
        printf("---------------------------------------------\n");
        return;
    }
    else { //Imprimir posição específica
        if (pos < 0 || pos >= tam) {
            printf("Erro: Posicao %d eh invalida. (Tamanho atual: %d)\n", pos, tam);
            return;
        }
        ListaProdutos* aux;
        if (pos <= tam / 2) {
            aux = head;
            for (int i = 0; i < pos; i++) {
                aux = aux->next;
            }
        }
        else {
            aux = tail;
            for (int i = tam - 1; i > pos; i--) {
                aux = aux->prev;
            }
        }
        printf("\n--- Imprimindo Indice [%d] ---\n", pos);
        printf("  Codigo: %d\n", aux->codigo);
        printf("  Tipo: %s\n", aux->tipo_produto);
        printf("  Descricao: %s\n", aux->descricao);
        printf("  Preco: R$ %.2f\n", aux->preco);
        printf("----------------------------------\n");
    }
}

void Remover(int pos) {
    if (head == NULL) {
        printf("Lista esta vazia.\n");
        return;
    }
    if (pos < 0 || pos >= tam) {
        printf("Erro: Posicao %d eh invalida. (Tamanho: %d)\n", pos, tam);
        return;
    }
    ListaProdutos* lixo = NULL;

    if (pos == 0) {
        lixo = head;
    }
    else if (pos == tam - 1) {
        lixo = tail;
    }
    else {
        if (pos <= tam / 2) {
            lixo = head;
            for (int i = 0; i < pos; i++) {
                lixo = lixo->next;
            }
        }
        else {
            lixo = tail;
            for (int i = tam - 1; i > pos; i--) {
                lixo = lixo->prev;
            }
        }
    }
    printf("\nVoce quer mesmo remover este item do sistema? \n");
    ImprimirProdutoNode(lixo, pos);

    printf("Escolha (S/N): ");
    char escolha = 'N';
    if (scanf_s(" %c", &escolha, 1) != 1) {
        limpar_buffer_stdin();
        printf("Entrada invalida. Cancelando.\n");
        return;
    }
    limpar_buffer_stdin();
    if (escolha == 'S' || escolha == 's') {
        if (tam == 1) {
            head = NULL;
            tail = NULL;
        }
        else if (lixo == head) {
            head = lixo->next;
            head->prev = NULL;
        }
        else if (lixo == tail) {
            tail = lixo->prev;
            tail->next = NULL;
        }
        else {
            lixo->prev->next = lixo->next;
            lixo->next->prev = lixo->prev;
        }

        free(lixo);
        tam--;
        printf("Produto da posicao %d removido com sucesso.\n", pos);
    }
    else {
        printf("Remocao cancelada.\n");
    }
}

void ImprimirVendaC(const char* tipo_produto) {
    if (head == NULL) {
        printf("Nao tem produtos cadastrados!");
        return;
    }
    ListaProdutos* aux = head;
    int i = 0;
    while (aux != NULL) {
        if (strcmp(aux->tipo_produto, tipo_produto) == 0) {
            ImprimirProdutoNode(aux, i);
        }
        aux = aux->next;
        i++;
    }
    return;
}

void ImprimirVendaP(float valorI, float valorF) {
    if (head == NULL) {
        printf("Nao tem produtos cadastrados!");
        return;
    }
    ListaProdutos* aux = head;
    int i = 0;
    while (aux != NULL) {
        if (aux->preco >= valorI && aux->preco <= valorF) {
            ImprimirProdutoNode(aux, i);
        }
        aux = aux->next;
        i++;
    }
    return;
}

//remove o produto da pilha de estoque e faz o equilíbrio
void RemoverProdutoDoEstoque(int codigoProduto, const char* tipo_produto) {
    int tipo_idx = tipo_index_from_string(tipo_produto);
    Caixa** pilha_ptrs[NUM_TYPES] = { &topoParafina, &topoLeash, &topoQuilha, &topoDeck };

    if (tipo_idx < 0 || tipo_idx >= NUM_TYPES) {
        printf("ERRO ESTOQUE: Tipo de produto '%s' desconhecido.\n", tipo_produto);
        return;
    }
    Caixa** pTopo = pilha_ptrs[tipo_idx];
    Caixa* pilhaAux = NULL;
    Caixa* caixaEncontrada = NULL;

    while (*pTopo != NULL) {
        Caixa* caixaAtual = *pTopo;
        *pTopo = (*pTopo)->below;
        caixaAtual->below = NULL;

        ListaCodigos* cod = caixaAtual->lista_codigos;
        ListaCodigos* prev = NULL;
        bool breakOuterLoop = false;

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
                breakOuterLoop = true;
                break;
            }
            prev = cod;
            cod = cod->next;
        }

        if (breakOuterLoop) {
            break;
        }
        else {
            //Não achou, joga na pilha auxiliar
            PushCaixa(&pilhaAux, caixaAtual); // <-- USANDO A FUNÇÃO PARA SALVAR NOSSAS VIDAS, AMEM
        }
    }

    //Se não encontrou o produto, restaura a pilha auxiliar e sai
    if (caixaEncontrada == NULL) {
        while (pilhaAux != NULL) {
            Caixa* temp = pilhaAux;
            pilhaAux = pilhaAux->below; //Pop da pilhaAux
            PushCaixa(pTopo, temp);    //Push na pilha principal
        }
        printf("Venda Falhou: Nao foi encontrado o codigo %d na pilha %s.\n", codigoProduto, tipo_produto);
        return;
    }
    if (pilhaAux != NULL && caixaEncontrada != NULL && caixaEncontrada->quantP > 0) {

        Caixa* caixaEquilibrio = pilhaAux;
        pilhaAux = pilhaAux->below;

        int codEquilibrio = PopCodigo(&(caixaEquilibrio->lista_codigos));

        if (codEquilibrio != -1) {
            PushCodigo(&(caixaEncontrada->lista_codigos), codEquilibrio);
            caixaEquilibrio->quantP--;
            caixaEncontrada->quantP++;

            if (caixaEquilibrio->quantP == 0) {
                printf("Info Estoque: Caixa %d ficou vazia e foi descartada.\n", caixaEquilibrio->codigo);
                free(caixaEquilibrio);
            }
            else {
                PushCaixa(pTopo, caixaEquilibrio);
            }
        }
        else {
            PushCaixa(pTopo, caixaEquilibrio);
        }
    }
    PushCaixa(pTopo, caixaEncontrada);

    while (pilhaAux != NULL) {
        Caixa* temp = pilhaAux;
        pilhaAux = pilhaAux->below;
        PushCaixa(pTopo, temp);
    }
}

ListaProdutos* VendaProduto(int codigo) {
    ListaProdutos* aux = head;
    while (aux != NULL) {
        if (aux->codigo == codigo) {
            break;
        }
        aux = aux->next;
    }

    if (aux == NULL) {
        printf("Venda Falhou: Nao foi encontrado nenhum produto com o codigo %d\n", codigo);
        return NULL;
    }

    RemoverProdutoDoEstoque(aux->codigo, aux->tipo_produto);
    if (aux == head) { //Removendo do início
        head = aux->next;
        if (head != NULL) {
            head->prev = NULL;
        }
        else {
            tail = NULL; //A lista ficou vazia
        }
    }
    else if (aux == tail) { //Removendo do fim
        tail = aux->prev;
        tail->next = NULL;
    }
    else { //Removendo do meio
        aux->prev->next = aux->next;
        aux->next->prev = aux->prev;
    }

    tam--;
    aux->next = NULL;
    aux->prev = NULL;

    printf("Produto (Cod: %d) removido da lista e do estoque.\n", aux->codigo);

    return aux;
}

void PushCaixa(Caixa** pTopo, Caixa* caixa) {
    if (caixa == NULL) {
        return; //Não podemos empilhar nada
    }
    caixa->below = *pTopo;
    *pTopo = caixa;
}

//remove o primeiro codigo da lista e retorna o codigo removido ou -1 se vazio
int PopCodigo(ListaCodigos** head) {
    if (head == NULL || *head == NULL) return -1;
    ListaCodigos* node = *head;
    int codigo = node->codigo;
    *head = node->next;
    free(node);
    return codigo;
}

//Adiciona um codigo no topo da lista de codigos
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
    ListaProdutos* cur = head;
    while (cur != NULL) {
        ListaProdutos* prox = cur->next;
        free(cur);
        cur = prox;
    }
    head = NULL;
    tail = NULL;
    tam = 0;
}

void LiberarTudo(void) {
    LiberarProdutos();
    LiberarCaixas(&topoParafina);
    LiberarCaixas(&topoLeash);
    LiberarCaixas(&topoQuilha);
    LiberarCaixas(&topoDeck);
}

//Imprime um nó da lista de produtos (usado para evitar revarrimento da lista)
void ImprimirProdutoNode(const ListaProdutos* node, int index) {
    if (node == NULL) return;
    printf("---------------------------------------------\n");
    printf("Indice [%d]:\n", index);
    printf("  Codigo: %d\n", node->codigo);
    printf("  Tipo: %s\n", node->tipo_produto);
    printf("  Descricao: %s\n", node->descricao);
    printf("  Preco: R$ %.2f\n", node->preco);
}
