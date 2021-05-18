#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/funcoes-gerais.h"
#include "../include/linha.h"
//#include <linha.h> em comentario por enquanto
//#include <veiculo.h> em comentario por enquanto

/*
            TRABALHO DESENVOLVIDO PARA A DISCIPLINA SCC-0215 ORGANIZACAO DE ARQUIVOS
            
    INTEGRANTES:
        ANTONIO RODRIGUES RIGOLINO - 11795791
        MATHEUS VENTURA DE SOUSA - 11345541

*/
int main(){
  int num_func;			        //indica qual funcionalidade sera pedida do programa
  scanf("%d",&num_func);

  char filenamesrc[50] = "\0";
  char filenamedest[50] = "\0";
  char tmp[50] = "\0";

  s_file_t *filesrc = NULL, *filedest = NULL;
  db_t *db = NULL;
  int nRegistros = 0;

  strcpy(tmp, FILEPATH);

  switch (num_func){             //para melhor eficiencia usa-se um switch
    case 1:

      break;
    case 2:
      scanf("%s", filenamesrc);
      strcat(tmp, filenamesrc);
      strcpy(filenamesrc, tmp);

      filesrc = openfile(filenamesrc, "r");

      db = readDB(filesrc, 10000000);

      closefile(filesrc);

      scanf("%s", filenamedest);
      strcpy(tmp, FILEPATH);
      strcat(tmp, filenamedest);
      strcpy(filenamedest, tmp);

      filedest = openfile(filenamedest, "wb");

      writeDB(filedest, db);

      closefile(filedest);

      binarioNaTela(filenamedest);      
      break;
    case 3:

      break;
    case 4:

      break;
    case 5:

      break;
    case 6:

      break;
    case 7:

      break;
    case 8:
      filesrc = openfile("stdin", "r");

      scanf("%s", filenamedest);
      strcpy(tmp, FILEPATH);
      strcat(tmp, filenamedest);
      strcpy(filenamedest, tmp);

      scanf("%d", &nRegistros);
      fgetc(stdin);

      db = readDB(filesrc, nRegistros);

      filedest = openfile(filenamedest, "wb");

      writeDB(filedest, db);

      closefile(filedest);

      binarioNaTela(filenamedest);
      break;
    default:
      printf ("ERRO CODIGO DA FUNCIONALIDADE INVALIDO!\n");         //codigo da funcionalidade fora do escopo
      break;
    }

  return 0;
}
