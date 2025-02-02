/****************************************************************************
 *                                                                          *
 *     This function workes with the unassigned stem strands.                *
 *                                                              MARTEN 2007 *
 ****************************************************************************/


bool ussm(int nh, int ssbp, int ssep, int FirstNonMapped)                                    
{  
  
  bool returnb = false;
  bool mbss  = false;    // Helix number positioned before the
			 // beginning of the unmapped stem strand
  bool mass  = false;    // Helix number positioned after the end of
			 // the unmapped stem strand.
  bool cmbss = false;    // Complementary Helix number of the Helix
			 // number positioned before the beginning of
			 // the unmapped stem strand
  bool cmass = false;    // Complementary Helix number of the Helix
			 // number positioned after the end of the
			 // unmapped stem strand.
  std::string hnbss;     // Name of Helix number positioned before the
			 // beginning unmapped stem strand.
  std::string hnass;     // Name of Helix number positioned after the
			 // end of the unmapped stem strand.
  std::string chnbss;    // Name of the complementary Helix number of
			 // the Helix number that is positioned before
			 // the beginning unmapped stem strand.
  std::string chnass;    // Name of complementary Helix number of
			 // the Helix number positioned after the
			 // end of the unmapped stem strand.
  
  int hnbssp;            // Position in HelixNamesArray (Helix number
			 // name before the unmapped stem strand.)
  int hnassp;            // Position in HelixNamesArray (Helix number
			 // name after the unmapped stem strand.)
  int chnbssp;           // Position of the Complementary Helix number
			 // in HelixNamesArray (Helix number name before the
			 // unmapped stem strand.)
  int chnassp;           // Position of the Complementary Helix number
			 // in HelixNamesArray (Helix number name after the
			 // unmapped stem strand.)
  
  // Find the names of the Helix numbers on each side of the stem
  // strand.
  for(int m=0; m<nh; m++) { 
    if(gstore.HelixPosArray_ReturnValue(m)   < ssbp &&
       gstore.HelixPosArray_ReturnValue(m+1) > ssep) {
      hnbss = gstore.HelixNamesArray_ReturnName(m);
    }
    if(gstore.HelixPosArray_ReturnValue(m-1) < ssbp &&
       gstore.HelixPosArray_ReturnValue(m) > ssep ) {
      hnass = gstore.HelixNamesArray_ReturnName(m);
    }
  }  
  
  // Finds the positions of both the Helix numbers and the
  // complementary Helix numbers.
  hnbssp  = gstore.HelixNamesArray_ReturnNamePos(hnbss);
  hnassp  = gstore.HelixNamesArray_ReturnNamePos(hnass);
  chnbssp = gstore.HelixNamesArray_FindComplementaryNamePos(hnbss);
  chnassp = gstore.HelixNamesArray_FindComplementaryNamePos(hnass);
  
  // Have the helix numbers or the complementary helix numbers been
  // assigned to a stem strand?

  if(gstore.HSSMMatrix_ReturnHelixMap(hnbssp)){
    mbss  = true;
  }
  if(gstore.HSSMMatrix_ReturnHelixMap(hnassp)){
    mass  = true;
  }
  if(gstore.HSSMMatrix_ReturnHelixMap(chnbssp)){
    cmbss = true;
  }
  if(gstore.HSSMMatrix_ReturnHelixMap(chnassp)){
    cmass = true;
  }

  // Updates the HSSMMatrix if only one complementary helix number of
  // the two original Helix numbers are assosiated with a stem strand.
  
  if(cmbss && cmass && mass && !mbss){
    gstore.HSSMMatrix_SetMatrixEntryToTrue(hnbssp, FirstNonMapped);
    returnb = true; 
  }
  else if(cmbss && cmass && !mass && mbss){
    gstore.HSSMMatrix_SetMatrixEntryToTrue(hnassp, FirstNonMapped);
    returnb = true; 
  }
  else if(!cmbss && !cmass && mass && !mbss){
    gstore.HSSMMatrix_SetMatrixEntryToTrue(hnbssp, FirstNonMapped);
    returnb = true; 
  }
  else if(!cmbss && !cmass && !mass && mbss){
    gstore.HSSMMatrix_SetMatrixEntryToTrue(hnassp, FirstNonMapped);
    returnb = true; 
  }
  
  return(returnb);
}
