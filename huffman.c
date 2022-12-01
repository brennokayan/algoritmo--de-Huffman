#include<stdio.h>
#include<stdlib.h>
#include<string.h>



#define TAM 256


typedef struct no {
    unsigned char caracter;
    int frequencia ;
    struct no *esq, *dir, *proximo;
}No;

typedef struct{
    No *inicio;
    int tam;
}Lista;


void Inicializa_tabela_com_zero(unsigned int tab[]){
    int i;
    for ( i = 0; i < TAM; i++)
    {
        tab[i] = 0;
    }
}
void Preenche_tabela_frequencia(unsigned char texto[], unsigned int tab[]){
    int i = 0;
    while (texto[i] != '\0')
    {
        tab[texto[i]]++;
        i++;
    }
}

void Imprimir_tabela_frequencia(unsigned int tab[]){
    int i = 0;
    printf("\tTabela de Frequencia\n");
    for ( i = 0; i < TAM; i++)
    {
        if(tab[i] > 0 ){
            printf("\t%d = %u = %c\n", i, tab[i], i);
        }
    }

}


void Criar_Lista(Lista *Lista){
    Lista -> inicio = NULL;
    Lista -> tam = 0;
}

void Inserir_Ordenado(Lista *Lista, No *no){
    No *aux;
    if(Lista -> inicio == NULL){
        Lista -> inicio = no;
        // Lista -> tam++;
    }
    else if (no -> frequencia < Lista -> inicio->frequencia ){
        no -> proximo = Lista -> inicio;
        Lista->inicio = no;
        // Lista -> tam++;
    }
    else{
        aux = Lista->inicio;
        while (aux -> proximo &&  aux -> proximo -> frequencia <= no->frequencia)
        {
            aux = aux -> proximo;
        }
        no -> proximo = aux -> proximo;
        aux -> proximo = no;
        // Lista -> tam++;
    }
    Lista -> tam++;
}

void Preencher_Lista(unsigned int tab[], Lista *Lista){
    int i;
    No *novo;
    for (i = 0; i < TAM; i++){
        if(tab[i]> 0){
            novo = malloc(sizeof(No));
            if (novo)
            {
                novo->caracter = i;
                novo -> frequencia = tab[i];
                novo->dir =NULL;
                novo -> esq = NULL;
                novo -> proximo = NULL;
                Inserir_Ordenado(Lista, novo);
            }
            else{
                printf("\tERRO: ao alocar memória");
                break;
            }

        }
    }
}

void Imprimir_Lista(Lista *Lista){
    No *aux = Lista -> inicio;
    printf("\tLista ordenada: Tamanho: %d\n", Lista -> tam);
    while(aux){
        printf("\n\tCaracter: %c Frequncia: %d",aux->caracter, aux->frequencia);
        aux = aux -> proximo;

    }
}

No *Remove_no_inicio(Lista *Lista){
    No *aux = NULL;

    if(Lista -> inicio != NULL){
        aux = Lista->inicio;
        Lista -> inicio = aux ->proximo;
        aux ->proximo = NULL;
        Lista -> tam--;
    }

    return aux;
}
No *Montar_Arvore(Lista *lista){
    No *primeiro , *segundo, *novo;
    while(lista -> tam > 1){
        primeiro = Remove_no_inicio(lista);
        segundo = Remove_no_inicio(lista);

        novo = malloc(sizeof(No));

        if(novo != NULL){
            novo->caracter = '+';
            novo->frequencia  = primeiro ->frequencia + segundo ->frequencia;
            novo->esq = primeiro;
            novo->dir = segundo;
            novo->proximo = NULL;

            Inserir_Ordenado(lista, novo);

        }else{
            printf("\tErro ao alocar memória");
            break;
        }
    }
    return lista -> inicio;
}

void Imprimir_Arvorre(No *Raiz, int tam){
    if(Raiz ->esq == NULL && Raiz -> dir == NULL){
        printf("\n\tFolha: %c, Altura: %d", Raiz -> caracter, tam);
    }
    else {
        Imprimir_Arvorre(Raiz -> esq, tam+1);
        Imprimir_Arvorre(Raiz -> dir, tam+1);
    }

}

int Altura_da_Arvore(No *Raiz){
    int esq, dir;
    if(Raiz == NULL){
        return 1;
    }else{
        esq = Altura_da_Arvore(Raiz -> esq) + 1;
        dir = Altura_da_Arvore(Raiz -> dir) + 1;
        if(esq > dir){
            return esq;
        }
        else{
            return dir;
        }

    }
}
char** Aloca_dicionario(int colunas){
    char **dicionario;
    int i;
    dicionario = malloc(sizeof(char*) * TAM);
    for(i = 0; i <TAM; i++){
        dicionario[i] = calloc(colunas, sizeof(char));
    }
    return dicionario;
}


void Gerar_dicionario(char **Dicionario, No *Raiz, char *Caminho, int Colunas){
    char esquerda[Colunas], direita[Colunas];

    if(Raiz -> esq == NULL && Raiz -> dir == NULL){
        strcpy(Dicionario[Raiz -> caracter], Caminho);
    }else {
        strcpy(esquerda, Caminho);
        strcpy(direita, Caminho);

        strcat(esquerda, "0");
        strcat(direita, "1");


        Gerar_dicionario(Dicionario, Raiz->esq, esquerda, Colunas);
        Gerar_dicionario(Dicionario, Raiz->dir, direita, Colunas);

    }
}

void Imprimir_Dicionario(char **Dicionario){
    int i;
    printf("\n\tDicionario: \n");
    for(i =0 ; i < TAM; i++){
        if(strlen(Dicionario[i]) > 0)
            printf("\t%3d: %s\n",i, Dicionario[i]);
    }
}
int Calcula_tamanho_string(char **Dicionario, unsigned char *texto){
    int i = 0, tam =0;
    while(texto[i] != '\0'){
        tam = tam + strlen( Dicionario[texto[i]]);
        i++;
    }

    return tam + 1;
}
char* Codificar(char **Dicionario, unsigned char *texto){
    int tam = Calcula_tamanho_string(Dicionario, texto);
    char *codigo = calloc(tam, sizeof(char));
    int i = 0;
    while(texto[i] != '\0'){
        strcat(codigo, Dicionario[texto[i]]);
        i++;

    }
    return codigo;
}


char* Decodificar(unsigned char texto[], No * Raiz){
    int i = 0;
    No *aux  = Raiz;
    char temporario[2];
    char *Decodificado = calloc(strlen(texto), sizeof(char));

    while(texto[i] != '\0'){
        if(texto[i] == '0'){
            aux = aux -> esq;
        }
        else{
            aux = aux -> dir;
        }
        if(aux -> esq == NULL && aux -> dir == NULL){
            temporario[0] = aux -> caracter;
            temporario[1] = '\0';

            strcat(Decodificado, temporario);
            aux = Raiz;
        }
        i++;
    }
    return Decodificado;
}


void Compactar(unsigned char str[]){
    FILE *arquivo = fopen("compactado.bkr", "wb");
    int i = 0, j = 7;
    unsigned char mascara, byte = 0;
    if (arquivo){
        while(str[i] != '\0'){
            mascara = 1;
            if(str[i] == '1'){
                mascara = mascara << j;
                byte = byte | mascara;
            }
            j--;

            if(j < 0){
                fwrite(&byte, sizeof(unsigned char),1, arquivo);
                byte = 0;
                j = 7;
            }
            i++;
        }
        if(j != 7){
                fwrite(&byte, sizeof(unsigned char),1, arquivo);
        }
        fclose(arquivo);
    }
    else{
        printf("Error: ao abrir ou criar o arquivo em compactar");
    }
}

unsigned int Eh_bit_um(unsigned char byte, int i){
    unsigned char mascara = (1 << i);
    return byte & mascara;
}

void Descompactar(No *Raiz){
    FILE *arquivo = fopen("compactado.bkr", "rb");
    unsigned char byte;
    int i = 0;
    No *aux = Raiz;
    if(arquivo){
        while(fread(&byte, sizeof(unsigned char), 1, arquivo)){
            for(i =7; i>=0; i--){
                if(Eh_bit_um(byte, i)){
                    aux = aux -> dir;
                }
                else{
                    aux = aux -> esq;
                }
                if(aux -> esq == NULL && aux -> dir == NULL){
                    printf("%c", aux -> caracter);
                    aux = Raiz;
                }
            }
        }
        fclose(arquivo);
    }
    else{
        printf("\nErro ao abrir o arquivo para leitura");
    }
}


int Descobir_Tamanho(){
    FILE *arq = fopen("text.txt","r");
    int tam = 0;
    if(arq){
        while(fgetc(arq) != -1){
            tam++;
        }
        fclose(arq);
    }
    else{
        printf("\n\nError ao abrir o camino de text.txt");
    }
    return tam;
}
void Ler_texto(unsigned char *texto){
    FILE *arq = fopen("text.txt","r");
    char letra;
    int i = 0;
    if(arq){
        while(!feof(arq)){
            letra = fgetc(arq);
            if(letra != -1){
                texto[i] = letra;
                i++;
            }
        }
        fclose(arq);
    }
    else{
        printf("\n\nError em Ler_texto");
    }
}

int main(){
    unsigned char *texto;

    // unsigned char text[] = "Vamos aprender a programar";
    unsigned int tabela_frequencia[TAM];
    Lista lista;
    No *arvore;
    int Colunas;
    char **Dicionarios;
    char *codificado;
    char *decodificado;


//parte da parte 7 e 8
    int tam = Descobir_Tamanho();
    printf("\nQuantidade: %d", tam);
    texto = calloc(tam + 2, sizeof(unsigned char));
    Ler_texto(texto);
    printf("\nTexto: \n %s", texto);


//parte 1
    Inicializa_tabela_com_zero(tabela_frequencia);
    Preenche_tabela_frequencia(texto, tabela_frequencia);
    Imprimir_tabela_frequencia(tabela_frequencia);
//parte 2
    Criar_Lista(&lista);
    Preencher_Lista(tabela_frequencia, &lista);
    Imprimir_Lista(&lista);
//parte 3
    arvore = Montar_Arvore(&lista);
    Imprimir_Arvorre(arvore, 0);

//parte 4
    Colunas = Altura_da_Arvore(arvore) + 1;
    Dicionarios = Aloca_dicionario(Colunas);
    Gerar_dicionario(Dicionarios, arvore, "", Colunas);
    Imprimir_Dicionario(Dicionarios);
//parte 5
    codificado = Codificar(Dicionarios, texto);
    printf("\n\tTexto Codicado: %s\n",codificado);

//parte 6
    decodificado = Decodificar(codificado, arvore);
    printf("\n\n\tTexto Decodificado: %s\n",decodificado);

//parte 7
    Compactar(codificado);
//parte 8
    printf("\n\tARQUIVO DESCOMPACTADO!\n");
    Descompactar(arvore);
    printf("\n\n");
    printf("\xC2\xAE\n");
    free(texto);
    free(codificado);
    free(decodificado);

    return 0;
}