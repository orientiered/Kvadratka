#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "error.h"
#include "quadrEquation.h"
#include "quadraticPrinter.h"
#include "colors.h"
#include "utils.h"

enum error printKvadr(const quadraticEquation_t* equation) {
    assert(equation != NULL);
    printf(YELLOW);
    int printedBefore = 0; //remembering if we printed something to put signs correctly

    double  a = equation->a,
            b = equation->b,
            c = equation->c;

    if (!isZero(a)) { //if not zero
        if (a < 0) printf("-"); //sign
        if (cmpDouble(fabs(a), 1)) printf("%g", fabs(a)); //1x^2 is the same as x^2
        printf("x^2 ");
        printedBefore = 1;
    }

    if (!isZero(b)) {
        if (printedBefore) printf((b < 0) ? "- " : "+ ");
        if (cmpDouble(fabs(b), 1)) printf("%g", printedBefore ? fabs(b) : b);
        //if a == 0 => we should print -b, not "- b"
        printf("x ");
        printedBefore = 1;
    }

    if (!(printedBefore && isZero(c))) { //x + 0 <=> x
        if (printedBefore) printf((c < 0) ? "- %g " : "+ %g ", fabs(c));
        else printf("%g ", c);
    }
    printf("= 0\n" RESET_C);
    return GOOD_EXIT;
}


void printAnswer(const quadraticEquation_t* equation) {
    assert(equation);

    switch(equation->answer.code) {
        case BLANK_ROOT:
            printf("Something went wrong\n");
            break;
        case ZERO_ROOTS:
            printf("There is no roots\n");
            break;
        case ONE_ROOT:
            printf("x = %lg\n", equation->answer.x1);
            break;
        case TWO_ROOTS:
            printf("x1 = %lg\nx2 = %lg\n", equation->answer.x1, equation->answer.x2);
            break;
        case INF_ROOTS:
            printf("x is any number\n");
            break;
        case BAD_INPUT:
            printf("Please check your input\n");
            break;
        default:
            printf("That's really bad :(\n");
            break;
    }
}
