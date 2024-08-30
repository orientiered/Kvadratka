/// @file
/// @brief Quadratic equation input functions

#ifndef INPUT_HANDLER
#define INPUT_HANDLER

const int ASCII_EOT = 4;
const int ASCII_SUB = 26;


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

#endif
