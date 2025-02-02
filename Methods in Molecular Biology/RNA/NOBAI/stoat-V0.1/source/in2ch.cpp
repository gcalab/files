/*****************************************************************************
 *                                                                           *
 *  This function transform an integer to a char. (Used in Paup matrix).     *
 *                                                                           *
 *                                                                STOAT 2007 *
 *****************************************************************************/
  
char in2ch(int i) {
  
  char cha_char;
  
  if(i == 0)  cha_char ='0';
  if(i == 1)  cha_char ='1';
  if(i == 2)  cha_char ='2';
  if(i == 3)  cha_char ='3';
  if(i == 4)  cha_char ='4';
  if(i == 5)  cha_char ='5';
  if(i == 6)  cha_char ='6';
  if(i == 7)  cha_char ='7';
  if(i == 8)  cha_char ='8';
  if(i == 9)  cha_char ='9';
  
  // A -> Z
  if(i == 10) cha_char ='A';
  if(i == 11) cha_char ='B';
  if(i == 12) cha_char ='C';
  if(i == 13) cha_char ='D';
  if(i == 14) cha_char ='E';
  if(i == 15) cha_char ='F';
  if(i == 16) cha_char ='G';
  if(i == 17) cha_char ='H';
  if(i == 18) cha_char ='I';
  if(i == 19) cha_char ='J';
  if(i == 20) cha_char ='K';
  if(i == 21) cha_char ='L';
  if(i == 22) cha_char ='M';
  if(i == 23) cha_char ='N';
  if(i == 24) cha_char ='O';
  if(i == 25) cha_char ='P';
  if(i == 26) cha_char ='Q';
  if(i == 27) cha_char ='R';
  if(i == 28) cha_char ='S';
  if(i == 29) cha_char ='T';
  if(i == 30) cha_char ='U';
  if(i == 31) cha_char ='V';
  if(i == 32) cha_char ='W';
  if(i == 33) cha_char ='X';
  if(i == 34) cha_char ='Y';
  if(i == 35) cha_char ='Z';
  
  // a -> z
  if(i == 36) cha_char ='a';
  if(i == 37) cha_char ='b';
  if(i == 38) cha_char ='c';
  if(i == 39) cha_char ='d';
  if(i == 40) cha_char ='e';
  if(i == 41) cha_char ='f';
  if(i == 42) cha_char ='g';
  if(i == 43) cha_char ='h';
  if(i == 44) cha_char ='i';
  if(i == 45) cha_char ='j';
  if(i == 46) cha_char ='k';
  if(i == 47) cha_char ='l';
  if(i == 48) cha_char ='m';
  if(i == 49) cha_char ='n';
  if(i == 50) cha_char ='o';
  if(i == 51) cha_char ='p';
  if(i == 52) cha_char ='q';
  if(i == 53) cha_char ='r';
  if(i == 54) cha_char ='s';
  if(i == 55) cha_char ='t';
  if(i == 56) cha_char ='u';  
  if(i == 57) cha_char ='v';  
  if(i == 58) cha_char ='w';  
  if(i == 59) cha_char ='x';  
  if(i == 60) cha_char ='y';  
  if(i == 61) cha_char ='z';  
  
  // @ and %
  if(i == 62) cha_char ='@';  
  if(i == 63) cha_char ='%';  
  
  if(i >= 63) cha_char ='%';  
  
  return(cha_char);
}
   
