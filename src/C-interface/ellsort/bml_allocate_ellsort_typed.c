#include "../macros.h"
#include "../typed.h"
#include "bml_allocate.h"
#include "bml_allocate_ellsort.h"
#include "bml_types.h"
#include "bml_types_ellsort.h"

#include <complex.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

/** Clear a matrix.
 *
 * Numbers of non-zeroes, indeces, and values are set to zero.
 *
 * \ingroup allocate_group
 *
 * \param A The matrix.
 */
void TYPED_FUNC(
    bml_clear_ellsort) (
    bml_matrix_ellsort_t * A)
{
    memset(A->nnz, 0, A->N * sizeof(int));
    memset(A->index, 0, A->N * A->M * sizeof(int));
    memset(A->value, 0.0, A->N * A->M * sizeof(REAL_T));
}

/** Allocate a matrix with uninitialized values.
 *
 *  Note that the matrix \f$ a \f$ will be newly allocated. If it is
 *  already allocated then the matrix will be deallocated in the
 *  process.
 *
 *  \ingroup allocate_group
 *
 *  \param matrix_precision The precision of the matrix. The default
 *  is double precision.
 *  \param N The matrix size.
 *  \param M The number of non-zeroes per row.
 *  \param distrib_mode The distribution mode.
 *  \return The matrix.
 */
bml_matrix_ellsort_t *TYPED_FUNC(
    bml_noinit_matrix_ellsort) (
    const int N,
    const int M,
    const bml_distribution_mode_t distrib_mode)
{
    bml_matrix_ellsort_t *A =
        bml_noinit_allocate_memory(sizeof(bml_matrix_ellsort_t));
    A->matrix_type = ellsort;
    A->matrix_precision = MATRIX_PRECISION;
    A->N = N;
    A->M = M;
    A->distribution_mode = distrib_mode;
    A->index = bml_noinit_allocate_memory(sizeof(int) * N * M);
    A->nnz = bml_allocate_memory(sizeof(int) * N);
    A->value = bml_noinit_allocate_memory(sizeof(REAL_T) * N * M);
    A->domain = bml_default_domain(N, M, distrib_mode);
    A->domain2 = bml_default_domain(N, M, distrib_mode);

    return A;
}

/** Allocate the zero matrix.
 *
 *  Note that the matrix \f$ a \f$ will be newly allocated. If it is
 *  already allocated then the matrix will be deallocated in the
 *  process.
 *
 *  \ingroup allocate_group
 *
 *  \param matrix_precision The precision of the matrix. The default
 *  is double precision.
 *  \param N The matrix size.
 *  \param M The number of non-zeroes per row.
 *  \param distrib_mode The distribution mode.
 *  \return The matrix.
 */
bml_matrix_ellsort_t *TYPED_FUNC(
    bml_zero_matrix_ellsort) (
    const int N,
    const int M,
    const bml_distribution_mode_t distrib_mode)
{
    bml_matrix_ellsort_t *A =
        bml_allocate_memory(sizeof(bml_matrix_ellsort_t));
    A->matrix_type = ellsort;
    A->matrix_precision = MATRIX_PRECISION;
    A->N = N;
    A->M = M;
    A->distribution_mode = distrib_mode;
    A->index = bml_allocate_memory(sizeof(int) * N * M);
    A->nnz = bml_allocate_memory(sizeof(int) * N);
    A->value = bml_allocate_memory(sizeof(REAL_T) * N * M);
    A->domain = bml_default_domain(N, M, distrib_mode);
    A->domain2 = bml_default_domain(N, M, distrib_mode);

    return A;
}

/** Allocate a banded random matrix.
 *
 *  Note that the matrix \f$ a \f$ will be newly allocated. If it is
 *  already allocated then the matrix will be deallocated in the
 *  process.
 *
 *  \ingroup allocate_group
 *
 *  \param matrix_precision The precision of the matrix. The default
 *  is double precision.
 *  \param N The matrix size.
 *  \param M The number of non-zeroes per row.
 *  \param distrib_mode The distribution mode.
 *  \return The matrix.
 */
bml_matrix_ellsort_t *TYPED_FUNC(
    bml_banded_matrix_ellsort) (
    const int N,
    const int M,
    const bml_distribution_mode_t distrib_mode)
{
    bml_matrix_ellsort_t *A =
        TYPED_FUNC(bml_zero_matrix_ellsort) (N, M, distrib_mode);

    REAL_T *A_value = A->value;
    int *A_index = A->index;
    int *A_nnz = A->nnz;

#pragma omp parallel for default(none) shared(A_value, A_index, A_nnz)
    for (int i = 0; i < N; i++)
    {
        int jind = 0;
        for (int j = (i - M / 2 >= 0 ? i - M / 2 : 0);
             j < (i - M / 2 + M <= N ? i - M / 2 + M : N); j++)
        {
            A_value[ROWMAJOR(i, jind, N, M)] = rand() / (REAL_T) RAND_MAX;
            A_index[ROWMAJOR(i, jind, N, M)] = j;
            jind++;
        }
        A_nnz[i] = jind;
    }
    A->domain = bml_default_domain(N, M, distrib_mode);
    A->domain2 = bml_default_domain(N, M, distrib_mode);
    return A;
}

/** Allocate a random matrix.
 *
 *  Note that the matrix \f$ a \f$ will be newly allocated. If it is
 *  already allocated then the matrix will be deallocated in the
 *  process.
 *
 *  \ingroup allocate_group
 *
 *  \param matrix_precision The precision of the matrix. The default
 *  is double precision.
 *  \param N The matrix size.
 *  \param M The number of non-zeroes per row.
 *  \param distrib_mode The distribution mode.
 *  \return The matrix.
 */
bml_matrix_ellsort_t *TYPED_FUNC(
    bml_random_matrix_ellsort) (
    const int N,
    const int M,
    const bml_distribution_mode_t distrib_mode)
{
    bml_matrix_ellsort_t *A =
        TYPED_FUNC(bml_zero_matrix_ellsort) (N, M, distrib_mode);

    REAL_T *A_value = A->value;
    int *A_index = A->index;
    int *A_nnz = A->nnz;

#pragma omp parallel for default(none) shared(A_value, A_index, A_nnz)
    for (int i = 0; i < N; i++)
    {
        int jind = 0;
        for (int j = 0; j < M; j++)
        {
            A_value[ROWMAJOR(i, jind, N, M)] = rand() / (REAL_T) RAND_MAX;
            A_index[ROWMAJOR(i, jind, N, M)] = j;
            jind++;
        }
        A_nnz[i] = jind;
    }
    A->domain = bml_default_domain(N, M, distrib_mode);
    A->domain2 = bml_default_domain(N, M, distrib_mode);
    return A;
}

/** Allocate the identity matrix.
 *
 *  Note that the matrix \f$ a \f$ will be newly allocated. If it is
 *  already allocated then the matrix will be deallocated in the
 *  process.
 *
 *  \ingroup allocate_group
 *
 *  \param matrix_precision The precision of the matrix. The default
 *  is double precision.
 *  \param N The matrix size.
 *  \param M The number of non-zeroes per row.
 *  \param distrib_mode The distribution mode.
 *  \return The matrix.
 */
bml_matrix_ellsort_t *TYPED_FUNC(
    bml_identity_matrix_ellsort) (
    const int N,
    const int M,
    const bml_distribution_mode_t distrib_mode)
{
    bml_matrix_ellsort_t *A =
        TYPED_FUNC(bml_zero_matrix_ellsort) (N, M, distrib_mode);

    REAL_T *A_value = A->value;
    int *A_index = A->index;
    int *A_nnz = A->nnz;

#pragma omp parallel for default(none) shared(A_value, A_index, A_nnz)
    for (int i = 0; i < N; i++)
    {
        A_value[ROWMAJOR(i, 0, N, M)] = (REAL_T) 1.0;
        A_index[ROWMAJOR(i, 0, N, M)] = i;
        A_nnz[i] = 1;
    }
    A->domain = bml_default_domain(N, M, distrib_mode);
    A->domain2 = bml_default_domain(N, M, distrib_mode);
    return A;
}
