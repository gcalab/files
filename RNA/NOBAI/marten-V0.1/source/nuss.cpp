/****************************************************************************
 *                                                                          *
 *     This function finds the number of non assigned stem strands between  *
 *     two assigned stem strands                                            *
 *                                                              MARTEN 2007 *
 ****************************************************************************/


bool nuss(int bnmss, int enmss, int nss)                                    
{  
  bool returnb = false;
  int bnmh     = 0;      // Begining of non mapped Helix numbers.
  int enmh     = 0;      // End      of non mapped Helix numbers.
    
  // Only one Helix number under the two stem strands is
  // allowed.
  if(
     (bnmss==0 && gstore.
      HSSMMatrix_ReturnStemStrandMapNumber(enmss+1)==1) 
     ||
     (enmss == nss-1 && gstore.
      HSSMMatrix_ReturnStemStrandMapNumber(bnmss-1)==1)
     ||
     (gstore.HSSMMatrix_ReturnStemStrandMapNumber(bnmss-1)==1  &&
      gstore.HSSMMatrix_ReturnStemStrandMapNumber(enmss+1)==1)
     ) {
    
    // Finds the Helix number positions.
    if(bnmss==0){
      bnmh=0;
      enmh=gstore.HSSMMatrix_ReturnStemStrandMapHelixPos(enmss+1)-1;
    }
    else if(enmss==nss-1){
      bnmh=gstore.HSSMMatrix_ReturnStemStrandMapHelixPos(bnmss-1)+1;
      enmh=nss-1;
    }
    else {
      bnmh=gstore.HSSMMatrix_ReturnStemStrandMapHelixPos(bnmss-1)+1;
      enmh=gstore.HSSMMatrix_ReturnStemStrandMapHelixPos(enmss+1)-1;
    }
    
    // Check if the number of missing Helix numbers are equal
    // to the number of missing stem strands.
    if( 
       (bnmss==0 && 
	gstore.HSSMMatrix_ReturnMatrixEntryState(enmh+1, enmss+1))
       ||
       (enmss==nss-1 && 
	gstore.HSSMMatrix_ReturnMatrixEntryState(bnmh-1, bnmss-1))
       ||
       ((gstore.HSSMMatrix_ReturnMatrixEntryState(bnmh-1, bnmss-1) &&
	gstore.HSSMMatrix_ReturnMatrixEntryState(enmh+1, enmss+1)) &&
	enmss-bnmss == enmh-bnmh)
       ) {
      
      // Goes through the missing stem strands and assign them
      // as true in the HSSMMatrix.
      for(int j=0; j<=enmss-bnmss; j++) {
	gstore.HSSMMatrix_SetMatrixEntryToTrue(bnmh+j, bnmss+j);
      }
      returnb = true;
    }
  }
  return(returnb);
}
	
