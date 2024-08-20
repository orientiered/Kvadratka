///@file
///Program for solving quadratic equation

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

const double EPSILON = 1e-10; //constant for comparing floats
#define EXIT_BAD_INPUT 1 //exit when input can't be parsed correctly


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


/// @brief Struct that stores coeffs and answers of quadratic equation
typedef struct quadraticEquation {
    double a, b, c;     //< Coefficients of quadratic polynomial
    solution_t answer;  //< structure with exit code and answers
} quadraticEquation_t;



/*!
    @brief Prints quadratic equation in nice format

    @param[in] equation Pointer to struct with coeffs

    @return Nothing

    Skips zero-coefs, and doesn't print 1
    Example: 0 1 -5
    => x - 5 = 0
*/
void printKvadr(const quadraticEquation_t* equation);



/*!
    @brief Fixes -0

    @param[in] num Number to fix

    @return Fixed number
*/
double fixMinusZero(double num); //fix -0



/*!
 *  @brief solves equation, prints some comments and returns struct with answers
 *
 *  @param[in, out] equation Pointer to struct that holds coeffs and answers
 *
 *  @returns solution_t pointer to struct with roots and exit code
 *
 *  Solves quadratic equation in form ax^2 + bx + c = 0
 *  Prints comments, if there aren't any roots. Also fixes -0 in answer
*/
quadraticEquation_t* solveQuadratic(quadraticEquation_t* equation);



/*!
    @brief Scans coeffs of quadratic eqaution from cmd args (if possible) or from console input

    @param[in, out] equation Pointer to struct that holds coeffs and answers
    @param[in] argc Number of cmd arguments
    @param[in] argv Cmd arguments

    @return 0 if problem occured, 1 if input is readed correctly
*/
int scanCoefficients(quadraticEquation_t* equation, int argc, char *argv[]);


/*!
    @brief Prints roots of solved equation

    @param[in] equation Pointer to struct that holds coeffs and answers
*/
void printAnswer(const quadraticEquation_t* equation);


/*!

*/
void flushScanfBuffer();

int main(int argc, char *argv[]) {
    printf("# Quadratic equation solver\n# orientiered 2024\n");


    quadraticEquation_t equation = {NAN, NAN, NAN, BLANK_SOLUTION};
    if (!scanCoefficients(&equation, argc, argv)) {
        printf("Wrong input format\n");
        exit(EXIT_BAD_INPUT);
    }


    printKvadr(&equation);
    solveQuadratic(&equation);
    printAnswer(&equation);

    return 0;
}


int scanCoefficients(quadraticEquation_t* equation, int argc, char *argv[]) {
    assert(equation);
    assert(argv);


    if (argc == 4) { //getting input from cmd args
        if (sscanf(*++argv, "%lf", &(equation->a)) != 1) {
            printf("Can't read first coefficient\n");
            return 0;
        }
        if (sscanf(*++argv, "%lf", &(equation->b)) != 1) {
            printf("Can't read second coefficient\n");
            return 0;
        }
        if (sscanf(*++argv, "%lf", &(equation->c)) != 1) {
            printf("Can't read third coefficient\n");
            return 0;
        }
        return 1;
    } else {
        printf("Enter coefficients of equation ax^2 + bx + c = 0\n");
        double *coeffsArray[] = {&(equation->a), &(equation->b), &(equation->c)};
        int index = 0;
        int remainingTries = 3;

        for (; index < 3 && remainingTries > 0; ) {
            if (scanf("%lf", coeffsArray[index]) != 1) {
                printf("Wrong input format\n");
                index = 0; remainingTries--;
                flushScanfBuffer();
            } else {
                int c = 0;
                if (!isspace(c = getchar())) {
                    ungetc((c == EOF) ? 0: c, stdin);
                    printf("Wrong input format\n");
                    index = 0;
                    flushScanfBuffer();
                } else index++;
            }
        }
        if (index == 3) return 1;
        return 0;
    }
    return 1;
}

void flushScanfBuffer() {
    int c = 0;
    while ((c = getchar()) != '\n' && c != '\0' && c != EOF);
    ungetc( (c == EOF) ? '\0' : c, stdin);
}

void printKvadr(const quadraticEquation_t* equation) {
    assert(equation != NULL);

    int printedBefore = 0; //remembering if we printed something to put signs correctly

    double  a = equation->a,
            b = equation->b,
            c = equation->c;

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


void printAnswer(const quadraticEquation_t* equation) {
    assert(equation != NULL);

    switch(equation->answer.code) {
        case BLANK_ROOT:
            printf("Something went wrong\n");
            break;
        case ZERO_ROOTS:
            break;
        case ONE_ROOT:
            printf("x = %lg\n", equation->answer.x1);
            break;
        case TWO_ROOTS:
            printf("x1 = %lg\nx2 = %lg\n", equation->answer.x1, equation->answer.x2);
            break;
        case INF_ROOTS:
            printf("x is any number");
            break;
        default:
            printf("That's really bad :(\n");
            break;
    }
}


quadraticEquation_t* solveQuadratic(quadraticEquation_t* equation) {
    assert(equation != NULL);

    double  a = equation->a,
            b = equation->b,
            c = equation->c;

    assert(isfinite(a)); //checking input for NaNs
    assert(isfinite(b));
    assert(isfinite(c));

    if (fabs(a) < EPSILON) { //checking for zeros in coefficients; we divide only by a, so this check is essential
        //a = 0
        if (fabs(b) < EPSILON) {
            //b = 0
            if (fabs(c) < EPSILON) { // 0 = 0
                equation->answer.code = INF_ROOTS;
            } else {
                printf("Equation can't be solved\n");
                equation->answer.code = ZERO_ROOTS;
            }
        } else {
            equation->answer.code = ONE_ROOT;
            equation->answer.x1 = -c/b;
        }
    } else {
        const double D = b*b - 4*a*c; //calculating discriminant
        if (fabs(D) < EPSILON) {            //D = 0
            equation->answer.code = ONE_ROOT;
            equation->answer.x1 = -b / (2*a);
        } else if (D < 0) {
            equation->answer.code = ZERO_ROOTS;              //D < 0
            printf("Equation can't be solved in R: D = %g < 0\n", D);
        } else {                            //D > 0
            equation->answer.code = TWO_ROOTS;
            const double D_sqrt = sqrt(D);
            equation->answer.x1 = (-b + D_sqrt)/(2*a);
            equation->answer.x2 = (-b - D_sqrt)/(2*a);
        }
    }
    //fix -0 case
    equation->answer.x1 = fixMinusZero(equation->answer.x1);
    equation->answer.x2 = fixMinusZero(equation->answer.x2);
    if (equation->answer.code == ONE_ROOT || equation->answer.code == TWO_ROOTS )
        assert(isfinite(equation->answer.x1));
    if (equation->answer.code == TWO_ROOTS)
        assert(isfinite(equation->answer.x2));
    return equation;
}

double fixMinusZero(const double num) {
    return (fabs(num) < EPSILON) ? fabs(num) : num;
}
