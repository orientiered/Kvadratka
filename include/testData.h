#ifndef TEST_DATA_INCLUDED
#define TEST_DATA_INCLUDED

const unitTest_t internalTestData[] = {
/*
        {                                                 unitTest_T
            {a, b, c, BLANK_SOLUTION},                    quadraticEquation_t
            {numberOfRoots, x1expected, x2expected}       solution_t
        }
*/
        {
            {0, 0, 0, BLANK_SOLUTION},
            {INF_ROOTS, NAN, NAN}
        },
        {
            {0, 0, 2.43, BLANK_SOLUTION},
            {ZERO_ROOTS, NAN, NAN}
        },
        {
            {1, 0, 3, BLANK_SOLUTION},
            {ZERO_ROOTS, NAN, NAN}
        },
        {
            {1, 0, -16, BLANK_SOLUTION},
            {TWO_ROOTS, -4, 4}
        },
        {
            {0, 2, 5, BLANK_SOLUTION},
            {ONE_ROOT, -2.5, NAN}
        },
        {
            {0, 54.234, 0, BLANK_SOLUTION},
            {ONE_ROOT, 0, NAN}
        },
        {
            {4, 2, 0, BLANK_SOLUTION},
            {TWO_ROOTS, -0.5, 0}
        },
        {
            {NAN, NAN, INFINITY, BLANK_SOLUTION},
            {BAD_INPUT, NAN, NAN}
        }
};

const unsigned int internalTestSize = sizeof(internalTestData) / sizeof(unitTest_t);

#endif
