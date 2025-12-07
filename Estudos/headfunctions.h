#pragma once
#include "structs.h"

bool AddPilhaCaixa(int, const char*);
void AdicionarLPV(int, const char*, const char*, float);
void AdicionarFE(const char*, const char*, int, const char*, int, const char*, ListaProdutos*);
void Imprimir(int);
void ImprimirPilha(const char*, Caixa*, int);
void ImprimirPilhas();
void ImprimirVendaC(const char*);
void ImprimirVendaP(float, float);
void ImprimirProdutoNode(const ListaProdutos*);
void ImprimirEntregaNode(const FilaEntrega*);
void LiberarCodigos(ListaCodigos*);
void LiberarCaixas(Caixa**);
void LiberarProdutos();
void LiberarTudo();
bool RemoverProdutoDaPilha(int, const char*);
ListaProdutos* VendaProduto(int);
void ComfirmacaoSE();
void limpar_buffer_stdin();
bool strings_equal_ci(const char*, const char*);
int tipo_index_para_string(const char*);
bool CodigoExiste(int);
