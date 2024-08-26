#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "colors.h"
#include "error.h"
#include "quadrEquation.h"
#include "inputHandler.h"

enum error scanFromConsole(quadraticEquation_t* equation) {
    MY_ASSERT(equation, return FAIL;);

    const char coeffsNames[3] = {'a', 'b', 'c'};
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
                printf("%c = %lg\n", coeffsNames[index], *coeffsArray[index]);
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


enum error scanFromCmdArgs(quadraticEquation_t* equation, char *argv[]) {
    MY_ASSERT(equation, return FAIL);
    MY_ASSERT(argv, return FAIL);

    double *coeffsArray[] = {&(equation->a), &(equation->b), &(equation->c)};
    for (int i = 0; i < 3; ++i) {
        if (sscanf(*argv++, "%lf", coeffsArray[i]) != 1) {
            printf("Can't read #%d coefficient\n", i+1);
            return BAD_EXIT;
        }
    }
    return GOOD_EXIT;
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
        } else if (strcmp(argv[flags->argPos], "-uf") == 0) {
            flags->unitTestF =1;
            flags->fileNamePos = ++flags->argPos;
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


void printHelp() {
    printf(GREEN_BKG "########################################################" RESET_C "\n"
                " This program solves quadratic equation\n"
                " Available cmd args:\n"
                "   -h --help  Prints this message, ignores other flags\n"
                "   -s         Silent mode, prints only essential info\n"
                "   -u         Unit tests, runs built-in unit tests\n"
                "   -uf        Unit tests from file, expects next argument to be name of file\n"
                " After args you can type coefficients of equation\n"
                " If coefficients can't be parsed, program will ask you to enter them from console\n"
                " You should separate numbers with any space characters or end of lines\n"
                GREEN_BKG "########################################################" RESET_C "\n");
}
