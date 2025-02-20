/**
 * @file vfast_vec.h
 * 
 * @brief Fast vector implementation using only macros
 * 
 * This header file implements C++ style vectors using
 * macros only. See vfast_vec.md for the usage guide.
 * 
 * Part of the vfast_* files in the SuperAlgos GitHub
 * repository (https://github.com/vibhav950/SuperAlgos).
 * 
 * @author vibhav950 on GitHub
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* Defailt size to initialize a vector. */
#define VEC_DEFAULT_CAPACITY (1024 / sizeof(long long))

#ifdef MIN
  #undef MIN
  #define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifdef MAX
  #undef MAX
  #define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

/* The vector type. */
#define vec(TYPE)    \
  struct {           \
    TYPE *p;         \
    size_t length;   \
    size_t capacity; \
  }

/* Return the capacity of the vector as a size_t. */
#define vec_capacity(NAME) ((size_t) (NAME).capacity)

/* Return the number of elements in the vector as a size_t. */
#define vec_length(NAME)   ((size_t) (NAME).length)

/* Return 1 if the vector is empty, 0 otherwise. */
#define vec_empty(NAME)    ((NAME).length == 0)

/* Return 1 if the vector is full, 1 otherwise. */
#define vec_full(NAME)     ((NAME).length == (NAME).capacity)

/* Return the element type of the vector. */
#define vec_type(NAME)     typeof((NAME).p)

/* Initialize the vector.
  
  Note: every vector MUST be initialized before use. */
#define vec_init(TYPE, NAME, CAPACITY)                    \
  do {                                                    \
    (NAME).length = 0;                                    \
    size_t sz_new =                                       \
      (CAPACITY) > 0 ? (CAPACITY) : VEC_DEFAULT_CAPACITY; \
    (NAME).capacity = sz_new;                             \
    TYPE *p =                                             \
        (TYPE *)malloc(sz_new * sizeof(TYPE));            \
    assert(p != NULL);                                    \
    (NAME).p = p;                                         \
  } while (0)

/* Unallocate the vector.
  
  Note: no operation can be performed on a 
  vector after it has been uninitialized. */
#define vec_free(NAME)                   \
  do {                                   \
    free((NAME).p);                      \
    (NAME).capacity = (NAME).length = 0; \
    (NAME).p = NULL;                     \
  } while (0)

/* Resize a vector up or down (minimum capacity of 1). */
#define vec_resize(NAME, CAPACITY)                          \
  do {                                                      \
    if ((NAME).capacity != (CAPACITY) && (CAPACITY) > 0) {  \
      typeof((NAME).p) p =                                  \
        (typeof((NAME).p))realloc((NAME).p,                 \
        (CAPACITY) * sizeof((NAME).p[0]));                  \
      assert(p != NULL);                                    \
      (NAME).capacity = CAPACITY;                           \
      (NAME).length = MIN(CAPACITY, (NAME).length);         \
      (NAME).p = p;                                         \
    }                                                       \
  } while (0)

/* Shrink the vector capacity to fit its length. */
#define vec_shrink(NAME)                      \
  do {                                        \
    if ((NAME).capacity > (NAME).length) {    \
      typeof((NAME).p) p =                    \
        (typeof((NAME).p))realloc((NAME).p,   \
        (NAME).length * sizeof((NAME).p[0])); \
      assert(p != NULL);                      \
      (NAME).p = p;                           \
      (NAME).capacity = (NAME).length;        \
    }                                         \
  } while (0)

/* Push a value to the end of the vector. */
#define vec_push(NAME, VAL)                       \
  do {                                            \
    if (vec_full(NAME)) {                         \
      size_t new_cap =                            \
        (NAME).capacity + ((NAME).capacity/2) + 1;\
      typeof((NAME).p) p =                        \
        (typeof((NAME).p))realloc((NAME).p,       \
        new_cap * sizeof((NAME).p[0]));           \
      assert(p != NULL);                          \
      (NAME).capacity = new_cap;                  \
      (NAME).p = p;                               \
    }                                             \
      (NAME).p[(NAME).length++] = VAL;            \
  } while (0)

/* Insert an element at the specified position
   in the vector */
#define vec_insert(NAME, VAL, POS)                \
  do {                                            \
    if (vec_full(NAME)) {                         \
      typeof((NAME).p) p =                        \
        (typeof((NAME).p))realloc((NAME).         \
        ++(NAME).capacity * sizeof((NAME).p[0])); \
      assert(p != NULL);                          \
      (NAME).p = p;                               \
    }                                                      \
    int i;                                                 \
    for (i = (NAME).length - 1; i >= 0 && i >= (POS); --i) \
      (NAME).p[i + 1] = (NAME).p[i];                       \
    (NAME).p[i + 1] = VAL;                                 \
    (NAME).length++;                                       \
  } while (0)

/* Remove the element at the end of the vector
   (does not change capacity). */
#define vec_pop(NAME)     \
  do {                    \
    if (vec_length(NAME)) \
      (NAME).length--;    \
  } while (0)

/* Delete the element at the end of the vector
   (reduces capacity by 1 at most). */
#define vec_delete_last(NAME)                \
  do {                                       \
    if (vec_length(NAME))                    \
      (NAME).length--;                       \
    vec_resize(NAME, (NAME).capacity - 1);   \
  } while (0)

/* Remove the element from the specified position
   in the vector (does not change capacity). */
#define vec_remove(NAME, POS)                       \
  do {                                              \
    if (vec_length(NAME) > (POS)) {                 \
      size_t pos = MAX(POS, 0);                     \
      for (int i = pos + 1; i < (NAME).length; ++i) \
        (NAME).p[i - 1] = (NAME).p[i];              \
      (NAME).length--;                              \
    }                                               \
  } while (0)

/* Delete the element from the specified position
   in the vector (reduces capacity by 1 at most). */
#define vec_delete(NAME, POS)                       \
  do {                                              \
    if (vec_length(NAME) > (POS)) {                 \
      size_t pos = MAX(POS, 0);                     \
      for (int i = pos + 1; i < (NAME).length; ++i) \
        (NAME).p[i - 1] = (NAME).p[i];              \
      (NAME).length--;                              \
      vec_resize(NAME, (NAME).capacity - 1);        \
    }                                               \
  } while (0)

/* Replaces the element at the specified position
   in the vector with the specified value. */
#define vec_replace(NAME, VAL, POS)        \
do {                                       \
  if ((POS) >= 0 && (POS) < (NAME).length) \
    (NAME).p[(POS)] = VAL;                 \
} while (0);

/* Copies the elements of one vector to the second.

   Note: the capacity of the dest vector will be
   just enough to fit the elements of the source vector. 
   
   Note: if the source vector is empty, the dest vector
   remains unchanged. */
#define vec_copy(DST, SRC)                          \
  do {                                              \
    if ((SRC).length) {                             \
      typeof((SRC).p) p =                           \
        (typeof((SRC).p))calloc((SRC).length,       \
        sizeof((SRC).p[0]));                        \
      assert(p != NULL);                            \
      if ((DST).capacity)                           \
        free((DST).p);                              \
      memcpy(p, (SRC).p,                            \
        (SRC).length * sizeof((SRC).p[0]));         \
      (DST).p = p;                                  \
      (DST).length = (DST).capacity = (SRC).length; \
    }                                               \
  } while (0)
