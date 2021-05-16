#include "../include/linha.h"

typedef struct header{
    char status;
    int64_t byteProxReg;
    int nroRegistros;
    int nroRegRemovidos;
    char descreveCodigo[15];
    char descreveCartao[13];
    char descreveNome[13];
    char descreveLinha[24];
} header_t;

typedef struct dataReg{
    char removido;
    int tamRegistro;
    int codLinha;
    char aceitaCartao;
    int tamNome;
    char nomeLinha[50]; // Tamanho Variável
    int tamCor;
    char corLinha[20]; // Tamanho Variável
} dataReg_t;

typedef struct db{
    dataReg_t *db;
} db_t;