#ifndef PHANTOM_H
#define PHANTOM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <openssl/md5.h>

// Funções de hashing
void hash_md5(const char *input, char *output);

// Funções de cracking
bool brute_force(const char *target_hash, const char *charset, int max_len);
bool dictionary_attack(const char *target_hash, const char *dictionary_path);

// Estrutura para armazenar os argumentos da linha de comando
typedef struct {
    char *target_hash;
    char *dictionary_path;
    char *charset;
    int max_len;
    bool brute_force_mode;
    bool dictionary_mode;
} PhantomArgs;

// Função para parsear os argumentos da linha de comando
void parse_arguments(int argc, char *argv[], PhantomArgs *args);

#endif // PHANTOM_H


