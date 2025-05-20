/*****************************************************************************
 *                                                                           *
 *  This file contains the class definitions with functions.                 *
 *                                                                           *
 *                                                                STOAT 2007 *
 *****************************************************************************/
 
#include <string>


class organism {
  public:
  // Normal variables.
  std::string name;             // Original sequence name provided by the user.
  std::string name_modified;    // Sequence without any white spaces.
  std::string rnafold_name;     // Sequence name assigned by STOAT.
  std::string sequence;         // The original sequence.
  std::string smodified;        // The modified sequence.
  std::string paup_l;           // The local paup format string.
  std::string paup_g;           // The global paup format string.
  std::string parenthesis;      // The secondary structure format.
  std::string pmodified;        // The modified secondary structure.

  std::string sequence_dcse;    // The sequence in DCSE format
  std::string helix_num_dcse;   // The Helix numbering string for the
				// DCSE format

  std::string parenthesis_user; // The secondary structure format
				// provided by the user.
  
  
  // Statistical properties.
  int         np;            // Number of paired nucleotides in
			     // secondary structure.
  int         ns;            // Number of stems in secondary structure.
  int         N;             // Sequence length.
  float       Qm;            // Statistical value used by the Shannon entropy.
  float       Q;             // The Shannon entropy.
  float       F;             // The Forbenius norm.
  float       S;             // The base-paring propensity.
  float       P;             // Mean stem length of helical stem structures.
};


