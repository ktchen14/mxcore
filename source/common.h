/// @file common.h

#ifndef MX_COMMON_H
#define MX_COMMON_H

#include <stdint.h>

/**
 * @brief A @c size_t sentinel to indicate the absence of an index into an array
 *
 * Because size_t can store the maximum size of a theoretically possible object
 * of any type (including array) and SIZE_MAX is the maximum value of an object
 * of size_t type, the largest possible index into an array of any type is
 * SIZE_MAX - 1. MX_ABSENT is defined as a sentinel to indicate an invalid index
 * and is equal to SIZE_MAX.
 */
#define MX_ABSENT SIZE_MAX

/**
 * @brief The equality function type to return whether @a a and @a b are
 *        equivalent
 *
 * An function of this type must return @c 0 if @a a and @a b aren't equivalent
 * and any @c int other than @c 0 if @a a and @a b are equivalent.
 */
typedef int (*eq_f)(const void *a, const void *b);

typedef int (*cmp_f)(const void *a, const void *b);

#endif /* MX_COMMON_H */
