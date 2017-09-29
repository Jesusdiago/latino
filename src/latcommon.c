#include "latino.h"

static int oct(int octalNumber) {
    int decimalNumber = 0, i = 0;
    while (octalNumber != 0) {
        decimalNumber += (octalNumber % 10) * pow(8, i);
        ++i;
        octalNumber /= 10;
    }
    i = 1;
    return decimalNumber;
}

char *tipo(int tipo) {
    switch (tipo) {
        case T_NULL:
            return "nulo";
            break;
        case T_BOOL:
            return "logico";
            break;
        case T_NUMERIC:
            return "decimal";
            break;
        case T_STR:
            return "cadena";
            break;
        case T_LIST:
            return "lista";
            break;
        case T_DIC:
            return "diccionario";
            break;
        case T_FUN:
            return "fun";
            break;
        case T_CFUN:
            return "cfun";
            break;
        default:
            return "indefinido";
            break;
    }
}

char *analizar_fmt(const char *s, size_t len) {
    char *ret = malloc(len + 1);
    int i = 0, j = 0, let = 0, c = 48;
    for (i = 0; i < ((int)len); i++) {
        switch (s[i]) {
            case '\\': {
                switch (s[i + 1]) {
                    case '/':
                        c = '/';
                        i++;
                        goto save;
                    case '"':
                        c = '\"';
                        i++;
                        goto save;
                    case 'a':
                        c = '\a';
                        i++;
                        goto save;
                    case 'b':
                        c = '\b';
                        i++;
                        goto save;
                    case 'f':
                        c = '\f';
                        i++;
                        goto save;
                    case 'n':
                        c = '\n';
                        i++;
                        goto save;
                    case 'r':
                        c = '\r';
                        i++;
                        goto save;
                    case 't':
                        c = '\t';
                        i++;
                        goto save;
                    case 'v':
                        c = '\v';
                        i++;
                        goto save;
                    case '\\':
                        c = '\\';
                        i++;
                        goto save;
                    case 'u':
                        c = s[i];
                        ret[j] = c;
                        j++;
                        i++;
                        int k;
                        for (k = 0; k <= 4; k++) {
                            c = s[i];
                            ret[j] = c;
                            j++;
                            i++;
                        }
                    default:
                        if
                            isdigit(s[i + 1]) {
                                while (isdigit(s[i + 1])) {
                                    let = (10 * let) + ((int)s[i + 1] - 48);
                                    i += 1;
                                };
                                c = oct(let);
                                let = 0;
                                goto save;
                            }
                        else {
                            c = s[i];
                            break;
                        }
                }
            } break;
            default:
                c = s[i];
                break;
        }
    save:
        ret[j] = c;
        j++;
    }
    ret[j] = '\0';
    return ret;
}

char *analizar(const char *s, size_t len) {
    char *ret = malloc(len + 1);
    int i = 0;
    int j = 0;
    int c = '@';
    for (i = 0; i < ((int)len); i++) {
        switch (s[i]) {
            case '\\': {
                switch (s[i + 1]) {
                    case '\\':
                        c = '\\';
                        i++;
                        ret[j] = c;
                        j++;
                        c = '\\';
                        i++;
                        ret[j] = c;
                        j++;
                    case 'u':
                        c = s[i];
                        ret[j] = c;
                        j++;
                        i++;
                        int k;
                        for (k = 0; k <= 4; k++) {
                            c = s[i];
                            ret[j] = c;
                            j++;
                            i++;
                        }
                    default:
                        c = s[i];
                        break;
                }
            } break;
            default:
                c = s[i];
                break;
        }
        ret[j] = c;
        j++;
    }
    ret[j] = '\0';
    // printf("ret: %s\n", ret);
    return ret;
}

char *decimal_acadena(double d) {
    char *buffer = calloc(1, 64);
    snprintf(buffer, 64, LAT_NUMERIC_FMT, d);
    return buffer;
}

char *logico_acadena(int i) {
    char *buffer = malloc(10);
    if (i)
        snprintf(buffer, 10, "%s", "verdadero");
    else
        snprintf(buffer, 10, "%s", "falso");
    buffer[9] = '\0';
    return buffer;
}

bool inicia_con(const char *base, const char *str) {
    return (strstr(base, str) - base) == 0;
}

bool termina_con(char *base, char *str) {
    int blen = strlen(base);
    int slen = strlen(str);
    return (blen >= slen) && (!strcmp(base + blen - slen, str));
}

static int intercambiar_pos(char *base, char *str, int startIndex) {
    int result;
    int baselen = strlen(base);
    if ((int)strlen(str) > baselen || startIndex > baselen) {
        result = -1;
    } else {
        if (startIndex < 0) {
            startIndex = 0;
        }
        char *pos = strstr(base + startIndex, str);
        if (pos == NULL) {
            result = -1;
        } else {
            result = pos - base;
        }
    }
    return result;
}

int ultima_pos(char *base, char *str) {
    int result;
    if (strlen(str) > strlen(base)) {
        result = -1;
    } else {
        int start = 0;
        int endinit = strlen(base) - strlen(str);
        int end = endinit;
        int endtmp = endinit;
        while (start != end) {
            start = intercambiar_pos(base, str, start);
            end = intercambiar_pos(base, str, end);
            if (start == -1) {
                end = -1;
            } else if (end == -1) {
                if (endtmp == (start + 1)) {
                    end = start;
                } else {
                    end = endtmp - (endtmp - start) / 2;
                    if (end <= start) {
                        end = start + 1;
                    }
                    endtmp = end;
                }
            } else {
                start = end;
                end = endinit;
            }
        }
        result = start;
    }
    return result;
}

char *insertar(char *dest, char *src, int pos) {
    int srclen = strlen(src);
    int dstlen = strlen(dest);
    if (pos < 0) {
        pos = dstlen + pos;
    }
    if (pos > dstlen) {
        pos = dstlen;
    }
    char *m = malloc(srclen + dstlen + 1);
    memcpy(m, dest, pos);
    memcpy(m + pos, src, srclen);
    memcpy(m + pos + srclen, dest + pos, dstlen - pos + 1);
    return m;
}

char *rellenar_izquierda(char *base, char *c, int n) {
    char *ret = malloc(MAX_STR_LENGTH);
    int len = strlen(base);
    int i, final = len - 1;
    for (i = 0; i < (n - final); i++) {
        ret = strcat(ret, c);
    }
    ret = strcat(ret, base);
    return ret;
}

char *rellenar_derecha(char *base, char *c, int n) {
    char *ret = malloc(MAX_STR_LENGTH);
    int len = strlen(base);
    strcpy(ret, base);
    int i, final = len - 1;
    for (i = 0; i < (n - final); i++) {
        ret = strcat(ret, c);
    }
    return ret;
}

char *reemplazar(char *o_string, char *s_string, char *r_string) {
    char *buffer = malloc(MAX_STR_LENGTH);
    char *ch;
    if (!(ch = strstr(o_string, s_string))) {
        strcpy(buffer, o_string);
        return buffer;
    }
    strncpy(buffer, o_string, ch - o_string);
    buffer[ch - o_string] = 0;
    sprintf(buffer + (ch - o_string), "%s%s", r_string, ch + strlen(s_string));
    // printf("REEMPLAZAR: %s\n", buffer);
    return buffer;
}

char *subcadena(const char *str, int beg, int n) {
    char *ret = malloc(n + 1);
    strncpy(ret, (str + beg), n);
    *(ret + n) = 0;
    return ret;
}

char *minusculas(const char *str) {
    int i = 0;
    int len = strlen(str);
    char *ret = malloc(len + 1);
    for (i = 0; i < len; i++) {
        ret[i] = tolower(str[i]);
    }
    ret[len] = 0;
    return ret;
}

char *mayusculas(const char *str) {
    int i = 0;
    int len = strlen(str);
    char *ret = malloc(len + 1);
    for (i = 0; i < len; i++) {
        ret[i] = toupper(str[i]);
    }
    ret[len] = 0;
    return ret;
}

char *quitar_espacios(const char *str) {
    char *start;
    char *end;
    for (start = (char *)str; *start; start++) {
        if (!isspace((unsigned char)start[0]))
            break;
    }
    for (end = start + strlen(start); end > start + 1; end--) {
        if (!isspace((unsigned char)end[-1]))
            break;
    }
    char *ret = malloc((end - start));
    *end = 0;
    if (start > str) {
        memcpy(ret, start, (end - start) + 1);
    } else {
        memcpy(ret, str, strlen(str));
    }
    return ret;
}
