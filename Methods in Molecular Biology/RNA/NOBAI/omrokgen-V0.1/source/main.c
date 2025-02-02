
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

main(argc, argv)
     int    argc;
     char  *argv[];
{  
  FILE  *inn, *ut;
  int randum_nr;
  int i;  
  float ii;
  time_t t;
	      
  int    antall_A,antall_C,antall_G,antall_T; /* antallet av en basene */
  int    j,k,l,n,s,antall_runder,q;
  char   bokstav,mellomlagring,ar[100];
  static char streng[5000000],streng_manu[5000000];
  
  inn=fopen(argv[1], "r"); /* AApner for lesing fra fil, plassert som 
			      nr.2 i kommandoen. */
  ut =fopen(argv[2], "w"); /* AApner for lesing til fil, plassert som 
			      nr.3 i kommandoen. */


/* ------------------------------------------------------------------------- */
/* ------------- *      Syntakskontroll     * ------------------------------ */
/* ------------------------------------------------------------------------- */
  
  
  if(argc < 3) 
    {
      printf("\nFeil syntaks! Husk aa ha med baade input og"
	     " output filer.\n\n");
      exit(0);
    }
  if(inn == 0) {
    printf("Finner ikke den spesifiserte input-filen: %s\n", argv[1]);
    exit(0);
  }
  if(ut == 0) {
    printf("Klarte ikke a opprette den spesifiserte output-filen: %s\n", 
	   argv[2]);
    exit(0);
  }

/* ------------------------------------------------------------------------- */
/* ------------- *        Programdel        * ------------------------------ */
/*-------------------------------------------------------------------------- */


  antall_A=0,antall_C=0,antall_G=0,antall_T=0;
  i=0,j=0,k=0,l=0,n=0,antall_runder=0,s=0;
  
  while(fscanf(inn, "%c", &bokstav) != EOF )     /* Leser fra input-fila */ 
    {
      if(bokstav!=' ' && bokstav!='\n')
	{
	  streng[i]=bokstav;
	  if(bokstav=='A') antall_A=antall_A+1;
	  if(bokstav=='C') antall_C=antall_C+1;
	  if(bokstav=='G') antall_G=antall_G+1;
	  if(bokstav=='T') antall_T=antall_T+1;
	  i=i+1;
	}
    }
  
  printf("\n\n");                                 /* Skriver ut overskriften*/
  printf("**************************************\n");
  printf("**                                  **\n");
  printf("**      DNA-STRENG OMROKERER        **\n");
  printf("**         USIT,UIO,NORWAY          **\n");
  printf("**           09.11.1999             **\n");
  printf("**                                  **\n");
  printf("**************************************\n");
  printf("\n\n");

  
  printf("Det totale antallet baser i strengen er : %d\n"
	 "Hvor antallet av de forskjellige basene er:\n\n",i);
  printf("A= %d, ",antall_A); 
  printf("C= %d, ",antall_C);
  printf("G= %d, ",antall_G);
  printf("T= %d\n\n",antall_T);
  
  printf("Kombinasjonene skrives til filen: %1s\n\n", argv[2]);
  
  printf("Under foelger den innleste DNA-strengen pluss alle"
	 " de andre omrokerte strengene:\n\n");
  printf(">%s\n\n\n",streng);
  
  
  (void) time(&t);
  srand(t);                              /* initialisering */
  ii=i-1;	    
  i=strlen(streng);
  antall_runder=atoi(argv[3]);           /* Koverterer fra en char to int */
  
  strcpy(streng_manu,streng);
  for(l=0; l<antall_runder; l++) { 
    for(j=0; j<3; j++) {
      for(k=0; k<i; k++) {
	
	randum_nr=1+((ii)*rand()/(RAND_MAX+1.0));  
	
	mellomlagring         =streng_manu[randum_nr];
	streng_manu[randum_nr]=streng_manu[k];
	streng_manu[k]        =mellomlagring;
      }  
    }
    printbase(ut,antall_A,antall_C,antall_G,antall_T,streng_manu,i);
  }
  printf("\n");
}

/* ------------------------------------------------------------------------- */
/* ------------- *        Funksjoner        * ------------------------------ */
/* ------------------------------------------------------------------------- */







int basetall_A(streng_manu,i)
     char streng_manu[300];
     int i;
{
  int c,A;
  c=0,A=0;
  while(c<i)
    {
      if(streng_manu[c]=='A') A=A+1;
      c=c+1; 
    }
  return A;
}

int basetall_C(streng_manu,i)
     char streng_manu[300];
     int i;
{
  int c,d,A,C,G,T;
  c=0,d=0,A=0,C=0,G=0,T=0;
  while(c<i)
    {
      if(streng_manu[c]=='C') C=C+1;
      c=c+1; 
    }
  return C;
}

int basetall_G(streng_manu,i)
     char streng_manu[300];
     int i;
{
  int c,G;
  c=0,G=0;
  while(c<i)
    {
      if(streng_manu[c]=='G') G=G+1;
      c=c+1; 
    }
  return G;
}

int basetall_T(streng_manu,i)
     char streng_manu[300];
     int i;
{
  int c,T;
  c=0,T=0;
  while(c<i)
    {
      if(streng_manu[c]=='T') T=T+1;
      c=c+1; 
    }
  return T;
}


int printbase(ut,antall_A,antall_C,antall_G,antall_T,streng_manu,i)
     char streng_manu[500];
     int  i,antall_A,antall_C,antall_G,antall_T;
     FILE *ut;
{
  int c,g,a_A,a_C,a_G,a_T;
  a_A=basetall_A(streng_manu,i); 
  a_C=basetall_C(streng_manu,i);
  a_G=basetall_G(streng_manu,i); 
  a_T=basetall_T(streng_manu,i);
  
  if(a_A==antall_A && a_C==antall_C && a_G==antall_G && a_T==antall_T)
    { 
      printf(">"); fprintf(ut,">");
      for(c=0; c<=i-1; c++) 
	{
	  printf("%c",streng_manu[c]); 
	  fprintf(ut,"%c",streng_manu[c]); 
	}
      printf("\n"); fprintf(ut,"\n");
    }
  return 1;
}









