#pragma once
#include "structs.h"
#include "definicoes.h"

void AddPilhaCaixa(int, const char*);
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
int PopCodigo(ListaCodigos**);
void PushCodigo(ListaCodigos**, int);
void PushCaixa(Caixa**, Caixa*);
void RemoverProdutoDaPilha(int, const char*);
ListaProdutos* VendaProduto(int);
void ComfirmacaoSE();
void limpar_buffer_stdin();
int tipo_index_from_string(const char*);
bool CodigoExiste(int);
