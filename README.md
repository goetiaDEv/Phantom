# Phantom - Ferramenta de Quebra de Senhas

Phantom é uma ferramenta de linha de comando para quebra de senhas usando ataques de força bruta e dicionário. É desenvolvida em C para alta performance.

## Logo ASCII

```
    .--.  .--.
   |  |  |  |
   |  |  |  |
   |  |  |  |
   |  `--`  |
    `-.  .-
      |  |
      |  |
      |  |
      |  |
      |  |
      `--`
```

## Como Compilar

Para compilar o Phantom, você precisará do `gcc` e das bibliotecas OpenSSL de desenvolvimento. No Ubuntu/Debian, você pode instalá-los com:

```bash
sudo apt-get update
sudo apt-get install -y build-essential libssl-dev
```

Após a instalação das dependências, navegue até o diretório `phantom_cracker` e compile o código:

```bash
gcc phantom.c -o phantom -lssl -lcrypto
```

## Como Usar

### Sintaxe

```bash
./phantom [opções]
```

### Opções

*   `-t <hash>`: Hash alvo para quebrar (obrigatório para ambos os modos).
*   `-b`: Ativa o modo de força bruta.
*   `-d <caminho>`: Ativa o modo de ataque de dicionário e especifica o caminho para o arquivo de dicionário.
*   `-c <charset>`: Conjunto de caracteres para o modo de força bruta (padrão: `abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789`).
*   `-l <tamanho>`: Comprimento máximo da senha para o modo de força bruta (padrão: `8`).
*   `-h`, `--help`: Exibe a mensagem de ajuda.

### Exemplos

#### Ataque de Dicionário

1.  Crie um arquivo de dicionário (ex: `dictionary.txt`):

    ```
    senha123
    password
    123456
    qwerty
    ```

2.  Obtenha o hash MD5 da senha que você quer quebrar (ex: `password`):

    ```bash
echo -n 'password' | md5sum
# Saída: 5f4dcc3b5aa765d61d8327deb882cf99
    ```

3.  Execute o Phantom no modo dicionário:

    ```bash
./phantom -t 5f4dcc3b5aa765d61d8327deb882cf99 -d dictionary.txt
    ```

#### Ataque de Força Bruta

1.  Obtenha o hash MD5 da senha que você quer quebrar (ex: `abc`):

    ```bash
echo -n 'abc' | md5sum
# Saída: 900150983cd24fb0d6963f7d28e17f72
    ```

2.  Execute o Phantom no modo força bruta, especificando o charset e o comprimento máximo:

    ```bash
./phantom -t 900150983cd24fb0d6963f7d28e17f72 -b -c abc -l 3
    ```

## Limitações

*   Atualmente, o Phantom suporta apenas hashes MD5.
*   A implementação do ataque de força bruta é simplificada e pode não ser eficiente para senhas longas ou charsets muito grandes.

## Contribuições

Contribuições são bem-vindas! Sinta-se à vontade para abrir issues ou pull requests para melhorias, como suporte a outros algoritmos de hashing, otimizações de desempenho ou novas funcionalidades.


