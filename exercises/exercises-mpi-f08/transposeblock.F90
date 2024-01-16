! -*- f90 -*-
!****************************************************************
!****
!**** This program file is part of the book 
!**** `Parallel programming for Science and Engineering'
!**** by Victor Eijkhout, eijkhout@tacc.utexas.edu
!****
!**** copyright Victor Eijkhout 2012-2023
!****
!**** MPI Exercise for transposing an array
!**** fortran 2008 version
!****
!****************************************************************/

Program TransposeBlock

  use mpi_f08
  implicit none

  integer :: nprocs, procno
  type(MPI_Comm) :: comm = MPI_COMM_WORLD

#define BLOCKSIZE 4

  !! data specifically for this program 
  integer :: blocksize_i,blocksize_j, isize,jsize, row,col
  integer,dimension(:),allocatable :: data_0,data_1

  call MPI_Init()

  call MPI_Comm_size(comm,nprocs)
  call MPI_Comm_rank(comm,procno)

  blocksize_i = BLOCKSIZE; blocksize_j = BLOCKSIZE;
  isize = nprocs * blocksize_i; jsize = nprocs * blocksize_j;

  allocate( data_0( blocksize_j*isize ) )
  allocate( data_1( blocksize_i*jsize ) )
  do col=0,blocksize_j-1
     do row=0,isize-1
        data_0( col*isize + row +1 ) =         & ! +1 for c-to-f indexing
	procno*isize*blocksize_j + col*isize + & ! get to my column
	row                                      ! get to my row
     end do
  end do

  ! printf("Input data:\n");
  ! for (int row=0; row<isize; row++) {
  !   printf("Row %3d:",row);
  !   for (int col=0; col<blocksize_j; col++)
  !     printf(" %3d",data_0[ row + col*isize ]);
  !   printf("\n");
  ! }

  /*
   * What are we sending to proc p? Block:
   * - starting at           : i=p*blocksize_i
   * - depth of block        : blocksize_i
   * - stride between blocks : isize
   * - how many blocks       : blocksize_j
   */
  MPI_Datatype sourceblock;
  MPI_Type_vector( blocksize_j,blocksize_i,isize,MPI_INT,&sourceblock);
  MPI_Type_commit( &sourceblock);

  /*
   * Each continguous source block becomes a strided column in the transpose:
   * - number of blocks      : blocksize_i
   * - size of block         : 1
   * - stride between blocks : jsize ( = line length of the transpose )
   */
  MPI_Datatype targetcolumn;
  MPI_Type_vector( blocksize_i,1,jsize, MPI_INT,&targetcolumn);
  MPI_Type_commit( &targetcolumn );
  MPI_Datatype skinnycolumn;
  /*
   * Exercise: 
   * - resize the targetcolumn type to make its extent smaller
   */
  MPI_Type_create_resized( targetcolumn,0,
			   //solution
			   sizeof(int),
			   //solved
			   &skinnycolumn );
  MPI_Type_commit( &skinnycolumn );

  /*
   * Exercise:
   * - make the `targetblock' type that packs a number of `skinnycolumn's together
   */
  MPI_Datatype targetblock;
  //solution
  MPI_Type_contiguous( blocksize_j,
		       //solved
		       skinnycolumn, // old type
		       &targetblock  // new type
		       );
  MPI_Type_commit( &targetblock );

  /*
   * Every rank initiates a gather
   */
  for (int p=0; p<nprocs; p++) {
    MPI_Gather( data_0+p*blocksize_i,1,sourceblock,
		data_1,              1,targetblock,
		p,comm );
    if (p==procno) {
      printf("Output data:\n");
      for (int row=0; row<blocksize_i; row++) {
	printf("Row %3d:",procno*blocksize_i+row);
	for (int col=0; col<jsize; col++)
	  printf(" %3d",data_1[ row*jsize + col ]);
	printf("\n");
      }
    }
  }

  if (procno==0)
    printf("Finished\n");

  MPI_Finalize();
  return 0;
}

End Program TransposeBlock
