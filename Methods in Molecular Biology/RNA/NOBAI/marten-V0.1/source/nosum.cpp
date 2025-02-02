/*****************************************************************************
 *                                                                           *
 *  This function sums the number of nucleotides outside the stems           *
 *  ('[]') of the sequences.                                                 *
 *                                                               MARTEN 2007 *
 *****************************************************************************/
  
void nosum(bool v_option, char bases[])                                    
{  
  int         i       = 0;
  int         nr_nuck = 0;
  int         nr_remo = 0;
  int         nr_find = 0;
  int         nr_brac = 0;
  
  std::string seq_tmp;         // Temporary string storage.
  std::string seq_mod;         // Modified  string storage.
  std::string seq_nam;         // The sequence name;
  std::string str_nuck; 
  std::string str_base;
    
  bool bracket = false;

  str_base = bases;

  seq_mod = gstore.sequence; 
  seq_nam = gstore.name;
  
  // This loop goes through each char in the sequence for the 
  // specified organism and sums the number of nucleotides outside
  // each stem.
  seq_tmp = seq_mod;
  for(i=0; i <= seq_tmp.length(); ++i) {
    if(seq_tmp[i] == '[' || i == seq_tmp.length()) { 
      bracket = true;
      ++nr_brac;
      // Inserts a whitespace.
      seq_mod.insert(i - nr_remo," ");
      // Updates the counter.
      --nr_remo; 
      // Have to convert the integer 'nr_nuck' to a string.
      std::stringstream ss;
      ss << nr_nuck;
      str_nuck = ss.str();
      // inserts the string version of the integer into the modified 
      // string.
      seq_mod.insert(i - nr_remo,str_nuck);
      // Updates the counter.
      nr_remo = nr_remo - str_nuck.length(); 
      // Resets the counters.
      str_nuck.erase(0,str_nuck.length());
      nr_nuck = 0; 
    }
    if(seq_tmp[i] == ']') { 
      bracket = false;
    } 
    nr_find = str_base.find(seq_tmp[i],0);
    if(nr_find != -1 && !bracket) { 
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
	      << " Tot. brackets: " << nr_brac << "\n"
	      << " Start length : " << gstore.sequence.length() << "\n"
	      << " Mod. length  : " << seq_mod.length()<< "\n\n";
  }
  
  
  // Assigns the new string to the variable sequence in the global 
  // class gstore.
  gstore.sequence = seq_mod;  
}

