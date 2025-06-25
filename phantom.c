#include "phantom.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função para exibir o logo e informações da ferramenta
void display_phantom_info() {
    printf("\n");
    printf("    .--.  .--.\n");
    printf("   |  |  |  |\n");
    printf("   |  |  |  |\n");
    printf("   |  |  |  |\n");
    printf("   |  `--`  |\n");
    printf("    `-.  .-\n");
    printf("      |  |\n");
    printf("      |  |\n");
    printf("      |  |\n");
    printf("      |  |\n");
    printf("      |  |\n");
    printf("      `--`\n");
    printf("\n");
    printf("  Phantom - Ferramenta de Quebra de Senhas\n");
    printf("  ---------------------------------------\n");
    printf("  Desenvolvido por Manus\n");
    printf("\n");
}

int main(int argc, char *argv[]) {
    display_phantom_info();

    PhantomArgs args;
    parse_arguments(argc, argv, &args);

    if (args.brute_force_mode) {
        printf("Modo: Força Bruta\n");
        printf("Hash Alvo: %s\n", args.target_hash);
        printf("Charset: %s\n", args.charset);
        printf("Comprimento Máximo: %d\n", args.max_len);
        brute_force(args.target_hash, args.charset, args.max_len);
    } else if (args.dictionary_mode) {
        printf("Modo: Dicionário\n");
        printf("Hash Alvo: %s\n", args.target_hash);
        printf("Caminho do Dicionário: %s\n", args.dictionary_path);
        dictionary_attack(args.target_hash, args.dictionary_path);
    }

    return 0;
}




// Implementação da função MD5
void hash_md5(const char *input, char *output) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5((unsigned char*)input, strlen(input), digest);

    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        sprintf(&output[i*2], "%02x", (unsigned int)digest[i]);
    }
    output[MD5_DIGEST_LENGTH*2] = '\0';
}




// Implementação da função de força bruta
bool brute_force(const char *target_hash, const char *charset, int max_len) {
    char password[max_len + 1];
    char hashed_password[MD5_DIGEST_LENGTH * 2 + 1];
    int charset_len = strlen(charset);

    // Inicializa o primeiro caractere
    for (int i = 0; i < max_len; i++) {
        password[i] = charset[0];
    }
    password[max_len] = '\0';

    printf("Iniciando ataque de força bruta...\n");

    // Loop para gerar e testar senhas
    // Esta é uma implementação simplificada e não otimizada para todas as combinações
    // Para uma implementação completa, seria necessário um algoritmo recursivo ou iterativo mais complexo
    // que gerasse todas as permutações de 'charset' até 'max_len'.
    // Por simplicidade e para demonstração, vamos apenas testar algumas combinações básicas.

    // Exemplo: testar senhas de comprimento 1 a max_len
    for (int len = 1; len <= max_len; len++) {
        // Resetar a senha para o comprimento atual
        for (int i = 0; i < len; i++) {
            password[i] = charset[0];
        }
        password[len] = '\0';

        bool done = false;
        while (!done) {
            hash_md5(password, hashed_password);
            if (strcmp(target_hash, hashed_password) == 0) {
                printf("Senha encontrada: %s\n", password);
                return true;
            }

            // Gerar próxima senha
            int i = len - 1;
            while (i >= 0) {
                if (password[i] == charset[charset_len - 1]) {
                    password[i] = charset[0];
                    i--;
                } else {
                    password[i] = charset[strchr(charset, password[i]) - charset + 1];
                    break;
                }
            }
            if (i < 0) {
                done = true;
            }
        }
    }

    printf("Senha não encontrada via força bruta.\n");
    return false;
}




// Implementação da função de ataque de dicionário
bool dictionary_attack(const char *target_hash, const char *dictionary_path) {
    FILE *fp;
    char line[256]; // Assumindo que nenhuma palavra no dicionário excederá 255 caracteres
    char hashed_password[MD5_DIGEST_LENGTH * 2 + 1];

    fp = fopen(dictionary_path, "r");
    if (fp == NULL) {
        perror("Erro ao abrir o arquivo de dicionário");
        return false;
    }

    printf("Iniciando ataque de dicionário...\n");

    while (fgets(line, sizeof(line), fp) != NULL) {
        // Remover o caractere de nova linha, se existir
        line[strcspn(line, "\n")] = 0;

        if (strlen(line) == 0) continue; // Pular linhas vazias

        hash_md5(line, hashed_password);
        if (strcmp(target_hash, hashed_password) == 0) {
            printf("Senha encontrada no dicionário: %s\n", line);
            fclose(fp);
            return true;
        }
    }

    fclose(fp);
    printf("Senha não encontrada no dicionário.\n");
    return false;
}




// Implementação da função para parsear os argumentos da linha de comando
void parse_arguments(int argc, char *argv[], PhantomArgs *args) {
    // Inicializa os argumentos com valores padrão
    args->target_hash = NULL;
    args->dictionary_path = NULL;
    args->charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"; // Default charset
    args->max_len = 8; // Default max length
    args->brute_force_mode = false;
    args->dictionary_mode = false;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printf("Uso: %s [opções]\n", argv[0]);
            printf("Opções:\n");
            printf("  -t <hash>      Hash alvo para quebrar (obrigatório para ambos os modos)\n");
            printf("  -b             Modo de força bruta\n");
            printf("  -d <caminho>   Modo de ataque de dicionário com o caminho do arquivo de dicionário\n");
            printf("  -c <charset>   Conjunto de caracteres para força bruta (padrão: alfanumérico)\n");
            printf("  -l <tamanho>   Comprimento máximo da senha para força bruta (padrão: 8)\n");
            printf("  -h, --help     Exibe esta mensagem de ajuda\n");
            exit(0);
        } else if (strcmp(argv[i], "-t") == 0) {
            if (i + 1 < argc) {
                args->target_hash = argv[++i];
            } else {
                fprintf(stderr, "Erro: -t requer um argumento.\n");
                exit(1);
            }
        } else if (strcmp(argv[i], "-b") == 0) {
            args->brute_force_mode = true;
        } else if (strcmp(argv[i], "-d") == 0) {
            args->dictionary_mode = true;
            if (i + 1 < argc) {
                args->dictionary_path = argv[++i];
            } else {
                fprintf(stderr, "Erro: -d requer um argumento.\n");
                exit(1);
            }
        } else if (strcmp(argv[i], "-c") == 0) {
            if (i + 1 < argc) {
                args->charset = argv[++i];
            } else {
                fprintf(stderr, "Erro: -c requer um argumento.\n");
                exit(1);
            }
        } else if (strcmp(argv[i], "-l") == 0) {
            if (i + 1 < argc) {
                args->max_len = atoi(argv[++i]);
            } else {
                fprintf(stderr, "Erro: -l requer um argumento.\n");
                exit(1);
            }
        } else {
            fprintf(stderr, "Erro: Opção desconhecida: %s\n", argv[i]);
            exit(1);
        }
    }

    // Validação básica: target_hash é obrigatório
    if (args->target_hash == NULL) {
        fprintf(stderr, "Erro: Hash alvo (-t) é obrigatório.\n");
        exit(1);
    }

    // Validação: Pelo menos um modo (brute-force ou dicionário) deve ser selecionado
    if (!args->brute_force_mode && !args->dictionary_mode) {
        fprintf(stderr, "Erro: Selecione o modo de força bruta (-b) ou modo de dicionário (-d).\n");
        exit(1);
    }

    // Validação: Não pode ter ambos os modos selecionados
    if (args->brute_force_mode && args->dictionary_mode) {
        fprintf(stderr, "Erro: Não é possível usar os modos de força bruta e dicionário simultaneamente.\n");
        exit(1);
    }
}


