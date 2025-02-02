/****************************************************************************
 *                                                                          *
 *     This function finds the positions of the Helix numbers.              *
 *                                                              MARTEN 2007 *
 ****************************************************************************/


void helixnames()                                    
{  
  int         spos = 0;
  int         epos = 0;
  int         j     = 0; // Number of helixes.
  std::string helixnamearray[gstore.sequence.length()];

  // Makes all white spaces to a '-'.
  for(int i=0; i < gstore.sequence.length(); ++i) {
    spos = gstore.sequence.find(" ",i);
    if(spos != -1) {
      gstore.sequence.replace(spos,1,"-");
      i=spos;
    }
  }
  
  // Find the actual positions.
  for(int i=0; i < gstore.sequence.length(); ++i) {
    spos = gstore.sequence.find_first_not_of("-",i);
    if(spos != -1) {
      epos = gstore.sequence.find_first_of("-",spos);
      if(epos != -1) {
	i=epos;
	helixnamearray[j]= gstore.sequence.substr(spos,epos-spos);
      }
      j++;
    }
  }
  
  // Redefines the size of the Helix names array in the global object.
  gstore.HelixNamesArray_ChangeSize(j);
  
  // Inserts the Helix number positions into the position array of the
  // global class gstore.
  for(int i=0; i < j; ++i) {
    gstore.HelixNamesArray_InsertName(i,helixnamearray[i]); 
  }
}



