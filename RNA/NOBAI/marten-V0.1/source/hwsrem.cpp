/*****************************************************************************
 *                                                                           *
 *  This function makes sure that there is only one white space between the  *
 *  the numbers in the Helix Numbering sequence.                             *
 *                                                               MARTEN 2007 *
 *****************************************************************************/
  
void hwsrem(bool v_option)                                    
{  
  int         i       = 0;
  int         nr_remo = 0;
  int         nr_blan = 0;
    
  std::string seq_tmp;         // Temporary string storage.
  std::string seq_mod;         // Modified  string storage.
  std::string seq_nam;         // The sequence name;
        
    
  seq_mod = gstore.sequence; 
  seq_nam = gstore.name;
  
  // This loop goes through each char in the sequence for the Helix numbering
  // and leaves one white space between each number.
  seq_tmp = seq_mod;
  for(i=0; i < seq_tmp.length(); ++i) {
    if(seq_tmp[i] == ' ') { 
      ++nr_blan;
    }
    if(seq_tmp[i] != ' ') { 
      nr_blan = 0;
    } 
    if( seq_tmp[i] == ' ' && nr_blan > 1) { 
      // Removes the char.
      seq_mod.replace(i - nr_remo,1,"");
      // Updates the counters.
      ++nr_remo;
    }
  }
  
  // Prints the result to std. out.
  if(v_option) {
    std::cout << " Name         : " << seq_nam << "\n"
	      << " Tot. removed : " << nr_remo << "\n"
	      << " Start length : " << gstore.sequence.length() << "\n"
	      << " Mod. length  : " << seq_mod.length()<< "\n\n";
  }
  
  // Assigns the new string to the variable sequence in the global 
  // class gstore.
  gstore.sequence = seq_mod;  
}
