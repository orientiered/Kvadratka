#include "quadraticSolver.h"

int scanCoefficients(quadraticEquation_t* equation, int argc, char *argv[]) {
    assert(equation);
    assert(argv);

    if (argc != 4)
        printf("Enter coefficients of equation ax^2 + bx + c = 0\n");
    ///TODO: sscanf with cmd args
    if (argc == 4) { //getting input from cmd args
        const int MAXLEN = 1000;
        char str[MAXLEN] = "";
        ++argv;
        while (strlen(str) + strlen(*argv) < MAXLEN && --argc) strcat(str, *argv++);
        if (argc > 0) {
            printf("Buffer size exception\n");
            return 0;
        }
        if (sscanf(str, "%lf %lf %lf", &(equation->a), &(equation->b), &(equation->c)) != 3)
            printf("Wrong input, try again\n");
        else return 1;

    } else {
        int remainingTries = 3;
        while (remainingTries--) {
            if (scanf("%lf %lf %lf", &(equation->a), &(equation->b), &(equation->c)) != 3) {
                printf("Wrong input, you can try again %d times\n", remainingTries);
            } else return 1;
        }
        return 0;
    }
    return 1;
}


void printKvadr(quadraticEquation_t* equation) {
    assert(equation != NULL);

    int printedBefore = 0; //remembering if we printed something to put signs correctly

    double  a = equation->a,
            b = equation->b,
            c = equation->c;

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

    if (!(printedBefore && fabs(c) < EPSILON)) { //x + 0 <=> x
        if (printedBefore) printf((c < 0) ? "- %g " : "+ %g ", fabs(c));
        else printf("%g ", c);
    }
    printf("= 0\n");

}


void printAnswer(quadraticEquation_t* equation) {
    assert(equation != NULL);

    switch(equation->answer.code) {
        case BLANK_ROOT:
            printf("Something went wrong\n");
            break;
        case ZERO_ROOTS:
            break;
        case ONE_ROOT:
            printf("x = %lg\n", equation->answer.x1);
            break;
        case TWO_ROOTS:
            printf("x1 = %lg\nx2 = %lg\n", equation->answer.x1, equation->answer.x2);
            break;
        case INF_ROOTS:
            printf("x is any number");
            break;
        default:
            printf("That's really bad :(\n");
            break;
    }
}


quadraticEquation_t* solveQuadratic(quadraticEquation_t* equation) {
    assert(equation != NULL);

    double  a = equation->a,
            b = equation->b,
            c = equation->c;

    assert(isfinite(a)); //checking input for NaNs
    assert(isfinite(b));
    assert(isfinite(c));

    if (fabs(a) < EPSILON) { //checking for zeros in coefficients; we divide only by a, so this check is essential
        //a = 0
        if (fabs(b) < EPSILON) {
            //b = 0
            if (fabs(c) < EPSILON) { // 0 = 0
                equation->answer.code = INF_ROOTS;
            } else {
                printf("Equation can't be solved\n");
                equation->answer.code = ZERO_ROOTS;
            }
        } else {
            equation->answer.code = ONE_ROOT;
            equation->answer.x1 = -c/b;
        }
    } else {
        const double D = b*b - 4*a*c; //calculating discriminant
        if (fabs(D) < EPSILON) {            //D = 0
            equation->answer.code = ONE_ROOT;
            equation->answer.x1 = -b / (2*a);
        } else if (D < 0) {
            equation->answer.code = ZERO_ROOTS;              //D < 0
            printf("Equation can't be solved in R: D = %g < 0\n", D);
        } else {                            //D > 0
            equation->answer.code = TWO_ROOTS;
            const double D_sqrt = sqrt(D);
            equation->answer.x1 = (-b + D_sqrt)/(2*a);
            equation->answer.x2 = (-b - D_sqrt)/(2*a);
        }
    }
    //fix -0 case
    equation->answer.x1 = fixMinusZero(equation->answer.x1);
    equation->answer.x2 = fixMinusZero(equation->answer.x2);
    if (equation->answer.code == ONE_ROOT || equation->answer.code == TWO_ROOTS )
        assert(isfinite(equation->answer.x1));
    if (equation->answer.code == TWO_ROOTS)
        assert(isfinite(equation->answer.x2));
    return equation;
}

double fixMinusZero(const double num) {
    return (fabs(num) < EPSILON) ? fabs(num) : num;
}
