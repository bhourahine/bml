#include "../bml_logger.h"
#include "../bml_normalize.h"
#include "../bml_types.h"
#include "bml_normalize_ellsort.h"
#include "bml_types_ellsort.h"

#include <stdlib.h>
#include <string.h>

/** Normalize ellsort matrix given gershgorin bounds.
 *
 *  \ingroup normalize_group
 *
 *  \param A The matrix
 *  \param mineval Calculated min value
 *  \param maxeval Calculated max value
 */
void
bml_normalize_ellsort(
    bml_matrix_ellsort_t * A,
    double mineval,
    double maxeval)
{
    switch (A->matrix_precision)
    {
        case single_real:
            bml_normalize_ellsort_single_real(A, mineval, maxeval);
            break;
        case double_real:
            bml_normalize_ellsort_double_real(A, mineval, maxeval);
            break;
        case single_complex:
            bml_normalize_ellsort_single_complex(A, mineval, maxeval);
            break;
        case double_complex:
            bml_normalize_ellsort_double_complex(A, mineval, maxeval);
            break;
        default:
            LOG_ERROR("unknown precision\n");
            break;
    }
}

/** Calculate gershgorin bounds for an ellsort matrix.
 *
 *  \ingroup normalize_group
 *
 *  \param A The matrix
 *  returns mineval Calculated min value
 *  returns maxeval Calculated max value
 */
void *
bml_gershgorin_ellsort(
    bml_matrix_ellsort_t * A)
{
    switch (A->matrix_precision)
    {
        case single_real:
            return bml_gershgorin_ellsort_single_real(A);
            break;
        case double_real:
            return bml_gershgorin_ellsort_double_real(A);
            break;
        case single_complex:
            return bml_gershgorin_ellsort_single_complex(A);
            break;
        case double_complex:
            return bml_gershgorin_ellsort_double_complex(A);
            break;
        default:
            LOG_ERROR("unknown precision\n");
            break;
    }
    return NULL;
}

/** Calculate gershgorin bounds for a partial ellsort matrix.
 *
 *  \ingroup normalize_group
 *
 *  \param A The matrix
 *  \param nrows Number of rows to use
 *  returns mineval Calculated min value
 *  returns maxeval Calculated max value
 */
void *
bml_gershgorin_partial_ellsort(
    bml_matrix_ellsort_t * A,
    int nrows)
{
    switch (A->matrix_precision)
    {
        case single_real:
            return bml_gershgorin_partial_ellsort_single_real(A, nrows);
            break;
        case double_real:
            return bml_gershgorin_partial_ellsort_double_real(A, nrows);
            break;
        case single_complex:
            return bml_gershgorin_partial_ellsort_single_complex(A, nrows);
            break;
        case double_complex:
            return bml_gershgorin_partial_ellsort_double_complex(A, nrows);
            break;
        default:
            LOG_ERROR("unknown precision\n");
            break;
    }
    return NULL;
}
