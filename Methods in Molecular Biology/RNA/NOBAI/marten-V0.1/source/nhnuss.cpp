/****************************************************************************
 *                                                                          *
 *     This function checks the number of Helix numbers underneath a stem   *
 *     strand.                                                              *
 *                                                              MARTEN 2007 *
 ****************************************************************************/


bool nhnuss(int nss, int nh)                                    
{  
  bool returnb = false;
  int np;                             // The name position
  std::string name;                   // The name
  std::string StemStrandHelixes[nh];  // Array with the name for the
				      // varios helixes under a stem
				      // strand.
  
  // Goes through each colum in the HSSMMatrix to find the number of
  // helixes that are located underneath each stem strand. A list of
  // the helixes are stored in the 'StemStrandHelixes' array.
  for(int n=0; n<nss; n++) {    
    
    // Number of Helix numbers underneath a stem strand.
    int nhuss = 0;                 
    
    // If there are multiple Helixes underneath a stem strand, then
    // perform a tests to decide which Helix number that should be
    // assigned to that s stem strand.
    if(gstore.HSSMMatrix_ReturnStemStrandMapNumber(n) > 1) {
      for(int m=0; m<nh; m++) {
	if(gstore.HSSMMatrix_ReturnMatrixEntryState(m,n)) {
	  StemStrandHelixes[nhuss] =
	    gstore.HelixNamesArray_ReturnName(m);
	  nhuss++;
	}
      }
      
      bool mappes[nhuss];
      int  nr_mappes = 0;
      
      // Prints a warning to standard out.
      if(gstore.v_option) {
	std::cout << endl 
		  << "WARNING!" << endl 
		  << "\""  << gstore.name << "\": "
		  << " The number of Helixes under stem strand " 
		  << n+1 << " is " << nhuss << ". (";
	for(int m=0; m<nhuss; m++) {
	  std::cout << StemStrandHelixes[m];
	  if(m < nhuss-2) {
	    std::cout << ", "; 
	  }
	  else if(m == nhuss-2) {
	    std::cout << " and "; 
	  }
	}	  
	std::cout << ") ";
      }
      
      // Tries to decide which Helix number to assign to the stem
      // strand. We look at the complementary helixes to see if they
      // have been assigned to a stem strand. If (only) one of the
      // complentary helixes are assigned, the assigned the
      // corresponding helix number to this stem strand.
      for(int m=0; m<nhuss; m++) {
	mappes[m] = false;
	name      = StemStrandHelixes[m];
	np        = gstore.HelixNamesArray_FindComplementaryNamePos(name);
	
	if(gstore.HSSMMatrix_ReturnHelixMap(np)) {
	  mappes[m] = true;
	  nr_mappes++;
	}
      }

      // If only one of the complementary helix numbers have been
      // assingd to a stem strand, then assign the corresponding helix
      // number to that stem strand.
      if(nr_mappes == 1) {
	for(int m=0; m<nhuss; m++) {
	  name = StemStrandHelixes[m];
	  np   = gstore.HelixNamesArray_ReturnNamePos(name);

	  // Update the HSSMMatrix matrix.
	  if(!mappes[m]) {
	    gstore.HSSMMatrix_SetMatrixEntryToFalse(np,n);
	    returnb = true;
	  }
	  else {
	    if(gstore.v_option) {
	      std::cout << "==> using " << StemStrandHelixes[m] << "." ;
	    } 
	  }
	}
      }
      else{
	if(gstore.v_option) { 
	  std::cout << "==> TRUBLE!!! MARTEN can not decide "
		    << "which Helix number to assign to the "
		    << "stem strand.";
	}
      }
      if(gstore.v_option) { 
	std::cout << endl;
      }
    }
  }
  return(returnb);
}
