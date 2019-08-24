#ifndef XFORM_H_
#define XFORM_H_

#include <stdint.h>

#define MAX_INT64_DIGITS 21
#define MAX_UINT64_DIGITS MAX_INT64_DIGITS
#define MAX_HEX_DIGITS MAX_INT64_DIGITS

#define ABS(x) x < 0 ? -(x) : x

void itoa(int64_t x, char* out);
void utoa(uint64_t x, char* out);
void hex(uint64_t x, char* out);

#endif /* XFORM_H_ */
