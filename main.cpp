///@file
///Program for solving quadratic equation

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "quadraticSolver.h"
#include "testData.h"
#include "colors.h"

const double EPSILON = 1e-10; //constant for comparing floats
const int ASCII_EOT = 4;
const int ASCII_SUB = 26;

#define EXIT_BAD_INPUT 1 //exit when input can't be parsed correctly
#define swapDouble(a, b) do{double c = b; b = a; a = c;}while(0)


int main(int argc, char *argv[]) {
    quadraticEquation_t equation = {NAN, NAN, NAN, BLANK_SOLUTION};

    cmdFlags_t flags = {0, 0, 0, 0, 0};
    parseCmdArgs(&flags, (unsigned) argc, argv);

    if (!flags.silent) {
        printf(CYAN "# Quadratic equation solver\n# orientiered 2024" RESET_C "\n");
        if (flags.unitTest) {
            int failedTest = 0;
            if ((failedTest = unitTesting()) != 0)
                printf(RED_BKG "UNIT TESTING FAILED on test %d" RESET_C "\n", failedTest);
        }
    }

    enum error scanResult = BAD_EXIT;

    if (flags.scanCoeffs)
        scanResult = scanFromCmdArgs(&equation, argv+flags.argPos);

    if (scanResult != GOOD_EXIT) {
        if (!flags.silent)
            printf(YELLOW_BKG "Enter coefficients of equation ax^2 + bx + c = 0" RESET_C "\n");

        scanResult = scanFromConsole(&equation);
    }

    if (scanResult != GOOD_EXIT) {
        if (!flags.silent)
            printf(RED_BKG "Wrong input format" RESET_C "\n");
        exit(EXIT_BAD_INPUT);
    }

    if (!flags.silent)
        printKvadr(&equation);
    solveQuadratic(&equation);
    printAnswer(&equation);

    return 0;
}


enum error scanFromCmdArgs(quadraticEquation_t* equation, char *argv[]) {
    assert(equation);
    assert(argv);

    if (sscanf(*argv++, "%lf", &(equation->a)) != 1) {
        printf("Can't read first coefficient\n");
        return BAD_EXIT;
    }
    if (sscanf(*argv++, "%lf", &(equation->b)) != 1) {
        printf("Can't read second coefficient\n");
        return BAD_EXIT;
    }
    if (sscanf(*argv++, "%lf", &(equation->c)) != 1) {
        printf("Can't read third coefficient\n");
        return BAD_EXIT;
    }
    return GOOD_EXIT;
}


enum error scanFromConsole(quadraticEquation_t* equation) {
    assert(equation);

    double *coeffsArray[] = {&(equation->a), &(equation->b), &(equation->c)};
    int index = 0;

    const int MAX_BUFFER_LEN = 1000;
    char strBuffer[MAX_BUFFER_LEN] = {};

    int scanfStatus = 0; //scanf return
    for (; index < 3; ) {
        if ((scanfStatus = scanf("%lf", coeffsArray[index])) == EOF) {
            break;
        } else if (scanfStatus != 1) {
            printf(RED_BKG "Wrong input format." RESET_C "\n");
            scanf("%s", strBuffer);
            printf(RED_BKG "Can't convert \"%s\" to double, skipping" RESET_C "\n", strBuffer);
            //flushScanfBuffer();
        } else {
            int c = 0;
            if (!isspace(c = getchar())) {
                ungetc(c, stdin);
                scanf("%s", strBuffer);
                printf(RED_BKG "Wrong input format" RESET_C "\n");
                printf(RED_BKG "Can convert to double, but \"%s\" is right after the number." RESET_C "\n", strBuffer);
                //flushScanfBuffer();
            } else index++;
        }
    }
    if (index == 3) return GOOD_EXIT;
    else return BAD_EXIT;
    return STRANGE_EXIT;
}


void flushScanfBuffer() {
    int c = 0;
    while (!isspace(c = getchar()) && c != '\n' && c != '\0' && c != EOF && c != ASCII_EOT && c != ASCII_SUB){}
    //printf("\n%p %d\n", stdin, c);
    //ungetc( (c == EOF) ? '\0' : c, stdin);
    //printf("%p %d\n", stdin, c);
}


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
        default:
            printf("That's really bad :(\n");
            break;
    }
}


int unitTesting() {
    #ifndef TEST_DATA_INCLUDED
        printf("Include test data\n");
    #else
    const int testSize = sizeof(testData) / sizeof(unitTest_t);

    for (int testIndex = 0; testIndex < testSize; testIndex++) {
        if (runTest(testData[testIndex]) != GOOD_EXIT)
            return testIndex + 1;
        else {
            printf(GREEN_BKG "Test #%d passed" RESET_C "\n", testIndex+1);
        }
    }
    #endif
    return 0;
}


enum error runTest(unitTest_t test) {
    solveQuadratic(&test.inputData);
    solution_t result = test.inputData.answer;

    if (result.code != test.expectedData.code) { //checking exit code first
        printKvadr(&test.inputData); //print equation
        printf("Exit code doesn't match: expected %d, got %d\n",
                test.expectedData.code, result.code);
        return BAD_EXIT;
    } else {
        switch (result.code) {
            case INF_ROOTS:
            case BLANK_ROOT:
            case ZERO_ROOTS:
                return GOOD_EXIT;
            case ONE_ROOT:
                if(cmpDouble(result.x1, test.expectedData.x1) != 0) {
                    printKvadr(&test.inputData); //print equation
                    printf( "Answers doesn't match: expected x = %lf, got x = %lf\n",
                    test.expectedData.x1, result.x1);
                    return BAD_EXIT;
                } else
                    return GOOD_EXIT;
            case TWO_ROOTS:
                if (result.x1 > result.x2)
                    swapDouble(result.x1, result.x2);
                if (cmpDouble(result.x1, test.expectedData.x1) != 0 || cmpDouble(result.x2, test.expectedData.x2) != 0) {
                    printKvadr(&test.inputData); //print equation
                    printf( "Answers doesn't match: expected x1 = %lf, x2 = %lf\n"
                            "Got x1 = %lf, x2 = %lf\n",
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


int cmpDouble(const double a, const double b) {
    assert(!isinf(a));
    assert(!isinf(b));
    if (isnan(a) && isnan(b)) return 0;
    if (fabs(a - b) < EPSILON) return 0;
    else return (a < b) ? -1 : 1;
}


int isZero(const double a) {
    return fabs(a) < EPSILON;
}


enum error solveQuadratic(quadraticEquation_t* equation) {
    assert(equation);

    double  a = equation->a,
            b = equation->b,
            c = equation->c;

    assert(isfinite(a)); //checking input for NaNs
    assert(isfinite(b));
    assert(isfinite(c));

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


double fixMinusZero(const double num) {
    return (isZero(num)) ? fabs(num) : num;
}


enum error parseCmdArgs(cmdFlags_t* flags, unsigned int argc, char *argv[]) {
    flags->argPos = 1;
    while (flags->argPos < argc) {
        if (strcmp(argv[flags->argPos], "--h") == 0) {
            flags->help = 1;
            return GOOD_EXIT;
        } else if (strcmp(argv[flags->argPos], "--s") == 0) {
            flags->silent = 1;
        } else if (strcmp(argv[flags->argPos], "--u") == 0) {
            flags->unitTest = 1;
        } else if (argc - flags->argPos == 3) {
            flags->scanCoeffs = 1;
            return GOOD_EXIT;
        }
        flags->argPos++;
    }
    return GOOD_EXIT;
}
