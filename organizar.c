#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define maxSensores 100
#define maxNome 64
#define quantidadeLeituras 2000

typedef enum { tipoInt, tipoFloat, tipoBoolean, tipoString } Tipo;

typedef struct {
    char nome[maxNome];
    Tipo tipo;
} Sensor;

bool data_valida(int dia, int mes, int ano, int hora, int min, int seg) {
    if (ano < 1970 || mes < 1 || mes > 12 || dia < 1 || dia > 31 ||
        hora < 0 || hora > 23 || min < 0 || min > 59 || seg < 0 || seg > 59)
        return false;
    int dias_mes[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (dia > dias_mes[mes-1]) return false;
    return true;
}

time_t converterDataHora(const char *dataHora) {
    int dia, mes, ano, hora, min, seg;
    struct tm t;
    if (sscanf(dataHora, "%d/%d/%d %d:%d:%d", &dia, &mes, &ano, &hora, &min, &seg) != 6)
        return -1;
    if (!data_valida(dia, mes, ano, hora, min, seg))
        return -1;
    t.tm_year = ano - 1900;
    t.tm_mon = mes - 1;
    t.tm_mday = dia;
    t.tm_hour = hora;
    t.tm_min = min;
    t.tm_sec = seg;
    t.tm_isdst = -1;
    return mktime(&t);
}


void gerarTexto(char *destino, int max) {
    int tamanho = rand() % (max - 3) + 4;
    for (int i = 0; i < tamanho; i++) {
        destino[i] = 'A' + rand() % 26;
    }
    destino[tamanho] = '\0';
}

Tipo tipoNome(const char *tipoStr) {
    if (strcmp(tipoStr, "int") == 0) return tipoInt;
    if (strcmp(tipoStr, "float") == 0) return tipoFloat;
    if (strcmp(tipoStr, "bool") == 0) return tipoBoolean;
    if (strcmp(tipoStr, "string") == 0) return tipoString;

    printf("Tipo desconhecido: %s\n", tipoStr);
    exit(1);
}

void gerarLeitura(FILE *arquivo, const char *nome, Tipo tipo, time_t inicio, time_t fim) {
    time_t ts = inicio + rand() % (fim - inicio + 1);
    char valor[64];

    switch (tipo) {
        case tipoInt:
            sprintf(valor, "%d", rand() % 10000);
            break;
        case tipoFloat:
            sprintf(valor, "%.2f", (float)(rand() % 100000) / 100.0);
            break;
        case tipoBoolean:
            sprintf(valor, "%s", rand() % 2 ? "true" : "false");
            break;
        case tipoString:
            gerarTexto(valor, 16);
            break;
    }

    fprintf(arquivo, "%ld %s %s\n", ts, nome, valor);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Uso: %s \"data_inicio\" (dd/mm/aaaa) \"data_fim\" (dd/mm/aaaa) sensor1:tipo [sensor2:tipo ...]\n", argv[0]);
        return 1;
    }

    srand(time(NULL)); 

    time_t inicio = converterDataHora(argv[1]);
    time_t fim = converterDataHora(argv[2]);
    if (fim <= inicio) {
        printf("Erro: data final deve ser apos a data inicial.\n");
        return 1;
    }

    Sensor sensores[maxSensores];
    int totalSensores = 0;

    for (int i = 3; i < argc; i++) {
        char *divisor = strchr(argv[i], ':');
        if (!divisor) {
            printf("Erro: sensor deve estar no formato nome:tipo\n");
            return 1;
        }

        *divisor = '\0'; 
        strcpy(sensores[totalSensores].nome, argv[i]);
        sensores[totalSensores].tipo = tipoNome(divisor + 1);
        totalSensores++;
    }

    FILE *arquivo = fopen("leituras_geradas.txt", "w");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo de saida");
        return 1;
    }

    for (int i = 0; i < totalSensores; i++) {
        for (int j = 0; j < quantidadeLeituras; j++) {
            gerarLeitura(arquivo, sensores[i].nome, sensores[i].tipo, inicio, fim);
        }
    }

    fclose(arquivo);
    printf("Arquivo 'leituras_geradas.txt' criado com sucesso!\n");
    return 0;
}
