/// @file common.h

#ifndef MX_COMMON_H
#define MX_COMMON_H

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
