#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "encrypt_conn.h"

char *defargs[] = {"/bin/sh", NULL};

int main(int argc, char **argv){
    char *key = "secret";
    if(argc > 1) key = argv[1];
    enable_encryption(STDOUT_FILENO, key, strlen(key));
    char **args = defargs;
    if(argc > 2) args = &argv[2];
    return execve(args[0], args, NULL);
}
