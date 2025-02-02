/*****************************************************************************
 *                                                                           *
 *  This function removes all the non-standard pairing information ('()')    * 
 *  within the stems ('[]') in the sequences.                                *
 *                                                               MARTEN 2007 *
 *****************************************************************************/
  
void nsprem(bool v_option)                                    
{  
  int         i       = 0;
  int         nr_pare = 0;
  int         nr_remo = 0;
  
  std::string seq_tmp;         // Temporary string storage.
  std::string seq_mod;         // Modified  string storage.
  std::string seq_nam;         // The sequence name;
  
  seq_mod = gstore.sequence; 
  seq_nam = gstore.name;

  // This loop goes through each char in the sequence for the 
  // specified organism and removes all the non-standard pairing 
  // information.
  seq_tmp = seq_mod;
  for(i=0; i < seq_tmp.length(); ++i) {
    if(seq_tmp[i] == '(') { 
      seq_mod.replace(i - nr_remo,1,"");
      ++nr_pare; 
      ++nr_remo;
    }
    if(seq_tmp[i] == ')') { 
      seq_mod.replace(i - nr_remo,1,"");
      ++nr_remo;
    }
  }
        
  // Prints the result to std. out.
  if(v_option) {
    std::cout << " Name         : " << seq_nam << "\n"
	      << " Non-st. pair : " << nr_pare << "\n"
	      << " Start length : " << gstore.sequence.length() << "\n"
	      << " Mod. length  : " << seq_mod.length()<< "\n\n";
  }
  
  // Assigns the new string to the variable sequence in the global 
  // class gstore.
  gstore.sequence = seq_mod;  
  
}
