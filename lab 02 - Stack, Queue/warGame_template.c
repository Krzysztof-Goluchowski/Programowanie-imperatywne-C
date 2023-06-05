
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int rand_from_interval(int a, int b){
    if (a > b){
        return INT_MIN;
    }
    if (b - a > RAND_MAX){
        return INT_MAX;
    }
    if (a == b){
        return a;
    }

    int ilosc_liczb = b+1-a;
    int wynik = rand()%ilosc_liczb+a;

    return wynik;
}

void swap (int array[], int i, int k) {
    int tmp;
    tmp = array[i];
    array[i] = array[k];
    array[k] = tmp;
}

// random permutation of integers from [0, n-1]
void rand_permutation(int n, int array[]) {
    for (int i = 0; i < n-1; i++){
        int k = rand_from_interval(i, n-1);
        swap(array, i, k);
    }
}

int lenA = 0;    // ilosc kart gracza A
int lenB = 0;    // ilosc kart gracza B
int outA = 0;    // index nastepnej kraty gracza A
int outB = 0;    // index nastepnej kraty gracza B
int playerA = 1; // gracz A
int playerB = 2; // gracz B
int graczA[52];  // karty gracza A
int graczB[52];  // karty gracza B

void cbuff_push(int nr_karty, int tab[], int gracz) {
    if (gracz == 1){
        int nr_ind = (outA+lenA)%52;
        tab[nr_ind] = nr_karty;
        lenA++;
    } else if (gracz == 2){
        int nr_ind = (outB+lenB)%52;
        tab[nr_ind] = nr_karty;
        lenB++;
    }
}

int cbuff_pop(int gracz) {
    if (gracz == 1){
        int nr_ind = outA;
        outA++;
        outA = outA%52;
        lenA--;
        return graczA[nr_ind];
    }else if (gracz == 2){
        int nr_ind = outB;
        outB++;
        outA = outB%52;
        lenB--;
        return graczB[nr_ind];
    }
}

int main(void){



    int seed, ver, conflicts;
    scanf("%d %d %d", &seed, &ver, &conflicts);
    srand((unsigned) seed);



        int talia[52];
        for (int i = 0; i < 52; i++) {
            talia[i] = i;
        }

        rand_permutation(52, talia); //tasujemy karty

        for (int i = 0; i < 26; i++) {  //rozdajemy potasowane karty dla gracza A
            cbuff_push(talia[i], graczA, playerA);

        }
        for (int i = 0; i < 26; i++) {  //rozdajemy potasowane karty dla gracza B
            cbuff_push(talia[i + 26], graczB, playerB);
        }

        int liczba_konfliktow = 0;
        int wojna[52]; // parzyste indeksy to karty gracza A, nieparzyste gracza B
        int ind = 0; // indeks tablicy wyzej

        if (ver == 0) {
            do {
                int X = lenA;
                int Y = lenB;
                int Z = outA;
                int G = outB;


                int A = cbuff_pop(playerA);
                int B = cbuff_pop(playerB);
                int A1 = A / 4;
                int B1 = B / 4;
                if (A1 > B1) {
                    cbuff_push(A, graczA, playerA);
                    cbuff_push(B, graczA, playerA);
                    liczba_konfliktow++;
                } else if (A1 < B1) {
                    cbuff_push(B, graczB, playerB);
                    cbuff_push(A, graczB, playerB);
                    liczba_konfliktow++;
                } else if (A1 == B1) {
                    int E = 1;
                    int F = 1;
                    liczba_konfliktow++;
                    wojna[ind] = A;
                    wojna[ind + 1] = B;
                    ind = ind + 2;
                    do {
                        X = lenA;
                        Y = lenB;
                        Z = outA;
                        G = outB;

                        int C = cbuff_pop(playerA);
                        int D = cbuff_pop(playerB);
                        wojna[ind] = C;
                        wojna[ind + 1] = D;
                        E = cbuff_pop(playerA);
                        F = cbuff_pop(playerB);
                        int E1 = E / 4;
                        int F1 = F / 4;
                        wojna[ind + 2] = E;
                        wojna[ind + 3] = F;
                        ind = ind + 4;
                        if (E1 > F1) {
                            liczba_konfliktow++;
                            for (int i = 0; i < ind; i = i + 2) {
                                cbuff_push(wojna[i], graczA, playerA);
                            }
                            for (int j = 1; j < ind; j = j + 2) {
                                cbuff_push(wojna[j], graczA, playerA);
                            }
                        } else if (E1 < F1) {
                            liczba_konfliktow++;
                            for (int i = 1; i < ind; i = i + 2) {
                                cbuff_push(wojna[i], graczB, playerB);
                            }
                            for (int i = 0; i < ind; i = i + 2) {
                                cbuff_push(wojna[i], graczB, playerB);
                            }
                        } else if (E1 == F1) {
                            liczba_konfliktow++;
                            E = E1;
                            F = F1;
                        }
                    } while (E == F && lenA > 2 && lenB > 2);
                    for (int i = 0; i < 52; i++) {
                        wojna[i] = 0;
                    }
                    ind = 0;
                }
            } while (conflicts > liczba_konfliktow && lenA > 0 && lenB > 0);

        } else if (ver == 1) {
            do {
                int A = cbuff_pop(playerA);
                int B = cbuff_pop(playerB);
                int A1 = A / 4;
                int B1 = B / 4;
                if (A1 > B1) {
                    cbuff_push(A, graczA, playerA);
                    cbuff_push(B, graczA, playerA);
                    liczba_konfliktow++;
                } else if (A1 < B1) {
                    cbuff_push(B, graczB, playerB);
                    cbuff_push(A, graczB, playerB);
                    liczba_konfliktow++;
                } else if (A1 == B1) {
                    cbuff_push(A, graczA, playerA);
                    cbuff_push(B, graczB, playerB);
                    liczba_konfliktow++;
                }
            } while (conflicts > liczba_konfliktow && lenA > 0 && lenB > 0);

        }

        if (lenA > 0 && lenB > 0 && liczba_konfliktow == conflicts) {
            printf("%d %d %d ", 0, lenA, lenB);
        }
        if (lenA > 0 && lenB > 0 && liczba_konfliktow < conflicts) {
            printf("%d %d %d ", 1, lenA, lenB);
        }
        if (lenB == 0) {
            printf("%d %d", 2, liczba_konfliktow);
        }
        if (lenA == 0) {
            printf("%d \n", 3);
            for (int i = 0; i < 52; i++) {
                printf("%d ", graczB[(outB + i) % 52]);
            }
        }

}
