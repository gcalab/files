/****************************************************************************
 *                                                                          *
 *     This function prints the program options.                            *
 *                                                               STOAT 2007 *
 ****************************************************************************/


#include <iostream>  // Include the iostream header for std::cout and std::endl

void phelp(void) {
    std::cout << "" << std::endl
              << " Options      :" << std::endl
              << "  -i    : The input  file." << std::endl
              << "  -o    : The output file." << std::endl
              << "  -x    : Change the number of character states" << std::endl
              << "  -h    : Prints help." << std::endl
              << "  -v    : Gives verbose output." << std::endl
              << "  -f    : Arguments to RNAfold" << std::endl
              << "  -t    : Use input structure to generate DCSE" << std::endl
              << "  -r    : Make a flipped paup matrix" << std::endl
              << std::endl << std::endl
              << " Correct usage: " << std::endl << std::endl
              << " \"stoat  -i name_of_input_file" << std::endl
              << "        [-o prefix_name_of_output_files]" << std::endl
              << "        [-f \"RNAfold_options\"]" << std::endl
              << "        [-x number_of_character_states]" << std::endl
              << "        [-v]\"" << std::endl
              << std::endl << std::endl;
}