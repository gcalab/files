/*****************************************************************************
 *                                                                           *
 *  This function removes all the gaps ('-') within the stems ('[]') in the  *
 *  sequences.                                                               *
 *                                                               MARTEN 2007 *
 *****************************************************************************/
  
void gsrem(bool v_option)                                    
{  
  int         i       = 0;
  int         nr_brac = 0;
  int         nr_gaps = 0;
  
  std::string seq_tmp;         // Temporary string storage.
  std::string seq_mod;         // Modified  string storage.
  std::string seq_nam;         // The sequence name;
  
  bool bracket = false;

  seq_mod = gstore.sequence; 
  seq_nam = gstore.name;
  
  // This loop goes through each char in the sequence for the 
  // specified organism and removes all the gaps in the stems ('-').
  seq_tmp = seq_mod;
  for(i=0; i < seq_tmp.length(); ++i) {
    if(seq_tmp[i] == '[') { ++nr_brac; bracket = true; }
    if(seq_tmp[i] == ']') { bracket = false; } 
    if( bracket && seq_tmp[i]=='-') {
      seq_mod.replace(i - nr_gaps,1,"");
      ++nr_gaps;
    }
  }
  
  
  // Prints the result to std. out.
  if(v_option) {
    std::cout << " Name         : " << seq_nam << "\n"
	      << " Stems        : " << nr_brac << "\n"
	      << " Start length : " << gstore.sequence.length() << "\n"
	      << " Mod. length  : " << seq_mod.length()<< "\n\n";
  }
  // Assigns the new string to the variable sequence in the global 
  // class gstore.
  gstore.sequence = seq_mod;  
}
    
