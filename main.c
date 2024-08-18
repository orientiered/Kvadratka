#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define EPSILON 1e-10 //constant for comparing floats

void printKvadr(double a, double b, double c); //output equation in nice format

typedef struct{
    int code = 0; //exit code, =number of found roots
    double x1 = 0;
    double x2 = 0;
} kvadrAnswer;

kvadrAnswer solve(double a, double b, double c); //solves equation, prints some comments and returns struct with answers


int main(int argc, char *argv[]) {
    if (argc != 4)
        printf("Enter coefficients of equation ax^2 + bx + c = 0\n");
    double a, b, c;
    a = b = c = 0;
    if (argc == 4) { //getting input from cmd args
        a = atof(*++argv);
        b = atof(*++argv);
        c = atof(*++argv);
    } else if (scanf("%lf %lf %lf", &a, &b, &c) != 3) { //getting input from user
        printf("Wrong input format\n");
        exit(1);
    }

    printKvadr(a, b, c);
    kvadrAnswer result = solve(a, b, c);
    if (result.code == 1) printf("x = %g\n", result.x1);
    else if (result.code == 2) printf("x1 = %g\nx2 = %g\n", result.x1, result.x2);

    return 0;
}

void printKvadr(double a, double b, double c) {
    int printedBefore = 0; //remembering if we printed something to put signs correctly
    if (fabs(a) > EPSILON) { //if not zero
        if (a < 0) printf("-"); //sign
        if (fabs(fabs(a)-1) > EPSILON) printf("%g", fabs(a)); //1x^2 is the same as x^2
        printf("x^2 ");
        printedBefore = 1;
    }
    if (fabs(b) > EPSILON) {
        if (printedBefore) printf((b < 0) ? "- " : "+ ");
        if (fabs(fabs(b)-1) > EPSILON) printf("%g", printedBefore ? fabs(b) : b);
        //if a == 0 => we should print -b, not "- b"
        printf("x ");
        printedBefore = 1;
    }

    if (printedBefore) printf((c < 0) ? "- %g = 0\n" : "+ %g = 0\n", fabs(c));
    else  printf("%g = 0\n", c);
}

kvadrAnswer solve(double a, double b, double c) {
    kvadrAnswer result;

    if (fabs(a) < EPSILON) { //checking for zeros in coefficients; we divide only by a, so this check is essential
        //printf("Linear equation detected\n");
        //a = 0
        if (fabs(b) < EPSILON) {
            //b = 0
            if (fabs(c) < EPSILON) // 0 = 0
                printf("x is any number\n");
            else
                printf("Equation can't be solved\n");
        } else {
            result.code = 1;
            result.x1 = -c/b;
        }
    } else {
        double D = b*b - 4*a*c; //calculating discriminant
        if (fabs(D) < EPSILON) {            //D = 0
            result.code = 1;
            result.x1 = -b / (2*a);
        } else if (D < 0) {                 //D < 0
            printf("Equation can't be solved in R: D = %g < 0\n", D);
        } else {                            //D > 0
            result.code = 2;
            result.x1 = (-b + sqrt(D))/(2*a);
            result.x2 = (-b - sqrt(D))/(2*a);
        }
    }
    return result;
}
