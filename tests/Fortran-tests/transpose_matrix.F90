module transpose_matrix

  use bml
  use prec
  use transpose_matrix_single_real
  use transpose_matrix_double_real
#ifdef BML_COMPLEX
  use transpose_matrix_single_complex
  use transpose_matrix_double_complex
#endif

  implicit none

  public :: test_transpose_matrix

contains

  function test_transpose_matrix(matrix_type, element_type, n, m) &
       & result(test_result)

    character(len=*), intent(in) :: matrix_type, element_type
    integer, intent(in) :: n, m
    character(20) :: element_kind
    logical :: test_result
    integer :: element_precision

    write(*,*)"Im in test_transpose_matrix"
    select case(element_type)
    case("single_real")
      element_kind = bml_real
      element_precision = sp
      test_result = test_transpose_matrix_single_real(matrix_type, element_kind,&
           &element_precision, n, m)
    case("double_real")
      element_kind = bml_real
      element_precision = dp
      test_result = test_transpose_matrix_double_real(matrix_type, element_kind,&
           &element_precision, n, m)
#ifdef BML_COMPLEX
    case("single_complex")
      element_kind = bml_complex
      element_precision = sp
      test_result = test_transpose_matrix_single_complex(matrix_type, element_kind,&
           &element_precision, n, m)
    case("double_complex")
      element_kind = bml_complex
      element_precision = dp
      test_result = test_transpose_matrix_double_complex(matrix_type, element_kind,&
           &element_precision, n, m)
#endif
    end select

  end function test_transpose_matrix

end module transpose_matrix
