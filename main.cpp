///@file
///Program for solving quadratic equation

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

const double EPSILON = 1e-10; //constant for comparing floats
const int EOT = 4;
const int SUB = 26;
#define EXIT_BAD_INPUT 1 //exit when input can't be parsed correctly
#define swapDouble(a, b) do{double c = b; b = a; a = c;}while(0)

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
    STRANGE_EXIT,        ///< Exit in condinitions that can't be reached
    FAIL                ///< Emergency exit
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


/// @brief Struct which stores data for unit-testing
typedef struct unitTest {
    quadraticEquation_t inputData; //< Struct with test coefficients
    solution_t expectedData;       //< Struct with expected solutions
} unitTest_t;


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

    @return Appropriate error code

    @see error
*/
enum error scanCoefficients(quadraticEquation_t* equation, int argc, char *argv[]);


/*!
    @brief Prints roots of solved equation

    @param[in] equation Pointer to struct that holds coeffs and answers
*/
void printAnswer(const quadraticEquation_t* equation);


/*!
    @brief Runs unit-tests

    @return Number of failed test; 0 if all tests are passed
*/
int unitTesting();


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
    @brief Clears stdin buffer using getchar() until end of line
*/
void flushScanfBuffer();

int main(int argc, char *argv[]) {
    printf("# Quadratic equation solver\n# orientiered 2024\n");

    int failedTest = 0;
    if ((failedTest = unitTesting()) != 0)
     printf("UNIT TESTING FAILED on test %d\n", failedTest);

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

//Enum error { GOOD_ENDING=1, BAD_ENDING=0, STRANGE_ENDING=};
enum error scanCoefficients(quadraticEquation_t* equation, int argc, char *argv[]) {
    assert(equation);
    assert(argv);


    if (argc == 4) { //getting input from cmd args
        if (sscanf(*++argv, "%lf", &(equation->a)) != 1) {
            printf("Can't read first coefficient\n");
            return BAD_EXIT;
        }
        if (sscanf(*++argv, "%lf", &(equation->b)) != 1) {
            printf("Can't read second coefficient\n");
            return BAD_EXIT;
        }
        if (sscanf(*++argv, "%lf", &(equation->c)) != 1) {
            printf("Can't read third coefficient\n");
            return BAD_EXIT;
        }
        return GOOD_EXIT;
    } else {
        printf("Enter coefficients of equation ax^2 + bx + c = 0\n");
        double *coeffsArray[] = {&(equation->a), &(equation->b), &(equation->c)};
        int index = 0;

        const int MAX_BUFFER_LEN = 1000;
        char strBuffer[MAX_BUFFER_LEN];

        int scanfStatus = 0;
        for (; index < 3; ) {
            if ((scanfStatus = scanf("%lf", coeffsArray[index])) == EOF) {
                break;
            } else if (scanfStatus != 1) {
                printf("Wrong input format. \n");
                scanf("%s", strBuffer);
                printf("Can't convert \"%s\" to double, skipping\n", strBuffer);
                //flushScanfBuffer();
            } else {
                int c = 0;
                if (!isspace(c = getchar())) {
                    ungetc(c, stdin);
                    scanf("%s", strBuffer);
                    printf("Wrong input format. \n");
                    printf("Can convert to double, but \"%s\" is right after the number.\n", strBuffer);
                    //flushScanfBuffer();
                } else index++;
            }
        }
        if (index == 3) return GOOD_EXIT;
        else return BAD_EXIT;
    }
    return STRANGE_EXIT;
}

void flushScanfBuffer() {
    int c = 0;
    while (!isspace(c = getchar()) && c != '\n' && c != '\0' && c != EOF && c != EOT && c != SUB){}
    //printf("\n%p %d\n", stdin, c);
    //ungetc( (c == EOF) ? '\0' : c, stdin);
    //printf("%p %d\n", stdin, c);
}

void printKvadr(const quadraticEquation_t* equation) {
    assert(equation != NULL);

    int printedBefore = 0; //remembering if we printed something to put signs correctly

    double  a = equation->a,
            b = equation->b,
            c = equation->c;

    if (!isZero(a)) { //if not zero
        if (a < 0) printf("-"); //sign
        if (!cmpDouble(fabs(a), 1)) printf("%g", fabs(a)); //1x^2 is the same as x^2
        printf("x^2 ");
        printedBefore = 1;
    }

    if (!isZero(b)) {
        if (printedBefore) printf((b < 0) ? "- " : "+ ");
        if (!cmpDouble(fabs(b), 1)) printf("%g", printedBefore ? fabs(b) : b);
        //if a == 0 => we should print -b, not "- b"
        printf("x ");
        printedBefore = 1;
    }

    if (!(printedBefore && isZero(c))) { //x + 0 <=> x
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


int unitTesting() {
    unitTest_t testData[] = {
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
        }
    };
    const int testSize = sizeof(testData) / sizeof(unitTest_t);

    for (int testIndex = 0; testIndex < testSize; testIndex++) {
        if (runTest(testData[testIndex]) != GOOD_EXIT)
            return testIndex + 1;
        else {
            printf("Test #%d passed\n", testIndex+1);
        }
    }
    return 0;
}



enum error runTest(unitTest_t test) {
    solveQuadratic(&test.inputData);
    solution_t result = test.inputData.answer;

    if (result.code != test.expectedData.code) { //checking exit code first
        printKvadr(&test.inputData); //print equation
        printf("Exit code doesn't match: expected %d, got %d\n",
                test.expectedData.code, result.code);
        return BAD_EXIT;

    } else {

        switch (result.code) {
            case INF_ROOTS:
            case BLANK_ROOT:
            case ZERO_ROOTS:
                return GOOD_EXIT;
            case ONE_ROOT:
                if(cmpDouble(result.x1, test.expectedData.x1) != 0) {
                    printKvadr(&test.inputData); //print equation
                    printf( "Answers doesn't match: expected x = %lf, got x = %lf\n",
                    test.expectedData.x1, result.x1);
                    return BAD_EXIT;
                } else
                    return GOOD_EXIT;
            case TWO_ROOTS:
                if (result.x1 > result.x2)
                    swapDouble(result.x1, result.x2);
                if (cmpDouble(result.x1, test.expectedData.x1) != 0 || cmpDouble(result.x2, test.expectedData.x2) != 0) {
                    printKvadr(&test.inputData); //print equation
                    printf( "Answers doesn't match: expected x1 = %lf, x2 = %lf\n"
                            "Got x1 = %lf, x2 = %lf\n",
                            test.expectedData.x1, test.expectedData.x2, result.x1, result.x2);
                    return BAD_EXIT;
                } else
                    return GOOD_EXIT;
            default:
                return BAD_EXIT;
        }
    }
    return STRANGE_EXIT;
}


int cmpDouble(const double a, const double b) {
    assert(!isinf(a));
    assert(!isinf(b));
    if (isnan(a) && isnan(b)) return 0;
    if (fabs(a - b) < EPSILON) return 0;
    else return (a < b) ? -1 : 1;
}


int isZero(const double a) {
    return fabs(a) < EPSILON;
}

quadraticEquation_t* solveQuadratic(quadraticEquation_t* equation) {
    assert(equation);

    double  a = equation->a,
            b = equation->b,
            c = equation->c;

    assert(isfinite(a)); //checking input for NaNs
    assert(isfinite(b));
    assert(isfinite(c));

    if (isZero(a)) { //checking for zeros in coefficients; we divide only by a, so this check is essential
        //a = 0
        if (isZero(b)) {
            //b = 0
            if (isZero(c)) { // 0 = 0
                equation->answer.code = INF_ROOTS;
            } else {
                //printf("Equation can't be solved\n");
                equation->answer.code = ZERO_ROOTS;
            }
        } else {
            equation->answer.code = ONE_ROOT;
            equation->answer.x1 = -c/b;
        }
    } else {
        const double D = b*b - 4*a*c; //calculating discriminant
        if (isZero(D)) {            //D = 0
            equation->answer.code = ONE_ROOT;
            equation->answer.x1 = -b / (2*a);
        } else if (D < 0) {
            equation->answer.code = ZERO_ROOTS;              //D < 0
            //printf("Equation can't be solved in R: D = %g < 0\n", D);
        } else {                            //D > 0
            equation->answer.code = TWO_ROOTS;
            const double D_sqrt = sqrt(D);
            equation->answer.x1 = (-b - D_sqrt)/(2*a);
            equation->answer.x2 = (-b + D_sqrt)/(2*a);
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
    return (isZero(num)) ? fabs(num) : num;
}
