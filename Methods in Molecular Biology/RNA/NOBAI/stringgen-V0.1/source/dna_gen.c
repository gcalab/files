        /* Dette programmet er skrevet av Vegeir Knudsen,
	   ansatt ved Universitetets Senter for Informasjons-
	   Teknologi, Universitetet I Oslo, Norway. Fullfoert 
	   29.09.1999. Koden til programmet skal ikke sprees
	   uten mitt samtykke!
	   
	                                 Vegeir Knudsen.      */




#include <stdio.h>
#include "dna_gen1.c"

main(argc, argv)
     int    argc;
     char  *argv[];
{  
  FILE  *inn, *ut;
  int    antall_A,antall_C,antall_G,antall_T;
  int    teller,i,j,k,l,n,s,antall_r,t,q=0;
  int    a_a_base_A,a_a_base_C,a_a_base_G,
         a_a_base_T;                     /* antallet av en basene */
  char   bokstav,streng[15];
  
  printheader();
  inn=fopen(argv[1], "r");
  ut =fopen(argv[2], "w");

/* Program-del */ 
  antall_A=0,antall_C=0,antall_G=0,antall_T=0;
  i=0,j=0,k=0,l=0,n=0,antall_r=0,s=0;
  printf("Den innleste DNA-strengen ser slik ut:\n\n");
  while(fscanf(inn, "%c", &bokstav) != EOF)
    {
      if(bokstav!=' ' && bokstav!='\n')
	{
	  streng[i]=bokstav;
	  if(bokstav=='A') antall_A=antall_A+1;
	  if(bokstav=='C') antall_C=antall_C+1;
	  if(bokstav=='G') antall_G=antall_G+1;
	  if(bokstav=='T') antall_T=antall_T+1;
	  printf("%c",streng[i]);
	  i=i+1;
	}
    }
  
  /* Sjekker om input dequencen er for lang */
  if(i > sizeof(streng)) {
    printf("\n\nInput sekvensen kan ikke bestaa av mer enn %d baser. Avslutter omrokgen!\n\n", 
	   sizeof(streng));
    exit(0);
  }
  
  
  printf("\n\n");
  printf("A'er= %d, ",antall_A); 
  printf("C'er= %d, ",antall_C);
  printf("G'er= %d, ",antall_G);
  printf("T'er= %d\n\n",antall_T);
  
  printf("Kombinasjonene skrives til filen: %1s\n\n", argv[2]);
  
  strengomformer(streng,i);
  q=prosedyre(ut,antall_A,antall_C,antall_G,antall_T,i,j,streng);
  
  printf("\n\nAntall genererte strenger er: %d\n\n",q);
  
  close(inn);
  fclose(ut);
}


