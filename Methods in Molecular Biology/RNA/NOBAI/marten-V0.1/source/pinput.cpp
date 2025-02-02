/****************************************************************************
 *                                                                          *
 *     This function prints the program inputs .                            *
 *                                                              MARTEN 2007 *
 ****************************************************************************/


void pinput(void)                                    
{
   
  std::cout << "\n\n                  MARTEN\n";
  std::cout << "                  ------\n";
  std::cout                      << endl;
  std::cout << " Options used :" << endl;
  
  if(gstore.v_option) { 
    std::cout << "  -v    : Verbose output."                           << endl;
  }
  if(gstore.e_option) { 
    std::cout << "  -e    : Eliminate gaps ('-') within []."           << endl;
  } 
  if(gstore.p_option) { 
    std::cout << "  -p    : Remove  non-standard pairing info."        << endl;
  }  
  if(gstore.s_option) { 
    std::cout << "  -s    : Sum bases in bulge. (Includes -e)"         << endl;
  }  
  if(gstore.a_option) { 
    std::cout << "  -a    : Sum all bulges. (Includes -s)"             << endl;
  }  
  if(gstore.m_option) { 
    std::cout << "  -m    : Sum bases outside stems.  "                << endl;
  }  
  if(gstore.n_option) { 
    std::cout << "  -n    : Sum bases in stems. (Includes -p)"         << endl;
  }  
  if(gstore.g_option) { 
    std::cout << "  -g    : Eliminate gaps ('-') outside []."          << endl;
  }   
  if(gstore.P_option) { 
    std::cout << "  -P    : Make PAUP matrix."                         << endl;
  }   
  if(gstore.x_option) { 
    std::cout << "  -x    : Specify helix search space (Default=5)."   << endl;
  }
  if(gstore.S_option) { 
    std::cout << "  -S    : Change number of character states (D=62)." << endl;
  }  
  if(gstore.l_option) { 
    std::cout << "  -l    : List min and max values for each colum."   << endl;
  }  
  if(gstore.r_option) { 
    std::cout << "  -r    : Make a flipped paup matrix."               << endl;
  } 
  if(gstore.R_option) { 
    std::cout << "  -R    : Add complementary numbers for each stem"   << endl;
  }  
}
