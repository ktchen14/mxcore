/// @file header/vector/insert.h

#ifndef VECTOR_INSERT_H
#define VECTOR_INSERT_H

#include <stddef.h>
#include "common.h"

#ifdef VECTOR_TEST
#define inline
#endif /* VECTOR_TEST */

/**
 * @brief Insert the data at @a elmt into the @a vector at index @a i
 *
 * The index of the inserted element will be @a i. If @a i is the length of the
 * @a vector then this will append the element to the @a vector. If @a elmt is
 * @c NULL then the inserted element will be uninitialized.
 *
 * If the resultant length of the @a vector would overflow a @c size_t then this
 * will set @c errno to @c ENOMEM and return @c NULL. Otherwise this will call
 * vector_ensure(). If that fails then the @a vector will be unmodified and the
 * value of @c errno set by realloc() will be retained.
 *
 * If @a i is neither an index in the @a vector or its length then the behavior
 * is undefined. If @a elmt isn't @c NULL and its type is incompatible with the
 * element type of the @a vector then the behavior is undefined. If @a elmt is a
 * location in the @a vector itself then the behavior is undefined.
 *
 * @param vector the vector to operate on
 * @param i the index in the @a vector to insert the element
 * @param elmt the location of the element to insert
 * @return the resultant vector on success; otherwise @c NULL
 *
 * @see vector_insert_z() - the explicit interface analogue
 */
//= inline vector_t
//= vector_insert(restrict vector_t vector, size_t i, const void *restrict elmt)
#define vector_insert(v, ...) vector_insert_z((v), __VA_ARGS__, VECTOR_Z((v)))

/**
 * @brief Insert the data at @a elmt into the @a vector at index @a i
 *
 * The index of the inserted element will be @a i. If @a i is the length of the
 * @a vector then this will append the element to the @a vector. If @a elmt is
 * @c NULL then the inserted element will be uninitialized.
 *
 * If the resultant length of the @a vector would overflow a @c size_t then this
 * will set @c errno to @c ENOMEM and return @c NULL. Otherwise this will call
 * vector_ensure_z(). If that fails then the @a vector will be unmodified and
 * the value of @c errno set by realloc() will be retained.
 *
 * If @a i is neither an index in the @a vector or its length then the behavior
 * is undefined. If @a elmt isn't @c NULL and its type is incompatible with the
 * element type of the @a vector then the behavior is undefined. If @a elmt is a
 * location in the @a vector itself then the behavior is undefined.
 *
 * @param vector the vector to operate on
 * @param i the index in the @a vector to insert the element
 * @param elmt the location of the element to insert
 * @param z the element size of the @a vector
 * @return the resultant vector on success; otherwise @c NULL
 *
 * @see vector_insert() - the implicit interface analogue
 */
inline vector_t vector_insert_z(
    restrict vector_t vector,
    size_t i,
    const void *restrict elmt,
    size_t z)
  __attribute__((nonnull(1), warn_unused_result));

/**
 * @brief Insert @a n elements from @a elmt into the @a vector starting at index
 *   @a i
 *
 * The index of the first inserted element will be @a i. If @a i is the length
 * of the @a vector then this will append the elements to the @a vector. If
 * @a elmt is @c NULL then the inserted elements will be uninitialized. This is
 * more efficient than calling vector_insert() @a n times as the elements after
 * <code>i + n</code> will be shifted only once.
 *
 * If the resultant length of the @a vector would overflow a @c size_t then this
 * will set @c errno to @c ENOMEM and return @c NULL. Otherwise this will call
 * vector_ensure(). If that fails then the @a vector will be unmodified and the
 * value of @c errno set by realloc() will be retained.
 *
 * If @a i is neither an index in the @a vector or its length then the behavior
 * is undefined. If @a elmt isn't @c NULL and its type is incompatible with the
 * element type of the @a vector then the behavior is undefined. If @a elmt is a
 * location in the @a vector itself then the behavior is undefined.
 *
 * @param vector the vector to operate on
 * @param i the index in the @a vector to insert the elements
 * @param elmt the location of the elements to insert
 * @param n the number of elements to insert from @a elmt
 * @return the resultant vector on success; otherwise @c NULL
 *
 * @see vector_inject_z() - the explicit interface analogue
 */
//= inline vector_t vector_inject(
//=   restrict vector_t vector, size_t i, const void *restrict elmt, size_t n)
#define vector_inject(v, ...) vector_inject_z((v), __VA_ARGS__, VECTOR_Z((v)))

/**
 * @brief Insert @a n elements from @a elmt into the @a vector starting at index
 *   @a i
 *
 * The index of the first inserted element will be @a i. If @a i is the length
 * of the @a vector then this will append the elements to the @a vector. If
 * @a elmt is @c NULL then the inserted elements will be uninitialized. This is
 * more efficient than calling vector_insert_z() @a n times as the elements
 * after <code>i + n</code> will be shifted only once.
 *
 * If the resultant length of the @a vector would overflow a @c size_t then this
 * will set @c errno to @c ENOMEM and return @c NULL. Otherwise this will call
 * vector_ensure_z(). If that fails then the @a vector will be unmodified and
 * the value of @c errno set by realloc() will be retained.
 *
 * If @a i is neither an index in the @a vector or its length then the behavior
 * is undefined. If @a elmt isn't @c NULL and its type is incompatible with the
 * element type of the @a vector then the behavior is undefined. If @a elmt is a
 * location in the @a vector itself then the behavior is undefined.
 *
 * @param vector the vector to operate on
 * @param i the index in the @a vector to insert the elements
 * @param elmt the location of the elements to insert
 * @param n the number of elements to insert from @a elmt
 * @param z the element size of the @a vector
 * @return the resultant vector on success; otherwise @c NULL
 *
 * @see vector_inject() - the implicit interface analogue
 */
inline vector_t vector_inject_z(
    restrict vector_t vector,
    size_t i,
    const void *restrict elmt,
    size_t n,
    size_t z)
  __attribute__((nonnull(1), warn_unused_result));

/**
 * @brief Insert the data at @a elmt as the last element in the @a vector
 *
 * If @a elmt is @c NULL then the inserted element will be uninitialized.
 *
 * If the resultant length of the @a vector would overflow a @c size_t then this
 * will set @c errno to @c ENOMEM and return @c NULL. Otherwise this will call
 * vector_ensure(). If that fails then the @a vector will be unmodified and the
 * value of @c errno set by realloc() will be retained.
 *
 * If @a elmt isn't @c NULL and its type is incompatible with the element type
 * of the @a vector then the behavior is undefined. If @a elmt is a location in
 * the @a vector itself then the behavior is undefined.
 *
 * @param vector the vector to operate on
 * @param elmt the location of the element to append
 * @return the resultant vector on success; otherwise @c NULL
 *
 * @see vector_append_z() - the explicit interface analogue
 */
//= inline vector_t
//= vector_append(restrict vector_t vector, const void *restrict elmt)
#define vector_append(v, ...) vector_append_z((v), __VA_ARGS__, VECTOR_Z((v)))

/**
 * @brief Insert the data at @a elmt as the last element in the @a vector
 *
 * If @a elmt is @c NULL then the inserted element will be uninitialized.
 *
 * If the resultant length of the @a vector would overflow a @c size_t then this
 * will set @c errno to @c ENOMEM and return @c NULL. Otherwise this will call
 * vector_ensure_z(). If that fails then the @a vector will be unmodified and
 * the value of @c errno set by realloc() will be retained.
 *
 * If @a elmt isn't @c NULL and its type is incompatible with the element type
 * of the @a vector then the behavior is undefined. If @a elmt is a location in
 * the @a vector itself then the behavior is undefined.
 *
 * @param vector the vector to operate on
 * @param elmt the location of the element to append
 * @param z the element size of the @a vector
 * @return the resultant vector on success; otherwise @c NULL
 *
 * @see vector_append_z() - the explicit interface analogue
 */
inline vector_t vector_append_z(
    restrict vector_t vector,
    const void *restrict elmt,
    size_t z)
  __attribute__((nonnull(1), warn_unused_result));

/**
 * @brief Append @a n elements from @a elmt to the tail of the @a vector
 *
 * If @a elmt is @c NULL then the inserted elements will be uninitialized.
 *
 * If the resultant length of the @a vector would overflow a @c size_t then this
 * will set @c errno to @c ENOMEM and return @c NULL. Otherwise this will call
 * vector_ensure(). If that fails then the @a vector will be unmodified and the
 * value of @c errno set by realloc() will be retained.
 *
 * If @a elmt isn't @c NULL and its type is incompatible with the element type
 * of the @a vector then the behavior is undefined. If @a elmt is a location in
 * the @a vector itself then the behavior is undefined.
 *
 * @param vector the vector to operate on
 * @param elmt the location of the elements to insert
 * @param n the number of elements to insert from @a elmt
 * @return the resultant vector on success; otherwise @c NULL
 *
 * @see vector_extend_z() - the explicit interface analogue
 */
//= inline vector_t
//= vector_extend(restrict vector_t vector, const void *restrict elmt, size_t n)
#define vector_extend(v, ...) vector_extend_z((v), __VA_ARGS__, VECTOR_Z((v)))

/**
 * @brief Append @a n elements from @a elmt to the tail of the @a vector
 *
 * If @a elmt is @c NULL then the inserted elements will be uninitialized.
 *
 * If the resultant length of the @a vector would overflow a @c size_t then this
 * will set @c errno to @c ENOMEM and return @c NULL. Otherwise this will call
 * vector_ensure_z(). If that fails then the @a vector will be unmodified and
 * the value of @c errno set by realloc() will be retained.
 *
 * If @a elmt isn't @c NULL and its type is incompatible with the element type
 * of the @a vector then the behavior is undefined. If @a elmt is a location in
 * the @a vector itself then the behavior is undefined.
 *
 * @param vector the vector to operate on
 * @param elmt the location of the elements to insert
 * @param n the number of elements to insert from @a elmt
 * @param z the element size of the @a vector
 * @return the resultant vector on success; otherwise @c NULL
 *
 * @see vector_extend_z() - the explicit interface analogue
 */
inline vector_t vector_extend_z(
    restrict vector_t vector,
    const void *restrict elmt,
    size_t n,
    size_t z)
  __attribute__((nonnull(1), warn_unused_result));

#ifdef VECTOR_TEST
#undef inline
#endif /* VECTOR_TEST */

#endif /* VECTOR_INSERT_H */

#ifndef VECTOR_TEST
#include "insert.c"
#endif /* VECTOR_TEST */
