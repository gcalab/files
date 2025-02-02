/****************************************************************************
 *                                                                          *
 *     This function prints the program options.                            *
 *                                                              MARTEN 2007 *
 ****************************************************************************/


void phelp(void)                                    
{
  std::cout << ""                                                    << endl
	    << " Options      :"                                     << endl
	    << "  -i    : Specifies the input  file."                << endl
	    << "  -o    : Specifies the output file."                << endl
	    << "  -e    : Eliminates gaps ('-') within []."          << endl
	    << "  -v    : Gives verbose output."                     << endl
	    << "  -s    : Sums nucleotides in bulges."               << endl
	    << "  -p    : Removes  non-standard pairing info."       << endl
	    << "  -a    : Sums all bulges. (Includes -s)"            << endl
	    << "  -m    : Sums bases outside stems.  "               << endl
	    << "  -n    : Sums bases in stems. (Includes -p)"        << endl
	    << "  -g    : Eliminates gaps ('-') outside []."         << endl
	    << "  -h    : Prints help."                              << endl
	    << "  -V    : Prints version number."                    << endl
	    << "  -P    : Makes PAUP matrix."                        << endl
	    << "  -x    : Specify helix search space (D=0)."         << endl
	    << "  -l    : List min and max values for each colum."   << endl
	    << "  -r    : Make a flipped paup matrix."               << endl
	    << "  -R    : Add complementary numbers for each stem"   << endl
	    << "  -S    : Change number of character states (D=64)." << endl
	    << endl                                                  << endl
	    << " Correct usage: "
	    << "\"marten -i input-file -o output-file ... options\""
	    << endl                                                  << endl;
}
