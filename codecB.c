void codecB_encode(char *msg) {
    for (int i = 0; msg[i]; i++) {
        msg[i] = msg[i] + 3;
    }
}

void codecB_decode(char *msg) {
    for (int i = 0; msg[i]; i++) {
        msg[i] = msg[i] - 3;
    }
}
