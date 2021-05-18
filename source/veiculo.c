#include "../include/veiculo.h"
#include "../include/funcoes-gerais.h"

typedef struct header{
    char status;
    int64_t byteProxReg;
    int nroRegistros;
    int nroRegRemovidos;
    char descrevePrefixo[19]; // Tam = 18 + '\0'
    char descreveData[36];    // Tam = 35 + '\0'
    char descreveLugares[43]; // Tam = 42 + '\0'
    char descreveLinha[27];   // Tam = 26 + '\0'
    char descreveModelo[18];  // Tam = 17 + '\0'
    char descreveCategoria[21]; // Tam = 20 + '\0'
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

