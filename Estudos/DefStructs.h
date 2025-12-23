#pragma once

typedef struct Produto {
    int codigo;
    char tipo_produto[32];
    char descricao[64];
    float preco;
    struct Produto* next;
    struct Produto* prev;
} ListaProdutos;

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

//os extern's servem para declarar mas sem definir. serve pra nao dar erro na linkagem: lnk2005 (quando tem multiplas definicoes)
extern ListaProdutos* headP;
extern ListaProdutos* tailP;
extern int tamP;

extern Caixa* topoParafina;
extern Caixa* topoLeash;
extern Caixa* topoQuilha;
extern Caixa* topoDeck;

extern FilaEntrega* headFE;
extern FilaEntrega* tailFE;
extern int tamFE;
