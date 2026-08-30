int str_starts_with(char * str, char * prefix);
int str_equals(char * str, char * str2);
int str_len(char * s);

int str_starts_with(char * str, char * prefix) {
    for (int i = 0; prefix[i] != '\0'; i++) {
        if (str[i] != prefix[i]) {
            return 0;
        }
    }

    return 1;
}

int str_equals(char * input, char * str) {
    if (str_len(input) != str_len(str)) {
        return 0;
    }

    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] != str[i]) {
            return 0;
        }
    }

    return 1;
}

int str_len(char * s) {
    char * p = s;
    while (*p) {
        p++;
    }

    return p - s;
}