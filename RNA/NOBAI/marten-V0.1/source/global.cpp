/*****************************************************************************
 *                                                                           *
 *  This file contains the class global.                                     *
 *                                                                           *
 *                                                               MARTEN 2007 *
 *****************************************************************************/
 

class global { 
public:
  std::string name; 
  std::string sequence; 
    
  std::string HelixNumberingStringName;
  std::string HelixNumberingString;
  std::string CharacterLabels;
 
  bool  e_option;
  bool  l_option;
  bool  r_option;
  bool  v_option;
  bool  p_option;
  bool  s_option;
  bool  a_option;
  bool  n_option;
  bool  m_option;
  bool  g_option;
  bool  P_option;
  bool  x_option;
  bool  S_option;
  bool  R_option;
  
  global (void);
  ~global(void);          
  
  // The Start and End positions for thr stem strands.
  void SandE_ChangeSize  (int sarray_size, int earray_size);
  void SandE_InsertValueS(int sarray_pos,  int sarray_value);
  void SandE_InsertValueE(int earray_pos,  int sarray_value);
  int  SandE_ReturnValueS(int sarray_pos);
  int  SandE_ReturnValueE(int earray_pos);
  int  SandE_ReturnSizeS (void);
  int  SandE_ReturnSizeE (void);
  void SandE_PrintArray  (void); 
  
  // The Helix and stem strand mapping matrix
  void HSSMMatrix_ChangeSize                  (int rows, int colums);
  void HSSMMatrix_PrintMatrix                 (void); 
  void HSSMMatrix_SetWholeMatrixFalse         (void);
  void HSSMMatrix_SetMatrixEntryToFalse       (int row, int colum);
  void HSSMMatrix_SetMatrixEntryToTrue        (int row, int colum);
  bool HSSMMatrix_ReturnMatrixEntryState      (int row, int colum);
  bool HSSMMatrix_ReturnHelixMap              (int row); 
  bool HSSMMatrix_ReturnStemStrandMap         (int colum);         
  int  HSSMMatrix_ReturnStemStrandMapNumber   (int colum);
  int  HSSMMatrix_ReturnStemStrandMapHelixPos (int colum);

  // The Helix Numbering positions
  void HelixPosArray_ChangeSize    (int helixpos_size);
  int  HelixPosArray_ReturnSize    (void);
  void HelixPosArray_PrintArray    (void);
  void HelixPosArray_InsertValue   (int array_pos, int string_pos);
  int  HelixPosArray_ReturnValue   (int array_pos);
  int  HelixPosArray_ReturnArrayPos(int string_pos);

  // The Helix Numbering names
  void         HelixNamesArray_ChangeSize   (int helixnames_size);
  int          HelixNamesArray_ReturnSize   (void);
  void         HelixNamesArray_PrintArray   (void);
  int          HelixNamesArray_ReturnNamePos(std::string name);
  std::string  HelixNamesArray_ReturnName   (int pos); 
  void         HelixNamesArray_InsertName   (int pos, std::string name);
  int          HelixNamesArray_FindComplementaryNamePos(std::string name);
private:
  int  *sarray;            // Start of brackets array. 
  int  *earray;            // End   of brackets array.
  bool *HSSMMatrix;        // A one dimensional array used as a two
		           // dimensional matrix.(HSSMMatrix = Helix Stem
		           // Strand Map Matrix.)
  int  *helixposarray;     // Array containing the helix positions in the
		           // helix numbering string.
  int   size_sarray;       // Size  of start of brackets array.
  int   size_earray;       // Size  of end   of brackets array.
  int   nr_rows;           // Number of rows in HSSMMatrix.
  int   nr_colums;         // Number of colums in HSSMMatrix. 
  int   returnv;           // return value;  
  bool  returnb;           // Return bool
  int   size_helixpos;     // The size of the Helix position array.
  int   size_helixnames;   // The size of the Helix names array.
  std::string  returnn;           // Return name;
  std::string *helixnamearray;    // Pointer for an array of stem strand names used
			          // to store the helix numbering.
};


global::global(void){      
  sarray        = new int[1];
  earray        = new int[1];
  HSSMMatrix    = new bool[1*1]; // [1*1] is just to indicate thet it
			         // is going to be used as a
			         // matrix. It could just as well just
			         // have written [1].
  helixposarray = new int[1];
  sarray[0]       = 0;
  earray[0]       = 0;
  HSSMMatrix[1*1] = false;

  size_sarray     = 1;  
  size_earray     = 1; 
  nr_rows         = 1;
  nr_colums       = 1;
  size_helixpos   = 1;
  size_helixnames = 1;
}

global::~global(void){            
  delete[ ] sarray;
  delete[ ] earray;
  delete[ ] HSSMMatrix;
  delete[ ] helixposarray;
  delete[ ] helixnamearray;
}






//***************************************************************************//
//                                                                           //
//      Functions related to the start and stop positions for stem strands   //
//                                                                           //
//***************************************************************************//

void global::SandE_ChangeSize(int sarray_size, int earray_size){ 
  delete[ ] sarray;
  delete[ ] earray;
  sarray    = new int[sarray_size];
  earray    = new int[sarray_size];
  size_sarray = sarray_size;
  size_earray = earray_size;
}

void global::SandE_PrintArray(void){         
  for(int i=0; i<size_sarray; i++){                 
    std::cout << sarray[i] << " ";  
  }              
  std::cout << endl;
  for(int i=0; i<size_earray; i++){                 
    std::cout << earray[i] << " "; 
  }      
  std::cout << endl;
}

void global::SandE_InsertValueS(int sarray_pos, int sarray_value){ 
  sarray[sarray_pos] = sarray_value;
}

void global::SandE_InsertValueE(int earray_pos, int earray_value){ 
  earray[earray_pos] = earray_value;
}

int global::SandE_ReturnValueS(int sarray_pos){ 
  returnv = sarray[sarray_pos];
  return (returnv);
}

int global::SandE_ReturnValueE(int earray_pos){ 
  returnv = earray[earray_pos];
  return (returnv);
}

int global::SandE_ReturnSizeS(void){ 
  returnv = size_sarray;
  return (returnv);
}

int global::SandE_ReturnSizeE(void){ 
  returnv = size_earray;
  return (returnv);
}


//***************************************************************************//
//                                                                           //
//      Functions related to the Helix and stem strand mapping matrix.       //
//                                                                           //
//***************************************************************************//

void global::HSSMMatrix_ChangeSize(int rows, int colums){ 
  delete[ ] HSSMMatrix;
  HSSMMatrix = new bool[rows * colums];
  nr_rows    = rows;
  nr_colums  = colums;
}

void global::HSSMMatrix_PrintMatrix(void) {         
  std::cout << "HSSMMatrix:           ";
  for(int i=0; i<nr_rows; i++){                 
    for(int j=0; j<nr_colums; j++) {                 
      std::cout << HSSMMatrix[(i*nr_colums) + j];
    } 
    std::cout << endl << "                      ";
  }
  std::cout << endl;
}

void global::HSSMMatrix_SetWholeMatrixFalse(void) {         
  for(int i=0; i<nr_rows; i++){                 
    for(int j=0; j<nr_colums; j++) {                 
      HSSMMatrix[(i*nr_colums) + j] = false; 
    } 
  }
}

void global::HSSMMatrix_SetMatrixEntryToFalse(int row, int colum) {         
  HSSMMatrix[(row*nr_colums) + colum] = false; 
} 

void global::HSSMMatrix_SetMatrixEntryToTrue(int row, int colum) {         
  HSSMMatrix[(row*nr_colums) + colum] = true; 
} 

bool global::HSSMMatrix_ReturnMatrixEntryState(int row, int colum) {         
  returnb = HSSMMatrix[(row*nr_colums) + colum]; 
  return (returnb);
}

bool global::HSSMMatrix_ReturnHelixMap(int row) {         
  returnb = false;
  for(int j=0; j<nr_colums; j++) {
    if(HSSMMatrix[(row*nr_colums) + j]) {
      returnb = true;
    } 
  }
  return (returnb);
}

bool global::HSSMMatrix_ReturnStemStrandMap(int colum) {         
  returnb= false;
  for(int j=0; j<nr_rows; j++) { 
    if(HSSMMatrix[(j*nr_colums) + colum]) {
      returnb = true;
    } 
  }
  return (returnb);
}

int global::HSSMMatrix_ReturnStemStrandMapNumber(int colum) {         
  returnv = 0;
  for(int i=0; i<nr_rows; i++) {                 
    if(HSSMMatrix[(i*nr_colums) + colum]) {
      returnv++;
    }
  }
  return (returnv);
}

int global::HSSMMatrix_ReturnStemStrandMapHelixPos(int colum) {         
  returnv = 0;
  for(int i=0; i<nr_rows; i++) {                 
    if(HSSMMatrix[(i*nr_colums) + colum]) {
      returnv = i;
    }
  }
  return (returnv);
}


//***************************************************************************//
//                                                                           //
//  Functions related to the positions of the Helix numbers in the HN string //
//                                                                           //
//***************************************************************************//

void global::HelixPosArray_ChangeSize (int helixpos_size){ 
  delete[ ] helixposarray;
  helixposarray     = new int[helixpos_size];
  size_helixpos     = helixpos_size;
  
  for(int i=0; i<size_helixpos; i++) {
    helixposarray[i] = 0;
  }
}

int  global::HelixPosArray_ReturnSize (void) {
  returnv = size_helixpos;
  return (returnv);
}

void global::HelixPosArray_PrintArray(void) {         
  std::cout << "Helix positions in the Helix numbering string: " << endl;
  for(int i=0; i<size_helixpos; i++){                 
    std::cout << helixposarray[i] << " ";  
  }   
  std::cout << endl << endl;
}

void global::HelixPosArray_InsertValue(int array_pos, int string_pos) {
   helixposarray[array_pos] = string_pos;
}

int global::HelixPosArray_ReturnValue(int array_pos) {
  returnv = helixposarray[array_pos]; 
  return (returnv);
}

int  global::HelixPosArray_ReturnArrayPos(int string_pos) {
  for(int i=0; i<size_helixpos; i++){  
    if(helixposarray[i] == string_pos) {
      returnv = i; 
      std::cout << helixposarray[i] << endl;
    }
  }
  return (returnv);
}

//***************************************************************************//
//                                                                           //
//      Functions related to the Helix numbering names                       //
//                                                                           //
//***************************************************************************//

void global::HelixNamesArray_ChangeSize (int helixnames_size){ 
  delete[ ] helixnamearray;
  helixnamearray      = new std::string[helixnames_size];
  size_helixnames = helixnames_size;
}

int  global::HelixNamesArray_ReturnSize (void) {
  returnv = size_helixnames;
  return (returnv);
}

void global::HelixNamesArray_PrintArray (void){         
  std::cout << "Helix numbering names: " << endl;
  for(int i=0; i<size_helixnames; i++){                 
    std::cout << helixnamearray[i] << " ";  
  }   
  std::cout << endl;
}

int  global::HelixNamesArray_ReturnNamePos (std::string name) {
  returnv = -1;
  for(int i=0; i<size_helixnames; i++){  
    if(name.compare(helixnamearray[i]) == 0) {
      returnv = i; 
    }
  }
  return (returnv);
}

std::string  global::HelixNamesArray_ReturnName (int pos) {
  returnn = helixnamearray[pos]; 
  return (returnn);
}

void global::HelixNamesArray_InsertName(int pos, std::string name){
   helixnamearray[pos] = name;
}


int global::HelixNamesArray_FindComplementaryNamePos(std::string name){
  std::string newname;
  returnv = -1; // The value to return if a complementary name is not
		// found.
    
  // Is there a "'" in the helix strand name or not.
  if(name.find("'",0) != std::string::npos){
    newname = name.substr(0,name.length()-1);
  }
  else {
    newname = name.append("'");
  }
    
  // Goes trough the Helix numbering names to look for the position of
  // the complementary Helix strand. 
  for(int i=0; i<size_helixnames; i++){ 
    if(newname.compare(helixnamearray[i]) == 0) {
      returnv = i; 
    }
  }
  return (returnv);
}
