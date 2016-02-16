#include "../macros.h"
#include "../typed.h"
#include "bml_add.h"
#include "bml_add_ellpack.h"
#include "bml_allocate.h"
#include "bml_allocate_ellpack.h"
#include "bml_logger.h"
#include "bml_multiply.h"
#include "bml_multiply_ellpack.h"
#include "bml_types.h"
#include "bml_types_ellpack.h"

#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _OPENMP
#include <omp.h>
#endif

/** Matrix multiply.
 *
 * \f$ C \leftarrow \alpha A \, B + \beta C \f$
 *
 * \ingroup multiply_group
 *
 * \param A Matrix A
 * \param B Matrix B
 * \param C Matrix C
 * \param alpha Scalar factor multiplied by A * B
 * \param beta Scalar factor multiplied by C
 * \param threshold Used for sparse multiply
 */
void TYPED_FUNC(
    bml_multiply_ellpack) (
    const bml_matrix_ellpack_t * A,
    const bml_matrix_ellpack_t * B,
    bml_matrix_ellpack_t * C,
    const double alpha,
    const double beta,
    const double threshold)
{
    const double ONE = 1.0;
    const double ZERO = 0.0;

    if (A != NULL && A == B && alpha == ONE && beta == ZERO)
    {
        TYPED_FUNC(bml_multiply_x2_ellpack) (A, C, threshold);
    }
    else
    {
        bml_matrix_ellpack_t *A2 =
            TYPED_FUNC(bml_zero_matrix_ellpack) (C->N, C->M);

        if (A != NULL && A == B)
        {
            TYPED_FUNC(bml_multiply_x2_ellpack) (A, A2, threshold);
        }
        else
        {
            TYPED_FUNC(bml_multiply_AB_ellpack) (A, B, A2, threshold);
        }

        TYPED_FUNC(bml_add_ellpack) (C, A2, beta, alpha, threshold);

        bml_deallocate_ellpack(A2);
    }
}

/** Matrix multiply.
 *
 * \f$ X^{2} \leftarrow X \, X \f$
 *
 * \ingroup multiply_group
 *
 * \param X Matrix X
 * \param X2 Matrix X2
 * \param threshold Used for sparse multiply
 */
void* TYPED_FUNC(
    bml_multiply_x2_ellpack) (
    const bml_matrix_ellpack_t * X,
    bml_matrix_ellpack_t * X2,
    const double threshold)
{
    int X_N = X->N;
    int X_M = X->M;
    int *X_index = X->index;
    int *X_nnz = X->nnz;

    int X2_N = X2->N;
    int X2_M = X2->M;
    int *X2_index = X2->index;
    int *X2_nnz = X2->nnz;

    int ix[X_N], jx[X_N];
    REAL_T x[X_N];

    REAL_T traceX = 0.0;
    REAL_T traceX2 = 0.0;
    REAL_T *X_value = (REAL_T *) X->value;
    REAL_T *X2_value = (REAL_T *) X2->value;

    double *trace = bml_allocate_memory(sizeof(double) * 2);

    memset(ix, 0, X_N * sizeof(int));
    memset(jx, 0, X_N * sizeof(int));
    memset(x, 0.0, X_N * sizeof(REAL_T));

#pragma omp parallel for \
    default(none) \
    firstprivate(ix, jx, x) \
    shared(X_N, X_M, X_index, X_nnz, X_value) \
    shared(X2_N, X2_M, X2_index, X2_nnz, X2_value) \
    reduction(+: traceX, traceX2)
    for (int i = 0; i < X_N; i++)       // CALCULATES THRESHOLDED X^2
    {
        int l = 0;
        for (int jp = 0; jp < X_nnz[i]; jp++)
        {
            REAL_T a = X_value[ROWMAJOR(i, jp, X_N, X_M)];
            int j = X_index[ROWMAJOR(i, jp, X_N, X_M)];
            if (j == i)
            {
                traceX = traceX + a;
            }
            for (int kp = 0; kp < X_nnz[j]; kp++)
            {
                int k = X_index[ROWMAJOR(j, kp, X_N, X_M)];
                if (ix[k] == 0)
                {
                    x[k] = 0.0;
                    //X2_index[ROWMAJOR(i, l, N, M)] = k;
                    jx[l] = k;
                    ix[k] = i + 1;
                    l++;
                }
                // TEMPORARY STORAGE VECTOR LENGTH FULL N
                x[k] = x[k] + a * X_value[ROWMAJOR(j, kp, X_N, X_M)];
            }
        }

        // Check for number of non-zeroes per row exceeded
        if (l > X2_M)
        {
            LOG_ERROR("Number of non-zeroes per row > M, Increase M\n");
        }

        int ll = 0;
        for (int j = 0; j < l; j++)
        {
            //int jp = X2_index[ROWMAJOR(i, j, N, M)];
            int jp = jx[j];
            REAL_T xtmp = x[jp];
            // The diagonal elements are stored in the first column
            if (jp == i)
            {
                traceX2 = traceX2 + xtmp;
                X2_value[ROWMAJOR(i, ll, X2_N, X2_M)] = xtmp;
                X2_index[ROWMAJOR(i, ll, X2_N, X2_M)] = jp;
                ll++;
            }
            else if (is_above_threshold(xtmp, threshold))
            {
                X2_value[ROWMAJOR(i, ll, X2_N, X2_M)] = xtmp;
                X2_index[ROWMAJOR(i, ll, X2_N, X2_M)] = jp;
                ll++;
            }
            ix[jp] = 0;
            x[jp] = 0.0;
        }
        X2_nnz[i] = ll;
    }

    trace[0] = traceX;
    trace[1] = traceX2;

    return trace; 
}

/** Matrix multiply.
 *
 * \f$ C \leftarrow B \, A \f$
 *
 * \ingroup multiply_group
 *
 * \param A Matrix A
 * \param B Matrix B
 * \param C Matrix C
 * \param threshold Used for sparse multiply
 */
void TYPED_FUNC(
    bml_multiply_AB_ellpack) (
    const bml_matrix_ellpack_t * A,
    const bml_matrix_ellpack_t * B,
    bml_matrix_ellpack_t * C,
    const double threshold)
{
    int A_N = A->N;
    int A_M = A->M;
    int *A_nnz = A->nnz;
    int *A_index = A->index;

    int B_N = B->N;
    int B_M = B->M;
    int *B_nnz = B->nnz;
    int *B_index = B->index;

    int C_N = C->N;
    int C_M = C->M;
    int *C_nnz = C->nnz;
    int *C_index = C->index;

    int ix[C->N], jx[C->N];
    REAL_T x[C->N];

    REAL_T *A_value = (REAL_T *) A->value;
    REAL_T *B_value = (REAL_T *) B->value;
    REAL_T *C_value = (REAL_T *) C->value;

    memset(ix, 0, C->N * sizeof(int));
    memset(jx, 0, C->N * sizeof(int));
    memset(x, 0.0, C->N * sizeof(REAL_T));

#pragma omp parallel for \
    default(none) \
    firstprivate(ix, jx, x) \
    shared(A_N, A_M, A_nnz, A_index, A_value) \
    shared(B_N, B_M, B_nnz, B_index, B_value) \
    shared(C_N, C_M, C_nnz, C_index, C_value)
    for (int i = 0; i < A_N; i++)
    {
        int l = 0;
        for (int jp = 0; jp < A_nnz[i]; jp++)
        {
            REAL_T a = A_value[ROWMAJOR(i, jp, A_N, A_M)];
            int j = A_index[ROWMAJOR(i, jp, A_N, A_M)];

            for (int kp = 0; kp < B_nnz[j]; kp++)
            {
                int k = B_index[ROWMAJOR(j, kp, B_N, B_M)];
                if (ix[k] == 0)
                {
                    x[k] = 0.0;
                    //C_index[ROWMAJOR(i, l, N, M)] = k;
                    jx[l] = k;
                    ix[k] = i + 1;
                    l++;
                }
                // TEMPORARY STORAGE VECTOR LENGTH FULL N
                x[k] = x[k] + a * B_value[ROWMAJOR(j, kp, B_N, B_M)];
            }
        }

        // Check for number of non-zeroes per row exceeded
        if (l > C_M)
        {
            LOG_ERROR("Number of non-zeroes per row > M, Increase M\n");
        }

        int ll = 0;
        for (int j = 0; j < l; j++)
        {
            //int jp = C_index[ROWMAJOR(i, j, N, M)];
            int jp = jx[j];
            REAL_T xtmp = x[jp];
            // Diagonal elements are saved in first column
            if (jp == i)
            {
                C_value[ROWMAJOR(i, ll, C_N, C_M)] = xtmp;
                C_index[ROWMAJOR(i, ll, C_N, C_M)] = jp;
                ll++;
            }
            else if (is_above_threshold(xtmp, threshold))
            {
                C_value[ROWMAJOR(i, ll, C_N, C_M)] = xtmp;
                C_index[ROWMAJOR(i, ll, C_N, C_M)] = jp;
                ll++;
            }
            ix[jp] = 0;
            x[jp] = 0.0;
        }
        C_nnz[i] = ll;
    }
}

/** Matrix multiply with threshold adjustment.
 *
 * \f$ C \leftarrow B \, A \f$
 *
 * \ingroup multiply_group
 *
 * \param A Matrix A
 * \param B Matrix B
 * \param C Matrix C
 * \param threshold Used for sparse multiply
 */
void TYPED_FUNC(
    bml_multiply_adjust_AB_ellpack) (
    const bml_matrix_ellpack_t * A,
    const bml_matrix_ellpack_t * B,
    bml_matrix_ellpack_t * C,
    const double threshold)
{
    int A_N = A->N;
    int A_M = A->M;
    int *A_nnz = A->nnz;
    int *A_index = A->index;

    int B_N = B->N;
    int B_M = B->M;
    int *B_nnz = B->nnz;
    int *B_index = B->index;

    int C_N = C->N;
    int C_M = C->M;
    int *C_nnz = C->nnz;
    int *C_index = C->index;

    int ix[C->N], jx[C->N];
    int aflag = 1;
    REAL_T x[C->N];

    REAL_T *A_value = (REAL_T *) A->value;
    REAL_T *B_value = (REAL_T *) B->value;
    REAL_T *C_value = (REAL_T *) C->value;

    REAL_T adjust_threshold = (REAL_T) threshold;

    memset(ix, 0, C->N * sizeof(int));
    memset(jx, 0, C->N * sizeof(int));
    memset(x, 0.0, C->N * sizeof(REAL_T));

    while (aflag > 0)
    {
        aflag = 0;

#pragma omp parallel for \
    default(none) \
    firstprivate(ix, jx, x) \
    shared(A_N, A_M, A_nnz, A_index, A_value) \
    shared(B_N, B_M, B_nnz, B_index, B_value) \
    shared(C_N, C_M, C_nnz, C_index, C_value) \
    shared(adjust_threshold) \
    reduction(+:aflag)
        for (int i = 0; i < A_N; i++)
        {
            int l = 0;
            for (int jp = 0; jp < A_nnz[i]; jp++)
            {
                REAL_T a = A_value[ROWMAJOR(i, jp, A_N, A_M)];
                int j = A_index[ROWMAJOR(i, jp, A_N, A_M)];

                for (int kp = 0; kp < B_nnz[j]; kp++)
                {
                    int k = B_index[ROWMAJOR(j, kp, B_N, B_M)];
                    if (ix[k] == 0)
                    {
                        x[k] = 0.0;
                        jx[l] = k;
                        ix[k] = i + 1;
                        l++;
                    }
                    // TEMPORARY STORAGE VECTOR LENGTH FULL N
                    x[k] = x[k] + a * B_value[ROWMAJOR(j, kp, B_N, B_M)];
                }
            }

            // Check for number of non-zeroes per row exceeded
            // Need to adjust threshold
            if (l > C_M)
            {
                aflag = 1;
            }

            int ll = 0;
            for (int j = 0; j < l; j++)
            {
                //int jp = C_index[ROWMAJOR(i, j, N, M)];
                int jp = jx[j];
                REAL_T xtmp = x[jp];
                // Diagonal elements are saved in first column
                if (jp == i)
                {
                    C_value[ROWMAJOR(i, ll, C_N, C_M)] = xtmp;
                    C_index[ROWMAJOR(i, ll, C_N, C_M)] = jp;
                    ll++;
                }
                else if (is_above_threshold(xtmp, adjust_threshold))
                {
                    C_value[ROWMAJOR(i, ll, C_N, C_M)] = xtmp;
                    C_index[ROWMAJOR(i, ll, C_N, C_M)] = jp;
                    ll++;
                }
                ix[jp] = 0;
                x[jp] = 0.0;
            }
            C_nnz[i] = ll;
        }

        adjust_threshold *= (REAL_T) 2.0;
    }
}