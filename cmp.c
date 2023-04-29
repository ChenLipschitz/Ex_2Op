
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int compare_files(FILE *file1, FILE *file2, int ignore_case) {
    int equal = 1;
    int c1, c2;
    while ((c1 = fgetc(file1)) != EOF && (c2 = fgetc(file2)) != EOF) {
        if (ignore_case) {
            c1 = tolower(c1);
            c2 = tolower(c2);
        }
        if (c1 != c2) {
            equal = 0;
            break;
        }
    }
    return equal;
}

void print_result(int equal, int verbose) {
    if (equal) {
        if (verbose) {
            printf("equal\n");
        }
    } else {
        if (verbose) {
            printf("distinct\n");
        }
    }
}

int main(int argc, char *argv[]) {

    if (argc == 4)  //if there are 3 arguments look if the input is in the right format
    {
        if (strcmp(argv[3], "-v") != 0 && strcmp(argv[3], "-i") != 0)
        {   
            printf("Usage: cmp <file1> <file2> [-v] [-i]\n");
            return 1;
        }
    }

    //check for the right number of arguments- the input is according to the format
    if (argc != 3 && argc != 4 && argc !=5) {
        printf("Usage: cmp <file1> <file2> [-v] [-i]\n");
        return 1;
    }

    // Open the files
    FILE *file1 = fopen(argv[1], "r");
    if (!file1) {
        printf("Unable to open %s\n", argv[1]);
        return 1;
    }
    FILE *file2 = fopen(argv[2], "r");
    if (!file2) {
        printf("Unable to open %s\n", argv[2]);
        fclose(file1);
        return 1;
    }


    // Check if we need to ignore case and/or print verbose output
    int ignore_case = 0;
    int verbose = 0;
    for (int i = 3; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0) {
            ignore_case = 1;
        } else if (strcmp(argv[i], "-v") == 0) {
            verbose = 1;
        }
    }

    int equal = compare_files(file1, file2, ignore_case);

    print_result(equal, verbose);

    // Close the files
    fclose(file1);
    fclose(file2);

    return 0;
}
