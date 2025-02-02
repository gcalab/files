/****************************************************************************
 *                                                                          *
 *     This function prints the program options.                            *
 *                                                               STOAT 2007 *
 ****************************************************************************/


void phelp(void)                                    
{
  std::cout << ""                                                  << endl
	    << " Options      :"                                   << endl
	    << "  -i    : The input  file."                        << endl
	    << "  -o    : The output file."                        << endl
	    << "  -x    : Change the number of character states"   << endl     
	    << "  -h    : Prints help."                            << endl
	    << "  -v    : Gives verbose output."                   << endl
	    << "  -f    : Arguments to RNAfold"                    << endl
	    << "  -t    : Use input structure to generate DCSE"    << endl
	    << "  -r    : Make a flipped paup matrix"              << endl
	    << endl                                                << endl
	    << " Correct usage: "                          << endl << endl
	    << " \"stoat  -i name_of_input_file"                   << endl 
	    << "        [-o prefix_name_of_output_files]"          << endl 
	    << "        [-f \"RNAfold_options\"]"                  << endl 
	    << "        [-x number_of_character_states]"           << endl 
	    << "        [-v]\""                                    << endl 
	    << endl                                                << endl;
}
