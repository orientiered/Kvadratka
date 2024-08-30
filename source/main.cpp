/// @file
/// @brief Program for solving quadratic equation

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "error.h"
#include "quadrEquation.h"

#include "quadraticSolver.h"
#include "quadraticPrinter.h"
#include "unitTester.h"
#include "colors.h"
#include "inputHandler.h"
#include "argvProcessor.h"
#include "utils.h"
#include "main.h"


/*!
    @brief Main function

    1. Reads arguments from argv to flags variable<br>
    2. Prints welcome messages <br>
    3. Runs unit tests based on flags
    4. Tries to read coefficients from argv (they're first priority) and solve equation <br>
    5. Runs loop, where <br>
        1. Reads coefficients from console <br>
        2. Solves equation and prints answer <br>
        3. Asks if user want to solve it again <br>
*/
int main(int argc, char *argv[]) {
    argVal_t flags[argsSize] = {};
    initFlags(flags);
    if (processArgs(flags, argc, argv) == BAD_EXIT) { //parsing flags from console args
        printf("Can't read cmd args\n");
        initFlags(flags);
    }

    initPrint(flags); //prints messages on start
    if (unitTester(flags) != GOOD_EXIT) //manages unit tests
        return 0;

    quadraticEquation_t equation = BLANK_QUADRATIC_EQUATION;
    enum error scanResult = BLANK;

    if (solveCmd(flags, &scanResult, &equation) != GOOD_EXIT) //solves equation if it's coeffs were entered from cmd args
        return 0;
    if (solveLoop(flags, &scanResult, &equation) != GOOD_EXIT) //loop that solves equation with coeffs from console
        return 0;

    return 0;
}


void initPrint(argVal_t flags[]) {
    if (flags[HELP].set) { //printing help message
        printHelpMessage();
        initFlags(flags); //--help ignores ALL other flags
        return;
    }

    if (!flags[SILENT].set) { //if not silent mode
        printf(CYAN "# Quadratic equation solver\n# orientiered 2024" RESET_C "\n");
    }
}


enum error unitTester(argVal_t flags[]) {
    #ifdef UNIT_TESTER_H
    if (flags[UNIT].set) {
        if (!flags[FILENAME].set)
            unitTestingInternal(flags[SILENT].set);
        else
            unitTestingFile(flags[FILENAME].val._string, flags[SILENT].set);
    }
    #else
        if (flags[UNIT].set)
            fprintf(stderr, "Unit tests are not supported in this build\n");
    #endif
    return GOOD_EXIT;
}


enum error solveCmd(argVal_t flags[], enum error* scanResult, quadraticEquation_t* equation) {
    if (flags[COEFFS].set) { //scanning from cmd args
        *scanResult = scanFromCmdArgs(equation, flags[COEFFS].val._arrayPtr);
        if (*scanResult != GOOD_EXIT) { //in this case we don't want to read again
            if (!flags[SILENT].set)
                printf(RED_BKG "Wrong input format" RESET_C "\n");
            return BAD_EXIT;
        }
        if (!flags[SILENT].set)
            printKvadr(equation);
        solveEquation(equation);
        printAnswer(equation);
    }
    return GOOD_EXIT;
}


enum error solveLoop(argVal_t flags[], enum error* scanResult, quadraticEquation_t* equation) {
    bool readFromConsole = !flags[COEFFS].set;

    while (readFromConsole) {
        if (*scanResult != GOOD_EXIT) {
            if (!flags[SILENT].set)
                printf(YELLOW_BKG "Enter coefficients of equation ax^2 + bx + c = 0" RESET_C "\n");
            *scanResult = scanFromConsole(equation);
        }

        if (*scanResult != GOOD_EXIT) {
            if (!flags[SILENT].set)
                printf(RED_BKG "Scan failed: encountered Ctrl+D, Ctrl+Z or file ended" RESET_C "\n");
            return BAD_EXIT;
        }

        if (!flags[SILENT].set)
            printKvadr(equation);
        solveEquation(equation);
        printAnswer(equation);

        flushScanfBufferHard();
        printf(CYAN_BKG "Would you like to solve another equation?" RESET_C "\n"
              "y/N\n");
        int c = getchar();
        if (tolower(c) == 'y') {
            flushScanfBufferHard();
            *scanResult = BLANK;
        } else
            readFromConsole = false;

    }
    return GOOD_EXIT;
}
