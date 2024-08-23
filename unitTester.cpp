#include <stdio.h>

#include "error.h"
#include "quadrEquation.h"
#include "colors.h"
#include "quadraticSolver.h"
#include "quadraticPrinter.h"
#include "unitTester.h"
#include "utils.h"

#include "testData.h"


enum error unitTesting(int silent) {
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
        else if (!silent) {
            printf(GREEN_BKG "Test #%d passed" RESET_C "\n", testIndex+1);
        }
    }

    #endif
    return GOOD_EXIT;
}


enum error runTest(unitTest_t test) {
    solveEquation(&test.inputData);
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
