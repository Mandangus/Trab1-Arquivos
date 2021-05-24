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
  db_t *db = NULL, *searchResult = NULL;
  header_t *header = NULL;
  int nRegistros = 0;

  char searchfieldname[30] = "\0";
  char find_key[100] = "\0";

  strcpy(tmp, FILEPATH);

  switch (num_func){             //para melhor eficiencia usa-se um switch
    case 1:

      break;
    case 2:
      // Manipulação de string do caminho do arquivo origem
      scanf("%s", filenamesrc);
      strcat(tmp, filenamesrc);
      strcpy(filenamesrc, tmp);

      filesrc = openfile(filenamesrc, "r");

      // readDBfromCSV - parâmetros: 
      //  - ponteiro da struct que contém o arquivo origem
      //  - número de registros: para indicar o infinito, foi usado o nº: 10000000
      db = readDBfromCSV(filesrc, 10000000);

      closefile(filesrc);

      // Manipulação de string do caminho do arquivo destino
      scanf("%s", filenamedest);
      strcpy(tmp, FILEPATH);
      strcat(tmp, filenamedest);
      strcpy(filenamedest, tmp);

      filedest = openfile(filenamedest, "wb");

      // Escreve o db no arquivo destino.

      writeDB(filedest, db, 0);

      closefile(filedest);

      binarioNaTela(filenamedest);      
      break;
    case 3:

      break;
    case 4:
      // Manipulação de string do caminho do arquivo origem
      scanf("%s", filenamesrc);
      strcat(tmp, filenamesrc);
      strcpy(filenamesrc, tmp);

      filesrc = openfile(filenamesrc, "rb");

      db = readDBfromBIN(filesrc);

      closefile(filesrc);

      // mostra na tela todos os registros encontrados no arquivo destino com exceção dos excluidos
      printSearchResult(db, NULL);

      break;
    case 5:

      break;
    case 6:
      // Manipulação de string do caminho do arquivo origem
      scanf("%s", filenamesrc);
      strcat(tmp, filenamesrc);
      strcpy(filenamesrc, tmp);

      filesrc = openfile(filenamesrc, "rb");

      db = readDBfromBIN(filesrc);

      closefile(filesrc);

      fgetc(stdin);
      scanf("%s ", searchfieldname);
      scanf("%s", find_key);

      // procura o campo desejado
      searchResult = searchAttrib(db, searchfieldname, find_key);
      if(searchResult != NULL) searchResult->header = db->header;

      // Mostra na tela todos os resultados encontrados, com exceção dos removidos
      printSearchResult(searchResult, searchfieldname);

      break;
    case 7:

      break;
    case 8:
      // Manipulação de string do caminho do arquivo destino
      scanf("%s", filenamedest);
      strcpy(tmp, FILEPATH);
      strcat(tmp, filenamedest);
      strcpy(filenamedest, tmp);

      scanf("%d", &nRegistros);
      fgetc(stdin);

      filedest = openfile(filenamedest, "rb+");

      // posiciona o ponteiro do arquivo para depois do status
      fseek(filedest->fp, 1, SEEK_SET);
      header = readHeaderfromBIN(filedest);

      db = readDBfromStdin(nRegistros, header);

      header = NULL;

      // posiciona o ponteiro no início do arquivo
      fseek(filedest->fp, 0, SEEK_SET);
      writeDB(filedest, db, 1);

      closefile(filedest);

      binarioNaTela(filenamedest);
      break;
    default:
      printf ("ERRO CODIGO DA FUNCIONALIDADE INVALIDO!\n");         //codigo da funcionalidade fora do escopo
      break;
    }

  return 0;
}



/*
 * 
 *
 */