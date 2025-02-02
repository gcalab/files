/*****************************************************************************
 *                                                                           *
 *  This function sums the number of nucleotides in all the bulges           *
 *  ('{}') within the stems ('[]') of the sequences.                         *
 *                                                               MARTEN 2007 *
 *****************************************************************************/
  
void absum(bool v_option)                                    
{  
  int         i       = 0;
  int         nr_brai = 0;
  int         nr_nuck = 0;
  int         nr_remo = 0;
  int         nr_size = 0;
  int         nr_bulg = 0;
  
  
  std::string seq_tmp;         // Temporary string storage.
  std::string seq_mod;         // Modified  string storage.
  std::string seq_nam;         // The sequence name;
  std::string str_nuck; 
  std::string str_base;
  std::string str_size;

  bool braises = false;
  
  seq_mod = gstore.sequence; 
  seq_nam = gstore.name;

  // This loop goes through each char in the sequence for the 
  // specified organism and sums the number of nucleotides inside
  // all of bulges in each stem.
  seq_tmp = seq_mod;
  for(i=0; i < seq_tmp.length(); ++i) {
    if(seq_tmp[i] == '{') { 
      braises = true; 
      // Removes the char in the modified string.
      seq_mod.replace(i - nr_remo,1,"");
      // Updates the counters.
      ++nr_brai;
      ++nr_remo;
    }
    if(seq_tmp[i] == '}') { 
      braises = false;
      // Removes the char in the modified string.
      seq_mod.replace(i - nr_remo,1,"");
      // Updates the counter.
      ++nr_remo; 
      // Have to convert the string str_size to an integer.
      nr_bulg = atoi(str_size.c_str());
      // Updates the total number of nucleotides inside '{}' so far.
      nr_size = nr_size + nr_bulg;
      // Resets the string containing info on the current number 
      // of nucleotides inside this '{}'.
      str_size.erase(0,str_size.length());
    } 
    if(braises && seq_tmp[i] != '{') { 
      // Adds the string integer of this  position inside the '{}' to the 
      // end of the temporary storage.
      str_size.append(seq_tmp.substr(i,1)); 
      // Removes the car.
      seq_mod.replace(i - nr_remo,1,"");
      // Updates the number of totally removed chars.
      ++nr_remo;
    }
    // AT the end of the stem we want to print the number inside one '{}'.
    if(seq_tmp[i] == ']') 
      { 
	// Have to convert the integer 'nr_size' to a string.
	std::stringstream ss;
	ss << nr_size;
	str_size = ss.str();
	// Inserts '{' to the modified string.
	seq_mod.insert(i - nr_remo,"{");
	// Updates the counter.
	--nr_remo;
	// inserts the string version of the integer into the modified 
	// string.
	seq_mod.insert(i - nr_remo,str_size);
	// Updates the counter.
	nr_remo = nr_remo - str_size.length(); 
	// At the end the '}' char is inserted to the modified string.
	seq_mod.insert(i - nr_remo,"}");
	// Updates and resets the counters.
	--nr_remo;
	nr_size = 0;
	// Resets the string containing info on the current number 
	// of nucleotides inside this '{}'.
	str_size.erase(0,str_size.length());
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

