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

#include "utils.h"
const int EXIT_BAD_INPUT =  1;      //exit when input can't be parsed correctly
const int EXIT_BAD_UNIT_TEST = 2;   //exit when unit tests failed


/*!
    @brief Prints help message and basic info about program

    @param[in] flags Pointer to flags (cmdFlags_t*)

    Prints help if -h flag is activated, <br>
    Basic info about program if -s flag isn't activated
*/
void initPrint(cmdFlags_t* flags);


/*!
    @brief Runs unit tests

    @param[in] flags Pointer to flags
    @param[in] argv Arguments of cmd

    Will run unit test from internal array or from specified in argv file if flags -u or -uf are activated
*/
enum error unitTester(cmdFlags_t* flags, char *argv[]);


/*!
    @brief Contains all logic to solve quadratic equation if coefficients are given in argv

    This function is made only to make main simpler
*/
enum error solveCmd(cmdFlags_t flags, enum error* scanResult, quadraticEquation_t* equation, char *argv[]);


/*!
    @brief Loop with all logic to solve quadartic equation many times

    Reads coefficients from console, solves equation, prints result, asks to solve again <br>
    This function is made only to make main simpler
*/
enum error solveLoop(cmdFlags_t flags, enum error* scanResult, quadraticEquation_t* equation);

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
    cmdFlags_t flags = BLANK_FLAGS;
    if (parseCmdArgs(&flags, (unsigned) argc, argv) == BAD_EXIT) { //parsing flags from console args
        printf("Can't read cmd args\n");
        flags = BLANK_FLAGS;
    }

    initPrint(&flags); //prints messages on start
    if (unitTester(&flags, argv) != GOOD_EXIT) //manages unit tests
        return 0;

    quadraticEquation_t equation = BLANK_QUADRATIC_EQUATION;
    enum error scanResult = BLANK;

    if (solveCmd(flags, &scanResult, &equation, argv) != GOOD_EXIT) //solves equation if it's coeffs were entered from cmd args
        return 0;
    if (solveLoop(flags, &scanResult, &equation) != GOOD_EXIT) //loop that solves equation with coeffs from console
        return 0;

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


enum error unitTester(cmdFlags_t* flags, char *argv[]) {
    #ifdef UNIT_TESTER_H
    if (flags->unitTest && unitTestingInternal(flags->silent) != GOOD_EXIT) {
        return BAD_EXIT;
    }

    if (flags->unitTestF && unitTestingFile(argv[flags->fileNamePos], flags->silent) != GOOD_EXIT) {
        return BAD_EXIT;
    }
    #else
        if (flags->unitTest || flags->unitTestF)
            fprintf(stderr, "Unit tests are not supported in this build\n");
    #endif
    return GOOD_EXIT;
}


enum error solveCmd(cmdFlags_t flags, enum error* scanResult, quadraticEquation_t* equation, char *argv[]) {
    if (flags.scanCoeffs) { //scanning from cmd args
        *scanResult = scanFromCmdArgs(equation, argv+flags.argPos);
        if (*scanResult != GOOD_EXIT) { ///in this case we don't want to read again
            if (!flags.silent)
                printf(RED_BKG "Wrong input format" RESET_C "\n");
            return BAD_EXIT;
        }
        if (!flags.silent)
            printKvadr(equation);
        solveEquation(equation);
        printAnswer(equation);
    }
    return GOOD_EXIT;
}


enum error solveLoop(cmdFlags_t flags, enum error* scanResult, quadraticEquation_t* equation) {
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
            return BAD_EXIT;
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
    return GOOD_EXIT;
}
