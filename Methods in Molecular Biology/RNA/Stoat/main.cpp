/*****************************************************************************
 *                                                                           *
 *                         ___  ____   __    __   ____                       *
 *                        / __)(_  _) /  \  (  ) (_  _)                      *
 *                        \__ \  )(  ( () ) /__\   )(                        *
 *                        (___/ (__)  \__/ (_)(_) (__)                       *
 *                      ---------------------------------                    *
 *                                                                           *
 *                                                                           *
 * 	   To install STOAT, please see the makefile in this directory.	    	 *
 *									    									 *
 * STOAT is written by Dr. Vegeir Knudsen and uses the RNAfold program from  *
 * the Vienna-package to generate four statistical values. The               *
 * result are presented as a PAUP matrix in a nexus formated file. The       *
 * statistical values are :                                                  *
 *                                                                           *
 * [F] - the Forbinus norm.                                                  *
 * [Q] - the Shannon entropy.                                                *
 * [P] - the base-paring propensity                                          *
 * [S] - the mean stem length                                                *
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
 *                                                           STOAT 2007-2025 *
 *****************************************************************************/

// System included files.
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <getopt.h>

// User included files.
#include "class.h"     // Defines the two classes in this program.
#include "phelp.h"     // Prints help to standard out.
#include "in2ch.h"     // Transforms an integer to a char.
#include "intvalue.h"  // Returns the char-integer value of the float.
#include "organism.h"
#include "class.h"

// Start of main program.
int main(int argc, char* argv[]) {
    /* ====================================================================== *
     *  Input Parameters                                                      *
     * ====================================================================== */
    int c;
    int MAX_ORG = 0;            // Max amount of input lines.
    int character_states = 31;  // The default number of character states is 31 if not specified
                                // differently by the user with the -s option.

    char* infile_name;
	char* outfile_name;
	std::string input_dir;  // Add this line to store the directory path

    char defaultinfile[11] = "infile.txt";
    char defaultoutfile[12] = "outfile";

    bool v_option = false;
    bool f_option = false;
    (void)f_option; // Suppresses unused variable warning
    bool t_option = false;
    bool r_option = false;

    std::string cmd = "-p -d2 --noLP"; // Command arguments passed to RNAfold.
    std::string buffer;               // Input buffer used by ifstream.


	// Initialize input and output file names
	infile_name = defaultinfile;
	outfile_name = defaultoutfile;

	// Process command-line options
	while ((c = getopt(argc, argv, "rthvi:o:x:f:")) != -1) {
    	switch (c) {
        	case 'i': {
            	infile_name = optarg;  // Update infile_name with the custom path
            	break;
        	}
        	case 'o': {
            	outfile_name = optarg;
            	break;
        	}
        	case 'v': {
            	v_option = true;
            	break;
        	}
        // Handle other options...
    	}
	}

	// Extract the directory path from the input file name
	input_dir = std::filesystem::path(infile_name).parent_path().string();
	if (input_dir.empty()) {
    	input_dir = ".";  // Set to current directory if input_dir is empty
	}

	// Debug prints
	std::cout << "Input file: " << infile_name << std::endl;
	std::cout << "Input directory: " << input_dir << std::endl;
    
    
    
    // Small check to control the number of input arguments.
    if (argc < 2) {
        std::cerr << "\n Stoat needs at least one input option!" << std::endl;
        std::cerr << " Try -h for help.\n" << std::endl;
        return EXIT_FAILURE;
    }

    while ((c = getopt(argc, argv, "rthvi:o:x:f:")) != -1) {
        switch (c) {
        case 'h': {
            phelp();
            return 0;
        }
        case 'i': {
            infile_name = optarg;
            break;
        }
        case 'o': {
            outfile_name = optarg;
            break;
        }
        case 'x': {
            character_states = atoi(optarg);
            if (character_states > 64 || character_states < 1) {
                std::cerr << "\nERROR! Number of character states must be between 1 and 64!\n" << std::endl;
                return EXIT_FAILURE;
            }
            break;
        }
        case 'f': {
            f_option = true;
            cmd = optarg;
            break;
        }
        case 't': {
            t_option = true;
            break;
        }
        case 'r': {
            r_option = true;
            break;
        }
        case 'v': {
            v_option = true;
            break;
        }
        }
    }

    // Gives the user some feedback on the chosen input parameters.
    if (v_option) {
    std::cout << "\n\n                  STOAT\n";
    std::cout << "                  -----\n";
    std::cout << std::endl;
    std::cout << " -i    : Input  file is \"" << infile_name << "\"" << std::endl;
    std::cout << " -o    : Output files are \"" << outfile_name << "_l.nex\" and \"" << outfile_name << "_g.nex\""
              << "\"" << std::endl;
    std::cout << " -s    : Number of character states are \"" << character_states << "\"" << std::endl;
    std::cout << " -r    : Arguments to be used with RNAfold are \"" << cmd << "\"" << std::endl;
    std::cout << "\n";
}
    
        /* ====================================================================== *
     *  Modify the sequence names in the infile and writes it to a new        *
     *  file with the sequence.                                               *
     * ====================================================================== */

    // Opens the input file.
    std::ifstream infile(infile_name, std::ios::in);
    if (!infile.is_open()) {
        std::cerr << "Error opening file: " << infile_name << "\n\n";
        return EXIT_FAILURE;
    }

    // Gets the number of nonempty lines in the input file.
    while (!infile.eof()) {
        while (std::getline(infile, buffer)) {
            if (buffer != "") ++MAX_ORG;
        }
    }
    infile.close();

    //
    // We now have the number of nonempty lines in the infile. It is
    // time to get the actual sequence names.
    //

    int nr_orga = 0;
    int i = 0;
    std::string string_name_suffix;
    std::string new_string_name = "0000000000";

    // Defines the ORGANISM class array.
    class organism org[MAX_ORG];

    // Opens the input file for reading one more time. (New file handle.)
    std::ifstream is(infile_name, std::ios::in);
    if (!is.is_open()) {
        std::cerr << "Error opening file: " << infile_name << "\n\n";
        return EXIT_FAILURE;
    }

    // Opens the modified RNAfold input file for writing.
	std::string modified_infile_name = input_dir + "/m_" + std::string(std::filesystem::path(infile_name).filename().string());
	std::ofstream minfile(modified_infile_name.c_str(), std::ios::out);

    while (!is.eof()) {
        // Gets a new line from the input file.
        while (std::getline(is, buffer)) {
            // Looks for lines starting with a ">". These gives the sequence names.
            if (buffer.substr(0, 1) == ">") {
                // Removes any white spaces in front of the sequence name.
                org[nr_orga].name =
                    buffer.substr(buffer.find_first_not_of("> "),
                        buffer.find_last_not_of(" ") -
                        buffer.find_first_not_of("> ") + 1);

                // Creates new sequence names for the new input file.
                // Have to convert the integer 'nr_orga+1' to a string.
                std::stringstream ss;
                ss << nr_orga + 1;
                string_name_suffix = ss.str();
                new_string_name.replace(new_string_name.length() -
                    string_name_suffix.length(),
                    string_name_suffix.length(),
                    string_name_suffix);
                // inserts the string version of the integer into the modified
                // string name.
                org[nr_orga].rnafold_name = new_string_name;
                minfile << ">" << org[nr_orga].rnafold_name << std::endl;
                i = 0;
                ++nr_orga;
            }
            else {
                minfile << buffer << std::endl;
            }
            if (t_option &&
                buffer.find("(") != std::string::npos &&
                buffer.find(")") != std::string::npos) {
                org[nr_orga - 1].parenthesis_user = buffer;
            }
            i++;
        }
    }
    // Closes the reading handle to the original input file.
    is.close();
    // Closes the writing handle to the modified RNAfold input file.
    minfile.close();

    // Short test to find the longest sequence name to adjust file output.
    int nr_slen = 0;
    for (i = 0; i < nr_orga; ++i) {
        if (org[i].name.length() > nr_slen) {
            nr_slen = org[i].name.length();
        }
    }

    // Gives the user some feedback on the name change.
    if (v_option) {
  std::cout << " Created a new RNAfold input file."
        << " Sequence names are changed to:" << std::endl;
    for (i = 0; i < nr_orga; ++i) {
        std::cout << std::setiosflags(std::ios::left)
            << " " << std::setw(nr_slen) << org[i].name
            << " -> " << org[i].rnafold_name << std::endl;
    }
}

    /* ====================================================================== *
     *  Run RNAfold                                                           *
     * ====================================================================== */

    // Modifies the command string.
	cmd = "cd " + input_dir + " && RNAfold " + cmd + " < " + std::filesystem::path(modified_infile_name).filename().string() + " > RNAfold.txt";

	
    // Print a message to standard out.
    if (v_option) {
    std::cout << std::endl
        << " Running RNAfold: "
        << std::endl
        << " \" " << cmd << "\" ...";
}

// Runs the command.
system(cmd.c_str());

// Prints a completion message.
if (v_option) {
    std::cout << " Done!" << std::endl;
}

    /* ====================================================================== *
     *  Processing the RNAfold results.                                       *
     * ====================================================================== */

    int nr_orga2 = 0;

    // Opens the RNAoutput.txt file and places the result in the objects.
    std::ifstream RNAfold((input_dir + "/RNAfold.txt").c_str(), std::ios::in);
    if (!RNAfold.is_open()) {
        std::cerr << "Error opening file: RNAfold.txt \n\n";
        return EXIT_FAILURE;
    }

    while (!RNAfold.eof()) {
        // Gets a new line from the input file.
        while (std::getline(RNAfold, buffer)) {
            if (buffer != "") {
                if (buffer.substr(0, 1) == ">") {
                    // Test if the two modified sequence names do match.
                    if (org[nr_orga2].rnafold_name.compare
                    (buffer.substr(buffer.find_first_not_of("> "),
                        buffer.find_last_not_of(" ") -
                        buffer.find_first_not_of("> ") + 1)) != 0) {
                        std::cerr << std::endl
                            << "WARNING! Mismatch between sequence names in: "
                            << "RNAfold.txt and " << modified_infile_name
                            << "." << std::endl;
                    }
                    i = 0;
                }
                if (i == 1) {
                    org[nr_orga2].sequence = buffer.substr(0, buffer.find(" ", 0));
                }
                if (i == 2) {
                    org[nr_orga2].parenthesis = buffer.substr(0, buffer.find(" ", 0));
                    org[nr_orga2].pmodified = buffer.substr(0, buffer.find(" ", 0));
                    ++nr_orga2;
                }
                ++i;
            }
        }
    }
    RNAfold.close();

    // Test to see if the number of organisms in the two files are equal.
    if (nr_orga2 != nr_orga) {
        std::cerr << std::endl
            << "WARNING! The number of organisms in RNAfold.txt "
            << "is different from the number of organisms in "
            << modified_infile_name << "." << std::endl;
    }

    // Opens the entropy files (*.ps) and start working on the statistics.
    std::string filename;
    int pos1 = 0;
    int pos2 = 0;
    int k = 0;
    int nr_pl = 0;
    int nr_pr = 0;
    int nr_pa = 0;
    int N = 0;
    float Qm = 0;
    float p = 0;
    float Q = 0;
    float F = 0;
    float P = 0;
    float S = 0;

    for (i = 0; i < nr_orga; i++) {
        filename = input_dir + "/" + org[i].rnafold_name;
		filename.append("_dp.ps");

        Q = 0;
        p = 0;
        Qm = 0;
        F = 0;
        P = 0;
        S = 0;

        std::ifstream entropyfile(filename.c_str(), std::ios::in);

        if (!entropyfile.is_open()) {
            std::cerr << "Error opening file: " << filename << "\n\n";
            return EXIT_FAILURE;
        }

        while (!entropyfile.eof()) {
            // Gets a new line from the input file.
            while (std::getline(entropyfile, buffer)) {
                if (buffer != "") {
                    if (buffer.find(" ubox", 0) != -1 && buffer[0] != '%') {
                        // Assigns the object name.
                        pos1 = buffer.find(" ", 0);
                        pos1 = buffer.find(" ", pos1 + 1);
                        pos2 = buffer.find(" u", 0);

                        // Remember that the file contains the square roots of the
                        // base pair probabilities in the form : i  j  sqrt(p(i,j)) ubox
                        p = pow(atof(buffer.substr(pos1 + 1, pos2 - pos1).c_str()), 2);
                        Q = Q + (p * log2(p));
                        F = F + pow(p, 2);
                    }
                }
            }

            // The sequence length.
            N = org[i].parenthesis.length();
            org[i].N = N;
            // Value used by the Shannon entropy.
            Qm = 0.5 * N * log2(N);
            org[i].Qm = Qm;
            // The Shannon entropy.
            Q = (-1) / Qm * Q;
            org[i].Q = Q;
            // The Frobenius norm.
            F = sqrt(F / N);
            org[i].F = F;

            nr_pa = 0; // Number of paired bases.
            nr_pl = 0; // Number of left  parenthesis.
            nr_pr = 0; // Number of right parenthesis.

            for (k = 0; k < org[i].parenthesis.length(); ++k) {
                if (org[i].parenthesis[k] == '(') {
                    ++nr_pa;
                    ++nr_pl;
                }
                if (org[i].parenthesis[k] == ')') {
                    ++nr_pa;
                    ++nr_pr;
                }
            }

            // Prints a warning if there is a mismatch of number of parenthesis.
            if (nr_pl != nr_pr) {
                std::cerr << " WARNING : Number of () is not equal for \""
                    << org[i].name << "\"" << std::endl;
            }
            org[i].np = nr_pa / 2;

            // The fraction of paired bases within the optimal structure.
            P = (0.5 * 2 * nr_pa / N);
            org[i].P = P;

            int asl[org[i].parenthesis.length()]; // Array of stem lengths.
            int sl = 0;                           // Stem length.
            int ns = 0;                           // Number of stems.
            asl[0] = 0;

            // Start to work on the statistical value S.
            if (org[i].parenthesis.find("(", 0) != -1 &&
                org[i].parenthesis.find(")", 0) != -1) {

                while (org[i].pmodified.find(")", 0) != -1) {
                    sl = 0;
                    // Finds the positions of the first hairpin at the end of
                    // paired nucleotides.
                    pos1 = org[i].pmodified.find(")", 0);
                    pos2 = org[i].pmodified.substr(0, pos1).find_last_of("(");
                    // Add one more to the stem counter.
                    ++ns;
                    ++sl;
                    // Finds the number of paired bases.
                    for (k = 0; k < org[i].pmodified.length(); k++) {
                        ++pos1; // Add one position to the right.
                        --pos2; // Subtract one position to the left.
                        // Updates the counter if the bases are paired.
                        if (org[i].pmodified[pos1] == ')' &&
                            org[i].pmodified[pos2] == '(') {
                            ++sl;
                        }
                        // If the bases is not paired, the value of the stem length
                        // are added to the stem length array and the stem length
                        // counter are initialized.
                        else {
                            k = org[i].pmodified.length();
                            asl[ns - 1] = sl;
                            sl = 0;
                        }
                    }
                    // Removes the investigated stem from the modified sequence.
                    org[i].pmodified.replace(pos2 + 1, pos1 - pos2 - 1, "");
                }
                // Assigns the value to the object.
                org[i].ns = ns;
            }

            // Prints to standard out.
            if (v_option) {
                std::cout << std::endl
                    << " Name            : " << org[i].name << std::endl
                    << " RNAfold name    : " << org[i].rnafold_name << std::endl
                    << " Length          : " << org[i].sequence.length() << std::endl
                    << " Number of stems : " << org[i].ns << std::endl
                    << " Number of pairs : " << org[i].np << std::endl
                    << " Stem lengths    : ";

                for (k = 0; k < ns; ++k) {
                    std::cout << asl[k] << " ";
                }
                std::cout << std::endl
                    << " Structure       : " << org[i].parenthesis << std::endl
                    << " Sequence        : " << org[i].sequence << std::endl;
            }

            // Calculates the actual S value.
            for (k = 0; k < ns; ++k) {
                S = S + asl[k];
            }
            if (S == 0 || ns == 0) {
                S = 0;
            }
            else {
                S = S / ns;
            }
            org[i].S = S;
        }
        entropyfile.close();
    }

    // Prints something to standard out if "v - option" is used.
    if (v_option) {
        std::cout << std::endl << std::endl
        << " Float values of statistical parameters."
        << std::endl
        << std::setiosflags(std::ios::left)
        << std::setw(nr_slen + 2) << " Name:"
        << std::setw(12) << "  F:"
        << std::setw(12) << "   Q:"
        << std::setw(12) << "    P:"
        << std::setw(12) << "     S:"
        << std::endl;

    for (i = 0; i < nr_orga; ++i) {
        std::cout << " " << std::setw(nr_slen + 2) << org[i].name
            << " " << std::setw(12) << org[i].F
            << " " << std::setw(12) << org[i].Q
            << " " << std::setw(12) << org[i].P
            << " " << std::setw(12) << org[i].S
            << std::endl;
    }
    std::cout << std::endl;
}

    //------------------------------------------------------------------------//
    //------------------------------------------------------------------------//
    //------------------------------------------------------------------------//
    //              WRITES TO FILES (NEXUS - FORMAT)                           //
    //------------------------------------------------------------------------//
    //------------------------------------------------------------------------//
    //------------------------------------------------------------------------//

    // Just assigning some numbers.
    float Fmax = org[0].F;
    float Qmax = org[0].Q;
    float Pmax = org[0].P;
    float Smax = org[0].P;

    float Fmin = org[0].F;
    float Qmin = org[0].Q;
    float Pmin = org[0].P;
    float Smin = org[0].S;

    char iF = '0';
    char iQ = '0';
    char iP = '0';
    char iS = '0';

    std::string str_value;

    // First we have to find the range (min|max) of the local intervals.
    for (i = 0; i < nr_orga; ++i) {
        if (Fmax < org[i].F) Fmax = org[i].F;
        if (Fmin > org[i].F) Fmin = org[i].F;
        if (Qmax < org[i].Q) Qmax = org[i].Q;
        if (Qmin > org[i].Q) Qmin = org[i].Q;
        if (Pmax < org[i].P) Pmax = org[i].P;
        if (Pmin > org[i].P) Pmin = org[i].P;
        if (Smax < org[i].S) Smax = org[i].S;
        if (Smin > org[i].S) Smin = org[i].S;
    }

    // Finds the integer value of the variable relatively to the number
    // of character states and local interval
    for (i = 0; i < nr_orga; ++i) {
        iF = intvalue(org[i].F, character_states, Fmin, Fmax);
        iQ = intvalue(org[i].Q, character_states, Qmin, Qmax);
        iP = intvalue(org[i].P, character_states, Pmin, Pmax);
        iS = intvalue(org[i].S, character_states, Smin, Smax);

        str_value = iF;
        org[i].paup_l.append(str_value);
        str_value = iQ;
        org[i].paup_l.append(str_value);
        str_value = iP;
        org[i].paup_l.append(str_value);
        str_value = iS;
        org[i].paup_l.append(str_value);
    }

    // Prints the PAUP matrix to standard out if "v - option" is used.
    if (v_option) {
        if (character_states <= 10) {
            std::cout << " Paup matrix (l): 0 -> "
                << character_states - 1
                << "." << std::endl;
        }
        if (character_states > 10 && character_states <= 36) {
            std::cout << " Paup matrix (l): 0 -> 9 and A -> "
                << in2ch(character_states - 1)
                << " (" << character_states << ")." << std::endl;
        }
        if (character_states > 36 && character_states <= 62) {
            std::cout << " Paup matrix (l): 0 -> 9, A -> Z and a -> "
                << in2ch(character_states - 1)
                << " (" << character_states << ")." << std::endl;
        }
        if (character_states > 62 && character_states <= 63) {
            std::cout << " Paup matrix (l): 0 -> 9, A -> Z, a -> z and "
                << in2ch(character_states - 1)
                << " (" << character_states << ")." << std::endl;
        }
        if (character_states > 63 && character_states <= 64) {
            std::cout << " Paup matrix (l): 0 -> 9, A -> Z, a -> z, @ and "
                << in2ch(character_states - 1)
                << " (" << character_states << ")." << std::endl;
        }
        for (i = 0; i < nr_orga; ++i) {
            std::cout << " "
                << std::setiosflags(std::ios::left)
                << std::setw(nr_slen + 2)
                << org[i].name
                << org[i].paup_l
                << std::endl;
        }
        std::cout << std::endl;
    }

    // Changes the name of the output file.
	std::string str_fngv;
	str_fngv = input_dir + "/" + outfile_name;
	str_fngv.append("_l.nex");
	
    // Opens the output file for writing.
    std::ofstream ofilene(str_fngv.c_str(), std::ios::out);

    ofilene << "#NEXUS" << std::endl << std::endl
        << "[!Data from : STOAT 2007-2025]"
        << std::endl << std::endl;

    ofilene << "BEGIN DATA;" << std::endl
        << "DIMENSIONS  NTAX=" << nr_orga
        << " NCHAR=" << org[0].paup_l.length()
        << ";" << std::endl
        << "FORMAT SYMBOLS = ";
    // Small check to see what to print in the file.
    if (character_states <= 10) {
        ofilene << "\"0~"
            << character_states - 1
            << "\" INTERLEAVE;" << std::endl;
    }
    if (character_states > 10 && character_states <= 36) {
        ofilene << "\"0~9A~"
            << in2ch(character_states - 1)
            << "\" INTERLEAVE;" << std::endl;
    }
    if (character_states > 36 && character_states <= 62) {
        ofilene << "\"0~9A~Za~"
            << in2ch(character_states - 1)
            << "\" INTERLEAVE;" << std::endl;
    }
    if (character_states > 62 && character_states <= 63) {
        ofilene << "\"0~9A~Za~z"
            << in2ch(character_states - 1)
            << "\" INTERLEAVE;" << std::endl;
    }
    if (character_states > 63 && character_states <= 64) {
        ofilene << "\"0~9A~Za~z@"
            << in2ch(character_states - 1)
            << "\" INTERLEAVE;" << std::endl;
    }

    ofilene << "CHARSTATELABELS  " << std::endl;
    ofilene << "    " << std::setw(8) << 1 << " F"
        << "," << std::endl
        << "    " << std::setw(8) << 2 << " Q"
        << "," << std::endl
        << "    " << std::setw(8) << 3 << " P"
        << "," << std::endl
        << "    " << std::setw(8) << 4 << " S"
        << std::endl
        << "    ;" << std::endl;

    ofilene << std::endl << "MATRIX" << std::endl << std::endl;

    ofilene << std::setiosflags(std::ios::left);
    for (i = 0; i < nr_orga; ++i) {
        ofilene << std::setw(nr_slen + 2)
            << org[i].name
            << org[i].paup_l
            << std::endl;
    }

    ofilene << "    ;" << std::endl
        << "end;" << std::endl;
        
    /* ===================================================================== *
     * This part of the code writes out the flipped paup matrix.             *
     *                                                                       *
     * ===================================================================== */
    if (r_option) {
        std::string P[nr_orga];
        std::string characterlabels;

        characterlabels.append("FPQS");

        for (k = 0; k < nr_orga; k++) {
            P[k] = org[k].paup_l;
        }

        ofilene << std::endl << "[ FLIPPED MATRIX" << std::endl;
        ofilene << std::endl << "CHARSTATELABELS  " << std::endl;
        for (int k = 0; k < nr_orga; k++) {
            ofilene << std::setw(11) << " "
                << k + 1 << " " << org[k].name;
            if (k < nr_orga - 1) {
                ofilene << ",";
            }
            ofilene << std::endl;
        }
        ofilene << "    ;" << std::endl;

        ofilene << std::endl << "MATRIX" << std::endl << std::endl;
        for (i = 0; i < characterlabels.length(); i++) {
            ofilene << characterlabels.substr(i, 1)
                << "  ";
            for (int j = 0; j < nr_orga; j++) {
                ofilene << P[j].substr(i, 1);
            }
            ofilene << std::endl;
        }
        ofilene << "    ;" << std::endl
            << "end;" << std::endl;
        ofilene << std::endl << "END FLIPPED MATRIX ]" << std::endl << std::endl;
    }

    // Closing the output file.
    ofilene.close();

    //
    //
    // Finished with the local value file. Starts with the global
    // value file.
    //
    //

    // Finds the integer value of the variable relatively to the number
    // of character states and global interval between 0 and 1.
    for (i = 0; i < nr_orga; ++i) {
        iF = intvalue(org[i].F, character_states, 0, 1);
        iQ = intvalue(org[i].Q, character_states, 0, 1);
        iP = intvalue(org[i].P, character_states, 0, 1);
        iS = intvalue(1 / org[i].S, character_states, 0, 1);

        str_value = iF;
        org[i].paup_g.append(str_value);
        str_value = iQ;
        org[i].paup_g.append(str_value);
        str_value = iP;
        org[i].paup_g.append(str_value);
        str_value = iS;
        org[i].paup_g.append(str_value);
    }

    // Prints the PAUP matrix to standard out if "v - option" is used.
    if (v_option) {
        if (character_states <= 10) {
            std::cout << " Paup matrix (g): 0 -> "
                << character_states - 1
                << "." << std::endl;
        }
        if (character_states > 10 && character_states <= 36) {
            std::cout << " Paup matrix (g): 0 -> 9 and A -> "
                << in2ch(character_states - 1)
                << " (" << character_states << ")." << std::endl;
        }
        if (character_states > 36 && character_states <= 62) {
            std::cout << " Paup matrix (g): 0 -> 9, A -> Z and a -> "
                << in2ch(character_states - 1)
                << " (" << character_states << ")." << std::endl;
        }
        if (character_states > 62 && character_states <= 63) {
            std::cout << " Paup matrix (g): 0 -> 9, A -> Z, a -> z and "
                << in2ch(character_states - 1)
                << " (" << character_states << ")." << std::endl;
        }
        if (character_states > 63 && character_states <= 64) {
            std::cout << " Paup matrix (g): 0 -> 9, A -> Z, a -> z, @ and "
                << in2ch(character_states - 1)
                << " (" << character_states << ")." << std::endl;
        }
        for (i = 0; i < nr_orga; ++i) {
            std::cout << " "
                << std::setiosflags(std::ios::left)
                << std::setw(nr_slen + 2)
                << org[i].name
                << org[i].paup_g
                << std::endl;
        }
        std::cout << std::endl;
    }

    // Changes the name of the output file.
	str_fngv = input_dir + "/" + outfile_name;
	str_fngv.append("_g.nex");
    
    // Opens the file for writing
    std::ofstream ofileg(str_fngv.c_str(), std::ios::out);

    ofileg << "#NEXUS" << std::endl << std::endl
        << "[!Data from : STOAT 2007-2025]"
        << std::endl << std::endl;

    ofileg << "BEGIN DATA;" << std::endl
        << "DIMENSIONS  NTAX=" << nr_orga
        << " NCHAR=" << org[0].paup_g.length()
        << ";" << std::endl
        << "FORMAT SYMBOLS = ";
    // Small check to see what to print in the file.
    if (character_states <= 10) {
        ofileg << "\"0~"
            << character_states - 1
            << "\" INTERLEAVE;" << std::endl;
    }
    if (character_states > 10 && character_states <= 36) {
        ofileg << "\"0~9A~"
            << in2ch(character_states - 1)
            << "\" INTERLEAVE;" << std::endl;
    }
    if (character_states > 36 && character_states <= 62) {
        ofileg << "\"0~9A~Za~"
            << in2ch(character_states - 1)
            << "\" INTERLEAVE;" << std::endl;
    }
    if (character_states > 62 && character_states <= 63) {
        ofileg << "\"0~9A~Za~z"
            << in2ch(character_states - 1)
            << "\" INTERLEAVE;" << std::endl;
    }
    if (character_states > 63 && character_states <= 64) {
        ofileg << "\"0~9A~Za~z@"
            << in2ch(character_states - 1)
            << "\" INTERLEAVE;" << std::endl;
    }

    ofileg << "CHARSTATELABELS  " << std::endl;
    ofileg << "    " << std::setw(8) << 1 << " F"
        << "," << std::endl
        << "    " << std::setw(8) << 2 << " Q"
        << "," << std::endl
        << "    " << std::setw(8) << 3 << " P"
        << "," << std::endl
        << "    " << std::setw(8) << 4 << " S [S^{-1}]"
        << std::endl
        << "    ;" << std::endl;

    ofileg << std::endl << "MATRIX" << std::endl << std::endl;

    ofileg << std::setiosflags(std::ios::left);
    for (i = 0; i < nr_orga; ++i) {
        ofileg << std::setw(nr_slen + 2)
            << org[i].name
            << org[i].paup_g
            << std::endl;
    }

    ofileg << "    ;" << std::endl
        << "end;" << std::endl;

    /* ===================================================================== *
     * This part of the code writes out the flipped paup matrix.             *
     *                                                                       *
     * ===================================================================== */
    if (r_option) {
        std::string P[nr_orga];
        std::string characterlabels;

        characterlabels.append("FPQS");

        for (k = 0; k < nr_orga; k++) {
            P[k] = org[k].paup_g;
        }

        ofileg << std::endl << "[ FLIPPED MATRIX" << std::endl;
        ofileg << std::endl << "CHARSTATELABELS  " << std::endl;
        for (int k = 0; k < nr_orga; k++) {
            ofileg << std::setw(11) << " "
                << k + 1 << " " << org[k].name;
            if (k < nr_orga - 1) {
                ofileg << ",";
            }
            ofileg << std::endl;
        }
        ofileg << "    ;" << std::endl;

        ofileg << std::endl << "MATRIX" << std::endl << std::endl;
        for (i = 0; i < characterlabels.length(); i++) {
            ofileg << characterlabels.substr(i, 1)
                << "  ";
            for (int j = 0; j < nr_orga; j++) {
                ofileg << P[j].substr(i, 1);
            }
            ofileg << std::endl;
        }
        ofileg << "    ;" << std::endl
            << "end;" << std::endl;
        ofileg << std::endl << "END FLIPPED MATRIX ]" << std::endl << std::endl;
    }

    // Closes the output file.
    ofileg.close();
    

    //------------------------------------------------------------------------//
    //------------------------------------------------------------------------//
    //------------------------------------------------------------------------//
    //    RENAMES THE RNAfold GENERATED FILES BACK TO THE SEQUENCE NAMES      //
    //------------------------------------------------------------------------//
    //------------------------------------------------------------------------//
    //------------------------------------------------------------------------//

    c = 0;
    if (v_option) {
        std::cout << " Renames RNAfold generated files: "
            << std::endl;
    }

	for (int k = 0; k < nr_orga; ++k) {
    	org[k].name_modified = input_dir + "/" + org[k].name;  // Add input_dir to the path
    	// Removes white spaces in the sequence names.
    	while (c < org[k].name_modified.length()) {
            if (org[k].name_modified.find(" ") != std::string::npos) {
                c = org[k].name_modified.find(" ");
                org[k].name_modified.replace(c, 1, "");
                c++;
            }
            else {
                c = org[k].name_modified.length();
            }
        }
        org[k].name_modified.append("_dp.ps");
        org[k].rnafold_name = input_dir + "/" + org[k].rnafold_name;  // Add input_dir to the path
		org[k].rnafold_name.append("_dp.ps");
			if (rename(org[k].rnafold_name.c_str(), org[k].name_modified.c_str())) {
            std::cout << std::endl
                << "WARNING! Could not rename "
                << org[k].rnafold_name
                << " to "
                << org[k].name_modified
                << std::endl;
        }
        else {
            if (v_option) {
                std::cout << " "
                    << org[k].rnafold_name
                    << " "
                    << " -> "
                    << org[k].name_modified << std::endl;
            }
        }

        org[k].name_modified.replace(org[k].name_modified.length() - 6, 6, "_ss.ps");
        org[k].rnafold_name.replace(org[k].rnafold_name.length() - 6, 6, "_ss.ps");
        if (rename(org[k].rnafold_name.c_str(), org[k].name_modified.c_str())) {
            std::cout << std::endl
                << "WARNING! Could not rename "
                << org[k].rnafold_name
                << " to "
                << org[k].name_modified
                << std::endl;
        }
        else {
            if (v_option) {
                std::cout << " "
                    << org[k].rnafold_name
                    << " "
                    << " -> "
                    << org[k].name_modified << std::endl;
            }
        }
    }

    //------------------------------------------------------------------------//
    //------------------------------------------------------------------------//
    //------------------------------------------------------------------------//
    //  TRANSLATE SECONDARY STRUCTURE FROM BRACKET NOTATION INTO DCSE FORMAT  //
    //------------------------------------------------------------------------//
    //------------------------------------------------------------------------//
    //------------------------------------------------------------------------//

    for (int k = 0; k < nr_orga; ++k) {
        // If the user provides the secondary structure, else use the
        // structure generated by RNAfold.
        if (t_option) {
            org[k].sequence_dcse = org[k].sequence;
            org[k].smodified = org[k].parenthesis_user;
            org[k].helix_num_dcse.resize(org[k].sequence.length(), '-');
        }
        else {
            org[k].sequence_dcse = org[k].sequence;
            org[k].smodified = org[k].parenthesis;
            org[k].helix_num_dcse.resize(org[k].sequence.length(), '-');
        }

        // Makes sure there are no white spaces in the sequence.
        c = 0;
        while (c < org[k].sequence_dcse.length()) {
            if (org[k].sequence_dcse.find(" ") != std::string::npos) {
                c = org[k].sequence_dcse.find(" ");
                org[k].sequence_dcse.replace(c, 1, "");
                c++;
            }
            else {
                c = org[k].sequence_dcse.length();
            }
        }

        // Makes sure there are no white spaces in the bracket notation of
        // the secondary structure.
        c = 0;
        while (c < org[k].smodified.length()) {
            if (org[k].smodified.find(" ") != std::string::npos) {
                c = org[k].smodified.find(" ");
                org[k].smodified.replace(c, 1, "");
                c++;
            }
            else {
                c = org[k].smodified.length();
            }
        }

        // Runs a test to see if the user has provided a secondary structure.
        if (org[k].smodified.empty() && t_option) {
            std::cout << std::endl
                << "WARNING! Could not find secondary structure information "
                << "for: "
                << org[k].name
                << ". DCSE-translator continues with structure"
                << " generated by RNAfold." << std::endl;
            org[k].smodified = org[k].parenthesis;
        }

        // Runs a test to see if the length of the secondary structure is
        // equal to the length of the sequence
        if (org[k].sequence_dcse.length() != org[k].smodified.length()) {
            std::cout << std::endl
                << "WARNING! The string length of the secondary structure "
                << "differs from the string length of the sequence for: "
                << org[k].name
                << "." << std::endl;
        }

        // Runs a test to check that the secondary structure string in the
        // bracket notation only contains brackets and dots.
        for (i = 0; i < org[k].smodified.length(); i++) {
            if (org[k].smodified.compare(i, 1, "(") != 0 &&
                org[k].smodified.compare(i, 1, ")") != 0 &&
                org[k].smodified.compare(i, 1, ".") != 0) {
                std::cout << std::endl
                    << "WARNING! Unknown character in secondary structure for: "
                    << org[k].name << std::endl;
            }
        }

        // Modifies the secondary structure and the sequence by inserting
        // white spaces between each position of the strings.
        for (i = 0; i < org[k].sequence.length(); i++) {
            org[k].sequence_dcse.insert(i * 2, 1, ' ');
            org[k].smodified.insert(i * 2, 1, ' ');
            org[k].helix_num_dcse.insert(i * 2, 1, ' ');
        }

        int pos1 = 0;    // Positions related to Helix numbers.
        int pos2 = 0;    // Positions related to the complementary Helix Numbers.
        int ns = 0;      // Number of stems.
        int sss = 0;     // Stem Strand Start.
        int sse = 0;     // Stem Strand End.
        int ssps = 0;    // Stem Strand Prime Start.
        int sspe = 0;    // Stem strand Prime End.
        int ahnnnr = 0;  // Alternative Helix Number Name Number. Used
                          // together with ahnn.
        std::string hn;  // Helix Number.
        std::string ahnn; // Alternative Helix Number Name.

        // Makes a string containing possible Helix numbers. Is to be used
        // with stem strands which are shorter in length than the Helix
        // number counter. In that way the Helix number only becomes two
        // characters long
        ahnn.append("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        ahnn.append("abcdefghijklmnopqrstuvwxyz");
        ahnn.append("0123456789");
        ahnn.append("~@#$%&*?");

        // MAIN LOOP IN THE TRANSLATOR
        while (org[k].smodified.find(")", 0) != std::string::npos &&
            org[k].smodified.find("(", 0) != std::string::npos) {
            // Finds the beginning and end positions of the first hairpin.
            ns++;
            pos2 = org[k].smodified.find(")", 0);
            pos1 = org[k].smodified.rfind("(", pos2);

            // Modifies the positions such that the inserted "[" and "]"
            // begins before and after the nucleotides matching the brackets
            // in the secondary structure.
            sse = pos1 + 1;
            ssps = pos2 - 1;

            // Inserts the "[" and "]" into the sequence. If there is a "["
            // and "]" at that position already, than insert a "^" instead.
            if (org[k].sequence_dcse.compare(pos2 - 1, 1, "]") == 0) {
                org[k].sequence_dcse.replace(pos2 - 1, 1, "^");
            }
            else {
                org[k].sequence_dcse.replace(pos2 - 1, 1, "[");
            }

            if (org[k].sequence_dcse.compare(pos1 + 1, 1, "[") == 0) {
                org[k].sequence_dcse.replace(pos1 + 1, 1, "^");
            }
            else {
                org[k].sequence_dcse.replace(pos1 + 1, 1, "]");
            }

            // Modifies the reference to be able to continue to search for
            // "(" and ")".
            org[k].smodified.replace(pos2, 1, "]");
            org[k].smodified.replace(pos1, 1, "[");

            // Finds the length of the stems. The stem is ended when there
            // is not matching a "(" to a ")" in the secondary structure
            // string.
            while (org[k].smodified[pos2 + 2] == ')' &&
                org[k].smodified[pos1 - 2] == '(') {
                org[k].smodified.replace(pos2 + 2, 1, "]");
                org[k].smodified.replace(pos1 - 2, 1, "[");

                pos2 = pos2 + 2; // Add two positions to the right.
                pos1 = pos1 - 2; // Subtract two positions to the left.
            }

            // The same as over, but now we closes the stems in the dcse
            // format by inserting "[" and "]".
            if (org[k].sequence_dcse.compare(pos2 + 1, 1, "[") == 0) {
                org[k].sequence_dcse.replace(pos2 + 1, 1, "^");
            }
            else {
                org[k].sequence_dcse.replace(pos2 + 1, 1, "]");
            }

            if (org[k].sequence_dcse.compare(pos1 - 1, 1, "]") == 0) {
                org[k].sequence_dcse.replace(pos1 - 1, 1, "^");
            }
            else {
                org[k].sequence_dcse.replace(pos1 - 1, 1, "[");
            }

            // Modifies the positions again. Now adding and subtracting in
            // the opposite order.
            sss = pos1 - 1;
            sspe = pos2 + 1;

            // This is going to be the beginning position of the printed
            // Helix number in the Helix Numbering string.
            pos1 = sss + int((sse - sss) / 2);
            pos2 = ssps + int((sspe - ssps) / 2);

            // Makes the Helix Number.
            hn.clear();
            hn.append("S");
            // Converts the counter to a string and adds it to the end of
            // the helix number.
            std::stringstream sns;
            sns << ns;
            hn.append(sns.str());

            // A small test to avoid that the Helix Numbers writes over each
            // other. Tries to adjust the start writing position to the
            // length of the Helix number and the length of the stem
            // strand. If the Helix Number is larger than the stem strand,
            // then use the next Helix Numbering Name in ahnn.
            if (((pos1 + hn.length()) > sse) ||
                ((pos2 + hn.length()) > sspe)) {
                if ((hn.length() > (sse - sss)) ||
                    (hn.length() > (sspe - ssps))) {
                    hn = ahnn[ahnnnr];
                    ahnnnr++;
                }
                else {
                    pos1 = pos1 - (pos1 + hn.length() - sse);
                    pos2 = pos2 - (pos2 + hn.length() - sspe);
                }
            }

            // Insert the two Helix numbers into the Helix numbering string.
            org[k].helix_num_dcse.replace(pos1, hn.length(), hn);
            // Adds the prime "'" to the Helix number.
            hn.append("'");
            org[k].helix_num_dcse.replace(pos2, hn.length(), hn);
        }

        // Re-modifies the secondary structure to the bracket notation
        for (i = 0; i < org[k].smodified.length(); i++) {
            if (org[k].smodified.compare(i, 1, "[") == 0) {
                org[k].smodified.replace(i, 1, "(");
            }
            if (org[k].smodified.compare(i, 1, "]") == 0) {
                org[k].smodified.replace(i, 1, ")");
            }
        }

        // Removes white spaces from the sequence names. To be used as
        // file names for the DCSE files.
        org[k].name_modified = input_dir + "/" + org[k].name;  // Add input_dir to the path
		// Removes white spaces in the sequence names.
		while (c < org[k].name_modified.length()) {
   			if (org[k].name_modified.find(" ") != std::string::npos) {
                c = org[k].name_modified.find(" ");
                org[k].name_modified.replace(c, 1, "");
                c++;
            }
            else {
                c = org[k].name_modified.length();
            }
        }


        // Writes the files in DCSE format.
    // Construct the .dcse file path
    std::string dcsefilename = input_dir + "/" + org[k].name + ".dcse";

    // Debug print
    if (v_option) {
        std::cout << "Creating DCSE file: " << dcsefilename << std::endl;
    }

    // Open the output file for writing
    std::ofstream dcsefile(dcsefilename.c_str(), std::ios::out);
    if (!dcsefile.is_open()) {
        std::cerr << "Error: Could not create DCSE file: " << dcsefilename << std::endl;
    } else {
        dcsefile << "[!Data from : STOAT 2007-2025]" 
                 << std::endl << std::endl << std::endl
                 << org[k].smodified      << "       " << "Bracket notation" << std::endl
                 << org[k].sequence_dcse  << "       " << org[k].name       << std::endl
                 << org[k].helix_num_dcse << "       " << "Helix numbering" << std::endl;

        // Close the output file
        dcsefile.close();

        if (v_option) {
            std::cout << "DCSE file created successfully: " << dcsefilename << std::endl;
        }
    }
}
    //------------------------------------------------------------------------//
    //------------------------------------------------------------------------//
    //------------------------------------------------------------------------//
    //  END OF PROGRAM                                                        //
    //------------------------------------------------------------------------//
    //------------------------------------------------------------------------//
    //------------------------------------------------------------------------//

    return 0;
}