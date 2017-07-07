/*
 * Objetivo: criar um jogo de memória utilizando C, CGI, HTML e CSS.
 * Aluno: João Gabriel Melo Barbirato
 * Revisor: Prof. Ricardo
 * Data: 09/09/16
 */

// Chamada de bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Constantes
const int maxLinhas = 4, maxColunas = 4, maxCartas = 8;

typedef struct// Definir registro
{
	int linha;
	int coluna;
	int carta;
} tJogada;

// Protótipos
    // construção da matriz
void inicializaMatriz(int matriz[][maxColunas], int nroLinhas, int nroColunas, int valor);
void pausa(time_t delay);
void escolhePosicao(int * linha, int * coluna, int m[][maxColunas]);
int escolheCarta( int *, int, int, int);

	// construção do cgi
int estado( int m[][maxColunas] );
void armazenaEspelho( int e[][maxColunas]);
void preencheRegistro( tJogada * x, int l, int c, int carta );
int jogada( char c[13], int m[][maxColunas], char * dados );
void vetorStatus(int *);
void compara( tJogada y, tJogada x, int e[][maxColunas]);
void pausa(time_t delay);

	// HTML
void escreveHTML(int m[][maxColunas], int * v, int e[][maxColunas]);
void vitoriaHTML();

// Algoritmo principal
int main()
{
	srand( time(NULL) ); // Otimizar random com o tempo da máquina

	// Declarar variáveis
	int matriz[maxLinhas][maxColunas], espelho[maxLinhas][maxColunas];
	int linha, coluna;// controladoras de laço
	int contadorPar = 0; // já armazenados
	int verificarPar[maxColunas*maxLinhas/2]; // verifica pares já repetidos
	int aux; // variável temporária de armazenamento do elemento randômico da matriz
	int * pAux; // ponteiro auxiliar para ler a matriz espelho do arquivo
	int status[3] = { 0, 0, 1}; // dá a informação de qual jogada está.
	char * dados = NULL; // dados coletados da URL
    FILE *naoJogoNovo, *naoEspelhoNovo, *ganhou; // se é um jogo novo/ se é espelho novo/ se ganhou

    ganhou = fopen("ganhou.bin", "rb"); // verifica se ganhou.
    if( ganhou ){
        vitoriaHTML();
        return (EXIT_SUCCESS);
    }

    naoJogoNovo = fopen("estado.bin", "rb");
    if( naoJogoNovo){ /*~~~~~~~~~~~~~~~BIG IF~~~~~~~~~~~~~~~~*/
        // Instruções de um tabuleiro já existente
        estado(matriz);
        naoEspelhoNovo = fopen("espelho.bin", "rb");
        if( naoEspelhoNovo ){ //  verifica se há espelho novo ou se é necessário lê-lo no arquivo.
            pAux = &espelho[0][0];
            for( linha = 0; linha < maxColunas*maxLinhas; linha++) // resgate do espelho
            {
                fread( pAux, sizeof(int), 1, naoEspelhoNovo );
                pAux++;
            }
            fclose(naoEspelhoNovo);
        }else{
            inicializaMatriz( espelho, maxLinhas, maxColunas, 1 );
        }
    }else{
        // Instruções de um jogo novo
        inicializaMatriz( matriz, maxLinhas, maxColunas, -1 );
        inicializaMatriz( espelho, maxLinhas, maxColunas, 0 );
        do	// Preencher a matriz
        {
            // ~~~~~~~~~~~~~~~~~~~~~PRIMEIRO ELEMENTO DO PAR~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // Escolhendo posição na matriz
            escolhePosicao( &linha, &coluna, espelho);
            // Verificar random do par
            aux = escolheCarta( verificarPar, contadorPar, linha, coluna);
            matriz[linha][coluna]=aux;
            // ~~~~~~~~~~~~~~~~~~~~~SEGUNDO ELEMENTO DO PAR~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // Verificar random do par
            escolhePosicao( &linha, &coluna, espelho);
            matriz[linha][coluna] = aux;
            verificarPar[contadorPar]=aux;

            contadorPar ++;
        }while(contadorPar < maxLinhas * maxColunas/2 );
        estado(matriz);
    }
	// Início do jogo
	dados = getenv("QUERY_STRING");
	vetorStatus(status); // verifica qual jogada está
	if(status[2]){
        if( !status[0] ){
            status[0] = 1;
            jogada( "jogada1.bin", matriz, dados);
        }else{
            status[1] = 1;
            jogada( "jogada2.bin", matriz, dados);
        }
	}
    escreveHTML( matriz, status, espelho ); // exibe na página o tabuleiro
	return (EXIT_SUCCESS);
}

/*~~~~~~~~~~~~~~~~~~~~~~PRODUÇÃO DA MATRIZ~~~~~~~~~~~~~~~~~~~~~~*/
// Objetivo: inicializar uma matriz de inteiros com um valor pré-determinado
// - matriz: matriz de inteiros ( saída )
// - nroLinhas: número de linhas da matriz ( entrada )
// - nroColunas: número de colunas da matriz ( entrada )
// - valor: valor que será atribuído aos elementos ( entrada )
void inicializaMatriz(int matriz[][maxColunas], int nroLinhas, int nroColunas, int valor)
{
   int i; // contador de laço
   int * p;

   p = &matriz[0][0]; // p = matriz
   for(i=0; i<nroLinhas*nroColunas; i++)
   {
      *p = valor;
      p++;
   }
} // fim de inicializaMatriz


// Objetivo: escolher uma posição aleatória na matriz principal ainda não ocupada.
// - linha: linha da matriz de cartas ( saída )
// - coluna: coluna da matriz de cartas ( saída )
// - espelho: matriz espelho ( entrada )
void escolhePosicao(int * linha, int * coluna, int espelho[][maxColunas])
{
    do
    {
        *linha = rand() % maxLinhas; // escolha randômica da linha
        *coluna = rand() % maxColunas; // escolha randômica da coluna
    }while(espelho[*linha][*coluna]);
    espelho[*linha][*coluna] = 1;
} // fim de escolhePosicao


// Objetivo: escolher uma carta aleatória cujo par ainda não ocupa a matriz principal
// - v: vetor de pares já armazenados
// - contadorPar: pares já incluidos no processo
// - linha: linha da matriz principal
// - coluna: coluna da matriz principal
// Retorno: carta escolhida
int escolheCarta( int * v, int contadorPar, int linha, int coluna)
{
    int posicao = 0; // do vetor
    int aux = rand() % maxCartas + 1; // primeira iteração da carta escolhida

    do
    {
        if(aux != v[posicao])
        {
            posicao++;
        }else{
            aux = rand() % maxCartas + 1; // random de cartas
            posicao = 0;
        }
    }while(posicao <= contadorPar);

    return aux;
} // fim de escolheCarta


/*----------------------------------CGI----------------------------------*/
// Objetivo: escrever o HTML da iteração e comparar as jogadas, caso haja duas.
// - m: matriz de inteiros ( entrada )
// - v: vetor de informações sobre as jogadas ( entrada )
// - e: matriz espelho ( entrada/saída )
void escreveHTML(int m[][maxColunas], int * v, int e[][maxColunas])
{
	int i, j; // controladores de laço
	tJogada j1, j2; // jogadas
    FILE *arqLeitura1, *arqLeitura2; // arquivos de jogada

    // inicializar arquivos de jogada
	j1 = (tJogada) {-1, -1, -1};
	j2 = (tJogada) {-1, -1, -1};

    // atribuições dos arquivos de jogada
    arqLeitura1 = fopen("jogada1.bin", "rb");
    if( arqLeitura1 ){ // jogada 1
         fread(&j1, sizeof(tJogada), 1, arqLeitura1);
         fclose(arqLeitura1);
    }
    arqLeitura2 = fopen("jogada2.bin", "rb");
    if( arqLeitura2 ){ // jogada 2
         fread(&j2, sizeof(tJogada), 1, arqLeitura2);
         fclose(arqLeitura2);
    }

    // atribuições ao espelho
    if( j1.linha != -1 || j1.coluna != -1 ){  // se já foi a primeira jogada
        e[j1.linha][j1.coluna] = 0;
        if( j2.linha != -1 || j2.coluna != -1)// se já foi a segunda jogada
            e[j2.linha][j2.coluna] = 0;
    }

    // começo do HTML da tabela
	printf("%s%c%c","Content-Type:text/html;charset=UTF-8",13,10);
	printf("\n<!DOCTYPE html>");
	printf("\n<html lang=\"pt\">");
	printf("\n<head><meta charset=\"utf-8\" />");
	printf("\n<title>Jogo da memória</title>");
	printf("\n<link href=\"memoria_estilo.css\" type=\"text/css\" rel=\"stylesheet\" />");
	printf("\n</head>");
	printf("\n<body background=\"http://cap.dc.ufscar.br/~726546/fundo.jpeg\" bgproperties=\"fixed\">");
	printf("\n<div id=\"tabela\">");
	printf("\n<table border=\"0\" align=\"center\">");

	for ( i = 0; i < maxLinhas; i++) // exibir a tabela
	{
        printf("\n<tr>");
		for( j = 0; j < maxColunas; j++)
		{
			printf("\n<td>");
			if( !v[2] ){ // primeira exibição
                if( !e[i][j]){ // resultados anteriores
//                  printf("\n<input type=\"submit\" id =\"a%d%d\"value=\"%d\" /> ", i+4, j, m[i][j]);
                    printf("<img src=\"http://cap.dc.ufscar.br/~726546/%d.png\" alt=\"Carta virada para cima\" height=\"100\">", m[i][j]);
                }else{ // demais
                    printf("\n<a href=\"http://cap.dc.ufscar.br/~726546/cgi-bin/memoria.cgi?linha=%d&coluna=%d\">", i, j);
 //                   printf("\n<input type=\"submit\" id =\"a%d%d\"value=\"%c\" /> ", i, j, 63);
                    printf("<img src=\"http://cap.dc.ufscar.br/~726546/K.png\" alt=\"Carta virada para cima\" height=\"100\">");
                }
			}else{
			    if( v[0] ){                        // Primeira Jogada
                    if( !v[1] && !e[i][j] ){     // exibição da primeira jogada
                        printf("\n<a href=\"http://cap.dc.ufscar.br/~726546/cgi-bin/memoria.cgi\">");
//                        printf("\n<input type=\"submit\" id =\"a%d%d\"value=\"%d\" /> ", i+4, j, m[i][j]);
                        printf("<img src=\"http://cap.dc.ufscar.br/~726546/%d.png\" alt=\"Carta virada para cima\" height=\"100\">", m[i][j]);
                    }else if( v[1] &&  !e[i][j]){ // Segunda jogada
                        // exibição da primeira e da segunda jogada
                        printf("\n<a href=\"http://cap.dc.ufscar.br/~726546/cgi-bin/memoria.cgi\">");
//                        printf("\n<input type=\"submit\" id =\"a%d%d\"value=\"%d\" /> ", i+4, j, m[i][j]);
                        printf("<img src=\"http://cap.dc.ufscar.br/~726546/%d.png\" alt=\"Carta virada para cima\" height=\"100\">", m[i][j]);
                    }else if( !v[1] && e[i][j] ){
                        // exibição das demais na primeira jogada
                        printf("\n<a href=\"http://cap.dc.ufscar.br/~726546/cgi-bin/memoria.cgi?linha=%d&coluna=%d\">", i, j);
//                        printf("\n<input type=\"submit\" id =\"a%d%d\"value=\"%c\" /> ", i, j, 63);
                        printf("<img src=\"http://cap.dc.ufscar.br/~726546/K.png\" alt=\"Carta virada para baixo\" height=\"100\">");
                    }else if( v[1] && e[i][j] ){
                        // exibição das demais na segunda jogada
                        printf("\n<a href=\"http://cap.dc.ufscar.br/~726546/cgi-bin/memoria.cgi?linha=%d&coluna=%d\">", i, j);
//                        printf("\n<input type=\"submit\" id =\"a%d%d\"value=\"%c\" /> ", i, j, 63);
                        printf("<img src=\"http://cap.dc.ufscar.br/~726546/K.png\" alt=\"Carta virada para baixo\" height=\"100\">");
                    }
			    }
			}
			printf("\n</a>");
			printf("\n</td>");
		}
		printf("\n</tr>");
	}
	printf("</div>");
	printf("</form>");
	printf("</body>");
	printf("</html>"); // fim do HTML

    // julgamento
	if( v[1] ){
        compara( j1, j2, e );
        armazenaEspelho( e );
    }
} // fim de escreveHTML


// Objetivo: guardar a matriz principal num arquivo.
// - m: matriz principal
// Retorno: 1 se for a primeira vez que a matriz é gerada, 0 se não.
int estado( int m[][maxColunas] )
{
	FILE * arquivo = NULL ;
	int i; // contadora de laço
	int * p; // ponteiro pra matriz

	p = &m[0][0];
	arquivo = fopen("estado.bin", "rb");
	if( arquivo != NULL ){
		for( i = 0; i < maxColunas*maxLinhas; i++)
		{
			fread( p, sizeof(int), 1, arquivo );
			p++;
		}
		fclose(arquivo);
		return 0;
	}else{
		arquivo = fopen("estado.bin", "wb");
		for( i = 0; i < maxColunas*maxLinhas; i++)
		{
			fwrite( p, sizeof(int), 1, arquivo );
			p++;
		}
		fclose(arquivo);
		return 1;
	}
} // fim de guardaEstado


// Objetivo: armazena a jogada num arquivo
// - c: string com nome do arquivo
// - m: matriz principal ( entrada )
// - dados: QUERY_STRING ( entrada )
// Retorno: 0 se não há dados na URL, 1 se o arquivo foi criado com sucesso.
int jogada( char c[12],int m[][maxColunas], char * dados )
{
	tJogada x;
	FILE * arquivo = NULL;
	arquivo = fopen(c, "rb");

	if( !dados ){
		return 0;
    }else{
        sscanf(dados, "linha=%d&coluna=%d", &x.linha, &x.coluna);
        x.carta = m[x.linha][x.coluna];
        arquivo = fopen(c, "wb");
        fwrite( &x, sizeof(tJogada), 1, arquivo );
        fclose(arquivo);
        return 1;
	}
} // fim de jogada

// Objetivo: armazenar a matriz espelho num arquivo.
// - e: matriz espelho
void armazenaEspelho( int e[][maxColunas])
{
	FILE * arquivo = NULL ;
	int i; // contadora de laço
	int * p; // ponteiro pra matriz
	p = &e[0][0];
	arquivo = fopen("espelho.bin", "wb");
    for( i = 0; i < maxColunas*maxLinhas; i++)
    {
        fwrite( p, sizeof(int), 1, arquivo );
        p++;
    }
    fclose(arquivo);
} // fim de armazenaEspelho


// Objetivo: verificar em qual jogada o jogador está e retornar um vetor com essa informação.
// - v: vetor com informações das jogadas.
void vetorStatus(int * v)
{
    FILE *arqJogada1 = NULL, *arqJogada2 = NULL, *arqTerceiro = NULL, *arqEspelho = NULL;
    arqEspelho = fopen("espelho.bin", "rb");
    arqTerceiro = fopen("arqTerc.bin", "rb");
    arqJogada1 = fopen("jogada1.bin", "rb");
    arqJogada2 = fopen("jogada2.bin", "rb");
    if(arqJogada1)
        v[0] = 1;
    if(arqJogada2)
        v[1] = 1;
    if(!arqJogada1 && !arqJogada2 && !arqTerceiro && !arqEspelho){
        v[2] = 0;
        arqTerceiro = fopen("arqTerc.bin", "wb");
        fwrite(&v[2], sizeof(int), 1, arqTerceiro);
        fclose(arqTerceiro);
    }else if(arqTerceiro || arqEspelho){
        if(arqTerceiro) // HORSE
            fclose(arqTerceiro);
        else
            fclose(arqEspelho);
    }
} // fim de vetorJogada


// Objetivo: verificar se as jogadas são iguais.
// - y: registro de jogada 1 ( entrada )
// - x: registro de jogada 2 ( entrada )
// - e: matriz espelho ( saída )
void compara( tJogada y, tJogada x, int e[][maxColunas])
{
    FILE * arqWin;
    if(y.carta != x.carta){
        e[y.linha][y.coluna] = 1;
        e[x.linha][x.coluna] = 1;
    }else{
        if( contaZeros( e ) == maxColunas * maxLinhas ){
            arqWin = fopen("ganhou.bin", "wr");
            pausa(2);
            printf("<meta http-equiv=\"refresh\" content=1; url=\"http://cap.dc.ufscar.br/~726546/cgi-bin/memoria.cgi\">");
        }
    }
        remove("arqTerc.bin");
        remove("jogada1.bin");
        remove("jogada2.bin");
}


// Objetivo: contar zeros na matriz
// - e: matriz ( entrada )
// Retorno: quantidade de zeros
int contaZeros( int e[][maxColunas] )
{
    int i;
    int cont = 0; // contador
    int * p;
    p = &e[0][0];
    for( i = 0; i < maxLinhas * maxColunas; i++ )
    {
        if( *p == 0 )
            cont++;
        p++;
    }
    return cont;
} // fim de contaZeros


// Objetivo: fazer a máquina pausar por um determinado tempo
// - delay: tempo de pausa ( entrada )
void pausa(time_t delay)
{
	time_t timer0, timer1;

	time( &timer0 ); // marcar tempo inicial
	do
	{
		time( &timer1 ); // tempo determinado
	}while( (timer1 - timer0) < delay );
} // fim de pausa


// Objetivo: exibir o HTML para quando o jogador ganhar.
void vitoriaHTML()
{
    // HTML:
    printf("%s%c%c","Content-Type:text/html;charset=UTF-8",13,10);
    printf("\n<!DOCTYPE html>");
    printf("\n<html>");
    printf("\n<head><meta charset=\"utf-8\" />");
    printf("\n<title>Jogo da memória</title>");
    printf("\n<link href=\"memoria_estilo.css\" type=\"text/css\" rel=\"stylesheet\" />");
    printf("\n</head>");
    printf("\n<body background=\"http://cap.dc.ufscar.br/~726546/fundo.jpeg\" bgproperties=\"fixed\">");
    printf("\n<div id=\"ganhou\" align=\"center\"> <b><h1> PARABÉNS, VOCÊ GANHOU!! </h1> </b></div>");
    printf("\n<div id = \"novamente\" align=\"center\">");
    printf("\n<label for=\"novamente\"> Deseja jogar novamente? </label>");
    printf("\n<form method=\"get\" action=\"http://cap.dc.ufscar.br/~726546/cgi-bin/memoria.cgi\">");
    printf("\n<input type=\"submit\" value=\"JOGAR NOVAMENTE\">");
    printf("\n</form>");
    printf("\n</div>");
    printf("\n</form>");
    printf("\n</body>");
    printf("\n</html>"); // fim do HTML

    // deletar arquivos
    remove("espelho.bin");
    remove("estado.bin");
    remove("ganhou.bin");
}
