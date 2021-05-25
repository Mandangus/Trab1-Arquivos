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

db_t *readDBfromCSV(s_file_t *dataFile, int nRegistros);
void writeDB(s_file_t *binFile, db_t *db, char writePartialHeaderFlag);
db_t *readDBfromBIN(s_file_t *dataFile);
db_t *readDBfromStdin(int nRegistros, header_t *header);
header_t *readHeaderfromBIN(s_file_t *dataFile);
//header_t *readHeaderfromBIN2(s_file_t *binFile);
db_t *searchAttrib(db_t * database, char *attribute, char *find_key);
void printSearchResult(db_t * searchResult);

#endif