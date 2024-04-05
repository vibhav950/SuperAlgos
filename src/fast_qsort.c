/**
 * fast_qsort.c - written by vibhav950 on GitHub.
 * 
 * I wanted to learn how glibc's qsort() is so fast :)
 * 
 * References:
 *   [1] "Engineering a Sort Function", JON L. BENTLEY
 *        and M. DOUGLAS McILROY.
 *   [2] https://github.com/lattera/glibc/blob/master/stdlib/qsort.c
*/

#include "fast_qsort.h"

static inline void swapfunc(void *a, void *b, size_t size, int swaptype) __attribute__(( always_inline ));

#define SWAPINIT(arr, size) \
    swaptype = (intptr_t)arr % sizeof(long) || size % sizeof(long) ? 2 : size == sizeof(long) ? 0 : 1;

#define EXCH(TYPE, a, b, size)                       \
    do                                               \
    {                                                \
        size_t __size = (size) / sizeof(TYPE);       \
        TYPE *__a = (TYPE *)(a), *__b = (TYPE *)(b); \
        do                                           \
        {                                            \
            TYPE __tmp = *__a;                       \
            *__a++ = *__b;                           \
            *__b++ = __tmp;                          \
        } while (--__size > 0);                      \
    } while (0)

/**
 * Select the kind of swapping based on the value of `swaptype`:
 * - 0 for single-word swaps
 * - 1 for general swapping by words
 * - 2 for swapping by bytes
 */
static inline void swapfunc(void *a, void *b, size_t size, int swaptype)
{
    if (swaptype == 0)
    {
        long tmp = *((long *)a);
        *((long *)a) = *((long *)b);
        *((long *)b) = tmp;
    }
    else if (swaptype == 1)
    {
        EXCH(long, a, b, size);
    }
    else
    {
        EXCH(char, a, b, size);
    }
}

#define SWAP(a, b) swapfunc((void *)a, (void *)b, (size_t)size, (int)swaptype)

#define min(x, y) ((x) < (y) ? (x) : (y))

#define CUTOFF_THRESH 10

typedef struct _stk_node
{
    char *lo;
    char *hi;
} stk_node;

/**
 * Macros for a fast stack implementation using inline operations.
 * Since on average a partition of half the subarray size is pushed
 * onto the stack on every iteration, the stack should have a depth
 * of `log2(arr_count)`. Since `arr_count` has type `size_t`, we get
 * `BIC * sizeof(size_t)` as the upper bound for `log2(arr_count)`.
 */
#define BIC (1 << 3)            /* Bits in char */
#define BIW (sizeof(long) << 3) /* Bits in word */
#define STACK_SIZE (BIC * sizeof(size_t))
#define PUSH(low, high) ((void)((top->lo = (low)), (top->hi = (high)), ++top))
#define POP(low, high) ((void)(--top, (low = top->lo), (high = top->hi)))
#define STACK_NOT_EMPTY (stack < top)

/**
 * Sort the given array using a quicksort/insertion sort hybrid. The following
 * optimizations are in place:
 *
 * 1. Use an explicit stack instead of using recursion. The larger of the two
 *    partitions is pushed onto the stack and the algorithm continues to run
 *    on the smaller partition. This ensures that the stack depth required is
 *    at most `256 * sizeof(stk_node) = 512 bytes` (or 1024 bytes for 64-bit).
 *    We allocate a stack of this maximum depth, which for larger arrays
 *    should be cheaper than the naive double tail recursion approach.
 *
 * 2. Run quicksort for only `arr_count / CUTOFF_THRESH` partitions and switch
 *    to insertion sort for subarrays of count <= CUTOFF_THRESH. It pays to do
 *    this since insertion sort is faster for small, partially sorted segments.
 *
 * 3. Choose the partitioning item based on median-of-three partitioning, which
 *    will give a slightly better partition at the cost of computing the median.
 *    To get the full effect of this strategy, it is important to also sort the
 *    three items. This implementation uses the first, middle, and last items.
 */
void fast_qsort(void *const arr, size_t arr_count, size_t size, cmp_f_t cmp)
{
    char *pbase = (char *)arr;

    const size_t cutoff_thresh = CUTOFF_THRESH * size;

    int swaptype;

    if (arr_count == 0)
        return;

    SWAPINIT(arr, size);

    if (arr_count > CUTOFF_THRESH)
    {
        char *lo = pbase;
        char *hi = lo + size * (arr_count - 1);
        stk_node stack[STACK_SIZE];
        stk_node *top = stack;

        PUSH(NULL, NULL);

        while (STACK_NOT_EMPTY)
        {
            char *i, *j;

            /* Select the median value among lo, hi, and (lo + hi)/2.
               Rearrange the three items so they are in sorted order,
               which helps avoid pathological quicksort cases. */
            char *p = lo + size * ((hi - lo) / size >> 1);

            if ((*cmp)((void *)p, (void *)lo) < 0)
                SWAP(p, lo);
            if ((*cmp)((void *)hi, (void *)p) < 0)
                SWAP(p, hi);
            else
                goto jmp;
            if ((*cmp)((void *)p, (void *)lo) < 0)
                SWAP(p, lo);

        jmp:
            i = lo + size;
            j = hi - size;

            do
            {
                while ((*cmp)((void *)i, (void *)p) < 0)
                    i += size;

                while ((*cmp)((void *)p, (void *)j) < 0)
                    j -= size;

                if (i < j)
                {
                    SWAP(i, j);
                    if (p == i)
                        p = j;
                    else if (p == j)
                        p = i;
                    i += size;
                    j -= size;
                }
                else if (i == j)
                {
                    i += size;
                    j -= size;
                    break;
                }
            } while (i <= j);

            /* Prepare partitions for next iteration */
            if ((size_t)(j - lo) <= cutoff_thresh)
            {
                if ((size_t)(hi - i) <= cutoff_thresh)
                    /* Both partitions too small */
                    POP(lo, hi);
                else
                    /* Left partition too small */
                    lo = i;
            }
            else if ((size_t)(hi - i) <= cutoff_thresh)
            {
                /* Right partition too small */
                hi = j;
            }
            else if ((j - lo) > (hi - i))
            {
                /* Push larger left partition */
                PUSH(lo, j);
                lo = i;
            }
            else
            {
                /* Push larger right partition */
                PUSH(i, hi);
                hi = j;
            }

            /* Continue quicksort on smaller partition */
        }
    }

    /* Switch to insertion sort for small segments */
    {
        char *const pend = pbase + size * (arr_count - 1);
        char *thresh = min(pend, pbase + cutoff_thresh);
        char *pa = pbase;
        char *pb;

        /* Place the smallest element in the threshold (also the smallest
           array element) at the beginning of the array by swapping. This
           gives insertion sort a head start, speeding up the inner loop.*/
        for (pb = pa + size; pb <= thresh; pb += size)
        {
            if ((*cmp)((void *)pb, (void *)pa) < 0)
                pa = pb;
        }

        if (pa != pbase)
            SWAP(pa, pbase);

        pb = pbase + size;
        while ((pb += size) <= pend)
        {
            pa = pb - size;
            while ((*cmp)((void *)pb, (void *)pa) < 0)
                pa -= size;

            pa += size;
            if (pa != pb)
            /* Insert */
            {
                /* Upper bound for the shift window */
                char *pc = pb + size;

                while (--pc >= pb)
                {
                    char ch = *pc;
                    char *hi, *lo;

                    for (lo = hi = pc; (lo -= size) >= pa; hi = lo)
                    {
                        *hi = *lo;
                    }
                    *hi = ch;
                }
            }
        }
    }
}
