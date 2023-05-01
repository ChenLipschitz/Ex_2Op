#include <ctype.h>

void codecA_encode(char *msg) {
    for (int i = 0; msg[i]; i++) {
        if (islower(msg[i])) {
            msg[i] = toupper(msg[i]);
        } else if (isupper(msg[i])) {
            msg[i] = tolower(msg[i]);
        }
    }
}

void codecA_decode(char *msg) {
    codecA_encode(msg);
}
