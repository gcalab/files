/*****************************************************************************
 *                                                                           *
 *  This file contains the class organism.                                   *
 *                                                                           *
 *                                                               MARTEN 2007 *
 *****************************************************************************/
 

class organism {
  public:
  std::string name;
  std::string sequence;
  std::string modified;
  std::string paup;
  std::string paupname;
  std::string paupadded;
  std::string paupadded_characther_labels;
  std::string fasta_sequence;
  std::string fasta_bracket;
  std::string RNAfold_constrain; 

  bool nss_not_a_multiple_of_two;
  bool nss_larger_than_nh;
  bool nss_smaller_than_nh;
  bool exclude_sequence;
  bool helix_mapping_needed;
  bool unequal_number_of_paired_bases;
 

  organism(void);  
  ~organism(void);          
  
  // The Start and End positions for the stem strands.
  void SandE_ChangeSize  (int s_size, int e_size);
  void SandE_InsertValueS(int s_pos,  int s_value);
  void SandE_InsertValueE(int e_pos,  int s_value);
  int  SandE_ReturnValueS(int s_pos);
  int  SandE_ReturnValueE(int e_pos);
  int  SandE_ReturnSizeS (void);
  int  SandE_ReturnSizeE (void);
  void SandE_PrintArray  (void); 
  
  // The mapping of Helix numbers to stem strands.
  void HelixMap_ChangeSize          (int helixmap_size);
  int  HelixMap_ReturnSize          (void);
  int  HelixMap_FindFirstNonMapped  (int start);
  int  HelixMap_FindFirstMapped     (int start);
  void HelixMap_PrintMap            (void); 
  void HelixMap_InsertMap           (int pos, bool value);
  bool HelixMap_ReturnMap           (int pos);
  
  // The mapping of stem strands to Helix numbers.
  void StemStrandMap_ChangeSize            (int stemstrandmap_size);
  int  StemStrandMap_ReturnSize            (void);
  int  StemStrandMap_FindFirstNonMapped    (int start);
  int  StemStrandMap_FindLastNonMapped     (int start);
  void StemStrandMap_PrintMap              (void); 
  int  StemStrandMap_FindNumberOfNonMapped (void);
  void StemStrandMap_InsertMap             (int pos, bool value);
  bool StemStrandMap_ReturnMap             (int pos);
  
private:
  int *sarray;               // Start of brackets array. 
  int *earray;               // End   of brackets array.
  int size_sarray;           // Size  of start of brackets array.
  int size_earray;           // Size  of end   of brackets array.
  int returnv;               // return value;
  
 
  int size_helixmap;         // The size of the stem array.
  int size_stemstrandmap;    // The size of the stem array.
  
  bool *helixmap;            // Boolean array.
  bool *stemstrandmap;       // Boolean array.
  bool returnb;              // Return boll
  std::string returnn;       // Return name;
};


organism::organism(void){      
  sarray        = new int[1];
  earray        = new int[1];
  helixmap      = new bool[1];
  stemstrandmap = new bool[1];
  sarray[0]          = 0;
  earray[0]          = 0;
  size_sarray        = 1;  
  size_earray        = 1; 
  size_helixmap      = 1;
  size_stemstrandmap = 1;
}

organism::~organism(void){            
  delete[ ] sarray;
  delete[ ] earray;
  delete[ ] helixmap;
  delete[ ] stemstrandmap;
}




//***************************************************************************//
//                                                                           //
//      Functions related to the start and stop positions for stem strands   //
//                                                                           //
//***************************************************************************//

void organism::SandE_ChangeSize(int s_size, int e_size){ 
  delete[ ] sarray;
  delete[ ] earray;
  sarray    = new int[s_size];
  earray    = new int[e_size];
  size_sarray = s_size;
  size_earray = e_size;
}

void organism::SandE_PrintArray(void){         
  for(int i=0; i<size_sarray; i++){                 
    std::cout << sarray[i] << " ";  
  }              
  std::cout << endl;
  for(int i=0; i<size_earray; i++){                 
    std::cout << earray[i] << " "; 
  }      
  std::cout << endl;
}

void organism::SandE_InsertValueS(int s_pos, int s_value){ 
  sarray[s_pos] = s_value;
}

void organism::SandE_InsertValueE(int e_pos, int e_value){ 
  earray[e_pos] = e_value;
}

int organism::SandE_ReturnValueS(int s_pos){ 
  returnv = sarray[s_pos];
  return (returnv);
}

int organism::SandE_ReturnValueE(int e_pos){ 
  returnv = earray[e_pos];
  return (returnv);
}

int organism::SandE_ReturnSizeS(void){ 
  returnv = size_sarray;
  return (returnv);
}

int organism::SandE_ReturnSizeE(void){ 
  returnv = size_earray;
  return (returnv);
}






//***************************************************************************//
//                                                                           //
//      Functions related to the mapping of Helix numbers to stem strands.   //
//                                                                           //
//***************************************************************************//

void organism::HelixMap_ChangeSize (int helixmap_size) { 
  delete[ ] helixmap;
  helixmap = new bool[helixmap_size];
  size_helixmap = helixmap_size;
}

int  organism::HelixMap_ReturnSize (void) {
  returnv = size_helixmap;
  return (returnv);
}

void organism::HelixMap_PrintMap (void){         
  std::cout << "Helix mapping:        ";
  for(int i=0; i<size_helixmap; i++){
    std::cout << helixmap[i];  
  }    
  std::cout << endl << endl; 
}

int  organism::HelixMap_FindFirstNonMapped (int start) {
  bool first = false;
  for(int i=start; i<size_helixmap; i++){
    if(!helixmap[i] && !first) {
      returnv = i;
      first = true;
    }
  }
  return (returnv);
}

int  organism::HelixMap_FindFirstMapped (int start) {
  bool first = false;
  for(int i=start; i<size_helixmap; i++){
    if(helixmap[i] && !first) {
      returnv = i;
      first = true;
    }
  }
  return (returnv);
}

void  organism::HelixMap_InsertMap(int pos, bool value) {
  helixmap[pos] = value;
}

bool organism::HelixMap_ReturnMap(int pos) {
  returnb = helixmap[pos];
  return (returnb);
}  



//***************************************************************************//
//                                                                           //
//      Functions related to the mapping of stem strands to Helix numbers.   //
//                                                                           //
//***************************************************************************//

void organism::StemStrandMap_ChangeSize(int stemstrandmap_size) { 
  delete[ ] stemstrandmap;
  stemstrandmap = new bool[stemstrandmap_size];
  size_stemstrandmap = stemstrandmap_size;
}

int  organism::StemStrandMap_ReturnSize (void) {
  returnv = size_stemstrandmap;
  return (returnv);
}

void organism::StemStrandMap_PrintMap(void){         
  std::cout << "Stem strand mapping:  ";
  for(int i=0; i<size_stemstrandmap; i++){
    std::cout << stemstrandmap[i];  
  }    
  std::cout << endl << endl; 
}

int  organism::StemStrandMap_FindFirstNonMapped (int start) {
  bool first = false;
  for(int i=start; i<size_stemstrandmap; i++){
    if(!stemstrandmap[i] && !first) {
      returnv = i;
      first = true;
    }
  }
  return (returnv);
}

int  organism::StemStrandMap_FindLastNonMapped (int start) {
  bool last = false;
  for(int i=start; i<size_stemstrandmap; i++){
    if(stemstrandmap[i] && !last) {
      returnv = i-1;
      last = true;
    }
    if(i == size_stemstrandmap-1 && !last){
      returnv = i;
      last = true;
    }
  }
  return (returnv);
}

int organism::StemStrandMap_FindNumberOfNonMapped (void) {
  int nnmss = 0; // Number of Non Mapped Stem Strands
  for(int i=0; i<size_stemstrandmap; i++){
    if(!stemstrandmap[i]) {
      nnmss++;
    }
  }
  returnv = nnmss;
  return (returnv);
}  
 
void organism::StemStrandMap_InsertMap(int pos, bool value) {
  stemstrandmap[pos] = value;
}  

bool organism::StemStrandMap_ReturnMap(int pos) {
  returnb = stemstrandmap[pos];
  return (returnb);
}  
 
