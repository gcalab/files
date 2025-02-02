/*****************************************************************************
 *                                                                           *
 *  This function transform an char to an integer. (Reads Paup matrix 
 *  values).                                                                 *
 *                                                                           *
 *                                                               MARTEN 2007 *
 *****************************************************************************/
  
int ch2in(char c) {
  
  int i;
  
  // 0 -> 9
  if(c == '0') i = 0;
  if(c == '1') i = 1;
  if(c == '2') i = 2;
  if(c == '3') i = 3;
  if(c == '4') i = 4;
  if(c == '5') i = 5;
  if(c == '6') i = 6;
  if(c == '7') i = 7;
  if(c == '8') i = 8;
  if(c == '9') i = 9;
  
  // A -> Z
  if(c == 'A') i = 10;
  if(c == 'B') i = 11;
  if(c == 'C') i = 12;
  if(c == 'D') i = 13;
  if(c == 'E') i = 14;
  if(c == 'F') i = 15;
  if(c == 'G') i = 16;
  if(c == 'H') i = 17;
  if(c == 'I') i = 18;
  if(c == 'J') i = 19;
  if(c == 'K') i = 20;
  if(c == 'L') i = 21;
  if(c == 'M') i = 22;
  if(c == 'N') i = 23;
  if(c == 'O') i = 24;
  if(c == 'P') i = 25;
  if(c == 'Q') i = 26;
  if(c == 'R') i = 27;
  if(c == 'S') i = 28;
  if(c == 'T') i = 29;
  if(c == 'U') i = 30;  
  if(c == 'V') i = 31;  
  if(c == 'W') i = 32;  
  if(c == 'X') i = 33;  
  if(c == 'Y') i = 34;  
  if(c == 'Z') i = 35;
  
  // a -> z
  if(c == 'a') i = 36;
  if(c == 'b') i = 37;
  if(c == 'c') i = 38;
  if(c == 'd') i = 39;
  if(c == 'e') i = 40;
  if(c == 'f') i = 41;
  if(c == 'g') i = 42;
  if(c == 'h') i = 43;
  if(c == 'i') i = 44;
  if(c == 'j') i = 45;
  if(c == 'k') i = 46;
  if(c == 'l') i = 47;
  if(c == 'm') i = 48;
  if(c == 'n') i = 49;
  if(c == 'o') i = 50;
  if(c == 'p') i = 51;
  if(c == 'q') i = 52;
  if(c == 'r') i = 53;
  if(c == 's') i = 54;
  if(c == 't') i = 55;
  if(c == 'u') i = 56;  
  if(c == 'v') i = 57;  
  if(c == 'w') i = 58;  
  if(c == 'x') i = 59;  
  if(c == 'y') i = 60;  
  if(c == 'z') i = 61;  
  
  // @ and %
  if(c == '@') i = 62;  
  if(c == '%') i = 63;  
 
  
  return(i);
}
