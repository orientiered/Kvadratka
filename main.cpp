///@file
///Program for solving quadratic equation

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


const int EXIT_BAD_INPUT =  1;      //exit when input can't be parsed correctly
const int EXIT_BAD_UNIT_TEST = 2;   //exit when unit tests failed


void initPrint(cmdFlags_t* flags);


void unitTester(cmdFlags_t* flags, char *argv[]);


void solveCmd(cmdFlags_t flags, enum error* scanResult, quadraticEquation_t* equation, char *argv[]);


void solveLoop(cmdFlags_t flags, enum error* scanResult, quadraticEquation_t* equation);


int main(int argc, char *argv[]) {
    cmdFlags_t flags = BLANK_FLAGS;
    if (parseCmdArgs(&flags, (unsigned) argc, argv) == BAD_EXIT) { //parsing flags from console args
        printf("Can't read cmd args\n");
        flags = BLANK_FLAGS;
    }

    initPrint(&flags);    //prints messages on start
    unitTester(&flags, argv);   //manages unit tests

    quadraticEquation_t equation = BLANK_QUADRATIC_EQUATION;
    enum error scanResult = BLANK;

    solveCmd(flags, &scanResult, &equation, argv); //solves equation if it's coeffs were entered from cmd args

    solveLoop(flags, &scanResult, &equation); //loop that solves equation with coeffs from console

    return 0;
}


void initPrint(cmdFlags_t* flags) {
    if (flags->help) { //printing help message
        printHelp();
        *flags = BLANK_FLAGS; //--help ignores ALL other flags
    }

    if (!flags->silent) { //if not silent mode
        printf(CYAN "# Quadratic equation solver\n# orientiered 2024" RESET_C "\n");
    }
}


void unitTester(cmdFlags_t* flags, char *argv[]) {
    #ifdef UNIT_TESTER_H
    if (flags->unitTest) {
        if (unitTestingInternal(flags->silent) != GOOD_EXIT)
            exit(EXIT_BAD_UNIT_TEST);
    }

    if (flags->unitTestF && unitTestingFile(argv[flags->fileNamePos], flags->silent) != GOOD_EXIT) {
        exit(EXIT_BAD_UNIT_TEST);
    }
    #else
        if (flags->unitTest || flags->unitTestF)
            printf("Unit tests are not supported in this build\n");
    #endif
}


void solveCmd(cmdFlags_t flags, enum error* scanResult, quadraticEquation_t* equation, char *argv[]) {
    if (flags.scanCoeffs) { //scanning from cmd args
        *scanResult = scanFromCmdArgs(equation, argv+flags.argPos);
        if (*scanResult != GOOD_EXIT) { ///in this case we don't want to read again
            if (!flags.silent)
                printf(RED_BKG "Wrong input format" RESET_C "\n");
            exit(EXIT_BAD_INPUT);
        }
        if (!flags.silent)
            printKvadr(equation);
        solveEquation(equation);
        printAnswer(equation);
    }
}


void solveLoop(cmdFlags_t flags, enum error* scanResult, quadraticEquation_t* equation) {
    bool readFromConsole = !flags.scanCoeffs;

    while (readFromConsole) {
        if (*scanResult != GOOD_EXIT) {
            if (!flags.silent)
                printf(YELLOW_BKG "Enter coefficients of equation ax^2 + bx + c = 0" RESET_C "\n");
            *scanResult = scanFromConsole(equation);
        }

        if (*scanResult != GOOD_EXIT) {
            if (!flags.silent)
                printf(RED_BKG "Scan failed: encountered Ctrl+D, Ctrl+Z or file ended" RESET_C "\n");
            exit(EXIT_BAD_INPUT);
        }

        if (!flags.silent)
            printKvadr(equation);
        solveEquation(equation);
        printAnswer(equation);

        flushScanfBufferHard();
        printf(CYAN_BKG "Would you like to solve another equation?" RESET_C "\n"
              "y/n\n");
        int c = getchar();
        if (tolower(c) == 'y') {
            flushScanfBufferHard();
            *scanResult = BLANK;
        } else
            readFromConsole = false;

    }
}
