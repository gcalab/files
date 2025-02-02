/****************************************************************************
 *                                                                          *
 *     This function finds the positions of the Helix numbers.              *
 *                                                              MARTEN 2007 *
 ****************************************************************************/


void helixpos()                                    
{  
  int         pos = 0;
  int         j   = 0;
  int         helixposarray[gstore.sequence.length()];

  
  // Makes all white spaces to a '-'.
  for(int i=0; i < gstore.sequence.length(); ++i) {
      pos = gstore.sequence.find(" ",i);
      if(pos != -1) {
	gstore.sequence.replace(pos,1,"-");
	i=pos;
      }
  }

  // Find the actual positions.
  for(int i=0; i < gstore.sequence.length(); ++i) {
    pos = gstore.sequence.find_first_not_of("-",i);
    if(pos != -1) {
      helixposarray[j] = pos ;
      pos = gstore.sequence.find_first_of("-",pos);
      if(pos != -1) {
	i=pos;
      }
      j++;
    }
  }
  
  // Redefines the size of the Helix number position array in the global
  // object.
  gstore.HelixPosArray_ChangeSize(j);
  
  // Inserts the Helix number positions into the position array of the
  // global class gstore.
  for(int i=0; i < j; ++i) {
    gstore.HelixPosArray_InsertValue(i,helixposarray[i]); 
  }
}
