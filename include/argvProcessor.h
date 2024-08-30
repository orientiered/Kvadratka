#ifndef ARGV_PROCESSOR_H
#define ARGV_PROCESSOR_H


enum argType {
    tBLANK = 0,
    tINT,
    tFLOAT,
    tSTRING,
    tARRAYPTR
};

typedef struct argDescriptor {
    enum argType type;
    const char *argShortName;
    const char *argFullName;
    const char *argHelp;
} argDescriptor_t;

typedef struct argVal {
    int set;
    enum argType type;
    union {
        int _int;
        double _float;
        const char *_string;
        char **_arrayPtr;
    } val;
} argVal_t;

enum argNamesEnum {
    SILENT,
    UNIT,
    FILENAME,
    COEFFS,
    HELP
};

const argDescriptor_t args[] {
    {tBLANK,    "-s",   "--silent", "Reduce amount of prints"},
    {tBLANK,    "-u",   "--unit",   "Run unit tests"},
    {tSTRING,   "-f",   "--file",   "Next argument is name of file, from which program will read unit test"},
    {tARRAYPTR, "-c",   "--coeffs", "Coefficients of equation: a, b, c"},
    {tBLANK,    "-h",   "--help",   "Prints help message"}
};

const size_t argsSize = sizeof(args)/sizeof(argDescriptor_t);

enum error processArgs(argVal_t flags[], int argc, char *argv[]);

//these functions return number of argv element remained to scan
int scanFullArgument(argVal_t flags[], int remainToScan, char *argv[]);

int scanShortArguments(argVal_t flags[], int remainToScan, char *argv[]);

int scanToFlag(argVal_t* flag, int remainToScan, char *argv[]);

void initFlags(argVal_t flags[]);

void printHelpMessage();
#endif
