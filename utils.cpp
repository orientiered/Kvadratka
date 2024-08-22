#include <math.h>
#include "utils.h"

int cmpDouble(const double a, const double b) {
    if (isinf(a) && isinf(b)) return 0;
    if (isnan(a) && isnan(b)) return 0;
    if (fabs(a - b) < EPSILON) return 0;
    else return (a < b) ? -1 : 1;
}

int isZero(const double a) {
    return fabs(a) < EPSILON;
}

double fixMinusZero(const double num) {
    return (isZero(num)) ? fabs(num) : num;
}
