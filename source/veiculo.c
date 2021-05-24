#include "veiculo.h"

//////////////////////////////////////////////FUNCOES DADAS/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void binarioNaTela(char *nomeArquivoBinario) { /* Você não precisa entender o código dessa função. */

	/* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
	*  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */

	unsigned long i, cs;
	unsigned char *mb;
	size_t fl;
	FILE *fs;
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}
	printf("%lf\n", (cs / (double) 100));
	free(mb);
	fclose(fs);
}

void scan_quote_string(char *str) {

	/*
	*	Use essa função para ler um campo string delimitado entre aspas (").
	*	Chame ela na hora que for ler tal campo. Por exemplo:
	*
	*	A entrada está da seguinte forma:
	*		nomeDoCampo "MARIA DA SILVA"
	*
	*	Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
	*		scanf("%s", str1); // Vai salvar nomeDoCampo em str1
	*		scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
	*
	*/

	char R;

	while((R = getchar()) != EOF && isspace(R)); // ignorar espaços, \r, \n...

	if(R == 'N' || R == 'n') { // campo NULO
		getchar(); getchar(); getchar(); // ignorar o "ULO" de NULO.
		strcpy(str, ""); // copia string vazia
	} else if(R == '\"') {
		if(scanf("%[^\"]", str) != 1) { // ler até o fechamento das aspas
			strcpy(str, "");
		}
		getchar(); // ignorar aspas fechando
	} else if(R != EOF){ // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então, pois deve ser algum inteiro ou algo assim...
		str[0] = R;
		scanf("%s", &str[1]);
	} else { // EOF
		strcpy(str, "");
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct header_veic
{
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
};

struct reg_veic
{
    char removido;
    int tamanhoRegistro;
    int tamanhoModelo;
    int tamanhoCategoria;
    char prefixo[5];
    char data[10]; // data de entrada na frota. formato: AAAA/MM/DD
    int quantidadeLugares;
    int codLinha;
    char* modelo;
    char* categoria;
};

int updateHeader(int status,int64_t offset,int regnum,int regrm,FILE*fp)//atualizar o header, se o valor for diferente de -1 mudamos o campo correspndente
{
	if (status!=-1)
	{
		fseek(fp,0,SEEK_SET);
		char set = status + 48;
		fwrite(&set,1,1,fp);
	}
	if (offset!=-1)
	{
		fseek(fp,1,SEEK_SET);
		fwrite(&offset,8,1,fp);
	}
	if (regnum!=-1)
	{
		fseek(fp,9,SEEK_SET);
		fwrite(&regnum,4,1,fp);
	}
	if (regrm!=-1)
	{
		fseek(fp,13,SEEK_SET);
		fwrite(&regrm,4,1,fp);
	}
	return 0;
}

void setStatus(int status,FILE*fp)//funcao para facilitar a sintaxe e leitura!
{
	updateHeader(status,-1,-1,-1,fp);
	return;
}

int64_t checkOffset(FILE*fp)//checa no header o offset para o proximo registro
{
	int64_t offset;
	fseek(fp,1,SEEK_SET);
	fread(&offset,8,1,fp);
	return offset;
}

int checkRegnum(FILE*fp)//checa o numero de registros no header
{
	int regnum;
	fseek(fp,9,SEEK_SET);
	fread(&regnum,4,1,fp);
	return regnum;
}

int checkregnumRm(FILE*fp)//checa o numero de removidos no header
{
	int regnum;
	fseek(fp,13,SEEK_SET);
	fread(&regnum,4,1,fp);
	return regnum;
}

int writeHeader(FILE* fp_csv,FILE*fp_bin)
{
	headv_t header;//inicializamos um header



	header.status='0';//0 enquanto mexemos no arquivo
	fwrite(&header.status,sizeof(char),1,fp_bin);

	header.byteProxReg=175;//colocamos o offset padrao do header
	fwrite(&header.byteProxReg,8,1,fp_bin);

	header.nroRegistros=0;//zeramos o nro de registros
	fwrite(&header.nroRegistros,4,1,fp_bin);

	header.nroRegRemovidos=0;//zeramos o removidos
	fwrite(&header.nroRegRemovidos,4,1,fp_bin);

	rewind(fp_csv);
	fscanf(fp_csv,"%[^,]",header.descrevePrefixo);			//coletamos o prefixo do csv...
	fseek(fp_csv,1,SEEK_CUR);								//pulando a virgula...
	fwrite(header.descrevePrefixo,18,1,fp_bin);

	fscanf(fp_csv,"%[^,]",header.descreveData);				//agora a data...
	fseek(fp_csv,1,SEEK_CUR);
	fwrite(header.descreveData,35,1,fp_bin);

	fscanf(fp_csv,"%[^,]",header.descreveLugares);			//os lugares...
	fseek(fp_csv,1,SEEK_CUR);
	fwrite(header.descreveLugares,42,1,fp_bin);

	fscanf(fp_csv,"%[^,]",header.descreveLinha);			//a linha...
	fseek(fp_csv,1,SEEK_CUR);
	fwrite(header.descreveLinha,26,1,fp_bin);

	fscanf(fp_csv,"%[^,]",header.descreveModelo);			//o modelo...
	fseek(fp_csv,1,SEEK_CUR);
	fwrite(header.descreveModelo,17,1,fp_bin);

	fscanf(fp_csv,"%[^\n]",header.descreveCategoria);		//e finalmente a categoria
	fseek(fp_csv,1,SEEK_CUR);
	fwrite(header.descreveCategoria,20,1,fp_bin);

	setStatus(1,fp_bin);									//ao terminar de escrever no documento mudamos o status para 1
	return 0;
}

regv_t scanReg(FILE* fp)
{
	regv_t registro;
	char temp[30];
	fscanf(fp,"%[^,]",registro.prefixo);
	if (registro.prefixo[0]=='*')
	{
		registro.removido = '0';
		for (int i = 1; i < 5; i++)
		{
			registro.prefixo[i-1]=registro.prefixo[i];
		}
		registro.prefixo[4]='\0';
	}else
	{
		registro.removido = '1';
	}
	fseek(fp,1,SEEK_CUR);
	fscanf(fp,"%[^,]",registro.data);
	if (strcmp(registro.data,"NULO")==0)
	{
		registro.data[0]='\0';
		for (int i = 1; i < 10; i++)
		{
			registro.data[i]='@';
		}
		
	}
	fseek(fp,1,SEEK_CUR);
	fscanf(fp,"%[^,]",temp);
	registro.quantidadeLugares=atoi(temp);
	fseek(fp,1,SEEK_CUR);
	fscanf(fp,"%[^,]",temp);
	if (strcmp("NULO",temp)==0)
	{
		registro.codLinha=-1;
	}else
	{
		registro.codLinha=atoi(temp);
	}
	fseek(fp,1,SEEK_CUR);
	fscanf(fp,"%[^,]",temp);
	if (strcmp("NULO",temp)==0)	
	{
		registro.tamanhoModelo=0;
		registro.modelo = NULL;
	}else
	{
		registro.tamanhoModelo = strlen(temp);
		registro.modelo = (char*)calloc(registro.tamanhoModelo,sizeof(char));
		strcpy(registro.modelo,temp);
	}
	fseek(fp,1,SEEK_CUR);
	fscanf(fp,"%[^\n]",temp);
	fseek(fp,1,SEEK_CUR);
	if (strcmp("NULO",temp)==0)	
	{
		registro.tamanhoCategoria=0;
		registro.categoria = NULL;
	}else
	{
		registro.tamanhoCategoria = strlen(temp);
		registro.categoria = (char*)calloc(registro.tamanhoCategoria,sizeof(char));
		strcpy(registro.categoria,temp);
	}
	registro.tamanhoRegistro = 36 + registro.tamanhoCategoria + registro.tamanhoModelo;
	return registro;
}

int insertBin(regv_t registro,FILE*fp)
{
	setStatus(0,fp);									//status 0 pois estamos mexendo no bin


	int64_t offset = checkOffset(fp);					//olhamos o offset
	int regnum = checkRegnum(fp);						//olhamos o numero de registros
	int regrm = checkregnumRm(fp);						//olhamos o numero de removidos


	fseek(fp,offset,SEEK_SET);							//pelo offset sabemos onde colocar o proximo registro
	registro.tamanhoRegistro-=5;
	fwrite(&registro.removido,1,1,fp);					//primeiro o removido
	fwrite(&registro.tamanhoRegistro,4,1,fp);			//depois o tamanho do registro
	fwrite(registro.prefixo,5,1,fp);					//prefixo
	fwrite(registro.data,10,1,fp);						//data de entrada
	fwrite(&registro.quantidadeLugares,4,1,fp);			//quantidade de lugares
	fwrite(&registro.codLinha,4,1,fp);					//codigo da linha
	fwrite(&registro.tamanhoModelo,4,1,fp);				//tamanho do campo modelo
	if(registro.tamanhoModelo!=0)
		fwrite(registro.modelo,registro.tamanhoModelo,1,fp); //se o tamanho n for 0 colocamos o modelo
	fwrite(&registro.tamanhoCategoria,4,1,fp);
	if(registro.tamanhoCategoria!=0)
		fwrite(registro.categoria,registro.tamanhoCategoria,1,fp);
	if(registro.removido == '0') regrm++;					//se for removido atualizamos tambem
	if(registro.removido == '1') regnum++;
	offset += registro.tamanhoRegistro+5;					//atualizamos o offset e regnum
	updateHeader(1,offset,regnum,regrm,fp);				//atualizamos o header e setamos o status para 1

	return 0;
}

int countLines(FILE*fp)
{
	char meuChar;
	int count = -1;
	while (meuChar!=EOF)
	{
		meuChar = getc(fp);
		if(meuChar=='\n') count++;
	}
	return count;
}
regv_t get_reg_bin(FILE* fp)
{
	regv_t registro;
	fread(&registro.removido,1,1,fp);
	fread(&registro.tamanhoRegistro,4,1,fp);
	fread(&registro.prefixo,5,1,fp);
	fread(&registro.data,10,1,fp);
	fread(&registro.quantidadeLugares,4,1,fp);
	fread(&registro.codLinha,4,1,fp);
	fread(&registro.tamanhoModelo,4,1,fp);
	if (registro.tamanhoModelo!=0)
	{
		registro.modelo = (char*)calloc(registro.tamanhoModelo,sizeof(char));
		for (int i = 0; i < registro.tamanhoModelo; i++)
		{
			char meuChar = getc(fp);
			registro.modelo[i]=meuChar;
		}
	}
	fread(&registro.tamanhoCategoria,4,1,fp);
	if (registro.tamanhoCategoria!=0)
	{
		registro.categoria = (char*)calloc(registro.tamanhoCategoria,sizeof(char));
		for (int i = 0; i < registro.tamanhoCategoria; i++)
		{
			char meuChar = getc(fp);
			registro.categoria[i]=meuChar;
		}
	}
	return registro;
}
void printReg(regv_t registro)
{
	char meses[12][20] = {
		{"janeiro"},{"fevereiro"},{"marco"},{"abril"},{"maio"},{"junho"},{"julho"},{"agosto"},{"setembro"},{"outubro"},{"novembro"},{"dezembro"}
	};
	printf("Prefixo do veiculo: ");
	for (int i = 0; i < 5; i++)
	{
		printf("%c",registro.prefixo[i]);
	}
	printf("\n");
	printf("Modelo do veiculo: ");
	if(registro.tamanhoModelo!=0)
	{
		printf("%s\n",registro.modelo);
	}else
	{
		printf("campo com valor nulo\n");
	}
	printf("Categoria do veiculo: ");
	if(registro.tamanhoCategoria!=0)
	{
		printf("%s\n",registro.categoria);
	}else
	{
		printf("campo com valor nulo\n");
	}
	printf("Data de entrada do veiculo na frota: ");
	if (registro.data[0]!='\0')
	{
		char dia[3]={registro.data[8],registro.data[9],'\0'};
		char ano[5]={registro.data[0],registro.data[1],registro.data[2],registro.data[3],'\0'};
		char mes[3]={registro.data[5],registro.data[6],'\0'};
		int num_mes = atoi(mes);
		printf("%s de %s de %s\n",dia,meses[num_mes-1],ano);
	}else
	{
		printf("campo com valor nulo\n");
	}
	printf("Quantidade de lugares sentados disponiveis: ");
	if (registro.quantidadeLugares!=0)
	{
		printf("%d\n",registro.quantidadeLugares);
	}else
	{
		printf("campo com valor nulo\n");
	}
	printf("\n");
	return;
}
void printBin(char*filename)
{
	FILE* fp = fopen(filename,"rb");//so vamos ler e printar
	int regnum = checkRegnum(fp);
	int cont = 0;
	fseek(fp,175,SEEK_SET);//vamos para o começo dos registros
	if (regnum==0)
	{
		printf("Registro inexistente.\n");
		return;
	}
	while (cont<regnum)
	{
		regv_t registro = get_reg_bin(fp);
		if (registro.removido=='1')
		{
			printReg(registro);
			cont++;
		}
		
	}

	return;
}


int getReg(regv_t *registro)
{
	char aux[45];
	scan_quote_string(registro->prefixo);//scan no prefixo com a mesma regra de tratamento do * da função scanReg
	if (registro->prefixo[0]=='*')
	{
		registro->removido = '0';
		for (int i = 1; i < 5; i++)
		{
			registro->prefixo[i-1]=registro->prefixo[i];
		}
		registro->prefixo[4]='\0';
	}else
	{
		registro->removido = '1';
	}
	scan_quote_string(registro->data);//scan da data tratando do campo nulo com \0@@@@@@
	if (strcmp(registro->data,"")==0)
	{
		registro->data[0]='\0';
		for (int i = 1; i < 10; i++)
		{
			registro->data[i]='@';
		}
		
	}
	scan_quote_string(aux);
	registro->quantidadeLugares = atoi(aux);
	scan_quote_string(aux);
	if(strcmp(aux,"")!=0)
	{
		registro->codLinha = atoi(aux);
	}else
	{
		registro->codLinha = -1;
	}
	scan_quote_string(aux);
	if(strcmp(aux,"")==0)
	{
		registro->modelo = NULL;
		registro->tamanhoModelo = 0;
	}else
	{
		registro->tamanhoModelo = strlen(aux);
		registro->modelo = (char*)calloc(registro->tamanhoModelo,sizeof(char));
		strcpy(registro->modelo,aux);
	}
	scan_quote_string(aux);
	if(strcmp(aux,"")==0)
	{
		registro->categoria = NULL;
		registro->tamanhoCategoria = 0;
	}else
	{
		registro->tamanhoCategoria = strlen(aux);
		registro->categoria = (char*)calloc(registro->tamanhoCategoria,sizeof(char));
		strcpy(registro->categoria,aux);
	}
	getchar(); //pulando o \n...
	registro->tamanhoRegistro = 36 + registro->tamanhoCategoria + registro->tamanhoModelo;
	return 0;
}

int insert_num_reg(int num_ins,char*filenamebin)
{
	FILE *fp_bin = fopen(filenamebin,"rb+");//escrite e leitura em binario
	for (int i = 0; i < num_ins; i++)
	{
		regv_t registro;
		getReg(&registro);
		insertBin(registro,fp_bin);
	}
	fclose(fp_bin);
	binarioNaTela(filenamebin);
	return 0;
}
int checkRem(FILE*fp)
{
	char meuChar;
	meuChar = getc(fp);
	if (meuChar==EOF)
	{
		return -1;
	}
	return atoi(&meuChar);
}
int checkTam(FILE*fp)
{
	int tam;
	fread(&tam,4,1,fp);
	return tam;
}
int getCampo(char*campo,char* valor)
{
	int qual = -1;
	char valores[5][50]={
		{"prefixo"},{"data"},{"modelo"},{"categoria"},{"quantidadeLugares"}
	};
	for (int i = 0; i < 5; i++)
	{
		if (strcmp(valores[i],campo)==0)
		{
			qual = i;
			break;
		}
	}
	scan_quote_string(valor);
	return qual;
}
int64_t searchOffset(FILE* fp,int qual,char* valor)
{
	int rem = 0;
	int64_t start = ftell(fp);
	while (rem != -1)
	{
		rem = checkRem(fp);
		int tam = checkTam(fp);
		if (rem==1)
		{
			switch (qual)
			{
			case 0://caso prefixo
			{
				char prefixo[5];
				fread(prefixo,5,1,fp);
				int k=0;
				for (int i = 0; i < 5; i++)
				{
					if (prefixo[i]==valor[i])
					{
						k++;
					}
					
				}
				if (k==4)
				{
					return start;
				}
				
			}	
			case 1://caso data
			{
				fseek(fp,5,SEEK_CUR);//pulamos prefixo
				char data[10];
				fread(data,10,1,fp);
				if (strcmp(valor,data)==0)
				{
					return start;
				}
				break;
			}	
			case 2://caso modelo
			{
				fseek(fp,23,SEEK_CUR);
				int tam_modelo;
				fread(&tam_modelo,4,1,fp);
				if (tam_modelo>0)
				{
					char*modelo = (char*)calloc(tam_modelo,sizeof(char));
					for (int i = 0; i < tam_modelo; i++)
					{
						char meuChar = getc(fp);
						modelo[i]=meuChar;
					}
					if(strcmp(valor,modelo)==0)
					{
						return start;
					}
				}
				break;
			}	
			case 3://caso categoria
			{
				fseek(fp,23,SEEK_CUR);//PRECISAMOS PEGAR O TAMANHO DO MODELO PRIMEIRO
				int tam_modelo;
				fread(&tam_modelo,4,1,fp);
				fseek(fp,tam_modelo,SEEK_CUR);
				int tam_categoria;
				fread(&tam_categoria,4,1,fp);
				if (tam_categoria>0)
				{
					char*categoria = (char*)calloc(tam_categoria,sizeof(char));
					for (int i = 0; i < tam_categoria; i++)
					{
						char meuChar = getc(fp);
						categoria[i]=meuChar;
					}
					if(strcmp(valor,categoria)==0)
					{
						return start;
					}
				}
				break;
			}	
			case 4://caso quantidadeLugares
			{
				fseek(fp,15,SEEK_CUR);
				int num = atoi(valor);
				int quantLug;
				fread(&quantLug,4,1,fp);
				if (num==quantLug)
				{
					return start;
				}
				break;
			}	
			default:
				printf("ERRO NO PROCESSAMENTO DO ARQUIVO\n");
				break;
			}
		}
		start = start + tam + 5;
		fseek(fp,start,SEEK_SET);
	}
	return -1;
}
	

int searchPrint(char* filename, char* campo)
{
	int64_t offset = 175;
	FILE *fp = fopen(filename,"rb");//somente leitura em binario
	if(fp==NULL) return -1;
	char valor[40];
	int qual_campo = getCampo(campo,valor);
	fseek(fp,offset,SEEK_SET);
	while (offset!=-1)
	{
		offset = searchOffset(fp,qual_campo,valor);
		if(offset!=-1)
		{
			fseek(fp,offset,SEEK_SET);
			regv_t registro = get_reg_bin(fp);
			printReg(registro);
		} 
	}
	return 0;
}


int writeBin(char* filenamecsv,char* filenamebin)
{
	FILE *fp_csv = fopen(filenamecsv,"r");
	if(fp_csv==NULL)
	{
		printf("ERRO NA LEITURA DO CSV\n");
		return -1;
	}
	FILE *fp_bin = fopen(filenamebin,"wb+");
	if(fp_bin==NULL)
	{
		printf("ERRO NA LEITURA DO CSV\n");
		return -1;
	}
	int regnum = countLines(fp_csv);
	writeHeader(fp_csv,fp_bin);
	int i =0;
	while (i<regnum)
	{
		regv_t registro = scanReg(fp_csv);
		insertBin(registro,fp_bin);
		i++;
	}
	fclose(fp_bin);
	fclose(fp_csv);
	binarioNaTela(filenamebin);
	return 0;
}