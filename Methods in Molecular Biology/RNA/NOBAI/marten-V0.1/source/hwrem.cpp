/*****************************************************************************
 *                                                                           *
 *  This function removes all the white spaces (' ') within the sequences    *
 *  and changes ('^') in to ('][').                                          *
 *                                                               MARTEN 2007 *
 *****************************************************************************/
  
void hwrem(bool v_option)                                    
{  
  int         i       = 0;
  int         nr_blan = 0;
  int         nr_nemp = 0;
  int         nr_hats = 0;
  
  std::string seq_tmp;         // Temporary string storage.
  std::string seq_mod;         // Modified  string storage.
  std::string seq_nam;         // The sequence name;
  
  seq_mod = gstore.sequence; 
  seq_nam = gstore.name;

  // This loop goes through each char in the sequence for the 
  // specified organism and removes all the white spaces (' ').
  seq_tmp = seq_mod;
  for(i=0; i < seq_tmp.length(); ++i) {
    if(seq_tmp[i] != ' ') {
      ++nr_nemp;
    }
    else {
      seq_mod.replace(i - nr_blan,1,"");
      ++nr_blan;
    }
  }
  
  // This inner loop goes through each char in the sequence for the 
  // specified organism and changes '^' in to ']['.
  seq_tmp = seq_mod; 
  for(i=0; i < seq_tmp.length(); ++i) {
    if(seq_tmp[i] == '^') {
      seq_mod.replace(i + nr_hats,1,"][");
      ++nr_hats;
    }
  }
  
  // Prints the result to std. out.
  if(v_option) {
    std::cout << " Name         : " << seq_nam << "\n"
	      << " Blanks       : " << nr_blan << "\n"
	      << " non-empty    : " << nr_nemp << "\n"
	      << " Hats         : " << nr_hats << "\n"
	      << " Start length : " << gstore.sequence.length() << "\n"
	      << " Mod.length   : " << seq_mod.length()<< "\n\n";
  }
  
  // Assigns the new string to the variable sequence in the global 
  // class gstore.
  gstore.sequence = seq_mod;
}




