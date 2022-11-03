int main(int argc,char **argv} {
  PetscLogDouble tt1,tt2;
  KSP ksp;
  //ierr = VecSet(c,one);
  ierr = VecSet(c,one);
  ierr = VecSet(u,one);
  ierr = VecSet(b,one);

  ierr = KSPCreate(PETSC_COMM_WORLD,&ksp); CHKERRQ(ierr);
  ierr = KSP_MatMult(ksp,A,x,Ax); CHKERRQ(ierr);


  ierr = PetscTime(&tt1);CHKERRQ(ierr);
  ierr = VecNormBegin(u,NORM_2,&norm1);
  ierr = PetscCommSplitReductionBegin(PetscObjectComm((PetscObject)Ax)); 
  ierr = KSP_MatMult(ksp,A,c,Ac); 
  ierr = VecNormEnd(u,NORM_2,&norm1);
  ierr = PetscTime(&tt2);CHKERRQ(ierr);

  ierr = PetscPrintf(PETSC_COMM_WORLD, "The time used for the asynchronous calculation: %f\n",tt2-tt1); CHKERRQ(ierr);
  ierr = PetscPrintf(PETSC_COMM_WORLD,"+ |u| =  %g\n",(double) norm1); CHKERRQ(ierr);


  ierr = PetscTime(&tt1);CHKERRQ(ierr);
  ierr = VecNorm(b,NORM_2,&norm2); CHKERRQ(ierr);
  ierr = KSP_MatMult(ksp,A,c,Ac); 
  ierr = PetscTime(&tt2);CHKERRQ(ierr);

  ierr = PetscPrintf(PETSC_COMM_WORLD, "The time used for the synchronous calculation: %f\n",tt2-tt1); CHKERRQ(ierr);
  ierr = PetscPrintf(PETSC_COMM_WORLD,"+ |b| =  %g\n",(double) norm2); CHKERRQ(ierr);
};
