#pragma once

#define CAP_PARAFINA 50
#define CAP_LEASH 25
#define CAP_QUILHA 10
#define CAP_DECK 5

enum { TYPE_PARAFINA = 0, TYPE_LEASH = 1, TYPE_QUILHA = 2, TYPE_DECK = 3, NUM_TYPES = 4 };
static const char* PRODUCT_TYPE_NAMES[NUM_TYPES] = { "Parafina", "Leash", "Quilha", "Deck" };

static int caixa_id_counters[NUM_TYPES] = { 0 };