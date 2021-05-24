#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <linha.h> em comentario por enquanto
#include "veiculo.h"

/*
            TRABALHO DESENVOLVIDO PARA A DISCIPLINA SCC-0215 ORGANIZACAO DE ARQUIVOS
            
    INTEGRANTES:
        ANTONIO RODRIGUES RIGOLINO - 11795791
        MATHEUS VENTURA DE SOUSA - 11345541
*/
int main(){
  int num_func;			        //indica qual funcionalidade sera pedida do programa
  scanf("%d",&num_func);        
  switch (num_func)             //para melhor eficiencia usa-se um switch
    {
    case 1:
    {
      char filenamecsv[20];
      char filenamebin[20];
      scanf("%s %s",filenamecsv,filenamebin);
      scanf("%*c");
      printf("file csv: %s\nfile bin: %s\n",filenamecsv,filenamebin);
      writeBin(filenamecsv,filenamebin);
      break;
    }
    case 2:
    
      break;
    case 3:
    {
      char filenamebin[20];
      scanf("%s",filenamebin);
      scanf("%*c");
      printBin(filenamebin);
      break;
    }
    case 4:

      break;
    case 5:
    {
      char filenamebin[20];
      char campo[50];
      scanf("%s",filenamebin);
      scanf("%*c");
      scanf("%s",campo);
      scanf("%*c");
      searchPrint(filenamebin,campo);
      break;
    }
    case 6:

      break;
    case 7:
    {
      char filenamebin[20];
      int num_inserts;
      scanf("%s %d",filenamebin,&num_inserts);
      scanf("%*c");
      insert_num_reg(num_inserts,filenamebin);
      break;
    }
    case 8:

      break;
    default:
      printf ("ERRO CODIGO DA FUNCIONALIDADE INVALIDO!\n");         //codigo da funcionalidade fora do escopo
      break;
    }

  return 0;
}