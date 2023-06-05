
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_BIGRAMS ((LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR))

#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

int count[MAX_BIGRAMS] = { 0 };

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp (const void *a, const void *b) {
    int va = *(int*)a;
    int vb = *(int*)b;
    if (count[va] == count[vb]) return va - vb;	return count[vb] - count[va];
}

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp_di (const void *a, const void *b) {
    int va = *(int*)a;
    int vb = *(int*)b;
    // sort according to second char if counts and the first char equal
    if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) return va % MAX_CHARS - vb % MAX_CHARS;
    // sort according to first char if counts equal
    if (count[va] == count[vb]) return va / MAX_CHARS - vb / MAX_CHARS;
    return count[vb] - count[va];
}

// count lines, words & chars in a given text file
void wc(int *nl, int *nw, int *nc){
    *nc = 0;
    *nw = 1;
    *nl = 1;
    int flag = 0;
    int ch = getchar();
    int a;
    if(isspace(ch)){
        *nw = 0;
    }
    while (ch != EOF) {
        if (FIRST_CHAR <= ch && ch <= LAST_CHAR && flag == 1) {
            *nw = *nw + 1;
        }
        if (FIRST_CHAR <= ch && ch <= LAST_CHAR) {
            flag = 0;
        }
        if(ch == NEWLINE){
            *nl = *nl + 1;
            flag = 0;
        }
        if(isspace(ch)){
            flag = 1;
        }

        *nc = *nc + 1;
        a = ch;
        ch = getchar();
    }
    if (a == NEWLINE){
        *nl = *nl - 1;
    }
}

void char_count(int char_no, int *n_char, int *cnt) {
    int how_many[MAX_CHARS];
    int chars[MAX_CHARS];

    for(int i = 0; i < MAX_CHARS; i++){
        how_many[i] = 0;
    }

    int ch = getchar();
    while (ch != EOF){
        how_many[ch - FIRST_CHAR] = how_many[ch - FIRST_CHAR] + 1;
        ch = getchar();
    }

    for (int i = 0; i < MAX_CHARS; i++) {
        chars[i] = how_many[i];
    }

    qsort(how_many, MAX_CHARS, sizeof(int), cmp);

    *cnt = how_many[MAX_CHARS - char_no];

    int char_index = 0;
    for (int i = 0; i < MAX_CHARS; i++) {
        if(chars[i] == *cnt){
            char_index = i;
        }
    }
    *n_char = char_index + FIRST_CHAR;
}

void bigram_count(int bigram_no, int bigram[]) {
    int countt[MAX_CHARS][MAX_CHARS] = {{ 0 }};
    int c0 = EOF, c1;
    while ((c1 = getchar()) != EOF) {
        if (c1 >= 'A' && c1 <= 'z' && c0 >= 'A' && c0 <= 'z') {
            countt[c0 - 'A'][c1 - 'A']++;
        }
        c0 = c1;
    }

    int tab[MAX_CHARS*MAX_CHARS];
    for (int i = 0; i < MAX_CHARS*MAX_CHARS; i++){
        tab[i] = 0;
    }
    int i = 0;
    for (c0 = 'A'; c0 <= 'z'; c0++) {
        for (c1 = 'A'; c1 <= 'z'; c1++) {
            int n = countt[c0 - 'A'][c1 - 'A'];
            if (n) {
                tab[i] = n;
                i++;
            }
        }
    }
    qsort(tab, MAX_CHARS*MAX_CHARS, sizeof(int), cmp_di);
    int cnt = 0;
    cnt = tab[MAX_CHARS*MAX_CHARS-bigram_no];
    bigram[2] = cnt;
    int jeden = 0;
    int dwa = 0;
    for (c0 = 'A'; c0 <= 'z'; c0++) {
        for (c1 = 'A'; c1 < 'z'; c1++) {
            int n = countt[c0 - 'A'][c1 - 'A'];
            if (n == cnt){
                bigram[0] = c0;
                bigram[1] = c1;
            }
        }
    }
}

void find_comments(int *line_comment_counter, int *block_comment_counter) {
    int ch = getchar();
    int a = 0;
    *line_comment_counter = 0;
    *block_comment_counter = 0;
    int flag_liniowe = 0;
    int flag_blokowe = 0;
    int zaczete = 0;
    while (ch != EOF){
        if (ch == NEWLINE){
            flag_liniowe = 0;
        }
        if (a == '/' && ch == '/'){
            if (flag_liniowe == 0 && flag_blokowe == 0){
                *line_comment_counter = *line_comment_counter + 1;
                flag_liniowe = 1;
            }
        }

        if (a == '/' && ch == '*'){
            if (flag_blokowe == 0 && flag_liniowe == 0){
                flag_blokowe = 1;
                *block_comment_counter = *block_comment_counter + 1;
                zaczete = 1;
                a = 0;
                ch = 0;
            }
        }

        if (a == '*' && ch == '/' && zaczete == 1){
            flag_blokowe = 0;
            zaczete = 0;
            ch = 0;
        }

        a = ch;
        ch = getchar();
    }
    if (zaczete == 1){
        *block_comment_counter = *block_comment_counter - 1;
    }
}

#define MAX_LINE 128

int read_int() {
    char line[MAX_LINE];
    fgets(line, MAX_LINE, stdin); // to get the whole line
    return (int)strtol(line, NULL, 10);
}

int main(void) {
    int to_do;
    int nl, nw, nc, char_no, n_char, cnt;
    int line_comment_counter, block_comment_counter;
    int bigram[3];

    to_do = read_int();
    switch (to_do) {
        case 1: // wc()
            wc (&nl, &nw, &nc);
            printf("%d %d %d\n", nl, nw, nc);
            break;
        case 2: // char_count()
            char_no = read_int();
            char_count(char_no, &n_char, &cnt);
            printf("%c %d\n", n_char, cnt);
            break;
        case 3: // bigram_count()
            char_no = read_int();
            bigram_count(char_no, bigram);
            printf("%c%c %d\n", bigram[0], bigram[1], bigram[2]);
            break;
        case 4:
            find_comments(&line_comment_counter, &block_comment_counter);
            printf("%d %d\n", block_comment_counter, line_comment_counter);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}
