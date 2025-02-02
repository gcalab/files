/*****************************************************************************
 *                                                                           *
 *  This function reads the input from the command line.                     *
 *                                                               MARTEN 2007 *
 *****************************************************************************/
  
void inputc(int c) {
  
  switch (c) {
  case 'V': { 
    std::cout << endl << endl << endl 
	      << "    MARTEN version 0.1" 
	      << endl << endl << endl ;
    exit(0);
  }
  case 'h': { 
    phelp ();
    exit(0);
  }
  case 'v': { 
    gstore.v_option = true;
    break;
  }
  case 'l': { 
    gstore.l_option = true;
    break;
  }
  case 'r': { 
    gstore.r_option = true;
    break;
  }
  case 'R': { 
    gstore.R_option = true;
    break;
  }
  case 'S': { 
    gstore.S_option = true;
    break;
  }
  case 'g': { 
    gstore.g_option = true;
    break;
  }
  case 'a': { 
    gstore.a_option = true;
    gstore.s_option = true;
    
    break;
  }
  case 'e': { 
    gstore.e_option = true;
    break;
  }
  case 'm': { 
    gstore.m_option = true;
    break;
  }
  case 'n': { 
    gstore.n_option = true;
    gstore.p_option = true;
        break;
  }
  case 's': { 
    gstore.s_option = true;
    gstore.e_option = true;
    break;
  }
  case 'p': { 
    gstore.p_option = true;
    break;
  }
  case 'P': { 
    gstore.P_option = true;
    gstore.e_option = true;
    gstore.s_option = true;
    gstore.p_option = true;
    gstore.a_option = true;
    gstore.m_option = true;
    gstore.n_option = true;
    gstore.g_option = true;
    break;
  }
  case '?': {
    std::cerr << "\n\n Invalid option. Try -h instead !!!! "       << endl;
    exit(0);
  }
    
  }
    
}
