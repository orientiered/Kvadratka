#ifndef quadraticSolver.h
#define quadraticSolver.h

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
void printKvadr(quadraticEquation_t* equation);



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
void printAnswer(quadraticEquation_t* equation);

#endif
