#include <math.h>
#include "utils.h"

int cmpDouble(const double a, const double b) {
    if (myIsInf(a) && myIsInf(b)) return 0;
    if (myIsNan(a) && myIsNan(b)) return 0;
    if (fabs(a - b) < EPSILON) return 0;
    else return (a < b) ? -1 : 1;
}

int isZero(const double a) {
    return fabs(a) < EPSILON;
}

int myIsInf(const double a) {
    const unsigned long long binA = *((const unsigned long long *)  &a);
    const int k = 11, n = 64, m = n - k -1;
    //double: sign  exponent mantissa
    //bits    1     k = 11   m = 52
    //total   64 -> (unsigned) long long int is also 64 bits
    //inf:          1111111  00000000 == 0
    //nan:          1111111  01100101 != 0
    // src: IEEE Std 754-2008 page 47
    const unsigned long long maskExponent = ((1ULL << k) - 1ULL) << m; // all 1 in exponent
    const unsigned long long maskMantissa = (1ULL << m) - 1ULL;        // all 1 in mantissa

    return !(maskExponent - (binA & maskExponent)) && !(binA & maskMantissa); //exponent is all 1 and mantissa is 0
}


int myIsNan(const double a) {
    const unsigned long long binA = *((const unsigned long long *) &a);
    const int k = 11, n = 64, m = n - k -1;
    //double: sign  exponent mantissa
    //bits    1     k = 11   m = 52
    //total   64 -> (unsigned) long long int is also 64 bits
    //inf:          1111111  00000000 == 0
    //nan:          1111111  01100101 != 0
    // src: IEEE Std 754-2008 page 47
    const unsigned long long maskExponent = ((1ULL << k ) - 1ULL) << m; // all 1 in exponent
    const unsigned long long maskMantissa = (1ULL << m ) - 1ULL;        // all 1 in mantissa

    return !(maskExponent - (binA & maskExponent)) && (binA & maskMantissa); //exponent is all 1 and mantissa != 0
}


double fixMinusZero(const double num) {
    return (isZero(num)) ? fabs(num) : num;
}
