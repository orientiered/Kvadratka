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
#define EXIT_BAD_UNIT_TEST 2 //exit when unit tests failed

#define SWAP_DOUBLE(a, b)           \
            do{                     \
                double c = b;       \
                b = a;              \
                a = c;              \
            }while(0)

#define PROPAGATE_ERROR(result)                         \
        do{                                             \
            enum error res = result;                    \
            if (res == BAD_EXIT || res == FAIL)         \
            {                                           \
                printf("Error on line %d\n", __LINE__); \
                return res;                             \
            }                                           \
        }while(0)
//__FUNCTION__ __PRETTY_FUNCTION__ __FILE__

int main(int argc, char *argv[]) {
    quadraticEquation_t equation = BLANK_QUADRATIC_EQUATION;

    cmdFlags_t flags = BLANK_FLAGS;
    if (parseCmdArgs(&flags, (unsigned) argc, argv) == BAD_EXIT) {
        printf("Can't read cmd args\n");
        flags = BLANK_FLAGS;
    }

    if (flags.help) {
        printf(GREEN_BKG "########################################################" RESET_C "\n"
                " This program solves quadratic equation\n"
                " Available cmd args:\n"
                "   -h --help  Prints this message, ignores other flags\n"
                "   -s         Silent mode, prints only essential info\n"
                "   -u         Unit tests, runs built-in unit tests\n"
                " After args you can type coefficients of equation\n"
                " If coefficients can't be parsed, program will ask you to enter them from console\n"
                " You should separate numbers with any space characters or end of lines\n"
                GREEN_BKG "########################################################" RESET_C "\n");
        flags = BLANK_FLAGS;
    }

    if (!flags.silent) {
        printf(CYAN "# Quadratic equation solver\n# orientiered 2024" RESET_C "\n");
        if (flags.unitTest) {
            if (unitTesting() != GOOD_EXIT)
                exit(EXIT_BAD_UNIT_TEST);
        }
    }

    enum error scanResult = BLANK;

    if (flags.scanCoeffs) { //scanning from cmd args
        scanResult = scanFromCmdArgs(&equation, argv+flags.argPos);
        if (scanResult != GOOD_EXIT) { ///in this case we don't want to read again
            if (!flags.silent)
                printf(RED_BKG "Wrong input format" RESET_C "\n");
            exit(EXIT_BAD_INPUT);
        }
        if (!flags.silent)
            printKvadr(&equation);
        solveQuadratic(&equation);
        printAnswer(&equation);
    }

    bool readFromConsole = !flags.scanCoeffs;

    while (readFromConsole) {
        if (scanResult != GOOD_EXIT) {
            if (!flags.silent)
                printf(YELLOW_BKG "Enter coefficients of equation ax^2 + bx + c = 0" RESET_C "\n");
            scanResult = scanFromConsole(&equation);
        }

        if (scanResult != GOOD_EXIT) {
            if (!flags.silent)
                printf(RED_BKG "Scan failed" RESET_C "\n");
            exit(EXIT_BAD_INPUT);
        }

        if (!flags.silent)
            printKvadr(&equation);
        solveQuadratic(&equation);
        printAnswer(&equation);
        flushScanfBufferHard();
        printf(CYAN_BKG "Would you like to solve another equation?" RESET_C "\n");
        printf("y/n\n");
        int c = getchar();
        if (tolower(c) == 'y') {
            flushScanfBufferHard();
            scanResult = BLANK;
        } else
            readFromConsole = false;

    };
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

    int scanfStatus = 0; //scanf return
    while (index < 3) {
        if ((scanfStatus = scanf("%lf", coeffsArray[index])) == EOF) {
            break;
        } else if (scanfStatus != 1) {
            printf(RED_BKG "Wrong input format." RESET_C "\n");
            PROPAGATE_ERROR(flushScanfBuffer());

        } else {
            int c = 0;
            if (!isspace(c = getchar())) {
                printf(RED_BKG "Wrong input format" RESET_C "\n");
                PROPAGATE_ERROR(flushScanfBuffer());
            } else {
                ungetc(c, stdin);
                printf("Last scanned number: %lg, total scanned: %d\n", *coeffsArray[index], index+1);
                index++;
            }
        }
    }
    if (index == 3) return GOOD_EXIT;
    else return BAD_EXIT;
    return STRANGE_EXIT;
}


enum error flushScanfBuffer() {
    int c = 0;
    while (!isspace(c = getchar()) && c != '\n' && c != '\0' && c != EOF && c != ASCII_EOT && c != ASCII_SUB){}
    //if (c == '\n') ungetc(c, stdin);
    if (c == EOF || c == ASCII_EOT || c == ASCII_SUB) return BAD_EXIT;
    return GOOD_EXIT;
}


enum error flushScanfBufferHard() {
    int c = 0;
    while ((c = getchar()) != '\n' && c != '\0' && c != EOF && c != ASCII_EOT && c != ASCII_SUB){}
    //if (c == '\n') ungetc(c, stdin);
    if (c == EOF || c == ASCII_EOT || c == ASCII_SUB) return BAD_EXIT;
    return GOOD_EXIT;
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


int cmpDouble(const double a, const double b) {
    if (isinf(a) && isinf(b)) return 0;
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


double fixMinusZero(const double num) {
    return (isZero(num)) ? fabs(num) : num;
}


enum error parseCmdArgs(cmdFlags_t* flags, unsigned int argc, char *argv[]) {
    flags->argPos = 1;
    while (flags->argPos < argc) {
        if (strcmp(argv[flags->argPos], "-h") == 0 ||
            strcmp(argv[flags->argPos], "--help") == 0) {
            flags->help = 1;
            return GOOD_EXIT;
        } else if (strcmp(argv[flags->argPos], "-s") == 0) {
            flags->silent = 1;
        } else if (strcmp(argv[flags->argPos], "-u") == 0) {
            flags->unitTest = 1;
        } else if (argc - flags->argPos == 3) {
            flags->scanCoeffs = 1;
            return GOOD_EXIT;
        } else {
            return BAD_EXIT;
        }
        flags->argPos++;
    }
    return GOOD_EXIT;
}
