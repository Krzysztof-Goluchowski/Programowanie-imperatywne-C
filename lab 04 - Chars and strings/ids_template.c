
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8

#define MAX_ID_LEN 64
#define MAX_IDS 1024

int index_cmp(const void*, const void*);
int cmp(const void*, const void*);

char identifiers[MAX_IDS][MAX_ID_LEN];

const char *keywords[] = {
	"auto", "break", "case", "char",
	"const", "continue", "default", "do",
	"double", "else", "enum", "extern",
	"float", "for", "goto", "if",
	"int", "long", "register", "return",
	"short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union",
	"unsigned", "void", "volatile", "while"
};

int find_idents() {
    int ch = getchar();
    char * unikaty[100];
    int ind_uni = 0;
    int licznik = 0;
    int i = 0;
    int flag = 0;
    while (ch != EOF){
        char tab[100];
        while (ch != ' ' && ch != '\t' && ch != '\n'){
            tab[i] = ch;
            i++;
            ch = getchar();
        }

        tab[i] = '\0';


        for (int j = 0; j < ind_uni; j++) {
            char str1[] = "slowo";
            strcpy(str1, tab);
            int elo = strcmp(unikaty[j], str1);
            if (elo == 0) {
                flag = 1;
            }
        }

        if (flag == 0){
            unikaty[ind_uni] = tab;
            ind_uni++;
            licznik++;
        }

        flag = 0;

        ch = getchar();
        i = 0;
    }
    return licznik;
}

int cmp(const void* first_arg, const void* second_arg) {
	char *a = *(char**)first_arg;
	char *b = *(char**)second_arg;
	return strcmp(a, b);
}

int index_cmp(const void* first_arg, const void* second_arg) {
	int a = *(int*)first_arg;
	int b = *(int*)second_arg;
	return strcmp(identifiers[a], identifiers[b]);
}

int main(void) {
	printf("%d\n", find_idents());
	return 0;
}

