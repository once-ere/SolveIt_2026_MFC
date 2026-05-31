/*        sledge.h --- header file for sledge.c                        */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef int logical; /* define a logical data type                     */

#define FALSE 0
#define TRUE  1
#define MAXK 40      /* maximum number of sampling points in classify  */
#define MAXF 200     /* maximum number of function evaluation in zzero */
#define PI 3.14159265358979324

void coeff(double, double*, double*, double*);

/*      declare functions in sledge.c      */

void    sledge(logical*, double*, logical*, int*, double*, double*,
               int*, double*, double*, int*);
int     interv(logical, double, double, double*, logical*, long int*,
               long int*);
double  aitken(double*, double, int, double*);
double  asymp_ev(int, double, double, double, double, double, double);
double  asymp_r(int, double, double, double, double);
int     bracket(int, double*, double*, double*, double*, double, double);
int     classify(double, logical, logical*, double*, long int*, logical*,
                 logical, logical*);
int     classify_end(double*, double*, double*, double*, double*, int, double*,
                  logical, int, double, double*, logical*, double*, logical*);
int     density_function(double*, logical*, int, int, int, double*,
                         double*, int*, long int*);
void    description(logical*, logical*, double*, long int);
logical extrapolation(double*, double, int, int, logical, logical, int,
                      double*, double*);
int     get_ef(double, double*, double*, logical*, logical*, double*,
               double*, double*);
int     get_r(double, double, logical*, double, double*);
int     get_rho(logical*, logical*, int*, int, double*, double*, double*);
int     mesh(logical, int, double);
int     power(double*, double*, double*, int, double, double*, double*,
              logical*, logical*);
int     pqrint(double, double*, double*);
int     regular(logical, logical, double*, int, double*, double*, double*);
int     shoot(double, long int*, double*);
int     start(logical*, double*, double*, int, int*, int*, int, double*);
int     step(double, double, double, double*, double*, double*, double*,
             double*, double*, double*, double*, int);
int     zero_calc(double*, double*, double*, double*, double, double);

/*       Utility functions in sledge.c      */

double   max_d(double, double);
double   min_d(double, double);
int      max_i(int, int);
int      min_i(int, int);
double   sign(double, double);
int      *vector_i(int);
double   *vector_d(int);
double   **matrix_d(int, int);
