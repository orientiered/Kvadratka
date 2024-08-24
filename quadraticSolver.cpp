#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "error.h"
#include "quadrEquation.h"
#include "quadraticSolver.h"
#include "colors.h"
#include "utils.h"


/*!
    @brief solves linear equation

    @param[in, out] equation Pointer to struct that holds coeffs and answers

    @return Enum with error code
*/
static enum error solveLinear(quadraticEquation_t* equation);


/*!
    @brief solves quadratic equation (a != 0)

    Note: function doesn't check a == o
    @param[in, out] equation Pointer to struct that holds coeffs and answers

    @return Enum with error code
*/
static enum error solveQuadratic(quadraticEquation_t* equation);


enum error solveEquation(quadraticEquation_t* equation) {
    MY_ASSERT(equation, return FAIL);//return BAEXIT

    double  a = equation->a,
            b = equation->b,
            c = equation->c;

    if (!isfinite(a) || !isfinite(b) || !isfinite(c)) { //checking input for NaNs
        equation->answer.code = BAD_INPUT;
        return FAIL;
    }


    if (isZero(a)) { // we divide only by a, so this check is essential
        solveLinear(equation);
    } else
        solveQuadratic(equation);

    //fix -0 case
    equation->answer.x1 = fixMinusZero(equation->answer.x1);
    equation->answer.x2 = fixMinusZero(equation->answer.x2);

    if (equation->answer.code == ONE_ROOT || equation->answer.code == TWO_ROOTS )
        MY_ASSERT(isfinite(equation->answer.x1), return FAIL);
    if (equation->answer.code == TWO_ROOTS)
        MY_ASSERT(isfinite(equation->answer.x2), return FAIL);

    return GOOD_EXIT;
}


static enum error solveLinear(quadraticEquation_t* equation) {
    if (isZero(equation->b)) { //checking for zeros in coefficients;
        //b = 0
        if (isZero(equation->c)) { // 0 = 0
            equation->answer.code = INF_ROOTS;
        } else {
            //printf("Equation can't be solved\n");
            equation->answer.code = ZERO_ROOTS;
        }
    } else {
        equation->answer.code = ONE_ROOT;
        equation->answer.x1 = -(equation->c)/(equation->b);
    }
    return GOOD_EXIT;
}


static enum error solveQuadratic(quadraticEquation_t* equation) {
    double  a = equation->a,
            b = equation->b,
            c = equation->c;

    const double D = b*b - 4*a*c; //calculating discriminant
    if (isZero(D)) {            //D = 0
        equation->answer.code = ONE_ROOT;
        equation->answer.x1 = -b / (2*a);
    } else if (cmpDouble(D, 0) == -1) {
        equation->answer.code = ZERO_ROOTS;              //D < 0
        //printf("Equation can't be solved in R: D = %g < 0\n", D);
    } else {                            //D > 0
        equation->answer.code = TWO_ROOTS;
        const double D_sqrt = sqrt(D);
        equation->answer.x1 = (-b - D_sqrt)/(2*a);
        equation->answer.x2 = (-b + D_sqrt)/(2*a);
    }
    return GOOD_EXIT;
}
