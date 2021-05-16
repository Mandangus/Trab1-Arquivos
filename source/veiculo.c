#include "../include/veiculo.h"

typedef struct header{
    char status;
    int64_t byteProxReg;
    int nroRegistros;
    int nroRegRemovidos;
    char descrevePrefixo[18];
    char descreveData[35];
    char descreveLugares[42];
    char descreveLinha[26];
    char descreveModelo[17];
    char descreveCategoria[20];
}header_t;

typedef struct dataReg{
    char removido;
    int tamRegistro;
    char prefixo[5];
    char data[10];
    int quantidadeLugares;
    int codLinha;
    int tamModelo;
    char modelo[50]; // Tamanho Variavel
    int tamCategoria;
    char categoria[50]; // Tamanho Variável
}dataReg_t;

typedef struct db{
    dataReg_t *db;
}db_t;