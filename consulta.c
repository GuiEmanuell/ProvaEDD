#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define linhasMax 10000
#define textoMax 64

typedef struct {
    long timestamp;
    char valor[textoMax];
} Leitura;

int lerArquivo(const char *nomeArquivo, Leitura leituras[]) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return -1;
    }

    int i = 0;
    while (fscanf(arquivo, "%ld %s", &leituras[i].timestamp, leituras[i].valor) == 2) {
        i++;
    }

    fclose(arquivo);
    return i;  
}


int buscaProx(Leitura leituras[], int n, long alvo) {
    int esquerda = 0, direita = n - 1;
    int maisProxima = 0;

    while (esquerda <= direita) {
        int meio = (esquerda + direita) / 2;

        if (labs(leituras[meio].timestamp - alvo) < labs(leituras[maisProxima].timestamp - alvo)) {
            maisProxima = meio;
        }

        if (leituras[meio].timestamp < alvo) {
            esquerda = meio + 1;
        } else if (leituras[meio].timestamp > alvo) {
            direita = meio - 1;
        } else {
            return meio; 
        }
    }

    return maisProxima;
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <nome_do_sensor> <timestamp_unix>\n", argv[0]);
        return 1;
    }

    char nomeSensor[textoMax];
    strcpy(nomeSensor, argv[1]);
    long buscarTimesTemp = atol(argv[2]);

   
    char nomeArquivo[128];
    sprintf(nomeArquivo, "sensor_%s.txt", nomeSensor);

   
    Leitura leituras[linhasMax];
    int total = lerArquivo(nomeArquivo, leituras);

    if (total <= 0) {
        printf("Nenhuma leitura encontrada no arquivo %s\n", nomeArquivo);
        return 1;
    }

    
    int posicao = buscaProx(leituras, total, buscarTimesTemp);

    if (posicao < 0 || posicao >= total) {
        printf("Erro ao buscar a leitura mais proxima\n");
        return 1;
    }
    printf("Leitura mais proxima:\n");
    printf("Timestamp: %ld\n", leituras[posicao].timestamp);
    printf("Valor: %s\n", leituras[posicao].valor);

    return 0;
}
