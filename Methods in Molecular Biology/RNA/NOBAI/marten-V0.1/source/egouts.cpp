/*****************************************************************************
 *                                                                           *
 *  This function eliminate gaps ('-') outside the stems ('[]') of the       *
 *  sequences.                                                               *
 *                                                               MARTEN 2007 *
 *****************************************************************************/
  
void egouts(bool v_option)                                    
{  
  int         i       = 0;
  int         nr_remo = 0;
  int         nr_gaps = 0;
  int         nr_brac = 0;
  
  std::string seq_tmp;         // Temporary string storage.
  std::string seq_mod;         // Modified  string storage.
  std::string seq_nam;         // The sequence name;
        
  bool bracket = false;

  
  seq_mod = gstore.sequence; 
  seq_nam = gstore.name;
  
  // This loop goes through each char in the sequence for the
  // specified organism and eliminates the gaps on the outside of the
  // stems.
  seq_tmp = seq_mod;
  for(i=0; i < seq_tmp.length(); ++i) {
    if(seq_tmp[i] == '[') { 
      bracket = true;
      ++nr_brac;
    }
    if(seq_tmp[i] == ']') { 
      bracket = false;
    } 
    if( seq_tmp[i] == '-' && !bracket) { 
      // Removes the char.
      seq_mod.replace(i - nr_remo,1,"");
      // Updates the counters.
      ++nr_remo;
      ++nr_gaps;
    }
  }
  
  // Prints the result to std. out.
  if(v_option) {
    std::cout << " Name         : " << seq_nam << "\n"
	      << " Tot. removed : " << nr_gaps << "\n"
	      << " Start length : " << gstore.sequence.length() << "\n"
	      << " Mod. length  : " << seq_mod.length()<< "\n\n";
  }
  
  // Assigns the new string to the variable sequence in the global 
  // class gstore.
  gstore.sequence = seq_mod;  
}
