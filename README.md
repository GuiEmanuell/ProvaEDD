# ProvaEDD
# Projeto de Processamento e Consulta de Leituras de Sensores

## Descrição

Este projeto consiste em três programas em C para geração, organização e consulta de leituras de sensores.

---

## Programas

### 1. **geracao.c** — Geração de Dados de Teste

Gera um arquivo de leituras aleatórias de sensores, conforme parâmetros fornecidos pelo usuário.

- **Uso:**
  ```
  ./geracao "AAAA-MM-DD HH:MM:SS" "AAAA-MM-DD HH:MM:SS" sensor1:tipo [sensor2:tipo ...]
  ```
  - Exemplo:
    ```
    ./geracao "2025-06-14 00:00:00" "2025-06-15 23:59:59" temp:int pres:float flag:bool cod:string
    ```
- **Saída:**  
  Cria o arquivo `leituras_geradas.txt` com 2000 leituras para cada sensor, no formato:
  ```
  <TIMESTAMP> <ID_SENSOR> <VALOR>
  ```

- **Tipos suportados:**  
  - int
  - float
  - bool
  - string

---

### 2. **organizar.c** — Organização dos Dados por Sensor

Lê o arquivo de leituras gerado, identifica os sensores e separa os dados em arquivos distintos, um para cada sensor, ordenando as leituras por timestamp.

- **Uso:**
  ```
  ./organizar <arquivo_entrada>
  ```
  - Exemplo:
    ```
    ./organizar leituras_geradas.txt
    ```
- **Saída:**  
  Cria arquivos `sensor_<ID_SENSOR>.txt` para cada sensor, contendo as leituras ordenadas por timestamp.

---

### 3. **consulta.c** — Consulta de Leitura Mais Próxima

Permite consultar a leitura de um sensor mais próxima de um timestamp informado, utilizando busca binária para eficiência.

- **Uso:**
  ```
  ./consulta <nome_do_sensor> <timestamp_unix>
  ```
  - Exemplo:
    ```
    ./consulta temp 1718325600
    ```
- **Saída:**  
  Exibe no terminal a leitura mais próxima do timestamp informado para o sensor escolhido.

---



## Exemplo de Execução Completa

```sh
gcc geracao.c -o geracao
gcc organizar.c -o organizar
gcc consulta.c -o consulta

./geracao "2025-06-14 00:00:00" "2025-06-15 23:59:59" temp:int pres:float flag:bool cod:string
./organizar leituras_geradas.txt
./consulta temp 1718325600
```

---

