#ifndef TEST_DATA_INCLUDED
#define TEST_DATA_INCLUDED

#include "quadraticSolver.h"

unitTest_t testData[] = {
/*
        {
            {a, b, c, BLANK_SOLUTION},
            {numberOfRoots, x1expected, x2expected}
        }
*/
        {
            {0, 0, 0, BLANK_SOLUTION},
            {INF_ROOTS, 0, 0}
        },
        {
            {0, 0, 2.43, BLANK_SOLUTION},
            {ZERO_ROOTS, 0, 0}
        },
        {
            {1, 0, 3, BLANK_SOLUTION},
            {ZERO_ROOTS, 0, 0}
        },
        {
            {1, 0, -16, BLANK_SOLUTION},
            {TWO_ROOTS, -4, 4}
        },
        {
            {0, 2, 5, BLANK_SOLUTION},
            {ONE_ROOT, -2.5, 0}
        },
        {
            {0, 54.234, 0, BLANK_SOLUTION},
            {ONE_ROOT, 0, 0}
        },
        {
            {4, 2, 0, BLANK_SOLUTION},
            {TWO_ROOTS, -0.5, 0}
        }
    };

#endif
