/*****************************************************************************
 *                                                                           *
 *  This function changes white spaces to underlines in the                  *
 *  sequence names.                                                          *
 *                                                               MARTEN 2007 *
 *****************************************************************************/
  
void pname(void)                                    
{  
  int         i       = 0;
  int         nr_remo = 0;
  
  
  std::string seq_tmp;         // Temporary name storage.
  std::string seq_mod;         // Modified  name storage.
  
        
  seq_mod = gstore.name; 
    
  // This loop goes through each char in the sequence for the 
  // specified organism and changes white spaces to underlines.
  seq_tmp = seq_mod;
  for(i=0; i < seq_tmp.length(); ++i) {
    if(seq_tmp[i] == ' ') { 
      // Replaces the char.
      seq_mod.replace(i - nr_remo,1,"_");
    }
  }
  
  // Assigns the new string to the variable sequence in the global 
  // class gstore.
  gstore.name = seq_mod;  
}
