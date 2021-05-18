#ifndef LINHA_H_
#define LINHA_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "../include/funcoes-gerais.h"

typedef struct header header_t;
typedef struct dataReg dataReg_t;

typedef struct db{
    header_t *header;
    dataReg_t **Regdatabase;
} db_t;

db_t *readDB(s_file_t *dataFile, int nRegistros);
void writeDB(s_file_t *binFile, db_t *db);

#endif