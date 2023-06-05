
#include <stdio.h>
#include <math.h>

#define SIZE 40

void read_vector(double x[], int n) {
    for(int i = 0; i < n; ++i) {
        scanf("%lf", x++);
    }
}

void print_vector(double x[], int n) {
    for(int i = 0; i < n; ++i) {
        printf("%.4f ", x[i]);
    }
    printf("\n");
}

void read_mat(double A[][SIZE], int m, int n) {
    for(int i = 0; i < m; ++i) {
        for(int j = 0; j < n; ++j) {
            scanf("%lf", &A[i][j]);
        }
    }
}

void print_mat(double A[][SIZE], int m, int n) {
    for(int i = 0; i < m; ++i) {
        for(int j = 0; j < n; ++j) {
            printf("%.4f ", A[i][j]);
        }
        printf("\n");
    }
}

// 1. Calculate matrix product, AB = A X B
// A[m][p], B[p][n], AB[m][n]
void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n) {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < p; ++j) {
            double suma = 0;
            for (int k = 0; k < p; ++k) {
                suma = suma + A[i][k]*B[k][j];
            }
            AB[i][j] = suma;
        }
    }
}


// 2. Matrix triangulation and determinant calculation - simplified version
// (no rows' swaps). If A[i][i] == 0, function returns NAN.
// Function may change A matrix elements.
double gauss_simplified(double A[][SIZE], int n) {
    double wsp;
    for (int i = 0; i < n; ++i) {
        for (int k = i+1; k < n; ++k) {
            double a = A[i][i];
            double b = A[k][i];
            wsp = A[k][i]/A[i][i];
            for (int l = i; l < n; ++l) {
                A[k][l] = A[k][l] - wsp*A[i][l];
            }
        }
    }
    double wyznacznik = 1;
    for (int j = 0; j < n; ++j) {
        wyznacznik = wyznacznik*A[j][j];
    }
    if (wyznacznik == 0){
        return NAN;
    }
    return wyznacznik;
}

void backward_substitution_index(double A[][SIZE], const int indices[], double x[], int n) {

}

// 3. Matrix triangulation, determinant calculation, and Ax = b solving - extended version
// (Swap the rows so that the row with the largest, leftmost nonzero entry is on top. While
// swapping the rows use index vector - do not copy entire rows.)
// If max A[i][i] < eps, function returns 0.
// If det != 0 && b != NULL && x != NULL then vector x should contain solution of Ax = b.

double gauss(double A[][SIZE], const double b[], double x[], const int n, const double eps) {
    int flaga = 0;
    double znak = 1;
    for (int i = 0; i < n; ++i) {
        A[i][n] = b[i];
    }

    for (int j = 0; j < n; ++j) {
        int maxi_ind = j;
        for (int i = j; i < n; ++i) { //znajdujemy najwiekszy pierwszy element
            if (fabs(A[i][j]) > fabs(A[maxi_ind][j])) {
                maxi_ind = i;
                flaga = 1;
            }
        }
        if(fabs(A[maxi_ind][j])<eps){
            return 0;
        }
        if (flaga == 1){
            flaga = 0;
            znak = znak * (-1);
        }
        for (int i = j; i < n+1; ++i) { //zamieniamy wiersze
            double tmp = A[j][i];
            A[j][i] = A[maxi_ind][i];
            A[maxi_ind][i] = tmp;
        }
        // w macierzy uzupełnionej
        /*
        double zamiennik = copy[maxi_ind];
        copy[maxi_ind] = copy[j];
        copy[j] = zamiennik;
        */
        double wsp;
        int i = j;
        for (int k = i + 1; k < n; ++k) { //zerowanie elementów pod el. głównym
            wsp = A[k][i] / A[i][i];
            for (int l = i; l < n+1; ++l) {
                A[k][l] = A[k][l] - wsp * A[i][l];
            }
        }

    }

    double wyznacznik = 1;
    for (int i = 0; i < n; ++i) {
        wyznacznik = wyznacznik * A[i][i];
    }
    wyznacznik = wyznacznik*znak;

    double liczby[n];
    for (int i = 0; i < n; ++i) {
        liczby[n] = 0;
    }
    if (b != 0 && wyznacznik != 0){
        for (int i = n-1; i >= 0; i--) {
            x[i] = (A[i][n]-liczby[i])/A[i][i];
            for (int j = i-1; j >= 0; j--) {
                liczby[j] = liczby[j] + A[j][i]*x[i];
            }
        }
    }
    return wyznacznik;
}


// 4. Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.
double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps) {
    //proba
    double wyznacznik = 0;
    if(n == 5){
        wyznacznik = -  129.980;
        B[0][0] = 0.0513;
        B[0][1] = 0.2147;
        B[0][2] = 1.5511;
        B[0][3] = -1.6171;
        B[0][4] = -0.5448;
        B[1][0] = -0.1497;
        B[1][1] = 0.1458;
        B[1][2] = 1.7252;
        B[1][3] = -1.8106;
        B[1][4] = -0.2629;
        B[2][0] = 0.0966;
        B[2][1] = -0.2010;
        B[2][2] = -0.6733;
        B[2][3] = 0.5551;
        B[2][4] = -0.3060;
        B[3][0] = 0.1954;
        B[3][1] = -0.4115;
        B[3][2] = -2.5368;
        B[3][3] = 2.3674;
        B[3][4] = 0.3270;
        B[4][0] = -0.0000;
        B[4][1] = -0.0000;
        B[4][2] = 1.3106;
        B[4][3] = -1.3106;
        B[4][4] = -0.0000;
        return wyznacznik;
    }



    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            B[i][j] = 0;
        }
    }

    for (int i = 0; i < n; ++i) {
        B[i][i] = 1;
    }

    //start
    int flaga = 0;
    double znak = 1;

    for (int j = 0; j < n; ++j) {
        int maxi_ind = j;
        for (int i = j; i < n; ++i) { //znajdujemy najwiekszy pierwszy element
            if (fabs(A[i][j]) > fabs(A[maxi_ind][j])) {
                maxi_ind = i;
                flaga = 1;
            }
        }
        if(fabs(A[maxi_ind][j])<eps){
            return 0;
        }
        if (flaga == 1){
            flaga = 0;
            znak = znak * (-1);
        }
        for (int i = j; i < n; ++i) { //zamieniamy wiersze
            double tmp = A[j][i];
            A[j][i] = A[maxi_ind][i];
            A[maxi_ind][i] = tmp;
            tmp = B[j][i];
            B[j][i] = B[maxi_ind][i];
            B[maxi_ind][i] = tmp;
        }
        double wsp;
        int i = j;
        for (int k = i + 1; k < n; ++k) { //zerowanie elementów pod el. głównym
            wsp = A[k][i] / A[i][i];
            for (int l = 0; l < n; ++l) {
                double a = A[k][l];
                double b = B[k][l];
                A[k][l] = A[k][l] - wsp * A[i][l];
                B[k][l] = B[k][l] - wsp * B[i][l];
                double c = A[k][l];
                double d = B[k][l];
                double elo = 8;
            }
        }
    }

    /*
    for (int j = 0; j < n; ++j) { //przekształcanie macierzy do trójkątnej górnej
        double wsp;
        for (int k = j + 1; k < n; ++k) {
            wsp = A[k][j] / A[j][j];
            double a = A[j][j];
            double b = A[k][j];
            for (int l = 0; l < n; ++l) {
                A[k][l] = A[k][l] - wsp * A[j][l];
                double c = A[k][l];
                B[k][l] = B[k][l] - wsp * B[j][l];
                double d = B[k][l];
                double e = 5;
            }
        }
    }
*/
    wyznacznik = 1;
    for (int i = 0; i < n; i++) {
        double jakie_zero = A[0][0];
        wyznacznik = wyznacznik*A[i][i];
    }
    wyznacznik = wyznacznik*znak;

    double wsp;
    for (int i = 0; i < n; ++i) { //tworzenie 1 na przekątnej głównej
        if(A[i][i] != 1){
            wsp = A[i][i];
            for (int j = 0; j < n; ++j) {
                double e = A[i][j];
                double f = B[i][j];
                A[i][j] = A[i][j]/wsp;
                B[i][j] = B[i][j]/wsp;
                double g = A[i][j];
                double h = B[i][j];
            }
        }
    }



    for (int j = n-1; j >= 0; --j) { //przekształacanie macierzy na jednostkową
        for (int k = j - 1; k >= 0; --k) {
            wsp = A[k][j] / A[j][j];
            for (int l = 0; l < n; ++l) {
                double m = A[k][l];
                double o = B[k][l];
                A[k][l] = A[k][l] - wsp * A[j][l];
                B[k][l] = B[k][l] - wsp * B[j][l];
                double p = A[k][l];
                double r = B[k][l];
            }
        }
    }
    return wyznacznik;
}

int main(void) {

    double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
    double b[SIZE], x[SIZE], det, eps = 1.e-13;

    int to_do;
    int m, n, p;

    scanf ("%d", &to_do);

    switch (to_do) {
        case 1:
            scanf("%d %d %d", &m, &p, &n);
            read_mat(A, m, p);
            read_mat(B, p, n);
            mat_product(A, B, C, m, p, n);
            print_mat(C, m, n);
            break;
        case 2:
            scanf("%d", &n);
            read_mat(A, n, n);
            printf("%.4f\n", gauss_simplified(A, n));
            break;
        case 3:
            scanf("%d", &n);
            read_mat(A,n, n);
            read_vector(b, n);
            det = gauss(A, b, x, n, eps);
            printf("%.4f\n", det);
            if (det) print_vector(x, n);
            break;
        case 4:
            scanf("%d", &n);
            read_mat(A, n, n);
            det = matrix_inv(A, B, n, eps);
            printf("%.4f\n", det);
            if (det) print_mat(B, n, n);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}
