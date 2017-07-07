#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void completandoVetor(int *v,char*dados);
void zerarVetor(int *v);
void letraNaPalavra(char letra , char *v,char *w,int tamanho,int *pontos); 
char *transparencia(int i);
char conversaoLetra(int valor);
char *url(int p,int i,int *v);

int main(){
	int estado; //indica o estado do jogo , 0 - inicio , 1-jogando , 2- ganhou
	FILE *arquivoLeitura;
	FILE *arquivoEscrita;
	char palavra[50]={'b'}; //vetor contendo a palavra
	char palavra1[50]={'b'}; //vetor onde a palavra está sendo encontrada
	int letra=-1;
	int endereco;
	int pontos=7,i,tamanho;	
	int randon = rand() % 50;
	int vetorLetras[27],vetorIndice[27];
	int *p= vetorIndice;
	char conteudo[200];
	char* dados=NULL;
	dados=getenv("QUERY_STRING");
	    
		sscanf(dados,"letra=%d",&letra);
		
		if(letra==-1){
			arquivoLeitura=fopen("palavras.txt","rt");
		    arquivoEscrita=fopen("palavra.txt","wt");
	 	for(i=0;i<=randon;i++){
			    fgets(palavra,sizeof(palavra),arquivoLeitura);
			}
			tamanho=strlen(palavra);
			for(i=0;i<tamanho-1;i++){
			    palavra1[i]='_';
			}			
		    fprintf(arquivoEscrita,"%s",palavra);		    
		    fprintf(arquivoEscrita,"%s\n",palavra1);
			fprintf(arquivoEscrita,"%d",pontos);
			zerarVetor(vetorLetras);
	     
            
		}else{ 
	        
		    arquivoLeitura=fopen("palavra.txt","rt");		   
		    fgets(palavra,sizeof(palavra),arquivoLeitura);				   
		    fgets(palavra1,sizeof(palavra1),arquivoLeitura);				   
		    fscanf(arquivoLeitura,"%d",&pontos);			
		    tamanho=strlen(palavra);
		    completandoVetor(vetorLetras,dados);		    
		    vetorLetras[letra]=1;
		   	letraNaPalavra(letra,palavra,palavra1,tamanho,&pontos);		   	
			remove("palavra.txt");
		   	arquivoEscrita=fopen("palavra.txt","wt");
		   	fprintf(arquivoEscrita,"%s",palavra);		    
		    fprintf(arquivoEscrita,"%s\n",palavra1);
			fprintf(arquivoEscrita,"%d",pontos);
			 
		     
		}
		if(strcmp(palavra,palavra1) !=0){
			printf("%s%c%c\n","Content-Type:text/html;charset=UTF-8",13,10);
	        printf("<!DOCTYPE html>");
            printf("<html lang=\"pt\">");
            printf("<head>");
            printf("<meta charset=\"utf-8\">");
            printf("<title>Jogo da forca</title>");
            printf("<link rel=\"stylesheet\" href=\"forca.css\" type=\"text/css\">");
            printf("</head>");
            printf("<body style=\"background: url(fundo.png)fixed no-repeat;background-size: 100% 100%;\" >");
            printf("<div class=\"titulo\">Jogo da Forca<br /></div>");
			printf("<div class=\"pagina\" ><table style=\"position:relative;left:60px;top:60px;\" ><tr><td><img src=\"morte%d.png\" style=\"width:480px;height:500px;\" /></td>",pontos);
			if(pontos==1){
			  printf("<td style=\"width:500px;font-size: 23px;top:10px;text-align: center;color: blue; font-family: sans-serif;\"><h1 ><br />Voce perdeu !!</h1><a href=\"http://cap.dc.ufscar.br/~726556/forca.html\"><input type=\"button\" value=\"Jogar Novamente\" style=\" font-family: sans-serif;  background-color: Transparent;  border: none;font-size: 30px;  text-align: center;  color: blue;\"  /></a> </td></tr></table></div>");
			} else{
				
			  printf("<td ><p class=\"palavra\">Palavra:</p><p class=\"texto\">%s </p><p class=\"palavra\"><br /><br /><br />Letras: </p><p class=\"letra\"><br /><br /><br />%d </p></td></tr></table>",palavra1,(tamanho-1));
			  printf("</div><div>");
			printf("<table style=\"position:absolute;left:450px;top:531px;\"><tr>");
			
			printf("<td><a href=\"%s\"><input type=\"button\" value=\"a\" class=\"botoes\"  style=\"%s\"  /></a></td>",url(vetorLetras[0],0,vetorLetras),transparencia(vetorLetras[0]));
			printf("<td><a href=\"%s\"><input type=\"button\" value=\"b\" class=\"botoes\"  style=\"%s\"  /></a></td>",url(vetorLetras[1],1,vetorLetras),transparencia(vetorLetras[1]));
			printf("<td><a href=\"%s\"><input type=\"button\" value=\"c\" class=\"botoes\"  style=\"%s\"  /></a></td>",url(vetorLetras[2],2,vetorLetras),transparencia(vetorLetras[2]));
			printf("<td><a href=\"%s\"><input type=\"button\" value=\"d\" class=\"botoes\"  style=\"%s\"  /></a></td>",url(vetorLetras[3],3,vetorLetras),transparencia(vetorLetras[3]));
			printf("<td><a href=\"%s\"><input type=\"button\" value=\"e\" class=\"botoes\"  style=\"%s\"  /></a></td>",url(vetorLetras[4],4,vetorLetras),transparencia(vetorLetras[4]));
			printf("<td><a href=\"%s\"><input type=\"button\" value=\"f\" class=\"botoes\"  style=\"%s\"  /></a></td>",url(vetorLetras[5],5,vetorLetras),transparencia(vetorLetras[5]));
			printf("<td><a href=\"%s\"><input type=\"button\" value=\"g\" class=\"botoes\"  style=\"%s\"  /></a></td>",url(vetorLetras[6],6,vetorLetras),transparencia(vetorLetras[6]));
			printf("<td><a href=\"%s\"><input type=\"button\" value=\"h\" class=\"botoes\"  style=\"%s\"  /></a></td>",url(vetorLetras[7],7,vetorLetras),transparencia(vetorLetras[7]));
			printf("<td><a href=\"%s\"><input type=\"button\" value=\"i\" class=\"botoes\"  style=\"%s\"  /></a></td>",url(vetorLetras[8],8,vetorLetras),transparencia(vetorLetras[8]));
			printf("<td><a href=\"%s\"><input type=\"button\" value=\"j\" class=\"botoes\"  style=\"%s\"  /></a></td>",url(vetorLetras[9],9,vetorLetras),transparencia(vetorLetras[9]));
			printf("<td><a href=\"%s\"><input type=\"button\" value=\"k\" class=\"botoes\"  style=\"%s\"  /></a></td>",url(vetorLetras[10],10,vetorLetras),transparencia(vetorLetras[10]));
			printf("<td><a href=\"%s\"><input type=\"button\" value=\"l\" class=\"botoes\"  style=\"%s\"  /></a></td>",url(vetorLetras[11],11,vetorLetras),transparencia(vetorLetras[11]));
			printf("<td><a href=\"%s\"><input type=\"button\" value=\"m\" class=\"botoes\"  style=\"%s\"  /></a></td>",url(vetorLetras[12],12,vetorLetras),transparencia(vetorLetras[12]));
			printf("</tr><tr>");
			printf("<td><a href=\"%s\"><input type=\"button\" value=\"n\" class=\"botoes\"  style=\"%s\"  /></a></td>",url(vetorLetras[13],13,vetorLetras),transparencia(vetorLetras[13]));
			printf("<td><a href=\"%s\"><input type=\"button\" value=\"o\" class=\"botoes\"  style=\"%s\"  /></a></td>",url(vetorLetras[14],14,vetorLetras),transparencia(vetorLetras[14]));
			printf("<td><a href=\"%s\"><input type=\"button\" value=\"p\" class=\"botoes\"  style=\"%s\"  /></a></td>",url(vetorLetras[15],15,vetorLetras),transparencia(vetorLetras[15]));
			printf("<td><a href=\"%s\"><input type=\"button\" value=\"q\" class=\"botoes\"  style=\"%s\"  /></a></td>",url(vetorLetras[16],16,vetorLetras),transparencia(vetorLetras[16]));
			printf("<td><a href=\"%s\"><input type=\"button\" value=\"r\" class=\"botoes\"  style=\"%s\"  /></a></td>",url(vetorLetras[17],17,vetorLetras),transparencia(vetorLetras[17]));
			printf("<td><a href=\"%s\"><input type=\"button\" value=\"s\" class=\"botoes\"  style=\"%s\"  /></a></td>",url(vetorLetras[18],18,vetorLetras),transparencia(vetorLetras[18]));
			printf("<td><a href=\"%s\"><input type=\"button\" value=\"t\" class=\"botoes\"  style=\"%s\"  /></a></td>",url(vetorLetras[19],19,vetorLetras),transparencia(vetorLetras[19]));
			printf("<td><a href=\"%s\"><input type=\"button\" value=\"u\" class=\"botoes\"  style=\"%s\"  /></a></td>",url(vetorLetras[20],20,vetorLetras),transparencia(vetorLetras[20]));
			printf("<td><a href=\"%s\"><input type=\"button\" value=\"v\" class=\"botoes\"  style=\"%s\"  /></a></td>",url(vetorLetras[21],21,vetorLetras),transparencia(vetorLetras[21]));
			printf("<td><a href=\"%s\"><input type=\"button\" value=\"w\" class=\"botoes\"  style=\"%s\"  /></a></td>",url(vetorLetras[22],22,vetorLetras),transparencia(vetorLetras[22]));
			printf("<td><a href=\"%s\"><input type=\"button\" value=\"x\" class=\"botoes\"  style=\"%s\"  /></a></td>",url(vetorLetras[23],23,vetorLetras),transparencia(vetorLetras[23]));
			printf("<td><a href=\"%s\"><input type=\"button\" value=\"y\" class=\"botoes\"  style=\"%s\"  /></a></td>",url(vetorLetras[24],24,vetorLetras),transparencia(vetorLetras[24]));
			printf("<td><a href=\"%s\"><input type=\"button\" value=\"z\" class=\"botoes\"  style=\"%s\"  /></a></td>",url(vetorLetras[25],25,vetorLetras),transparencia(vetorLetras[25]));
			printf(" </tr></table>");
			printf("</div>");
			}
			
			printf("</body></html> ");
        }else{
        	printf("%s%c%c\n","Content-Type:text/html;charset=UTF-8",13,10);
	        printf("<!DOCTYPE html>");
            printf("<html lang=\"pt\">");
            printf("<head>");
            printf("<meta charset=\"utf-8\">");
            printf("<title>Jogo da forca</title>");
            printf("<link rel=\"stylesheet\" href=\"forca.css\" type=\"text/css\">");
            printf("</head>");
            printf("<body style=\"background: url(fundo.png)fixed no-repeat;background-size: 100% 100%;\" >");
            printf("<div class=\"titulo\">Jogo da Forca<br /></div>");
            printf("<center><img src=\"ganhou.png\" style=\"width:300px;height:340px;\" />");
            printf("<div class=\"titulo\">Parabens voce ganhou!!!<br /><a href=\"http://cap.dc.ufscar.br/~726556/forca.html\"><input type=\"button\" value=\"Jogar Novamente\" style=\" font-family: sans-serif;  background-color: Transparent;  border: none;font-size: 30px;  text-align: center;  color: blue;\"  /></a></div></center></body></html> ");
		}
		
		
	return 0;
}



char conversaoLetra(int valor){
	return (97+valor);
	
}

char * url(int e,int i,int *v){
	char caminho[250];
	if(e==1){		
		return "";
	}else{
		   snprintf(caminho, sizeof (caminho),"http://cap.dc.ufscar.br/~726556/programa.cgi?letra=%d&a=%d&b=%d&c=%d&d=%d&e=%d&f=%d&g=%d&h=%d&i=%d&j=%d&k=%d&l=%d&m=%d&n=%d&o=%d&p=%d&q=%d&r=%d&s=%d&t=%d&u=%d&v=%d&w=%d&y=%d&z=%d",i,v[0],v[1],v[2],v[3],v[4],v[5],v[6],v[7],v[8],v[9],v[10],v[11],v[12],v[13],v[14],v[15],v[16],v[17],v[18],v[19],v[20],v[21],v[22],v[23],v[24],v[25]);
		return caminho;
		
			   
		
				
		}
	
}

char *transparencia(int i){
	if(i==0){
		return "";
	}else{
		return "opacity: 0.6;cursor: not-allowed;";
	}
	
	
}



void letraNaPalavra(char letra , char *v,char *w,int tamanho,int *pontos){
	int i,q=0;
	for(i=0;i<tamanho;i++){
		if(*v == conversaoLetra(letra)){
		    *w=conversaoLetra(letra);
		    q++;
		}
		w++;
		v++;
	}
	if(q==0){
	   *pontos=*pontos-1;
	}

	
} 

void completandoVetor(int *v,char *dados){
		
		sscanf(dados,"letra=%*d&a=%d&b=%d&c=%d&d=%d&e=%d&f=%d&g=%d&h=%d&i=%d&j=%d&k=%d&l=%d&m=%d&n=%d&o=%d&p=%d&q=%d&r=%d&s=%d&t=%d&u=%d&v=%d&w=%d&y=%d&z=%d",&v[0],&v[1],&v[2],&v[3],&v[4],&v[5],&v[6],&v[7],&v[8],&v[9],&v[10],&v[11],&v[12],&v[13],&v[14],&v[15],&v[16],&v[17],&v[18],&v[19],&v[20],&v[21],&v[22],&v[23],&v[24],&v[25]);
	
}
		

void zerarVetor(int *v){
	    int i;
		for(i=0;i<26;i++){
			*v=0;
			v++;
		}
}
