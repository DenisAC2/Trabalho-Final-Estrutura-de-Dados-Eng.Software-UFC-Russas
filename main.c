#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Para strcspn e strncpy_s

typedef struct No {
    int codigo;
    char tipo_produto[32];
    char descricao[64];
    float preco;
    struct No* next;
    struct No* prev;
} ListaProdutos;

ListaProdutos* inicio = NULL;
ListaProdutos* fim = NULL;
int tam = 0;
//Marcin, isso é chamado prototipo de função, isso é mais por estética do código.
void Adicionar(int, const char*, const char*, float);
void Imprimir(int);
void Remover(int);


//Essa função é para limpar o '\n' que o scanf_s deixa no buffer, que o professor de Arq explicou.
void limpar_buffer_stdin();
//Todo scanf tem um valor de retorno, quando inserido o valor correto que ele espera...
//Temos o retorno 1, mas quando não...
//Temos o retorno 0, por isso limpamos e voltamos ao inicio do loop.
int main() {
    int escolha = -1;
    do {
        escolha = -1;
        static int pos = -1;
        printf("\n--- MENU DE PRODUTOS ---\n");
        printf("1 - Adicionar\n");
        printf("2 - Remover\n");
        printf("3 - Imprimir\n");
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
            printf("Digite o tipo de produto: ");
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

            Adicionar(codigo, tipo_produto, descricao, preco);
            printf("Produto adicionado com sucesso!\n");
            break;
        }
        case 2: {
            if (tam == 0) {
                printf("Lista esta vazia. Nada para remover.\n");
                break;
            }
            printf("Digite a posicao para remover (0 a %d): ", tam - 1);
            if (scanf_s("%d", &pos) != 1) {
                printf("Entrada invalida.\n");
                limpar_buffer_stdin();
                break;
            }
            Remover(pos);
            break;
        }
        case 3: {
            printf("Digite a posicao desejada (-1 para todos): ");
            if (scanf_s("%d", &pos) != 1) {
                printf("Entrada invalida.\n");
                limpar_buffer_stdin();
                break;
            }
            Imprimir(pos);
            break;
        }
        case 0:
            printf("Obrigado! Ate mais.\n");
            break;
        default:
            printf("Opcao invalida. Tente novamente.\n");
            break;
        }
    } while (escolha != 0);

    // Limpa a lista antes de sair
    while (inicio != NULL) {
        Remover(0);
    }
    return 0;
}

void limpar_buffer_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void Adicionar(int codigo, const char* tipo_produto, const char* descricao, float preco) {
    ListaProdutos* novo = (ListaProdutos*)malloc(sizeof(ListaProdutos));
    if (novo == NULL) {
        perror("malloc falhou");
        return;
    }

    novo->codigo = codigo;
    //Marcin, isso daqui é um dos problemas de strings ou const char*.
    //Precisamos usar essa função da biblioteca string.h para adicionar...
    //Uma string de "recurso" para uma string "destinataria", mas para...
    //não ter estouro a gente passa o tamanho de buffer necessario.
    strncpy_s(novo->tipo_produto, sizeof(novo->tipo_produto), tipo_produto, _TRUNCATE);
    strncpy_s(novo->descricao, sizeof(novo->descricao), descricao, _TRUNCATE);
    novo->preco = preco;
    novo->next = novo->prev = NULL;

    if (inicio == NULL) { // Lista vazia
        inicio = novo;
        fim = novo;
    }
    else {
        if (novo->preco < inicio->preco) { // Inserir no início
            novo->next = inicio;
            inicio->prev = novo;
            inicio = novo;
        }
        else if (novo->preco >= fim->preco) { // Inserir no fim
            fim->next = novo;
            novo->prev = fim;
            fim = novo;
        }
        else { // Inserir no meio
            ListaProdutos* aux = inicio->next; // Começa do segundo item
            while (aux != NULL) {
                if (novo->preco < aux->preco) {
                    novo->next = aux;
                    novo->prev = aux->prev;
                    aux->prev->next = novo; // O anterior ao aux aponta pro novo
                    aux->prev = novo;       // O aux aponta pro novo
                    break;
                }
                aux = aux->next;
            }
        }
    }
    tam++;
}

void Imprimir(int pos) {
    if (inicio == NULL) {
        printf("\nLista esta vazia.\n");
        return;
    }

    if (pos == -1) { // Imprimir tudo
        ListaProdutos* aux = inicio;
        printf("\n--- Imprimindo Lista Completa (%d elementos) ---\n", tam);
        int i = 0;
        while (aux != NULL) {
            printf("---------------------------------------------\n");
            printf("Indice [%d]:\n", i);
            printf("  Codigo: %d\n", aux->codigo);
            printf("  Tipo: %s\n", aux->tipo_produto);
            printf("  Descricao: %s\n", aux->descricao);
            printf("  Preco: R$ %.2f\n", aux->preco);
            aux = aux->next;
            i++;
        }
        printf("---------------------------------------------\n");
    }
    else { // Imprimir posição específica
        if (pos < 0 || pos >= tam) {
            printf("Erro: Posicao %d eh invalida. (Tamanho atual: %d)\n", pos, tam);
            return;
        }
        ListaProdutos* aux;
        if (pos <= tam / 2) {
            aux = inicio;
            for (int i = 0; i < pos; i++) {
                aux = aux->next;
            }
        }
        else {
            aux = fim;
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
    if (inicio == NULL) {
        printf("Lista esta vazia.\n");
        return;
    }
    if (pos < 0 || pos >= tam) {
        printf("Erro: Posicao %d eh invalida. (Tamanho: %d)\n", pos, tam);
        return;
    }

    ListaProdutos* lixo = NULL;

    if (tam == 1) { // Caso especial: Removendo o único item
        //lembrando que não chegará aqui se ele digitar um valor de posição inexistente.
        lixo = inicio;
        inicio = NULL;
        fim = NULL;
    }
    else if (pos == 0) { // Removendo do início
        lixo = inicio;
        inicio = inicio->next;
        inicio->prev = NULL;
    }
    else if (pos == tam - 1) { // Removendo do fim
        lixo = fim;
        fim = fim->prev;
        fim->next = NULL;
    }
    else { // Removendo do meio
        //Para eu utilizar o ponteiro anterior, eu criei essa forma de chegar mais rapido...
        //a posição desejada percorrendo o menor caminho.
        if (pos <= tam / 2) {
            lixo = inicio;
            for (int i = 0; i < pos; i++) {
                lixo = lixo->next;
            }
        }
        else {
            lixo = fim;
            for (int i = tam - 1; i > pos; i--) {
                lixo = lixo->prev;
            }
        }
        lixo->prev->next = lixo->next;
        lixo->next->prev = lixo->prev;
    }

    char escolha;
    printf("\nVoce quer mesmo remover este item do sistema? \n");
    Imprimir(pos);
    printf("Escolha (S/N): ");
    scanf_s("%c", &escolha, 1);
    limpar_buffer_stdin();

    if (escolha == 'S' || escolha == 's') {
        free(lixo);
        tam--;
        printf("Produto da posicao %d removido com sucesso.\n", pos);
        return;
    }
    else {
        printf("Remocao cancelada.\n");
        return;
    }
}