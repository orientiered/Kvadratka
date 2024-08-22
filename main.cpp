///@file
///Program for solving quadratic equation

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
#include "inputHandler.h"

#define EXIT_BAD_INPUT 1 //exit when input can't be parsed correctly
#define EXIT_BAD_UNIT_TEST 2 //exit when unit tests failed

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
