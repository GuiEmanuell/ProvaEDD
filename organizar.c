#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define maxSensores 100
#define maxLeituras 1000
#define ID_tam 64
#define valorTam 64

typedef enum {tipoInt, tipoFloat, tipoBoolean, tipoString } TipoValor;

typedef struct {
    long timestamp;
    char valor[valorTam];
} Leitura;

typedef struct {
    char id[ID_tam];
    TipoValor tipo;
    Leitura leituras[maxLeituras];
    int total;
} Sensor;

Sensor sensores[maxSensores];
int totalSensores = 0;

TipoValor identificarTipo(const char *valor) {
    if (strcmp(valor, "true") == 0 || strcmp(valor, "false") == 0)
        return tipoBoolean;

    char *end;
    strtol(valor, &end, 10);
    if (*end == '\0') return tipoInt;

    strtod(valor, &end);
    if (*end == '\0') return tipoFloat;

    return tipoString;
}

Sensor* CriarSensor(const char *id, const char *valor) {
    for (int i = 0; i < totalSensores; i++) {
        if (strcmp(sensores[i].id, id) == 0)
            return &sensores[i];
    }

    if (totalSensores >= maxSensores) {
        fprintf(stderr, "Erro: muitos sensores\n");
        exit(1);
    }

    Sensor *novo = &sensores[totalSensores++];
    strcpy(novo->id, id);
    novo->tipo = identificarTipo(valor);
    novo->total = 0;
    return novo;
}

int comparartimestamp (const void *a, const void *b) {
    return ((Leitura *)a)->timestamp - ((Leitura *)b)->timestamp;
}

void processarArquivo(const char *nome) {
    FILE *arquivo = fopen(nome, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), arquivo)) {
        long timestamp;
        char id[ID_tam], valor[valorTam];

        if (sscanf(linha, "%ld %s %s", &timestamp, id, valor) != 3) {
            fprintf(stderr, "Linha ignorada: formato invalido: %s", linha);
            continue;
        }

        Sensor *s = CriarSensor(id, valor);
        if (s->total >= maxLeituras) continue;

        s->leituras[s->total].timestamp = timestamp;
        strcpy(s->leituras[s->total].valor, valor);
        s->total++;
    }

    fclose(arquivo);
}

void salvarSensor() { 
    for (int i = 0; i < totalSensores; i++) {
        Sensor *s = &sensores[i];
        qsort(s->leituras, s->total, sizeof(Leitura), comparartimestamp );

        char nomeArquivo[128];
        sprintf(nomeArquivo, "sensor_%s.txt", s->id);
        FILE *out = fopen(nomeArquivo, "w");

        for (int j = 0; j < s->total; j++)
            fprintf(out, "%ld %s\n", s->leituras[j].timestamp, s->leituras[j].valor);

        fclose(out);
        printf(">> Arquivo criado: %s\n", nomeArquivo);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <arquivo_entrada>\n", argv[0]);
        return 1;
    }

    processarArquivo(argv[1]);
    salvarSensor();
    return 0;
}
