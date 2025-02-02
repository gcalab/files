/****************************************************************************
 *                                                                          *
 *     This function find the beginning and end positions of the stems.     *
 *                                                              MARTEN 2007 *
 ****************************************************************************/


void ssstem()                                    
{  
  int         i       = 0;
  int         k       = 0;
  int         j       = 0;
    
  std::string seq_tmp;         // Temporary string storage.
    
  int         sarray[gstore.sequence.length()];
  int         earray[gstore.sequence.length()];  
  
  seq_tmp = gstore.sequence; 
  
  // This loop goes through each char in the sequence for the 
  // specified organism and finds the start and end of every bracket.
  for(i=0; i < seq_tmp.length(); ++i) {
    if(seq_tmp[i] == '[') 
      { 
	sarray[k] = i;
	k++;
      } 
    
    if(seq_tmp[i] == ']') 
      { 
	earray[j]= i;
	j++;
      } 
    if(seq_tmp[i] == '^') 
      { 
	earray[j]= i;
	j++;
	sarray[k] = i;
	k++;
      }
  }
  
  // Redefines the size of the arrays i global store.
  gstore.SandE_ChangeSize(k, j);
  
  // Assigns the new arrays to the variable arrays in the global 
  // class gstore.
  for(i=0; i < k; ++i) {
    gstore.SandE_InsertValueS(i,sarray[i]); 
  }
  
  for(i=0; i < j; ++i) {
    gstore.SandE_InsertValueE(i,earray[i]); 
  }
}
