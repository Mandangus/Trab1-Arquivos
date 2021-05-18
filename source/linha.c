#include "../include/linha.h"

#define DB_BUFFER 512

typedef long long int ll;

typedef struct header{
    char status;
    ll byteProxReg;
    int nroRegistros;
    int nroRegRemovidos;
    char descreveCodigo[16];   // Tam = 15 + '\0'
    char descreveCartao[14];   // Tam = 13 + '\0'
    char descreveNome[14];     // Tam = 13 + '\0'
    char descreveLinha[25];    // Tam = 24 + '\0'
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

header_t *readHeader(s_file_t *dataFile)
{
    // Verifica se há ponteiro para o arquivo
    if (dataFile->fp == NULL)
    {
        printf("Arquivo inexistente!\n");
        return NULL;
    }
    // Verifica se o arquivo está vazio
    if (dataFile->EmptyFile == '1')
    {
        printf("Arquivo vazio!\n");
        return NULL;
    }

    header_t *header = (header_t *)calloc(1, sizeof(header_t));
    // Assumindo inconsistência para próxima vez que a informação for escrita
    header->status = '0';
    header->byteProxReg = 83;
    // Lendo o header do arquivo de entrada
    fscanf(dataFile->fp, "%[^,]", header->descreveCodigo);
    fgetc(dataFile->fp);
    fscanf(dataFile->fp, "%[^,]", header->descreveCartao);
    fgetc(dataFile->fp);
    fscanf(dataFile->fp, "%[^,]", header->descreveNome);
    fgetc(dataFile->fp);
    fscanf(dataFile->fp, "%[^\n]", header->descreveLinha);
    fgetc(dataFile->fp);
    return header;
}

void writeHeader(header_t *header, s_file_t *binFile)
{
    // Verifica se há ponteiro para o arquivo
    if (binFile->fp == NULL)
    {
        printf("Arquivo inexistente!\n");
        return;
    }
    if(header == NULL){
        printf("Cabeçalho inexistente!\n");
        return;
    }

    // escrevendo o header no binário
    fwrite(&header->status, sizeof(char), 1, binFile->fp);
    fwrite(&header->byteProxReg, sizeof(ll), 1, binFile->fp);
    fwrite(&header->nroRegistros, sizeof(int), 1, binFile->fp);
    fwrite(&header->nroRegRemovidos, sizeof(int), 1, binFile->fp);
    fwrite(header->descreveCodigo, sizeof(char), strlen(header->descreveCodigo), binFile->fp);
    fwrite(header->descreveCartao, sizeof(char), strlen(header->descreveCartao), binFile->fp);
    fwrite(header->descreveNome, sizeof(char), strlen(header->descreveNome), binFile->fp);
    fwrite(header->descreveLinha, sizeof(char), strlen(header->descreveLinha), binFile->fp);
}

dataReg_t *readReg(s_file_t *dataFile){
    // Verifica se há ponteiro para o arquivo
    if (dataFile->fp == NULL)
    {
        printf("Arquivo inexistente!\n");
        return NULL;
    }
    // Verifica se o arquivo está vazio
    if (dataFile->EmptyFile == '1')
    {
        printf("Arquivo vazio!\n");
        return NULL;
    }

    char tmp[7] = "\0";

    char checkEOF = 0;
    // lendo o registro do arquivo de entrada
    dataReg_t *reg = (dataReg_t *)calloc(1, sizeof(dataReg_t));
    fscanf(dataFile->fp, "%[^,]", tmp);
    checkEOF = fgetc(dataFile->fp);
    // Coloca o registro como removido caso tenha asterisco no primeiro campo
    if(tmp[0] == '*'){
        reg->removido = '0';
        // Caso o registro tenha sido removido, coloca-se como posição inicial do ponteiro o tmp[1] para conversão do codLinha para inteiro (pula o asterisco)
        reg->codLinha = atoi(tmp+1);
    }
    // Caso não tenha, coloca a flag como não removido.
    else{
        reg->removido = '1';
        reg->codLinha = atoi(tmp);
    }
    fscanf(dataFile->fp, "%c", &reg->aceitaCartao);
    checkEOF = fgetc(dataFile->fp);
    fscanf(dataFile->fp, "%[^,]", reg->nomeLinha);
    checkEOF = fgetc(dataFile->fp);
    // caso o campo tenha "NULO" como valor, atribui-se o valor 0 como tamanho
    reg->tamNome = (strcmp(reg->nomeLinha,"NULO") == 0)? 0 : strlen(reg->nomeLinha);
    fscanf(dataFile->fp, "%[^\n]", reg->corLinha);
    checkEOF = fgetc(dataFile->fp);
    reg->tamCor = (strcmp(reg->corLinha,"NULO") == 0)? 0 : strlen(reg->corLinha);
    reg->tamRegistro = sizeof(reg->codLinha) + sizeof(reg->aceitaCartao) + reg->tamNome + reg->tamCor;
    
    if(checkEOF == -1){
        free(reg);
        reg = NULL;
    }
    return reg;
}

void writeReg(s_file_t *binFile, dataReg_t *reg){
    // Verifica se há ponteiro para o arquivo
    if (binFile->fp == NULL)
    {
        printf("Arquivo inexistente!\n");
        return;
    }
    if(reg == NULL){
        printf("Registro inexistente!\n");
        return;
    }

    // Escrevendo o registro no binário

    fwrite(&reg->removido, sizeof(char), 1, binFile->fp);
    fwrite(&reg->tamRegistro, sizeof(int), 1, binFile->fp);
    fwrite(&reg->codLinha, sizeof(int), 1, binFile->fp);
    fwrite(&reg->aceitaCartao, sizeof(char), 1, binFile->fp);
    fwrite(&reg->tamNome, sizeof(int), 1, binFile->fp);
    if(reg->tamNome != 0)
        fwrite(reg->nomeLinha, sizeof(char), strlen(reg->nomeLinha), binFile->fp);
    fwrite(&reg->tamCor, sizeof(int), 1, binFile->fp);
    if(reg->tamCor != 0)
        fwrite(reg->corLinha, sizeof(char), strlen(reg->corLinha), binFile->fp);
}

db_t *readDB(s_file_t *dataFile, int nRegistros){
    // Verifica se há ponteiro para o arquivo
    if (dataFile->fp == NULL)
    {
        printf("Arquivo inexistente!\n");
        return NULL;
    }
    // Verifica se o arquivo está vazio
    if (dataFile->EmptyFile == '1')
    {
        printf("Arquivo vazio!\n");
        return NULL;
    }

    db_t *db = (db_t *)calloc(1, sizeof(db_t));
    db->Regdatabase = (dataReg_t **)calloc(DB_BUFFER, sizeof(dataReg_t *));

    db->header = readHeader(dataFile);
    dataReg_t *tmp = NULL;

    int pos = 0;
    while(pos < nRegistros && (tmp = readReg(dataFile)) != NULL){
        db->Regdatabase[pos] = tmp;
        if(db->Regdatabase[pos]->removido == '0') db->header->nroRegRemovidos++;
        else db->header->nroRegistros++;
        pos++;
    }
    printf("%d\n%d\n", db->header->nroRegistros, db->header->nroRegRemovidos);
    return db;
}

void writeDB(s_file_t *binFile, db_t *db){
    // Verifica se há ponteiro para o arquivo
    if (binFile->fp == NULL)
    {
        printf("Arquivo inexistente!\n");
        return;
    }
    if(db == NULL){
        printf("Banco de dados inexistente!\n");
        return;
    }

    writeHeader(db->header, binFile);

    binFile->EmptyFile = '0';
    int pos = 0;
    while(db->Regdatabase[pos] != NULL){
        writeReg(binFile, db->Regdatabase[pos]);
        pos++;
    }

    fseek(binFile->fp, 0, SEEK_SET);
    fwrite("1", sizeof(char), 1, binFile->fp);
}

