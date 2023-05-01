#include <stdio.h>
#include <dlfcn.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <codec> <message>\n", argv[0]);
        return 1;
    }

    char *codec = argv[1];
    char *msg = argv[2];

    void *handle = dlopen(strcat(codec, ".so"), RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Error: %s\n", dlerror());
        return 1;
    }

    char (*decode)(char*);
    decode = dlsym(handle, "codecA_decode");
    if (!decode) {
        decode = dlsym(handle, "codecB_decode");
        if (!decode) {
            fprintf(stderr, "Error: %s\n", dlerror());
            return 1;
        }
    }

    decode(msg);
    printf("%s\n", msg);

    dlclose(handle);
    return 0;
}
