#ifndef __BML_ALLOCATE_ELLSORT_H
#define __BML_ALLOCATE_ELLSORT_H

#include "bml_types_ellsort.h"

void bml_deallocate_ellsort(
    bml_matrix_ellsort_t * A);

void bml_clear_ellsort(
    bml_matrix_ellsort_t * A);

void bml_clear_ellsort_single_real(
    bml_matrix_ellsort_t * A);

void bml_clear_ellsort_double_real(
    bml_matrix_ellsort_t * A);

void bml_clear_ellsort_single_complex(
    bml_matrix_ellsort_t * A);

void bml_clear_ellsort_double_complex(
    bml_matrix_ellsort_t * A);

bml_matrix_ellsort_t *bml_noinit_matrix_ellsort(
    const bml_matrix_precision_t matrix_precision,
    const int N,
    const int M,
    const bml_distribution_mode_t distrib_mode);

bml_matrix_ellsort_t *bml_noinit_matrix_ellsort_single_real(
    const int N,
    const int M,
    const bml_distribution_mode_t distrib_mode);

bml_matrix_ellsort_t *bml_noinit_matrix_ellsort_double_real(
    const int N,
    const int M,
    const bml_distribution_mode_t distrib_mode);

bml_matrix_ellsort_t *bml_noinit_matrix_ellsort_single_complex(
    const int N,
    const int M,
    const bml_distribution_mode_t distrib_mode);

bml_matrix_ellsort_t *bml_noinit_matrix_ellsort_double_complex(
    const int N,
    const int M,
    const bml_distribution_mode_t distrib_mode);

bml_matrix_ellsort_t *bml_zero_matrix_ellsort(
    const bml_matrix_precision_t matrix_precision,
    const int N,
    const int M,
    const bml_distribution_mode_t distrib_mode);

bml_matrix_ellsort_t *bml_zero_matrix_ellsort_single_real(
    const int N,
    const int M,
    const bml_distribution_mode_t distrib_mode);

bml_matrix_ellsort_t *bml_zero_matrix_ellsort_double_real(
    const int N,
    const int M,
    const bml_distribution_mode_t distrib_mode);

bml_matrix_ellsort_t *bml_zero_matrix_ellsort_single_complex(
    const int N,
    const int M,
    const bml_distribution_mode_t distrib_mode);

bml_matrix_ellsort_t *bml_zero_matrix_ellsort_double_complex(
    const int N,
    const int M,
    const bml_distribution_mode_t distrib_mode);

bml_matrix_ellsort_t *bml_banded_matrix_ellsort(
    const bml_matrix_precision_t matrix_precision,
    const int N,
    const int M,
    const bml_distribution_mode_t distrib_mode);

bml_matrix_ellsort_t *bml_banded_matrix_ellsort_single_real(
    const int N,
    const int M,
    const bml_distribution_mode_t distrib_mode);

bml_matrix_ellsort_t *bml_banded_matrix_ellsort_double_real(
    const int N,
    const int M,
    const bml_distribution_mode_t distrib_mode);

bml_matrix_ellsort_t *bml_banded_matrix_ellsort_single_complex(
    const int N,
    const int M,
    const bml_distribution_mode_t distrib_mode);

bml_matrix_ellsort_t *bml_banded_matrix_ellsort_double_complex(
    const int N,
    const int M,
    const bml_distribution_mode_t distrib_mode);

bml_matrix_ellsort_t *bml_random_matrix_ellsort(
    const bml_matrix_precision_t matrix_precision,
    const int N,
    const int M,
    const bml_distribution_mode_t distrib_mode);

bml_matrix_ellsort_t *bml_random_matrix_ellsort_single_real(
    const int N,
    const int M,
    const bml_distribution_mode_t distrib_mode);

bml_matrix_ellsort_t *bml_random_matrix_ellsort_double_real(
    const int N,
    const int M,
    const bml_distribution_mode_t distrib_mode);

bml_matrix_ellsort_t *bml_random_matrix_ellsort_single_complex(
    const int N,
    const int M,
    const bml_distribution_mode_t distrib_mode);

bml_matrix_ellsort_t *bml_random_matrix_ellsort_double_complex(
    const int N,
    const int M,
    const bml_distribution_mode_t distrib_mode);

bml_matrix_ellsort_t *bml_identity_matrix_ellsort(
    const bml_matrix_precision_t matrix_precision,
    const int N,
    const int M,
    const bml_distribution_mode_t distrib_mode);

bml_matrix_ellsort_t *bml_identity_matrix_ellsort_single_real(
    const int N,
    const int M,
    const bml_distribution_mode_t distrib_mode);

bml_matrix_ellsort_t *bml_identity_matrix_ellsort_double_real(
    const int N,
    const int M,
    const bml_distribution_mode_t distrib_mode);

bml_matrix_ellsort_t *bml_identity_matrix_ellsort_single_complex(
    const int N,
    const int M,
    const bml_distribution_mode_t distrib_mode);

bml_matrix_ellsort_t *bml_identity_matrix_ellsort_double_complex(
    const int N,
    const int M,
    const bml_distribution_mode_t distrib_mode);

void bml_update_domain_ellsort(
    bml_matrix_ellsort_t * A,
    int *localPartMin,
    int *localPartMax,
    int *nnodesInPart);

#endif
