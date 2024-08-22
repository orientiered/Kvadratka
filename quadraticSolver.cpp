#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "error.h"
#include "quadrEquation.h"
#include "quadraticSolver.h"
#include "colors.h"
#include "testData.h"

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
            printf("x is any number");
            break;
        case BAD_INPUT:
            printf("Please check your input\n");
            break;
        default:
            printf("That's really bad :(\n");
            break;
    }
}


enum error unitTesting() {
    #ifndef TEST_DATA_INCLUDED
        printf("Include test data\n");
        return BAD_EXIT;
    #else
    const int testSize = sizeof(testData) / sizeof(unitTest_t);

    for (int testIndex = 0; testIndex < testSize; testIndex++) {
        if (runTest(testData[testIndex]) != GOOD_EXIT) {
            printf(RED_BKG "UNIT TESTING FAILED on test %d" RESET_C "\n", testIndex + 1);
            return BAD_EXIT;
        }
        else {
            printf(GREEN_BKG "Test #%d passed" RESET_C "\n", testIndex+1);
        }
    }

    #endif
    return GOOD_EXIT;
}


enum error runTest(unitTest_t test) {
    solveQuadratic(&test.inputData);
    solution_t result = test.inputData.answer;

    if (result.code != test.expectedData.code) { //checking exit code first
        printKvadr(&test.inputData); //print equation
        printf(RED_BKG "Exit code doesn't match: " GREEN_BKG "expected %d, " CYAN_BKG "got %d" RESET_C "\n",
                test.expectedData.code, result.code);
        return BAD_EXIT;
    } else {
        switch (result.code) {
            case INF_ROOTS:
            case BLANK_ROOT:
            case BAD_INPUT:
            case ZERO_ROOTS:
                return GOOD_EXIT;
            case ONE_ROOT:
                if(cmpDouble(result.x1, test.expectedData.x1) != 0) {
                    printKvadr(&test.inputData); //print equation
                    printf(RED_BKG "Answers doesn't match: " GREEN_BKG "expected x = %lg, " CYAN_BKG "got x = %lg" RESET_C "\n",
                    test.expectedData.x1, result.x1);
                    return BAD_EXIT;
                } else
                    return GOOD_EXIT;
            case TWO_ROOTS:
                if (result.x1 > result.x2)
                    SWAP_DOUBLE(result.x1, result.x2);
                if (cmpDouble(result.x1, test.expectedData.x1) != 0 || cmpDouble(result.x2, test.expectedData.x2) != 0) {
                    printKvadr(&test.inputData); //print equation
                    printf(RED_BKG "Answers doesn't match: " GREEN_BKG "expected x1 = %lg, x2 = %lg" RESET_C "\n"
                            CYAN_BKG "Got x1 = %lg, x2 = %lg" RESET_C "\n",
                            test.expectedData.x1, test.expectedData.x2, result.x1, result.x2);
                    return BAD_EXIT;
                } else
                    return GOOD_EXIT;
            default:
                return BAD_EXIT;
        }
    }
    return STRANGE_EXIT;
}


enum error solveQuadratic(quadraticEquation_t* equation) {
    assert(equation);

    double  a = equation->a,
            b = equation->b,
            c = equation->c;

    if (!isfinite(a) || !isfinite(b) || !isfinite(c)) {
        equation->answer.code = BAD_INPUT;
        return FAIL;
    }
    //checking input for NaNs


    if (isZero(a)) { //checking for zeros in coefficients; we divide only by a, so this check is essential
        //a = 0
        if (isZero(b)) {
            //b = 0
            if (isZero(c)) { // 0 = 0
                equation->answer.code = INF_ROOTS;
            } else {
                //printf("Equation can't be solved\n");
                equation->answer.code = ZERO_ROOTS;
            }
        } else {
            equation->answer.code = ONE_ROOT;
            equation->answer.x1 = -c/b;
        }
    } else {
        const double D = b*b - 4*a*c; //calculating discriminant
        if (isZero(D)) {            //D = 0
            equation->answer.code = ONE_ROOT;
            equation->answer.x1 = -b / (2*a);
        } else if (D < 0) {
            equation->answer.code = ZERO_ROOTS;              //D < 0
            //printf("Equation can't be solved in R: D = %g < 0\n", D);
        } else {                            //D > 0
            equation->answer.code = TWO_ROOTS;
            const double D_sqrt = sqrt(D);
            equation->answer.x1 = (-b - D_sqrt)/(2*a);
            equation->answer.x2 = (-b + D_sqrt)/(2*a);
        }
    }
    //fix -0 case
    equation->answer.x1 = fixMinusZero(equation->answer.x1);
    equation->answer.x2 = fixMinusZero(equation->answer.x2);
    if (equation->answer.code == ONE_ROOT || equation->answer.code == TWO_ROOTS )
        assert(isfinite(equation->answer.x1));
    if (equation->answer.code == TWO_ROOTS)
        assert(isfinite(equation->answer.x2));
    return GOOD_EXIT;
}
