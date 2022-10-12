/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%
   %%%% This program file is part of the book and course
   %%%% "Parallel Computing"
   %%%% by Victor Eijkhout, copyright 2013-5
   %%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

//codesnippet chkerr
#define CHK(x) if (x) {						 \
    char errtxt[200]; int len=200;				 \
  MPI_Error_string(x,errtxt,&len);				 \
  printf("p=%d, line=%d, err=%d, %s\n",procno,__LINE__,x,errtxt); \
  return x;}
//codesnippet end
#define ASSERT(p) if (!(p)) {printf("Assertion failed for proc %d at line %d\n",procno,__LINE__); return -1;}
#define ASSERTm(p,m) if (!(p)) {printf("Message<<%s>> for proc %d at line %d\n",m,procno,__LINE__); return -1;}
#define MOD(i,n) (i+n)%(n)
#define ABS(x) ((x)<0 ? (-x) : (x))

  MPI_Comm comm;
  int procno=-1,nprocs,ierr,err;
  err = MPI_Init(&argc,&argv); CHK(err);
  comm = MPI_COMM_WORLD;
  err = MPI_Comm_rank(comm,&procno); CHK(err);
  err = MPI_Comm_size(comm,&nprocs); CHK(err);
  err = MPI_Comm_set_errhandler(comm,MPI_ERRORS_RETURN); CHK(err);

  // Initialize the random number generator
  srand((int)(procno*(double)RAND_MAX/nprocs));
