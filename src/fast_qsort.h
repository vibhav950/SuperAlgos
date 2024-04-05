#pragma once

#include <stddef.h>

typedef int cmp_f_t(const void *, const void *);

void fast_qsort(void *const arr, size_t arr_count, size_t size, cmp_f_t cmp);
