#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define BUFSIZE 4096

int copy_file(char *src_filename, char *dest_filename, int verbose, int force) {
    FILE *src_file, *dest_file;
    char buffer[BUFSIZE];
    size_t bytes_read;

    // Open source file for reading
    src_file = fopen(src_filename, "r");
    if (!src_file) {
        printf("general failure\n");
        return 1;
    }

    // Check if destination file exists
    dest_file = fopen(dest_filename, "r");
    if (dest_file) {
        fclose(dest_file);
        if (!force) {
            if (verbose == 1) 
                printf("target file exist\n");
            fclose(src_file);
            return 1;
        }
    }

    // Open destination file for writing
    dest_file = fopen(dest_filename, "w");
    if (!dest_file) {
        printf("general failure\n");
        fclose(src_file);
        return 1;
    }

    // Copy data from source file to destination file
    while ((bytes_read = fread(buffer, 1, BUFSIZE, src_file)) > 0) {
        fwrite(buffer, 1, bytes_read, dest_file);
    }

    // Close files
    fclose(src_file);
    fclose(dest_file);

    if (verbose) {
        printf("success\n");
    }

    return 0;
}

int main(int argc, char *argv[]) {
    int verbose = 0, force = 0;

    if (argc == 4)  //if there are 3 arguments look if the input is in the right format
    {
        if (strcmp(argv[3], "-v") != 0 && strcmp(argv[3], "-f") != 0)
        {   
            printf("Usage: copy <source_file> <dest_file> [-v] [-f]\n");
            return 1;
        }
    }

    //check for the right number of arguments- the input is according to the format
    if (argc != 3 && argc != 4 && argc !=5) {
        printf("Usage: copy <source_file> <dest_file> [-v] [-f]\n");
        return 1;
    }

    // Check if there are verbose and force flags
    for (int i = 3; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0) {
            verbose = 1;
        } else if (strcmp(argv[i], "-f") == 0) {
            force = 1;
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "Usage: copy <source_file> <dest_file> [-v] [-f]\n");
            return 1;
        }
    }

    // Perform file copy
    if (copy_file(argv[1], argv[2], verbose, force) == 0) {
        return 0;
    } else {
        return 1;
    }
}
