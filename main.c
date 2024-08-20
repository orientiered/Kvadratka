///@file
///Program for solving quadratic equation

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#define EPSILON 1e-10 //constant for comparing floats
#define EXIT_BAD_INPUT 1 //exit when input can't be parsed correctly

/*!
    @brief Prints quadratic equation in nice format

    @param[in] a First coefficient
    @param[in] b Second coefficient
    @param[in] c Third coefficient

    @return Nothing

    Skips zero-coefs, and doesn't print 1
    Example: 0 1 -5
    => x - 5 = 0
*/
void printKvadr(double a, double b, double c);


/*!
    @brief Fixes -0

    @param[in] num Number to fix

    @return Fixed number
*/
double fixMinusZero(double num); //fix -0


/// @brief Exit codes used in solution_t struct
/// @see solution
enum solutionCode {
    ZERO_ROOTS,         ///< 0 roots
    ONE_ROOT,           ///< 1 root
    TWO_ROOTS,          ///< 2 roots
    INF_ROOTS,          ///< infinity roots, 0 = 0
    BLANK_ROOT = -1     ///< This code is used when solution_t is initialized
};


/*!
    \brief Struct that stores solutions
*/
typedef struct solution{
    enum solutionCode code; ///< enum with exit codes; in basic cases = number of roots
    double x1;              ///< first root
    double x2;              ///< second root
} solution_t;


const solution_t BLANK_SOLUTION = {BLANK_ROOT, NAN, NAN};

/*!
 *  @brief solves equation, prints some comments and returns struct with answers
 *
 *  @param[in] a first coefficient
 *  @param[in] b second coefficient
 *  @param[in] c third coefficient
 *
 *  @returns solution_t struct with roots and exit code
 *
 *  Solves quadratic equation in form ax^2 + bx + c = 0
 *  Prints comments, if there aren't any roots. Also fixes -0 in answer
*/
solution_t solveQuadratic(double a, double b, double c);


int main(int argc, char *argv[]) {
    printf("# Quadratic equation solver\n# orientiered 2024\n");
    if (argc != 4)
        printf("Enter coefficients of equation ax^2 + bx + c = 0\n");

    double a, b, c;
    a = b = c = 0;

    if (argc == 4) { //getting input from cmd args
        a = atof(*++argv);
        b = atof(*++argv);
        c = atof(*++argv);
    } else if (scanf("%lf %lf %lf", &a, &b, &c) != 3) { //getting input from user
        printf("Wrong input format\n");
        exit(EXIT_BAD_INPUT);
    }

    printKvadr(a, b, c);
    solution_t result = solveQuadratic(a, b, c);

    switch(result.code) {
        case BLANK_ROOT:
            printf("Something went wrong\n");
            break;
        case ZERO_ROOTS:
            break;
        case ONE_ROOT:
            printf("x = %lg\n", result.x1);
            break;
        case TWO_ROOTS:
            printf("x1 = %lg\nx2 = %lg\n", result.x1, result.x2);
            break;
        case INF_ROOTS:
            printf("x is any number");
            break;
        default:
            printf("That's really bad :(\n");
            break;
    }
    return 0;
}

void printKvadr(double a, double b, double c) {
    int printedBefore = 0; //remembering if we printed something to put signs correctly

    if (fabs(a) > EPSILON) { //if not zero
        if (a < 0) printf("-"); //sign
        if (fabs(fabs(a)-1) > EPSILON) printf("%g", fabs(a)); //1x^2 is the same as x^2
        printf("x^2 ");
        printedBefore = 1;
    }

    if (fabs(b) > EPSILON) {
        if (printedBefore) printf((b < 0) ? "- " : "+ ");
        if (fabs(fabs(b)-1) > EPSILON) printf("%g", printedBefore ? fabs(b) : b);
        //if a == 0 => we should print -b, not "- b"
        printf("x ");
        printedBefore = 1;
    }

    if (!(printedBefore && fabs(c) < EPSILON)) { //x + 0 <=> x
        if (printedBefore) printf((c < 0) ? "- %g " : "+ %g ", fabs(c));
        else printf("%g ", c);
    }
    printf("= 0\n");

}

solution_t solveQuadratic(double a, double b, double c) {

    assert(isfinite(a)); //checking input for NaNs
    assert(isfinite(b));
    assert(isfinite(c));

    solution_t result = BLANK_SOLUTION;

    if (fabs(a) < EPSILON) { //checking for zeros in coefficients; we divide only by a, so this check is essential
        //a = 0
        if (fabs(b) < EPSILON) {
            //b = 0
            if (fabs(c) < EPSILON) { // 0 = 0
                result.code = INF_ROOTS;
            } else {
                printf("Equation can't be solved\n");
                result.code = ZERO_ROOTS;
            }
        } else {
            result.code = ONE_ROOT;
            result.x1 = -c/b;
        }
    } else {
        double D = b*b - 4*a*c; //calculating discriminant
        if (fabs(D) < EPSILON) {            //D = 0
            result.code = ONE_ROOT;
            result.x1 = -b / (2*a);
        } else if (D < 0) {
            result.code = ZERO_ROOTS;              //D < 0
            printf("Equation can't be solved in R: D = %g < 0\n", D);
        } else {                            //D > 0
            result.code = TWO_ROOTS;
            double D_sqrt = sqrt(D);
            result.x1 = (-b + D_sqrt)/(2*a);
            result.x2 = (-b - D_sqrt)/(2*a);
        }
    }
    //fix -0 case
    result.x1 = fixMinusZero(result.x1);
    result.x2 = fixMinusZero(result.x2);
    return result;
}

double fixMinusZero(double num) {
    return (fabs(num) < EPSILON) ? fabs(num) : num;
}
