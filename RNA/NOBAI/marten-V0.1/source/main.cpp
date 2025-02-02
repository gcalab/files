/*****************************************************************************
 *                                                                           *
 *                     __  __   __   ___   ____  ___  _  _                   *
 *                    (  \/  ) (  ) (  ,) (_  _)(  _)( \( )                  *
 *                     )    (  /__\  )  \   )(   ) _) )  (                   *
 *                    (_/\/\_)(_)(_)(_)\_) (__) (___)(_)\_)                  *
 *                    -------------------------------------                  *
 *                                                                           *
 * 	   To install MARTEN, please see the makefile in this directory.     *
 *									     *
 * MARTEN is written by Dr. Vegeir Knudsen and it uses DCSE formated         *
 * inputfiles to calculate the number of nucleotides within different        *
 * geometrical structures, such as stems, bulges and unpaired. Depending on  *
 * the input parameters, the result is presented as a PAUP matrix in a nexus *
 * formated file.                                                            *
 *                                                                           *
 *                                                                           *
 * Contact :        Gustavo Caetano-Anolles          Vegeir Knudsen          *
 *                  NSRC, UIUC                       USIT, UiO               *
 *                  1101 West Peabody Drive          Postboks 1059 Blindern  *
 *                  Urbana, Illinois 61801           0316 Oslo               *
 *                  USA                              Norway                  *
 *                                                                           *
 *                  gca@uiuc.edu                     vegeirk@usit.uio.no     *
 *                                                                           *
 *                                                               MARTEN 2007 *
 *****************************************************************************/


// System included files.
#include <iostream>                          
#include <cstdlib>
#include <fstream.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector.h>
#include <sstream>
#include <iomanip>


// User included files: 

// Defines the two types of clases used in this program.
#include "global.cpp"    
#include "organism.cpp"  

// Starts the class here to make it global. 
// (gstore = GlobalStore).
class global gstore;

#include "phelp.cpp"      // Prints help to std. out.
#include "hwrem.cpp"      // Removes white spaces and replaces hats with "[]".
#include "gsrem.cpp"      // Removes gapes in stems.
#include "nsprem.cpp"     // Removes all the non-standard in stems.
#include "busum.cpp"      // Sums the number of nucleotides in a bulge.
#include "absum.cpp"      // Sums the number of nucleotides in all bulges.
#include "snsum.cpp"      // Sums the number of nucleotides in stems.
#include "nosum.cpp"      // sums the number of nucleotides outside stems.
#include "egouts.cpp"     // Eliminate gaps outside stems.
#include "inputc.cpp"     // Reads the input from the command line.
#include "pinput.cpp"     // Prints the input parameters.
#include "hwsrem.cpp"     // Only one white space in the Helix Numbering.
#include "in2ch.cpp"      // Transforms an integer to a char.
#include "ch2in.cpp"      // Transforms a char to an integer.
#include "pname.cpp"      // Changes white spaces to underlines in seq. name.
#include "ssstem.cpp"     // Findes start and stop positions of stems.
#include "voutput.cpp"    // Prints some verbose output to standard out.
#include "helixpos.cpp"   // Finds the position of the Helixes.
#include "helixnames.cpp" // Finds the Helix names.
#include "nhnuss.cpp"     // Finds the number of Helixes under a stem strand.
#include "nuss.cpp"       // Finds the number of unmapped stem strands.
#include "ussm.cpp"


// Start of main program.
int main (int argc, char* argv[]) {
  int   c;
  int   nr_sfhnnn = 0; // New number of positions to search for Helix
		       // numbers outside the stem strands. (Search
		       // for Helix numbering nams new)
  int   nr_sfhnnd = 0; // Default number of positions to search for
		       // Helix numbers outside the stem strands.
  char* infile_name; 
  char* outfile_name;
  char  bases[60] = "AaCcGgTtUuRrMmKkWwSsBbDdHhVvNnOo";

  std::string   buffer;    // Input buffer used by ifstram.
  
  int           MAX_ORG=0;     // Largest amount of organisms.
  int  character_states = 64;  // The default number of character
			       // states is 64 if not spesified
                               // differently by the user with the -S
                               // option.
  
  // Initiating values of the global storage class gstore.
  gstore.name = "zero"; 
  gstore.sequence = "zero"; 
  gstore.e_option = false;
  gstore.v_option = false;
  gstore.p_option = false;
  gstore.s_option = false;
  gstore.a_option = false;
  gstore.n_option = false;
  gstore.m_option = false;
  gstore.g_option = false;
  gstore.P_option = false;
  gstore.x_option = false;
  gstore.S_option = false;
  gstore.l_option = false;
  gstore.r_option = false;
  gstore.R_option = false;
  
  
  char defaultinfile[11] = "infile.cgi";
  char defaultoutfile[8] = "outfile";
  
  infile_name = defaultinfile;
  outfile_name = defaultoutfile;

  
  /* ======================================================================= *
   *  Input Parameters                                                       *
   * ======================================================================= */
  
  // Small check to controll the number of input arguments.
  if( argc < 2) {
    std::cout << endl << " Marten needs at least one input option !"
	      << endl << " Try -h for help."
	      << endl << endl;
    exit(-1);
  }
  
  while ((c = getopt(argc, argv, "RrlhvVepsanmgPi:o:x:S:")) != -1) { 
    inputc (c);
    switch (c) {
    case 'i': { 
      infile_name = optarg;
      break;
    }
    case 'o': { 
      outfile_name = optarg;
      break;
    }
    case 'x': { 
      nr_sfhnnn = atoi(optarg);
      gstore.x_option = true;
      break;
    }    
    case 'S': { 
      character_states = atoi(optarg);
      if(character_states > 64 || character_states < 1) {
	std::cout << endl << "ERROR! Number of character states "
		  << "can not be larger than 64 or less than 1!" 
		  << endl << endl; 
	exit (1); 
      }  
      break;
    }
    }
  }
  
  // Assigns the new value for the helix search.
  if (gstore.x_option) {
    nr_sfhnnd = nr_sfhnnn;
  }
  
  // Gives the user some feedback on the chosen input parameters.
  if(gstore.v_option) { 
    pinput();
    std::cout << "  -i    : Input  file is \""  << infile_name     
	      << "\""                           << endl;
    std::cout << "  -o    : Output file is \""  << outfile_name     
	      << "\""                           << endl;
    std::cout << "\n";
  }
  

  
  /* ======================================================================= *
   *  Read from file and create objects                                      *
   * ======================================================================= */
  
  //
  // First we need to get the number of non-empty lines in the input file.
  // This number is used to create a sufficiently large array of classes for 
  // the different sequences. 
  //
    
  ifstream is (infile_name, ios::in);
  if (! is.is_open()) { 
    std::cout << "Error opening file : " << infile_name << "\n\n"; 
    exit (1); 
  }  
  
  while (! is.eof()) {
    while (getline(is , buffer)) {
      if(buffer != "" ) ++MAX_ORG;
    }
  }
  is.close();
  
  
  //
  // We have now got the number of non-empty lines and starts to get the 
  // actual sequences from the input file.
  //
  
  int i       = 0; // Simple input file line Counter.
  int nr_orga = 0; // Organism counter
  int ibl     = 0; // Input Buffer Length.
  int send    = 0; // End of sequence and start of name.
  int k       = 0; // Loop counter
  int nr_warn = 1; // Number of printeded warnings.

  // Defines the class/object array.
  class organism org[MAX_ORG];

  // Opens the input file for reading.
  ifstream infile (infile_name, ios::in);
  if (! infile.is_open()) { 
    std::cout << "Error opening file         : " << infile << "\n\n"; 
    exit (1);     
  } 
  while (! infile.eof() ) {             
    // Updates the line counter.
    i++;
    // Gets a new line.
    getline (infile,buffer);
    // Finds the length of the line. (Input buffer length.)
    ibl = buffer.length();
    
    // Have we reached the sequence part of the input file ?
    if(i >= 5 && ibl > 10) {
      // Turns the in buffer to a string.
      std::string full_sequence = buffer; 
      // finds the end of the sequence. (Seven blank spaces.)
      send = full_sequence.find("       ");   
      // Finds the sequence name.
      std::string seq_name = full_sequence.substr(send + 7,ibl);
      // Finds the actual sequence.
      std::string nucleotides = full_sequence.substr(0,send);
      
      // Assigns the object parameters.
      org[nr_orga].name     = seq_name;
      org[nr_orga].sequence = nucleotides; // Original.
      org[nr_orga].modified = nucleotides; // Modified.
      // Updates the organism counter.
      nr_orga++;
    }
  }
  infile.close();
  
  // writes out the extracted information from the input file.
  if(gstore.v_option) { 
    std::cout << "\n\n";
    std::cout << " Input file   : " << "\n";
    for(k=0; k < nr_orga; ++k){
      std::cout << "  " << k << ".    : " << org[k].name << "\n";
    }
    std::cout << "  Lines : " << MAX_ORG << "  (non-empty).\n";
    std::cout << "  Lines : " << i       << "  (total).\n";
  }
  

  int nr_brack1 = 0;
  int nr_brack2 = 0;
  int a         = 0;
  int b         = 0;

  // Short test to see if the number of brackets match.
  for(a=0; a < nr_orga-1; ++a) {
    nr_brack1 = 0;
    nr_brack2 = 0;
    for(b=0; b < org[a].sequence.length(); ++b) {
      if(org[a].sequence[b] == '[') {
	nr_brack1++;
      }
      if(org[a].sequence[b] == ']') {
	nr_brack2++;
      }
      if(org[a].sequence[b] == '^') {
	nr_brack1++;
	nr_brack2++;
      }
    }
    // Prints an ERROR message to the user.
    if(nr_brack1 != nr_brack2) {
      std::cout << endl << endl 
		<< "ERROR! Number of \"[\" and \"]\" does not match for "
		<< "sequence \"" << org[a].name << "\" " 
		<< "([="<< nr_brack1   << " and ]=" << nr_brack2 << "). " 
		<< "Aborting MARTEN!" << endl << endl;
      exit(1);
    }
  }
  
  
  //########################################################################// 
  //########################################################################// 
  //##########                                                   ###########// 
  //##########                      START                        ###########// 
  //##########                                                   ###########// 
  //##########            PROCESSING THE SEQUENCES               ###########//
  //##########                                                   ###########// 
  //########################################################################// 
  //########################################################################// 
  
  /* ======================================================================= *
   *  This loop removes all the white spaces (' ') within the sequences      *
   *  and changes '^' in to ']['.                                            *
   * ======================================================================= */
  
  // Prints some user info to std. out
  if(gstore.v_option) {
    voutput(1);
  }
  // This loop goes through the organisms.
  for(k=0; k < nr_orga - 1; ++k) {
    gstore.sequence = org[k].modified;
    gstore.name     = org[k].name;
    hwrem(gstore.v_option); 
    org[k].modified = gstore.sequence;
  }

  
  /* ======================================================================= *
   *  This loop removes all the gaps ('-') within the stems ('[]') in the    *
   *  sequences.                                                             *
   * ======================================================================= */
  
  // Is this option wanted by the user ?
  if(gstore.e_option) {
    // Prints some user info to std. out
    if(gstore.v_option) {
      voutput(2);
    }
    
    // This loop goes through the organisms.
    for(k=0; k < nr_orga - 1; ++k) {
      gstore.sequence = org[k].modified;
      gstore.name     = org[k].name;
      gsrem(gstore.v_option);
      org[k].modified = gstore.sequence;
    }
  }


  /* ======================================================================= *
   *  This loop removes all the non-standard pairing information ('()')      *
   *  within the stems ('[]') in the sequences.                              *
   * ======================================================================= */
  
  // Is this option wanted by the user ?
  if(gstore.p_option) {
    // Prints some user info to std. out
    if(gstore.v_option) {
      voutput(3);
    }
    
    // This loop goes through the organisms.
    for(k=0; k < nr_orga - 1; ++k) {
      gstore.sequence = org[k].modified;
      gstore.name     = org[k].name;
      nsprem(gstore.v_option);
      org[k].modified = gstore.sequence;
    }
  }
    

  /* ======================================================================= *
   *  This loop sums the number of nucleotides in the stems ('[]'),          *
   *  (but leaves the bulges), of the sequences.                             *
   * ======================================================================= */
  
  // Is this option wanted by the user ?
  if(gstore.n_option) {
    // Prints some user info to std. out
    if(gstore.v_option) {
      voutput(4);
    }
    
    // This loop goes through the organisms.
    for(k=0; k < nr_orga - 1; ++k) {
      gstore.sequence = org[k].modified;
      gstore.name     = org[k].name;
      snsum(gstore.v_option,bases);
      org[k].modified = gstore.sequence;      
    }
  }
  
  
  /* ======================================================================= *
   *  This loop sums the number of nucleotides in a bulge ('{}')             *
   *  within the stems ('[]') of the sequences.                              *
   * ======================================================================= */
  
  // Is this option wanted by the user ?
  if(gstore.s_option) {
    // Prints some user info to std. out
    if(gstore.v_option) {
      voutput(5);
    }
    
    // This loop goes through the organisms.
    for(k=0; k < nr_orga - 1; ++k) {
      gstore.sequence = org[k].modified;
      gstore.name     = org[k].name;
      busum(gstore.v_option,bases);
      org[k].modified = gstore.sequence;
    }
  }
    
  
  /* ======================================================================= *
   *  This loop sums the number of nucleotides in all the bulges             *
   *  ('{}') within the stems ('[]') of the sequences.                       *
   * ======================================================================= */
  
  // Is this option wanted by the user ?
  if(gstore.a_option) {
    // Prints some user info to std. out
    if(gstore.v_option) {
      voutput(6);
    }
    
    // This loop goes through the organisms.
    for(k=0; k < nr_orga - 1; ++k) {
      gstore.sequence = org[k].modified;
      gstore.name     = org[k].name;
      absum(gstore.v_option);
      org[k].modified = gstore.sequence;
    }
  }
  
  
  /* ======================================================================= *
   *  This loop sums the number of nucleotides outside the stems             *
   *  ('[]') of the sequences.                                               *
   * ======================================================================= */
  
  // Is this option wanted by the user ?
  if(gstore.m_option) {
    // Prints some user info to std. out
    if(gstore.v_option) {
      voutput(7);
    }
    
    // This loop goes through the organisms.
    for(k=0; k < nr_orga - 1; ++k) {
      gstore.sequence = org[k].modified;
      gstore.name     = org[k].name;
      nosum(gstore.v_option,bases);
      org[k].modified = gstore.sequence;     
    }
  }
  
    
  /* ======================================================================= *
   *  This loop eliminate gaps ('-') outside the stems ('[]') of the         *
   *  sequences.                                                             *
   * ======================================================================= */
  
  // Is this option wanted by the user ?
  if(gstore.g_option) {
    // Prints some user info to std. out
    if(gstore.v_option) {
      voutput(8);
    }
    
    // This loop goes through the organisms.
    for(k=0; k < nr_orga - 1; ++k) {
      gstore.sequence = org[k].modified;
      gstore.name     = org[k].name;
      egouts(gstore.v_option);
      org[k].modified = gstore.sequence;     
    }
  }
  
    
  //########################################################################// 
  //########################################################################// 
  //##########                                                   ###########// 
  //##########                      START                        ###########// 
  //##########                                                   ###########// 
  //##########               PAUP STRING GENERATION              ###########//
  //##########                                                   ###########// 
  //########################################################################// 
  //########################################################################// 
  
  bool helix = false;
  
  if(gstore.P_option) {
    
    /*====================================================================== *
     *  Start to work on the Helix numbering Sequence                        *
     *                                                                       *
     * ===================================================================== */
 
    // The helix information is stored in the last ot the org[]-objects
    gstore.HelixNumberingStringName = org[nr_orga - 1].name;
    gstore.HelixNumberingString     = org[nr_orga - 1].sequence;
       
    // Prints some user info to std.out
    if(gstore.v_option) {
      voutput(9);
    }
    
    // A short test to see if the last line is named "Helix
    // numbering".
    if(gstore.HelixNumberingStringName.
       substr(0,15).compare("Helix numbering") != 0)  {
      std::cout << endl 
		<< "************** ERROR! **************" << endl << endl
		<< "The name at the end of the last input line does "
		<< "not begin with \"Helix numbering\" ==> Aborting MARTEN!"
		<< endl << endl;
      exit(1);
    }
        
    // We have to find the position of the Helix numbers in the sequence.
    // Copies the helix numbering over to the global sequence string
    gstore.sequence = gstore.HelixNumberingString;
    // Runs the functions which loads the Helix numbering positions
    // into the global array
    helixpos();
    
    // We have to find the names of the Helix numbers in the sequence.
    // Copies the helix numbering over to the global sequence string
    gstore.sequence = gstore.HelixNumberingString;
    // Runs the functions which loads the Helix numbering names
    // into the global array
    helixnames();
    
    // A test to see if all the Helix strand have a complementary
    // Helix strand. (Does for example "D1" have a "D1'".)
    for(int d=0; d<gstore.HelixNamesArray_ReturnSize(); d++) {
      if(gstore.HelixNamesArray_FindComplementaryNamePos(
	 gstore.HelixNamesArray_ReturnName(d)) == -1) {
	std::cout << endl 
		  << "************** ERROR! **************" << endl << endl
		  << "The Helix numbering strand \""
		  << gstore.HelixNamesArray_ReturnName(d) << "\" does not "
		  << "have complementary strand in the Helix numbering "
		  << "==> Aborting MARTEN!" << endl << endl;
	exit(1);
      }
    }
    
    /*====================================================================== *
     *  Start to work on the other sequences                                 *
     *                                                                       *
     * ===================================================================== */
    
    // This loop goes through the the organisms and gets the two
    // arrays for the start and end positions of the stem strand
    // brackets. First this value is stored in global store, then it
    // is stored in the variuos sequence objects.
    for(k=0; k < nr_orga-1; ++k) {

      gstore.sequence = org[k].sequence;
      ssstem();
      org[k].SandE_ChangeSize(gstore.SandE_ReturnSizeS(),
			      gstore.SandE_ReturnSizeE());
      // Loads the walues into the class arrays.
      for(i = 0; i < gstore.SandE_ReturnSizeS(); ++i) {
	org[k].SandE_InsertValueS(i,gstore.SandE_ReturnValueS(i));
      }
      for(i = 0; i < gstore.SandE_ReturnSizeE(); ++i) {
	org[k].SandE_InsertValueE(i,gstore.SandE_ReturnValueE(i));
      }
    }
    
    //////////////////////////////////////////////////////////////////////////
    // We now have the different names and positions of the Helix           //
    // numbers in the Helix numbering string and we have the                //
    // beginnings and ends of the stem strands in the sequences. The        //
    // next thing is to try to relate the helix numbering to the            //
    // varios stem strands.                                                 //
    //////////////////////////////////////////////////////////////////////////
    
    // Loop for assigning helix numbers to the stem strands for each
    // sequence.
    for(k=0; k < nr_orga - 1; ++k) {
      
      // Total number of stem strands.
      int nss = org[k].SandE_ReturnSizeS();
      
      // Total number of Helixes.
      int nh  = gstore.HelixNamesArray_ReturnSize(); 
      
      // Change the size of the Helix stem strand map matrix.
      gstore.HSSMMatrix_ChangeSize(nh,nss);
      
      // Reset the Helix stem strand map matrix to false.
      gstore.HSSMMatrix_SetWholeMatrixFalse();
      
      // Change the size of the helix mapping array and stem strand
      // mapping array.
      org[k].HelixMap_ChangeSize(nh);
      org[k].StemStrandMap_ChangeSize(nss);
            
      // If the number of helix numbers is the same as the number of
      // stem strands, then assign the helix numbers to the stem
      // strand names. If not, try to relate the Helix numbering to
      // the various stems strands. Includes also a test to see if the
      // number of stem strands in the sequence is a multiple of two.
      
      org[k].nss_not_a_multiple_of_two      = false;
      org[k].nss_larger_than_nh             = false;
      org[k].nss_smaller_than_nh            = false; 
      org[k].exclude_sequence               = false;
      org[k].helix_mapping_needed           = false;
      org[k].unequal_number_of_paired_bases = false;
            
      if(nss%2!=0) {  
	org[k].nss_not_a_multiple_of_two = true;
	org[k].exclude_sequence          = true;
      }
      if(nss == nh) {
	for(i=0; i < nh; i++) {
	  gstore.HSSMMatrix_SetMatrixEntryToTrue(i,i);
	  }
      } 
      else if(nss > nh) {
	org[k].nss_larger_than_nh   = true;
	org[k].exclude_sequence     = true;
      }
      else if(nss < nh) {
	org[k].nss_smaller_than_nh  = true;
	org[k].helix_mapping_needed = true;
      }
      
      // Try to relate the helix numbers to the various stem strands.
      if(org[k].helix_mapping_needed) {
	// Goes through all the helix numbers.
	for(int m=0; m<nh; m++) {
	  // Goes through all the stem strands in the sequence.
	  for(int n = 0; n < nss; n++) {
	    // Is the position of the Helix number between the two
	    // brackets '[' and ']' for a stem strand? If so, assign
	    // the Helix number to that stem strand. This might in
	    // some cases give zero or several helix numbers assigned
	    // to one stem strand.
	    if(org[k].SandE_ReturnValueS(n) - nr_sfhnnd <= 
	       gstore.HelixPosArray_ReturnValue(m) &&
	       org[k].SandE_ReturnValueE(n) + nr_sfhnnd >= 
	       gstore.HelixPosArray_ReturnValue(m)) {
	      gstore.HSSMMatrix_SetMatrixEntryToTrue(m,n);
	    }
	  }
	}
      }
	
      // Updates the array over the stem strands that have been
      // assign to a helix number.
      for(int m=0; m<nss; m++) {
	org[k].StemStrandMap_InsertMap(m,gstore.
				       HSSMMatrix_ReturnStemStrandMap(m)); 
      }
      // Updates the array over the Helix numbers that have been
      // assign to a stem strand.
      for(int m=0; m<nh; m++) {
	org[k].HelixMap_InsertMap(m,gstore.HSSMMatrix_ReturnHelixMap(m)); 
      }
	
      // gstore.HSSMMatrix_PrintMatrix();
      
      //First we load the sequence name into the global object
      gstore.name = org[k].name;
      
      // Defines some bool parameters to control the manipulation of
      // the HSSMMatrix
      bool nhnussb = true;  // Related to the nhnuss function.
      bool nussb   = true;  // Related to the nuss function.
      bool ussmb   = true;  // Related to the ussm function.
      
      // Runs a while loop to manipulate the HSSMMatrix. The loop stops
      // when there have not been a performed a change to the matrix.
      // START WHILE LOOP.
      while((nhnussb || nussb || ussmb) && org[k].helix_mapping_needed) {
	
	nhnussb = false;
	nussb   = false;
	ussmb   = false;
	
	/* ----------------------------------------------------------------- *
	 * If the number of Helixes under a stem strand is larger than       *
	 * one, then we tries to decide which Helix number to be used for    *
	 * that stem strand.                                                 * 
	 * ----------------------------------------------------------------- */
	nhnussb = nhnuss(nss, nh);
	
	// Updates the array over the stem strands that have been
	// assign to a helix number.
	for(int m=0; m<nss; m++) {
	  org[k].StemStrandMap_InsertMap(m,gstore.
					 HSSMMatrix_ReturnStemStrandMap(m)); 
	}
	// Updates the array over the Helix numbers that have been
	// assign to a stem strand.
	for(int m=0; m<nh; m++) {
	  org[k].HelixMap_InsertMap(m,gstore.HSSMMatrix_ReturnHelixMap(m)); 
	}
	
	/*
	  if(nhnussb) { 
	  std::cout << "nhnussb" << endl;
	  gstore.HSSMMatrix_PrintMatrix();
	  }   
	*/
		
	/* ----------------------------------------------------------------- *
	 * Are all stem strands assigned to a helix number? If not, can we   *
	 * change that by looking at the helix and stem strand mapping       *
	 * arrays?                                                           *
	 * ----------------------------------------------------------------- */

	if(org[k].StemStrandMap_FindNumberOfNonMapped() != 0) {  	    
	  // Goes through the stem strand map array to see if the
	  // number of non assigned stem strands between two assigned
	  // stem strands is the same as the number of unassigned
	  // Helix numbers. If so, uppdate the HSSMMatrix.
	  for(int m=0; m<nss; m++) {
	    int bnmss = 0;      // Begining of non mapped stem strands.
	    int enmss = 0;      // End      of non mapped stem strands.
	    
	    bnmss = org[k].StemStrandMap_FindFirstNonMapped(m);
	    enmss = org[k].StemStrandMap_FindLastNonMapped(bnmss); 
	    nussb = nuss(bnmss, enmss, nss);
	    
	    // Makes the counter jump to the next stem strand that
	    // have been mapped to a helix number.
	    if(m < enmss) {
	      m = enmss;
	    }
	  }	    
	}
	
	// Updates the array over the stem stands that have been
	// assign to a helix number.
	for(int m=0; m<nss; m++) {
	  org[k].StemStrandMap_InsertMap
	    (m,gstore.HSSMMatrix_ReturnStemStrandMap(m)); 
	}
	// Updates the array over the Helix numbers that have been
	// assign to a stem strand.
	for(int m=0; m<nh; m++) {
	  org[k].HelixMap_InsertMap(m,gstore.HSSMMatrix_ReturnHelixMap(m)); 
	}
	
	/*
	  if(nussb) { 
	  std::cout << "nussb" << endl;
	  gstore.HSSMMatrix_PrintMatrix();
	  } 
	*/
			
	/* ----------------------------------------------------------------- *
	 * Are all stem strands assigned to a helix number? If not, can we   *
	 * change that by looking at the helix numbering before and after    *
	 * the stem strand                                                   *
	 * ----------------------------------------------------------------- */
	if(org[k].StemStrandMap_FindNumberOfNonMapped() != 0) { 
	  
	  // Goes through the stem strand map array to see if the non
	  // assigned stem strand is positioned between Helixes where
	  // only one of them have the complementary Helix number
	  // assigned to a stem strand.
	  if(org[k].StemStrandMap_FindNumberOfNonMapped() > 0 &&
	     org[k].StemStrandMap_FindFirstNonMapped(0)   > 0 &&
	     org[k].StemStrandMap_FindFirstNonMapped(0)   < nss-1) { 
	    
	    int  ssbp    = 0;      // Stem strand begin position.
	    int  ssep    = 0;      // Stem strand end position.
	    int  FirstNonMapped = 0;
	    
	    FirstNonMapped = org[k].StemStrandMap_FindFirstNonMapped(0);
	    ssbp           = org[k].SandE_ReturnValueS(FirstNonMapped);
	    ssep           = org[k].SandE_ReturnValueE(FirstNonMapped);
	    ussmb = ussm(nh, ssbp, ssep, FirstNonMapped);
	  }
	}
	
	/*
	  if(ussmb) { 
	  std::cout << "ussm" << endl;
	  gstore.HSSMMatrix_PrintMatrix();
	  } 
	*/  
	
	// Updates the array over the stem stands that have been
	// assign to a helix number.
	for(int m=0; m<nss; m++) {
	  org[k].StemStrandMap_InsertMap(m,gstore.
					 HSSMMatrix_ReturnStemStrandMap(m)); 
	}
	// Updates the array over the Helix numbers that have been
	// assign to a stem strand.
	for(int m=0; m<nh; m++) {
	  org[k].HelixMap_InsertMap(m,gstore.HSSMMatrix_ReturnHelixMap(m)); 
	}
      } // END WHILE LOOP.
      
      /*
	gstore.HSSMMatrix_PrintMatrix();
	org[k].StemStrandMap_PrintMap();
	org[k].HelixMap_PrintMap();
      */
      
      /* =================================================================== *
       * Perform a test to see if all stem strands have been assigned to a   *
       * helix number and if only one helix number is assigned to each stem  *
       * strand.                                                             *
       * =================================================================== */
      for(int m = 0; m < nss; m++) {
	if(!org[k].StemStrandMap_ReturnMap(m)) {
	  org[k].exclude_sequence = true;
	  if(gstore.v_option) {
	    std::cout << endl << "WARNING!"<< endl 
		      << "\"" << org[k].name << "\": Stem strand number "
		      << m << " has not been assigned to to a Helix number."
		      << endl;
	  }
	}
	if(gstore.HSSMMatrix_ReturnStemStrandMapNumber(m)>1) {
	  org[k].exclude_sequence = true;
	  if(gstore.v_option) {
	    std::cout << endl << "WARNING!" << endl
		      << "\"" << org[k].name << "\": there are more than one " 
		      << "Helix number under stem strand nr. "
		      << m << "." << endl;
	  }
	}
      }
      
      //
      // Hopefully we now have a one to one mapping of the stem
      // strands to the Helix numbers. It is time to make the string
      // (paup) of the counted nucleotides that are going to be used
      // in the PAUP matrix.
      //
      if(!org[k].exclude_sequence) {
	int pos1 = 0, pos2 = 0, pos3 = 0, pos4 = 0, pos5 = 0;
	std::string unpaired, paired, bulged;
	int nr_unpaired = 0, nr_paired = 0, nr_bulged = 0;
	int y = 0, x = 0;
	
	// Goes through the Helix map array to see if the stem strand
	// has benn assigned to a Helig number. If there is a map,
	// then insert the three numbers (number of unpaired, paired
	// and bulged) from the modified sequence into the paup
	// string. If there is no map, then insert three zeros.
	for(x=0; x<nh; x++) {
	  if(org[k].HelixMap_ReturnMap(x) && y <nss) {
	    pos1 = org[k].modified.find_first_not_of(" ",pos5);
	    pos2 = org[k].modified.find_first_of("[",pos1);
	    pos3 = org[k].modified.find_first_of("{",pos2);
	    pos4 = org[k].modified.find_first_of("}",pos3);
	    pos5 = org[k].modified.find_first_of("]",pos4)+1;
	    
	    unpaired = org[k].modified.substr(pos1,pos2-pos1);
	    paired   = org[k].modified.substr(pos2+1,pos3-(pos2+1));
	    bulged   = org[k].modified.substr(pos3+1,pos4-(pos3+1));
	    
	    // From string to integer
	    nr_unpaired = atoi(unpaired.c_str());
	    nr_paired   = atoi(paired.c_str());
	    nr_bulged   = atoi(bulged.c_str());
	    
	    // Make shure we do not exceed the character states.
	    if(nr_unpaired > character_states - 1) {
	      nr_unpaired = character_states - 1;
	    }
	    if(nr_paired > character_states - 1) {
	      nr_paired = character_states - 1;
	    }
	    if(nr_bulged > character_states - 1) {
	      nr_bulged = character_states - 1;
	    }
	    
	    // From integer to char.
	    unpaired = in2ch(nr_unpaired);
	    paired   = in2ch(nr_paired);
	    bulged   = in2ch(nr_bulged);
	    
	    // Add the three numbers to the paup string
	    org[k].paup.append(unpaired);
	    org[k].paup.append(paired); 
	    org[k].paup.append(bulged);
	    
	    y++; // Number of stem strands.
	  }
	  else {
	    org[k].paup.append("000");
	  }
	  // After the last stem strand in the modified sequence there
	  // is a digit that gives the number of unpaired nucleotides
	  // behind the last stem strand. We have to get this number
	  // also.
	  if (x == nh -1) {
	    pos1 = org[k].modified.rfind(" ", org[k].modified.length());
	    unpaired = org[k].modified.substr(pos1+1,org[k].modified.length());
	    
	    nr_unpaired = atoi(unpaired.c_str());
	    
	    if(nr_unpaired > character_states - 1) {
	      nr_unpaired = character_states - 1;
	    }
	    unpaired = in2ch(nr_unpaired);
	    org[k].paup.append(unpaired);
	  }
	}
	
	// Modifies the names of the sequences. Are to be used in the
	// printing of the matrix.
	gstore.name = org[k].name;
	pname();
	org[k].paupname = gstore.name;
      }
      
      // A test to see of the length of the paup string is equal to
      // its theoretical value.
      if(org[k].paup.length() != 3*nh + 1) {
	org[k].exclude_sequence = true;
	if(gstore.v_option) {
	  std::cout << endl << "WARNING!"<< endl 
		    << "\"" << org[k].name << "\": Generation of the "
		    << "character states for the NEXUS file failed. "
		    << " The number of entries " 
		    << "differ from the number of helixes!" 
		    << endl << endl;
	}
      }
      
      // Goes through the paup strings to add together the number of
      // complementary numbers for each string. This loop allso
      // controlls that the number of paired nucleodies is equal in
      // the two strands of each stem.
      if(!org[k].exclude_sequence) {      
	std::string hnn; // The helix numbering name.
	int chnnp;       // Complementary Helix numbering name position.
	int nr_unpaired, nr_paired, nr_bulged;
	int nr_unpaired1, nr_paired1, nr_bulged1;
	int nr_unpaired2, nr_paired2, nr_bulged2;
	std::string unpaired, paired, bulged;
	
	for(int j=0; j < gstore.HelixNamesArray_ReturnSize(); j++) {
	  hnn   = gstore.HelixNamesArray_ReturnName(j);
	  if(hnn.find("'") == std::string::npos) {
	    chnnp = gstore.HelixNamesArray_FindComplementaryNamePos(hnn);
	    	    
	    // From string to integer
	    nr_unpaired1 = ch2in(org[k].paup[j*3 + 0]);
	    nr_paired1   = ch2in(org[k].paup[j*3 + 1]);
	    nr_bulged1   = ch2in(org[k].paup[j*3 + 2]);

	    // From string to integer
	    nr_unpaired2 = ch2in(org[k].paup[chnnp*3 + 0]);
	    nr_paired2   = ch2in(org[k].paup[chnnp*3 + 1]);
	    nr_bulged2   = ch2in(org[k].paup[chnnp*3 + 2]);

	    // Add the two numbers together
	    nr_unpaired = nr_unpaired1 + nr_unpaired2;
	    nr_paired   = nr_paired1   + nr_paired2;
	    nr_bulged   = nr_bulged1    + nr_bulged2;

	    // Make shure we do not exceed the character states.
	    if(nr_unpaired > character_states - 1) {
	      nr_unpaired = character_states - 1;
	    }
	    if(nr_paired > character_states - 1) {
	      nr_paired = character_states - 1;
	    }
	    if(nr_bulged > character_states - 1) {
	      nr_bulged = character_states - 1;
	    }
	    
	    // From integer to char.
	    unpaired = in2ch(nr_unpaired);
	    paired   = in2ch(nr_paired);
	    bulged   = in2ch(nr_bulged);
	    
	    // Add the three numbers to the new paup string
	    org[k].paupadded.append(unpaired);
	    org[k].paupadded.append(paired); 
	    org[k].paupadded.append(bulged);
	    
	    // Add the character labels to the string
	    org[k].paupadded_characther_labels.append(" u");
	    org[k].paupadded_characther_labels.append(hnn);
	    org[k].paupadded_characther_labels.append(", s");
	    org[k].paupadded_characther_labels.append(hnn);
	    org[k].paupadded_characther_labels.append(", b");
	    org[k].paupadded_characther_labels.append(hnn);
	    org[k].paupadded_characther_labels.append(",");
	    
	    // Runs the test to see if the the two numbers of paired
	    // bases are equal for this stem. If not, print a warning.
	    if(nr_paired1 != nr_paired2) {
	      org[k].unequal_number_of_paired_bases = true;
	      if(gstore.v_option) {
		std::cout << endl << "WARNING!"<< endl 
			  << "\"" << org[k].name << "\": The number of paried "
			  << "bases in the stem strand assigned to \""
			  << hnn <<"\" differs from the number of paried "
			  << "bases in the stem strand assigned to \""
			  << hnn <<"'\""
			  << endl << endl;
		std::cout << org[k].HelixMap_ReturnMap(j) << " " 
			  << org[k].HelixMap_ReturnMap(chnnp)
			  << endl;
	      }
	    }
	  }
	}
	// Have to remember the number of unpaired nucleotides after
	// the last stem strand.
	org[k].paupadded.append(org[k].paup.substr(org[k].paup.length()-1,1));
	org[k].paupadded_characther_labels.append(" u");
	    
      }
    } // END FOR EACH SEQUENCE
    
    
    

    /* ===================================================================== *
     * Writes a summary to standard out                                      *
     * ===================================================================== */
    std::cout << endl << " SUMMARY:" << endl << endl << " ";
    std::cout << std::setiosflags(std::ios::left) 
	      << std::setw(3)  << "A" 
	      << std::setw(3)  << "B" 
	      << std::setw(3)  << "C"
	      << std::setw(3)  << "D"
      	      << std::setw(3)  << "E"
      	      << std::setw(7) << "Status"
	      << "Sequence name" << endl << endl;
    for(k=0; k < nr_orga - 1; ++k) {
      std::string A, B, C, D, E, Status;
      if(org[k].nss_larger_than_nh)            { A = "Y"; } else {A = "N"; } 
      if(org[k].nss_smaller_than_nh)           { B = "Y"; } else {B = "N"; }
      if(org[k].helix_mapping_needed)          { C = "Y"; } else {C = "N"; }
      if(org[k].nss_not_a_multiple_of_two)     { D = "Y"; } else {D = "N"; }
      if(org[k].unequal_number_of_paired_bases){ E = "Y"; } else {E = "N"; }
      if(org[k].exclude_sequence){ Status = "FAILED";} else {Status= "OK";}
      
      std::cout << " "
		<< std::setw(3) << A
		<< std::setw(3) << B
		<< std::setw(3) << C
		<< std::setw(3) << D
		<< std::setw(3) << E
		<< std::setw(7) << Status
		<< org[k].name << endl;
    }
    std::cout << endl
	      << " A: The number of stem strands is larger than the number "
	      << "of Helixes." << endl
	      << " B: The number of stem strands is smaller than the number " 
	      << "of Helixes." << endl
	      << " C: Marten have tried to relate Helix "
	      << "strands to stem strands." << endl
	      << " D: The number of stem strands is not a multiple of two."
	      << endl
	      << " E: The number of paired nucleotides differs in a stem." 
	      << endl
	      << " N: No" << endl
	      << " Y: Yes" << endl;

    
  } // END MAKING OF PAUP STRING.
  
      
  //########################################################################// 
  //########################################################################// 
  //##########                                                   ###########// 
  //##########                      START                        ###########// 
  //##########                                                   ###########// 
  //##########              WRITING MARTEN FILE                  ###########//
  //##########                                                   ###########// 
  //########################################################################// 
  //########################################################################// 
  
  
  std::string str_fnma;      // The name of the file in marten format
  int         nr_seqna = 0;  // Length of sequence name.

  // Short test to find the longest sequence name. Used to adjust file
  // output.
  for(i=0; i<nr_orga-1; ++i) {
    if(org[i].paupname.length() > nr_seqna) {
      nr_seqna = org[i].name.length(); 
    }
  }
      
  str_fnma = outfile_name; 
  str_fnma.append(".mar");
  // Opens the file for writing
  ofstream ofileno(str_fnma.c_str(), ios::out); 
  
  ofileno << "MARTEN v. 0.1 @2007." << endl;
  ofileno << "Sequences (total) : " << nr_orga - 1 << endl << endl;
  // This loop goes through the the organisms.
  for(k=0; k < nr_orga - 1; ++k) {
    ofileno << std::setiosflags(std::ios::left)
	  << std::setw(nr_seqna+2) << org[k].name << org[k].modified << endl;
  }
  
  // Closes the output file.
  ofileno.close();  
  
  
  
  //########################################################################// 
  //########################################################################// 
  //##########                                                   ###########// 
  //##########                      START                        ###########// 
  //##########                                                   ###########// 
  //##########               WRITING NEXUS FILE                  ###########//
  //##########                                                   ###########// 
  //########################################################################// 
  //########################################################################// 
  
  if(gstore.P_option) {
    int nr_char = 0;
    
    std::string str_fnne; // The name of the file in the nexus format
    
    // Number of nucleotides pr. line.
    int nr_nnpl  = 50; 
    int nr_nnpl2 = 50; 
    // Just a counter.
    int j       = 0; 
    int l       = 0; 
    int n       = 0; 
    // One matrix dimension.
    int nr_leng = (gstore.HelixNamesArray_ReturnSize()*3)+1;

    str_fnne = outfile_name; 
    str_fnne.append(".nex");
    
    // Opens the file for writing
    ofstream ofilene(str_fnne.c_str(), ios::out); 
    
    ofilene << "#NEXUS" << endl << endl
	    << "[!Data from : Marten V. 0.1 2007]"   
	    << endl << endl;

    ofilene << "BEGIN DATA;" << endl 
	    << "DIMENSIONS  NTAX=" << nr_orga -1 
	    << " NCHAR="    << gstore.HelixNamesArray_ReturnSize()*3 +1
	    << ";" << endl
	    << "FORMAT SYMBOLS = ";
    
    // Small check to see what to print in the file.
    if(character_states <= 10) {
      ofilene << "\"0~"
	      << character_states - 1
	      << "\" INTERLEAVE;" << endl;
    }
    if(character_states > 10 && character_states <= 36) {
      ofilene << "\"0~9A~"
	      << in2ch(character_states - 1)
	      << "\" INTERLEAVE;" << endl;
    }
    if(character_states > 36 && character_states <= 62) {
      ofilene << "\"0~9A~Za~"
	      << in2ch(character_states - 1)
	      << "\" INTERLEAVE;" << endl;
    }
    if(character_states > 62 && character_states <= 63) {
      ofilene << "\"0~9A~Za~z"
	      << in2ch(character_states - 1)
	      << "\" INTERLEAVE;" << endl;
    }
    if(character_states > 63 && character_states <= 64) {
      ofilene << "\"0~9A~Za~z@"
	      << in2ch(character_states - 1)
	      << "\" INTERLEAVE;" << endl;
    }
    ofilene << "CHARSTATELABELS  " << endl;   
    
    std::string nex;
    std::string nnex;
            
    nex = org[nr_orga - 1].modified;
    ofilene << std::setiosflags(std::ios::right);

    // Short test to find the longest sequence name to adjust file output.
    for(k=0; k<nr_orga-1; ++k) {
      if(org[k].paupname.length() > nr_seqna && !org[k].exclude_sequence) {
	nr_seqna = org[k].paupname.length(); 
      }
    }
    
    // Makes a string with all the charater labels. The labels are
    // seperated by a ",".
    std::string charlab;
    for(i=0; i <= gstore.HelixNamesArray_ReturnSize(); ++i) {
      if(i < gstore.HelixNamesArray_ReturnSize()) {
	charlab = gstore.HelixNamesArray_ReturnName(i);
	// Does the Helix number have a "'"?
	if(charlab.find("'",0)!= std::string::npos){
	  charlab.insert(0,"  ");
	  charlab.append("'',");
	  charlab.replace(0,2,"'u");
	  gstore.CharacterLabels.append(charlab);
	  charlab.replace(0,2,"'s");
	  gstore.CharacterLabels.append(charlab);
	  charlab.replace(0,2,"'b");
	  gstore.CharacterLabels.append(charlab);
	}
	else {
	  charlab.insert(0,"  ");
	  charlab.append(",");
	  charlab.replace(0,2," u");
	  gstore.CharacterLabels.append(charlab);
	  charlab.replace(0,2," s");
	  gstore.CharacterLabels.append(charlab);
	  charlab.replace(0,2," b");
	  gstore.CharacterLabels.append(charlab);
	}
      }
      // We must remember the last number, which gives the number of
      // unpaired behined the last stem strand.
      else {
	gstore.CharacterLabels.append(" u");
      }
    }
    
    // Prints the character states to the NEXUS file
    int pos1=0;
    int pos2=0;
    int nrcl=0;
    while(nrcl < (gstore.HelixNamesArray_ReturnSize()*3)+1) {
      if(gstore.CharacterLabels.find_first_of(",",pos1) != std::string::npos) {
	pos2 = gstore.CharacterLabels.find_first_of(",",pos1);
      }
      // Have to remember the last number again
      else {
	pos2 = gstore.CharacterLabels.length();
      }
      nrcl++;
      ofilene  << std::setw(12) << nrcl << " "
	       << gstore.CharacterLabels.substr(pos1,pos2-pos1+1) 
	       << endl;
      pos1 = pos2+1;
    }
    
    // Adjusts the formated output.
    ofilene << std::resetiosflags(std::ios::right);
    ofilene << std::setiosflags(std::ios::left);
    
    ofilene << "     ;" << endl;
    
    nr_char = (gstore.HelixNamesArray_ReturnSize()*3)+1;
    
    ofilene << std::resetiosflags(std::ios::right);
    ofilene << std::setiosflags(std::ios::left);
            
    ofilene << endl << "MATRIX" << endl << endl;
    
    // This loop goes through the organisms.
    for(i=0; i < nr_char; ++i) {
      if((i == nr_nnpl * j) && (nr_nnpl * j < nr_leng)) {
	ofilene << std::setw(nr_seqna+2) << " " 
		<< "[         " ;
	n = 1;
	for(l=1; l<=nr_nnpl; l++) {
	  if(l == n*10) {
	    ofilene << std::setw(10) << j*nr_nnpl+l ;
	    ++n;
	  }
	}
	ofilene << "]" << endl;
	
	ofilene << std::setw(nr_seqna+2) << " " 
		<< "[         ";
	n = 1;
	for(l=0; l<=nr_nnpl; l++) {
	  if(l == n*10) {
	    ofilene << std::setw(10) << "." ;
	    ++n;
	  }
	}
	ofilene << "]" << endl;
	for(k=0; k < nr_orga - 1; ++k) {
	  if(!org[k].exclude_sequence) {
	    // Make sure we don't passes the size of the arrays when
	    // writing the caracter states.
	    if(nr_nnpl * j + nr_nnpl -1 > org[k].paup.length()) {
	      nr_nnpl2 = org[k].paup.length()-(nr_nnpl * j);
	    }
	    else {
	      nr_nnpl2 = nr_nnpl;
	    }
	    ofilene << std::setw(nr_seqna+3) << org[k].paupname 
		    << org[k].paup.substr(nr_nnpl * j, nr_nnpl2) 
		    << endl;
	  }
	}
	ofilene << endl;
	++j;
      }
    }
    ofilene << "    ;" << endl
	    << "end;"  << endl;
    
    
    /* ===================================================================== *
     * This part of the code writes MIN and MAX values for each columb       *
     * in the matrix.                                .                       *
     * ===================================================================== */
    std::string paupmin;
    std::string paupmax;
    std::string ma;
    std::string mi;
    std::string ss2;
    int max = 0;
    int min = 0;
    
    if(gstore.l_option) {
      for(k=0; k < (gstore.HelixNamesArray_ReturnSize()*3 + 1); ++k) {
	max = ch2in(org[0].paup[k]);
	min = ch2in(org[0].paup[k]);
	for(i=0; i < nr_orga-1; ++i) {
	  if (ch2in(org[i].paup[k]) > max &&
	      !org[i].exclude_sequence) {
	    max = ch2in(org[i].paup[k]);
	  }
	  if (ch2in(org[i].paup[k]) < min &&
	      !org[i].exclude_sequence) {
	    min = ch2in(org[i].paup[k]);
	  }
	}
	ma = in2ch(max);
	mi = in2ch(min);
	paupmin.append(mi);
	paupmax.append(ma);
      }
    
      // Writes the MIN values for each colum in the matrix to the nex file.
      ofilene << endl << endl;
      ofilene << "[ MIN";
      j = 0;
      for(i=0; i < paupmin.length(); ++i) {
	if(i == j*5) { 
	  ofilene << endl << "              ";
	  j++;
	}
	std::stringstream ss1;
	ss1 << i;
	ss2 = ss1.str();
	if(i == paupmin.length() - 1) {
	  ss2 = ss2.append(";");
	}
	else {
	  ss2 = ss2.append(",");
	}
	
	ofilene << paupmin.substr(i,1) << ":" 
		<< std::setw(8) << ss2;
      }
      ofilene << endl << std::setw(65) << "" << "]" << endl ;
      
      // Writes the MAX values for each colum in the matrix to the nex file.
      ofilene << endl << endl;
      ofilene << "[ MAX";
      j = 0;
      for(i=0; i < paupmin.length(); ++i) {
	if(i == j*5) { 
	  ofilene << endl << "              ";
	  j++;
	}
	std::stringstream ss1;
	ss1 << i;
	ss2 = ss1.str();
	if(i == paupmin.length() - 1) {
	  ss2 = ss2.append(";");
	}
	else {
	  ss2 = ss2.append(",");
	}
	
	ofilene << paupmax.substr(i,1) << ":" 
		<< std::setw(8) << ss2;
      }
      ofilene << endl << std::setw(65) << "" << "]" << endl << endl;
      
    }


    /* ===================================================================== *
     * This part of the code writes out the flipped paup matrix.             *
     *                                                                       *
     * ===================================================================== */
    if(gstore.r_option) {
      std::string  P[nr_orga-1];
      int psne = 0; // The number of PAUP strings that is not excluded.
      nrcl  = 0;
      pos1  = 0;
      pos2  = 0;
      
      for(k=0; k < nr_orga-1; k++) {
	if(!org[k].exclude_sequence) {
	  P[psne] = org[k].paup;
	  psne++;
	}
      }
      ofilene << "[ FLIPPED MATRIX" << endl;
      for(i=0;  i < (gstore.HelixNamesArray_ReturnSize()*3)+1; i++) {
	if(nrcl < (gstore.HelixNamesArray_ReturnSize()*3)+1) {
	  if(gstore.CharacterLabels.find_first_of(",",pos1) 
	     != std::string::npos) {
	    pos2 = gstore.CharacterLabels.find_first_of(",",pos1);
	  }
	  // Have to remember the last number again
	  else {
	    pos2 = gstore.CharacterLabels.length();
	  }
	  nrcl++;
	  ofilene  << std::setw(13) 
		   << gstore.CharacterLabels.substr(pos1,pos2-pos1)
		   << " ";
	  pos1 = pos2+1;
	}
	for(int j=0; j < psne; j++) {
	  ofilene  << P[j].substr(i,1);
	}
	ofilene  << endl;
      }
      ofilene << std::setw(13) << " "<< "]" << endl << endl;
    }
    

    /* ===================================================================== *
     * This part of the code writes a new matrix which contains the number   *
     * of paired, unpaired and bulged nucleotides related to a stem.         *
     * ===================================================================== */
    
    if(gstore.R_option) {
      std::string  Padded[nr_orga-1];
      std::string  PCharacterLabels;
      int psne = 0; // The number of PAUP strings that is not excluded.
      nrcl  = 0;
      pos1  = 0;
      pos2  = 0;
      
      for(k=0; k < nr_orga-1; k++) {
	if(!org[k].exclude_sequence) {
	  Padded[psne]     = org[k].paupadded;
	  PCharacterLabels = org[k].paupadded_characther_labels;
	  psne++;
	}
      }
      
      ofilene << "[ FLIPPED MATRIX: COMPLEMENTARY NUMBERS ADDED" << endl;
      for(i=0;  i < (gstore.HelixNamesArray_ReturnSize()*3/2)+1; i++) {
	if(PCharacterLabels.find_first_of(",",pos1) 
	   != std::string::npos) {
	  pos2 = PCharacterLabels.find_first_of(",",pos1);
	}
	// Have to remember the last number again
	else {
	  pos2 = PCharacterLabels.length();
	}
	ofilene  << std::setw(13) 
		 <<  PCharacterLabels.substr(pos1,pos2-pos1)
		 << " ";
	pos1 = pos2+1;
      
	for(j=0; j < psne; j++) {
	  ofilene  << Padded[j].substr(i,1);
	}
	ofilene  << endl;
      }
      ofilene << std::setw(13) << " "<< "]" << endl ;
    }
    
    
    ofilene << endl;
    // Closes the output file.
    ofilene.close();  
  }
  
 
  //########################################################################// 
  //########################################################################// 
  //##########                                                   ###########// 
  //##########                      START                        ###########// 
  //##########                                                   ###########// 
  //##########    VIENNA/FASTA and "RNAfold constrain" FORMAT    ###########//
  //##########                                                   ###########// 
  //########################################################################// 
  //########################################################################// 
  
  if(gstore.P_option) {
    
    int nr_ss             = 0;    // Number of stem strands.  (Sequense)
    int nr_hs             = 0;    // Number of helix strands. (Helix numbering)
    std::string str_bases = bases;
    std::string bracket;
    std::string sequence;
    bool inside           = false;
  
    for(k=0; k < nr_orga-1; k++) {
      
      if(!org[k].exclude_sequence) {
	gstore.sequence = org[k].sequence;
	
	// Removes nonstandard pairing info from the sequence. Nucleotides
	// inside brackets.
	inside = false;
	for(i=0; i < gstore.sequence.length(); i++) {
	  if(gstore.sequence.compare(i,1,"(") == 0 ||
	     gstore.sequence.compare(i,1,")") == 0) {
	    gstore.sequence.replace(i,1," ");
	  }
	}
	
	// Changes unpaired in stems "{}" into dots and remove the curled
	// brackets.
	inside  = false;
	
	for(i=0; i < gstore.sequence.length(); i++) {
	  if(gstore.sequence.compare(i,1,"{") == 0) {
	    gstore.sequence.replace(i,1," ");
	    inside = true;
	  }
	  if(gstore.sequence.compare(i,1,"}") == 0) {
	    gstore.sequence.replace(i,1," ");
	    inside = false;
	  }
	  if(inside && 
	     str_bases.find(gstore.sequence[i],0) != std::string::npos) {
	    gstore.sequence.replace(i,1,".");
	  }
	}
	
	
	// Changes unpaired outside stem strands "[]" into dots.
	inside  = false;
	
	for(i=0; i < gstore.sequence.length(); i++) {
	  if(gstore.sequence.compare(i,1,"[") == 0) {
	    inside = true;
	  }
	  if(gstore.sequence.compare(i,1,"]") == 0) {
	    inside = false;
	  }
	  if(!inside && 
	     str_bases.find(gstore.sequence[i],0) != std::string::npos) {
	    gstore.sequence.replace(i,1,".");
	  }
	}
	
	// Changes the paired nucleotides into brackets
	nr_ss   = 0;
	nr_hs   = 0;
	inside  = false;
	
	for(i=0; i < gstore.sequence.length(); i++) {
	  if(gstore.sequence.compare(i,1,"[") == 0 ||
	     gstore.sequence.compare(i,1,"^") == 0) {
	    gstore.sequence.replace(i,1," ");
	    inside = true;
	    nr_hs = org[k].HelixMap_FindFirstMapped(nr_hs);
	    if(gstore.HelixNamesArray_ReturnName(nr_hs).find("'",0) == 
	       std::string::npos) {
	      bracket = "(";
	    }
	    else {
	      bracket = ")";
	    }
	    
	    nr_ss++;
	    nr_hs++;
	  }
	  if(gstore.sequence.compare(i,1,"]") == 0) {
	    gstore.sequence.replace(i,1," ");
	    inside = true;
	  }
	  if(inside && 
	     str_bases.find(gstore.sequence[i],0) != std::string::npos) {
	    gstore.sequence.replace(i,1,bracket);
	  }
	}
	
	org[k].fasta_bracket = gstore.sequence;
	
	gstore.sequence = org[k].sequence;
	// Removes all secondary structure information. But the
	// alignment is kept.
	for(i=0; i < gstore.sequence.length(); i++) {
	  if(gstore.sequence.compare(i,1,"(") == 0 ||
	     gstore.sequence.compare(i,1,")") == 0 ||
	     gstore.sequence.compare(i,1,"{") == 0 ||
	     gstore.sequence.compare(i,1,"}") == 0 ||
	     gstore.sequence.compare(i,1,"[") == 0 ||
	     gstore.sequence.compare(i,1,"]") == 0 ||
	     gstore.sequence.compare(i,1,"^") == 0) {
	    gstore.sequence.replace(i,1," ");
	  }
	}
	org[k].fasta_sequence = gstore.sequence;
      }
    }
    

    // Removes the alignment and creates sequences with only the
    // nucleotides or only the dots and brackets.
    for(k=0; k < nr_orga - 1; ++k) {
      if(!org[k].exclude_sequence) {
	
	sequence.clear();
	for(i=0; i < org[k].fasta_sequence.length(); i++) {
	  if(org[k].fasta_sequence.compare(i,1," ") != 0 &&
	     org[k].fasta_sequence.compare(i,1,"-") != 0) {
	    sequence.append(org[k].fasta_sequence.substr(i,1));
	  }
	}
	org[k].fasta_sequence = sequence;
	
	bracket.clear();
	for(i=0; i < org[k].fasta_bracket.length(); i++) {
	  if(org[k].fasta_bracket.compare(i,1," ") != 0 &&
	     org[k].fasta_bracket.compare(i,1,"-") != 0) {
	    bracket.append(org[k].fasta_bracket.substr(i,1));
	  }
	}
	org[k].fasta_bracket = bracket;
	
      } 
    }
    
    /* ===================================================================== *
     * Writes the file in FASTA format.                                      *
     *                                                                       *
     * ===================================================================== */
    std::string str_fnfa;  // The name of the file in FASTA format
    
    str_fnfa = outfile_name; 
    str_fnfa.append(".fas");
    // Opens the file for writing
    ofstream fastafile(str_fnfa.c_str(), ios::out); 
    
    // This loop goes through the the organisms.
    for(k=0; k < nr_orga - 1; ++k) {
      if(!org[k].exclude_sequence) {
	fastafile << ">" <<  org[k].name          << endl
		  << org[k].fasta_sequence << endl
		  << org[k].fasta_bracket  << endl ;
      }
    }
    // Closes the output file.
    fastafile.close();  
    
    
    // Replaces "." with "x" to symbolize unpaired.
    // Goes through the the organisms.
    for(k=0; k < nr_orga - 1; ++k) {
      if(!org[k].exclude_sequence) {
	org[k].RNAfold_constrain = org[k].fasta_bracket;
	for(i=0; i < org[k].RNAfold_constrain.length(); i++) {
	  if(org[k].RNAfold_constrain.compare(i,1,".") == 0) {
	    org[k].RNAfold_constrain.replace(i,1,"x");
	  }
	}
      } 
    }
    
    /* ===================================================================== *
     * Writes the file to be used with RNAfold constrain the structure       * 
     *                                                                       *
     * ===================================================================== */
    std::string str_fnrnaf; // The name of the file in RNAfold constrain format
    str_fnrnaf = outfile_name; 
    str_fnrnaf.append(".con");
    // Opens the file for writing
    ofstream constrainfile(str_fnrnaf.c_str(), ios::out); 
    
    // This loop goes through the the organisms.
    for(k=0; k < nr_orga - 1; ++k) {
      if(!org[k].exclude_sequence) {
	constrainfile << ">" <<  org[k].name      << endl
		      << org[k].fasta_sequence    << endl
		      << org[k].RNAfold_constrain << endl ;
      }
    }
    // Closes the output file.
    constrainfile.close();  
  
  }
  
  // Notify the user that the program has ended.
  if(gstore.v_option) { 
    std::cout << "\n\n Done ! \n\n";
  }
  return(0);
}
