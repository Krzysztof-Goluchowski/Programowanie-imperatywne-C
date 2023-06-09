#include <stdio.h>
#include <math.h>

#define RECURS_LEVEL_MAX  10
#define N_MAX             10

// pointer to function of one variable
typedef double (*Func1vFp)(double);

// pointer to quadrature function
typedef double (*QuadratureFp)(Func1vFp, double a, double b, int n);

// pointer to function of two variables
typedef double (*Func2vFp)(double, double);

// example functions of one variable
double f_poly(double x) { // polynomial a[0] + a[1]x + ... + a[n]x^n
    double wynik = 0;
    double a = pow(x, 5);
    double b = pow(x, 4);
    double c = pow(x, 2);
    wynik = 2 * a - 4 * b + 3.5 * c + 1.35 * x - 6.25;
    return wynik;
}

double f_rat(double x) {
    double wynik = 0;
    double mianownik = 0;
    mianownik = pow(x-0.5, 2) + 0.01;
    wynik = 1/mianownik;
    return wynik;
}

double f_exp(double x) {
    double wynik = 0;
    wynik = 2 * x * exp(-1.5*x) - 1;
    return wynik;
}

double f_trig(double x) {
    double wynik = 0;
    wynik = x * tan(x);
    return wynik;
}

// Quadratures

// rectangle rule, leftpoint
double quad_rect_left(Func1vFp f1, double a, double b, int n) {
    double pieces = (b-a)/n;
    double result = 0;
    for (int i = 0; i < n; ++i) {
        result = result + ((b-i*pieces)-(b-i*pieces-pieces)) * f1((b-i*pieces-pieces));
    }
    return result;
}

// rectangle rule, rightpoint
double quad_rect_right(Func1vFp f1, double a, double b, int n) {
    double pieces = (b-a)/n;
    double result = 0;
    for (int i = 0; i < n; ++i) {
        result = result + ((b-i*pieces)-(b-i*pieces-pieces)) * f1(b-i*pieces);
    }
    return result;
}

// rectangle rule, midpoint
double quad_rect_mid(Func1vFp f1, double a, double b, int n) {
    double pieces = (b-a)/n;
    double result = 0;
    for (int i = 0; i < n; ++i) {
        double midpoint = ((b-i*pieces)+(b-i*pieces-pieces))/2;
        result = result + ((b-i*pieces)-(b-i*pieces-pieces)) * f1(midpoint);
    }
    return result;
}

// trapezoidal rule
double quad_trap(Func1vFp func, double a, double b, int n) {
    double pieces = (b-a)/n;
    double result = 0;
    for (int i = 0; i < n; ++i) {
        result = result + (((b-i*pieces)-(b-i*pieces-pieces))/2) * (func(b-i*pieces-pieces) + func(b-i*pieces));
    }
    return result;
}

// Simpson's rule
double quad_simpson(Func1vFp f, double a, double b, int n) {
    double pieces = (b-a)/n;
    double result = 0;
    for (int i = 0; i < n; ++i) {
        double midpoint = ((b-i*pieces)+(b-i*pieces-pieces))/2;
        result = result + (((b-i*pieces)-(b-i*pieces-pieces))/6) * (f(b-i*pieces-pieces) + 4 * f(midpoint) + f(b-i*pieces));
    }
    return result;
}


// array of pointers to integrand functions
Func1vFp func_tab[] = { f_poly, f_rat, f_trig, f_exp };

// array of pointers to quadrature functions
QuadratureFp quad_tab[] = {
        quad_rect_left, quad_rect_right, quad_rect_mid, quad_trap, quad_simpson };

// calls 'quad_no' quadrature function for 'fun_no' integrand function
// on interval [a, b] and n subintervals
double quad_select(int fun_no, int quad_no, double a, double b, int n) {
    QuadratureFp to_do = quad_tab[quad_no];
    Func1vFp f = func_tab[fun_no];
    double result = to_do(f, a, b, n);
    return result;
}

// adaptive algorithm
double recurs(Func1vFp f, double a, double b, double S, double delta, QuadratureFp quad, int level) {
    if  (RECURS_LEVEL_MAX == level){
        return NAN;
    }
    double c = (a+b)/2;
    double S1 = quad(f, a, c, 1);
    double S2 = quad(f, c, b, 1);
    double roznica = fabs(S1+S2-S);
    if (roznica <= delta){
        return S1+S2;
    }
    double result = recurs(f, a, c, S1, delta/2, quad, level+1)+recurs(f, c, b, S2, delta/2, quad, level+1);

    return result;

}

// initialization for adaptive algorithm
double init_recurs(Func1vFp f, double a, double b, double delta, QuadratureFp quad) {
    double S = quad(f, a, b, 1);
    double result = recurs(f, a, b, S, delta, quad, 0);
    return result;
}

// double integrals


double func2v_2(double x, double y) {
    return 2 - x*x - y*y*y;
}

// sample functions to define the normal domain

double lower_bound2(double x) {
    return 0.7*exp(-2*x*x);
}
double upper_bound2(double x) {
    return sin(10*x);
}


// rectangle rule (leftpoint) - double integral over rectangular domain
double dbl_integr(Func2vFp f, double x1, double x2, int nx, double y1, double y2, int ny) {
    double interval_x = (x2 - x1) / nx;
    double interval_y = (y2 - y1) / ny;
    double result = 0;
    double curr_x = x1;
    double curr_y = y1;

    for (int i = 0; i < nx; i++){
        for (int j = 0; j < ny; j++) {
            result += f(curr_x, curr_y) * interval_x * interval_y;
            curr_y += interval_y;
        }
        curr_y = y1;
        curr_x += interval_x;
    }
    return result;
}

// rectangle rule (midpoint) - double integral over normal domain with respect to the x-axis
double dbl_integr_normal_1(Func2vFp f, double x1, double x2, int nx, double hy,
                           Func1vFp fg, Func1vFp fh) {
    double interval_x = (x2 - x1) / nx;
    double result = 0;
    double curr_x = x1;
    double curr_y = fg(x1);


    for (int i = 0; i < nx; i++){
        double ny = (fh(curr_x) - fg(curr_x)) / hy;
        int n_y = ny;
        if (n_y < ny){
            n_y += 1;
        }
        double interval_y = (fh(curr_x) - fg(curr_x)) / n_y;
        for (int j = 0; j < n_y; j++) {
            result += f(curr_x, curr_y) * interval_x * interval_y;
            curr_y += interval_y;
        }
        curr_x += interval_x;
        curr_y = fg(curr_x);

    }
    return result;
}

// rectangle rule (leftpoint) - double integral over multiple normal
// domains with respect to the x-axis
double dbl_integr_normal_n(Func2vFp f, double x1, double x2, int nx, double y1, double y2,
                           int ny, Func1vFp fg, Func1vFp fh)  {
    double hx = (x2 - x1) / nx, hy = (y2 - y1) / ny, sum = 0, xi, gi, hi, yi;

    xi = x1;
    for(int i = 0; i < nx; i++) {
        gi = fg(xi);
        hi = fh(xi);
        double upBound = fmin(y2, hi);
        double downBound = fmax(y1, gi);

        int currNy = ceil((upBound - downBound) / hy);
        double currHy = (upBound - downBound) / currNy;

        yi = fmax(y1, gi);
        for (int j = 0; j < currNy; j++) {
            sum += currHy * f(xi, yi);
            yi += currHy;
        }
        xi += hx;
    }
    return hx * sum;
}

// multiple quadratures

typedef double (*FuncNvFp)(const double*, int);
typedef int (*BoundNvFp)(const double*, int);

// sample function of three variables
double func3v(const double v[], int n) {
    return v[0] - v[1] + 2*v[2];
}

// sample predicate in 3D
int bound3v(const double v[], int n) { // a cylinder
    return v[0] > 0 && v[0] < 0.5 && v[1]*v[1] + (v[2]-1)*(v[2]-1) < 1;
}

// sample function of n variables
double funcNv(const double v[], int n) {
    double fv = 1.;
    for (int i = 1; i < n; ++i) {
        fv += sin(i*v[i]);
    }
    return fv;
}
// sample n-dimensional predicate (n-dim hypersphere)
int boundNv(const double v[], int n) {
    double r = 0.0;
    for (int i = 0; i < n; ++i) r += (v[i]-1)*(v[i]-1);
    return r <= 1.;
}

// multiple integrals over a cuboid with predicate (if boundary != NULL)
// rectangular rule (rightpoint)
double trpl_quad_rect(FuncNvFp f, double variable_lim[][2], const int tn[], BoundNvFp boundary) {
    double interval_x = (variable_lim[0][1]-variable_lim[0][0])/tn[0];
    double interval_y = (variable_lim[1][1]-variable_lim[1][0])/tn[1];
    double interval_z = (variable_lim[2][1]-variable_lim[2][0])/tn[2];
    double curr_x = variable_lim[0][0]+interval_x;
    double curr_y = variable_lim[1][0]+interval_y;
    double curr_z = variable_lim[2][0]+interval_z;
    double result = 0;
    double tab[3];
    tab[0] = curr_x;
    tab[1] = curr_y;
    tab[2] = curr_z;

    if (boundary){
        for (int i = 0; i < tn[0]; ++i) {
            for (int j = 0; j < tn[1]; ++j) {
                for (int k = 0; k < tn[2]; ++k) {
                    if (bound3v(tab, 3)){
                        result += f(tab, 3) * interval_x * interval_y * interval_z;
                    }
                    curr_z += interval_z;
                    tab[2] = curr_z;
                }
                curr_z = variable_lim[2][0]+interval_z;
                tab[2] = curr_z;
                curr_y += interval_y;
                tab[1] = curr_y;
            }
            curr_y = variable_lim[1][0]+interval_y;
            tab[1] = curr_y;
            curr_z = variable_lim[2][0]+interval_z;
            tab[2] = curr_z;
            curr_x += interval_x;
            tab[0] = curr_x;
        }
    } else{
        for (int i = 0; i < tn[0]; ++i) {
            for (int j = 0; j < tn[1]; ++j) {
                for (int k = 0; k < tn[2]; ++k) {
                    result += f(tab, 3) * interval_x * interval_y * interval_z;
                    curr_z += interval_z;
                    tab[2] = curr_z;
                }
                curr_z = variable_lim[2][0]+interval_z;
                tab[2] = curr_z;
                curr_y += interval_y;
                tab[1] = curr_y;
            }
            curr_y = variable_lim[1][0]+interval_y;
            tab[1] = curr_y;
            curr_z = variable_lim[2][0]+interval_z;
            tab[2] = curr_z;
            curr_x += interval_x;
            tab[0] = curr_x;
        }
    }
    return result;
}

// multiple integrals over a n-dim hypercuboid with predicate (if boundary != NULL)
// rectangular rule (midpoint)
void recur_quad_rect_mid(double *psum, FuncNvFp f, int variable_no, double tvariable[], const double variable_lim[][2], const int tn[], int level, BoundNvFp boundary) {
    if (level == variable_no) {
        double mult = 1;
        for (int i = 0; i < variable_no; i++) {
            mult *= (variable_lim[i][1] - variable_lim[i][0]) / tn[i];
        }
        if (boundary == NULL || boundary(tvariable, variable_no) == 1) {
            *psum += f(tvariable, variable_no) * mult;
        }
        return;
    }

    double h = (variable_lim[level][1] - variable_lim[level][0]) / tn[level];
    double x = variable_lim[level][0] + 0.5 * h;
    for (int i = 0; i < tn[level]; i++) {
        tvariable[level] = x;
        x += h;
        recur_quad_rect_mid(psum, f, variable_no, tvariable, variable_lim, tn, level + 1, boundary);
    }
}

int main(void) {
    int to_do, n, nx, ny, integrand_fun_no, method_no, flag;
    int no_funcs = sizeof(func_tab) / sizeof(Func1vFp);
    int no_quads = sizeof(quad_tab) / sizeof(QuadratureFp);
    double a, b, x1, x2, y1, y2, hy, sum, delta;
    double t_variable[N_MAX], variable_lim[N_MAX][2];
    int tn[N_MAX];

    scanf("%d", &to_do);
    switch (to_do) {
        case 1: // loop over quadratures and integrands
            scanf("%lf %lf %d", &a, &b, &n);
            for(int q = 0; q < no_quads; ++q) {
                for(int f = 0; f < no_funcs; ++f) {
                    printf("%.5f ",quad_select(f, q, a, b, n));
                }
                printf("\n");
            }
            break;
        case 2: // adaptive algorithm
            scanf("%d %d",&integrand_fun_no,&method_no);
            scanf("%lf %lf %lf", &a, &b, &delta);
            printf("%.5f\n", init_recurs(func_tab[integrand_fun_no],a,b,delta,quad_tab[method_no]));
            break;
        case 3: // double integral over a rectangle
            scanf("%lf %lf %d", &x1, &x2, &nx);
            scanf("%lf %lf %d", &y1, &y2, &ny);
            printf("%.5f\n", dbl_integr(func2v_2, x1, x2, nx, y1, y2, ny));
            break;
        case 4: // double integral over normal domain
            scanf("%lf %lf %d", &x1, &x2, &nx);
            scanf("%lf", &hy);
            printf("%.5f\n", dbl_integr_normal_1(func2v_2, x1, x2, nx, hy, lower_bound2, upper_bound2));
            break;
        case 5: // double integral over multiple normal domains
            scanf("%lf %lf %d", &x1, &x2, &nx);
            scanf("%lf %lf %d", &y1, &y2, &ny);
            printf("%.5f\n",dbl_integr_normal_n(func2v_2, x1, x2, nx, y1, y2, ny, lower_bound2, upper_bound2));
            break;
        case 6: // triple integral over a cuboid
            scanf("%lf %lf %d", &variable_lim[0][0], &variable_lim[0][1], tn);
            scanf("%lf %lf %d", &variable_lim[1][0], &variable_lim[1][1], tn+1);
            scanf("%lf %lf %d", &variable_lim[2][0], &variable_lim[2][1], tn+2);
            scanf("%d", &flag);
            printf("%.5f\n", trpl_quad_rect(func3v, variable_lim, tn, flag ? bound3v : NULL));
            break;
        case 7: // multiple integral over hyper-cuboid
            scanf("%d", &n);
            if(n > N_MAX) break;
            for(int i = 0; i < n; ++i) {
                scanf("%lf %lf %d", &variable_lim[i][0], &variable_lim[i][1], tn+i);
            }
            scanf("%d", &flag);
            sum = 0.;
            recur_quad_rect_mid(&sum, funcNv, n, t_variable, variable_lim, tn, 0, flag ? boundNv : NULL);
            printf("%.5f\n", sum);
            break;
        default:
            printf("Nothing to do for %d\n", to_do);
            break;
    }
    return 0;
}
