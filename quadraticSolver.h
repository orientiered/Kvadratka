/// @file
/// Headers of structs and functions


#ifndef QUADRATIC_SOLVER
#define QUADRATIC_SOLVER


/// @brief Exit codes used in solution_t struct
/// @see solution
enum solutionCode {
    ZERO_ROOTS,         ///< 0 roots
    ONE_ROOT,           ///< 1 root
    TWO_ROOTS,          ///< 2 roots
    INF_ROOTS,          ///< infinity roots, 0 = 0
    BLANK_ROOT = -1     ///< This code is used when solution_t is initialized
};


/// @brief Error codes which can be used in many functions
enum error {
    BAD_EXIT,           ///< Function handled fail inside it correctly
    GOOD_EXIT,          ///< Function worked correctly and all is as expected
    STRANGE_EXIT,       ///< Exit in condinitions that can't be reached
    FAIL                ///< Emergency exit
};


/// @brief Struct with flags used in cmd args
typedef struct cmdFlags {
    unsigned int silent: 1;     ///< Don't print anything except answer
    unsigned int unitTest: 1;   ///< Run unitTests before execution
    unsigned int help: 1;       ///< Print help

    unsigned int scanCoeffs: 1; ///< Indicates that program should try to parse cmd args as equation coeffs
    unsigned int argPos;        ///< Position in argv from which program will parse coeffs
} cmdFlags_t;

const cmdFlags_t BLANK_FLAGS = {0, 0, 0, 0, 0};


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

const quadraticEquation_t BLANK_QUADRATIC_EQUATION = {NAN, NAN, NAN, BLANK_SOLUTION};


/// @brief Struct which stores data for unit-testing
typedef struct unitTest {
    quadraticEquation_t inputData; //< Struct with test coefficients
    solution_t expectedData;       //< Struct with expected solutions
} unitTest_t;


/*!
    @brief Prints quadratic equation in nice format

    @param[in] equation Pointer to struct with coeffs

    @returns Enum with error code
    In current implementation return is always GOOD_EXIT

    Skips zero-coefs, and doesn't print 1
    Example: 0 1 -5
    => x - 5 = 0
*/
enum error printKvadr(const quadraticEquation_t* equation);


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
 *  @returns Enum with error code
 *  In current implementation return is always GOOD_EXIT
 *
 *  Solves quadratic equation in form ax^2 + bx + c = 0
 *  Prints comments, if there aren't any roots. Also fixes -0 in answer
*/
enum error solveQuadratic(quadraticEquation_t* equation);


/*!
    @brief Scans coeffs of quadratic eqaution from console

    @param[in, out] equation Pointer to struct that holds coeffs and answers

    @return Appropriate error code

    @see error
*/
enum error scanFromConsole(quadraticEquation_t* equation);


/*!
    @brief Tries to scan coeffs from current argv position

    @param[in, out] equation Pointer to struct that holds coeffs and answers
    @param[in] argv Arguments of cmd

    @return error code

    @see error
*/
enum error scanFromCmdArgs(quadraticEquation_t* equation, char *argv[]);


/*!
    @brief Prints roots of solved equation

    @param[in] equation Pointer to struct that holds coeffs and answers
*/
void printAnswer(const quadraticEquation_t* equation);


/*!
    @brief Runs unit-tests

    @return error code
*/
enum error unitTesting();


/*!
    @brief Runs exactly one test

    @param[in] test Struct with test data and expected data

    @return 1 if test is passed, 0 if failed
*/
enum error runTest(unitTest_t test);


/*!
    @brief Safe double comparison

    @param[in] a First number
    @param[in] b Second number

    @return -1 if a < b, 0 if a == b, 1 if a > b

    @see EPSILON
    Using EPSILON const to compare
*/
int cmpDouble(const double a, const double b);


/*!
    @brief Checks double on zero

    @return 1 if a == 0 else 0
*/
int isZero(const double a);


/*!
    @brief Clears stdin buffer using getchar() until it sees space character

    Ctrl+Z, Ctrl+D and EOF are also interpreted as space character
*/
void flushScanfBuffer();


/*!
    @brief Parses cmd args to cmdFlags struct

    Expected input format: --flags a b c
    Available flags: --help --h --u --s
    Example: .\main --u 1 2 3 => runs program with unit tests and coefficients 1 2 3
*/
enum error parseCmdArgs(cmdFlags_t* flags, unsigned int argc, char *argv[]);
#endif
