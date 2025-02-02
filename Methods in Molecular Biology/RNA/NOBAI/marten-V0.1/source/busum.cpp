/*****************************************************************************
 *                                                                           *
 *  This function sums the number of nucleotides in a bulge ('{}')           *
 *  within the stems ('[]') of the sequences.                                *
 *                                                               MARTEN 2007 *
 *****************************************************************************/
  
void busum(bool v_option, char bases[])                                    
{  
  int         i       = 0;
  int         nr_brai = 0;
  int         nr_nuck = 0;
  int         nr_remo = 0;
  int         nr_find = 0;
  
  std::string seq_tmp;         // Temporary string storage.
  std::string seq_mod;         // Modified  string storage.
  std::string seq_nam;         // The sequence name;
  std::string str_nuck; 
  std::string str_base;
    
  bool braises = false;
  
  str_base = bases;

  seq_mod = gstore.sequence; 
  seq_nam = gstore.name;

  // This loop goes through each char in the sequence for the 
  // specified organism and sums the number of nucleotides inside
  // of bulges in each stem.
  seq_tmp = seq_mod;
  for(i=0; i < seq_tmp.length(); ++i) {
    if(seq_tmp[i] == '{') { 
      braises = true; 
      ++nr_brai;
    }
    if(seq_tmp[i] == '}') 
      { 
	braises = false; 
	// Have to convert the integer 'nr_nuck' to a string.
	std::stringstream ss;
	ss << nr_nuck;
	str_nuck = ss.str();
	// Writes the number in to the modified string.
	seq_mod.insert(i - nr_remo,str_nuck);
	// Resets the value again.
	nr_nuck = 0;
	// Updates the number of removed chars in the modified string.
	// This is '-' since we added a number.
	nr_remo = nr_remo - str_nuck.length();
      } 
    // Is this char a part of the specified set of nucleotides 
    // specified in the string bases.
    nr_find = str_base.find(seq_tmp[i],0);
    if(braises && nr_find != -1 ) { 
      // Removes the char.
      seq_mod.replace(i - nr_remo,1,"");
      // Updates the counters.
      ++nr_remo;
      ++nr_nuck;
    }
  }
  
  
  // Prints the result to std. out.
  if(v_option) {
    std::cout << " Name         : " << seq_nam << "\n"
	      << " Tot. braises : " << nr_brai << "\n"
	      << " Start length : " << gstore.sequence.length() << "\n"
	      << " Mod. length  : " << seq_mod.length()<< "\n\n";
  }
  
  // Assigns the new string to the variable sequence in the global 
  // class gstore.
  gstore.sequence = seq_mod;  
}

