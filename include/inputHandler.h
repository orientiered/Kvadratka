/// @file
/// @brief Quadratic equation input functions

#ifndef INPUT_HANDLER
#define INPUT_HANDLER

const int ASCII_EOT = 4;
const int ASCII_SUB = 26;


/// @brief Struct with flags used in cmd args
typedef struct cmdFlags {
    unsigned int silent: 1;         ///< Don't print anything except answer
    unsigned int unitTest: 1;       ///< Run unitTests before execution
    unsigned int unitTestF: 1;      ///< Run unitTests from file
    unsigned int fileNamePos;       ///< At this argv position lies name of file with unit tests
    unsigned int help: 1;           ///< Print help

    unsigned int scanCoeffs: 1;     ///< Indicates that program should try to parse cmd args as equation coeffs
    unsigned int argPos;            ///< Position in argv from which program will parse coeffs
} cmdFlags_t;

const cmdFlags_t BLANK_FLAGS = {0, 0, 0, 0, 0, 0, 0}; /// Empty initializer for cmdFlags_t


/*!
    @brief Scans coeffs of quadratic equation from console

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
    @brief Clears stdin buffer using getchar() until it sees space character

    Ctrl+Z, Ctrl+D and EOF are also interpreted as space character <br>
    This function can be used to skip input to next space character

    @return BAD_EXIT if sees Ctrl+D, Ctrl+Z or EOF else GOOD_EXIT
*/
enum error flushScanfBuffer();


/*!
    @brief Clears stdin buffer using getchar() until it sees EOL of EOF

    @return BAD_EXIT if sees Ctrl+D, Ctrl+Z or EOF else GOOD_EXIT

    You can use this function to clear input until new line to prepare it for new reads
*/
enum error flushScanfBufferHard();


/*!
    @brief Parses cmd args to cmdFlags struct

    Expected input format: --flags a b c <br>
    Available flags: --help -h -u -uf -s <br>
    Example: .\main -uf tests.txt 1 2 3  <br>
     => runs program with unit tests from file "tests.txt" and coefficients 1 2 3 <br>
*/
enum error parseCmdArgs(cmdFlags_t* flags, unsigned int argc, char *argv[]);


/*!
    @brief prints help message

    Used when --help argument is passed
*/
void printHelp();
#endif
