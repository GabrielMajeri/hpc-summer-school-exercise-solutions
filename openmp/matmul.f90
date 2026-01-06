module matrix
   implicit none

   private

   public t_matrix, initialize_matrix, multiply_matrices, free_matrix

   type :: t_matrix
      integer :: size
      double precision, dimension(:, :), allocatable :: data
   end type

contains

   function initialize_matrix(size)
      integer, intent(in) :: size
      type(t_matrix) :: initialize_matrix
      type(t_matrix) :: mat

      mat%size = size
      allocate(mat%data(size, size))

      initialize_matrix = mat
   end function

   function multiply_matrices(a, b)
      type(t_matrix), intent(in) :: a, b
      type(t_matrix) :: multiply_matrices
      double precision, dimension(:, :), allocatable :: c
      integer :: n, i, j, k
      double precision :: acc

      n = a%size
      allocate(c(n, n))

      !$omp parallel private(acc) shared(c)
      !$omp do
      do i = 1, n
         do j = 1, n
            acc = 0
            do k = 1, n
               acc = acc + a%data(i, k) * b%data(k, j)
            end do
            c(i, j) = acc
         end do
      end do
      !$omp end parallel

      multiply_matrices%size = n
      multiply_matrices%data = c
   end function multiply_matrices

   subroutine free_matrix(a)
      type(t_matrix), intent(inout) :: a

      deallocate(a%data)
      a%size = 0
   end subroutine free_matrix

end module matrix


program matmul
   use matrix
   use omp_lib
   implicit none

   integer :: size = 512
   type(t_matrix) :: a, b, c
   double precision :: start_time, end_time, duration

   print *, "Initializing matrices with random data..."

   a = initialize_matrix(size)
   b = initialize_matrix(size)

   print *, "Multiplying matrices..."

   start_time = omp_get_wtime()
   c = multiply_matrices(a, b)
   end_time = omp_get_wtime()
   duration = end_time - start_time
   print '("Multiplication took ", F0.4, " seconds")', duration

   call free_matrix(a)
   call free_matrix(b)
   call free_matrix(c)

end program matmul
