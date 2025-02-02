int basetall_A(ord,teller)
     char ord[30];
     int teller;
{
  int c,A;
  c=0,A=0;
  while(c<teller)
    {
      if(ord[c]=='A') A=A+1;
      c=c+1; 
    }
  return A;
}

int basetall_C(ord,teller)
     char ord[30];
     int teller;
{
  int c,d,A,C,G,T;
  c=0,d=0,A=0,C=0,G=0,T=0;
  while(c<teller)
    {
      if(ord[c]=='C') C=C+1;
      c=c+1; 
    }
  return C;
}

int basetall_G(ord,teller)
     char ord[30];
     int teller;
{
  int c,G;
  c=0,G=0;
  while(c<teller)
    {
      if(ord[c]=='G') G=G+1;
      c=c+1; 
    }
  return G;
}

int basetall_T(ord,teller)
     char ord[30];
     int teller;
{
  int c,T;
  c=0,T=0;
  while(c<teller)
    {
      if(ord[c]=='T') T=T+1;
      c=c+1; 
    }
  return T;
}
 
printbase(ut,antall_A,antall_C,antall_G,antall_T,ord,teller)
     char ord[30];
     int  teller,antall_A,antall_C,antall_G,antall_T;
     
{
  int c,g,a_A,a_C,a_G,a_T,q=0;
  a_A=basetall_A(ord,teller); 
  a_C=basetall_C(ord,teller);
  a_G=basetall_G(ord,teller); 
  a_T=basetall_T(ord,teller);
  
  if(a_A==antall_A && a_C==antall_C && a_G==antall_G && a_T==antall_T)
    { 
      for(c=0; c<=teller-1; c++) 
	{
	  /* printf("%c",ord[c]); */ 
	  fprintf(ut,"%c",ord[c]); 
	}
      q=q+1;
      /* printf("\n"); */ fprintf(ut,"\n"); 
    } 
  return q;
}


void printheader()
{ printf("\n\n");
  printf("**************************************\n");
  printf("**                                  **\n");
  printf("**      DNA-STRENG GENERATOR        **\n");
  printf("**         USIT,UIO,NORWAY          **\n");
  printf("**           29.09.1999             **\n");
  printf("**                                  **\n");
  printf("**************************************\n");
  printf("\n\n"); 
}

void strengomformer(streng,i)
     char streng[30];
     int  i;
{
  int k;
  for(k=0; k<i; k++)
    {
      streng[k]='A'; 
      
    }
  printf("\n");
}

void vertikalomformer_A(streng,j)
     char streng[30];
     int  j;
{
  streng[j]='A';
}

void vertikalomformer_C(streng,j)
     char streng[30];
     int  j;
{
  streng[j]='C';
}

void vertikalomformer_G(streng,j)
     char streng[30];
     int  j;
{
  streng[j]='G';
}

void vertikalomformer_T(streng,j)
     char streng[30];
     int  j;
{
  streng[j]='T';
}



void vertikalrokering(j,n,streng)
     int j,n;
     char streng[30];
{
  if(n==0) vertikalomformer_A(streng,j);
  if(n==1) vertikalomformer_C(streng,j);
  if(n==2) vertikalomformer_G(streng,j);
  if(n==3) vertikalomformer_T(streng,j);
}


int prosedyre(ut,antall_A,antall_C,antall_G,antall_T,i,j,streng)
     int j,i,antall_A,antall_C,antall_G,antall_T;
     char streng[30];
{
  int n,s,q=0;
  s=0; 
  while(j>=0 && j<i)
	{
	  for(n=0; n<4; n++)
	    {
	      if(i==1) { vertikalrokering(j,n,streng);
	      printbase(ut,antall_A,antall_C,antall_G,antall_T,streng,i); }
	      if(j==i-1 && i!=1) { strengomformer(streng,i);
	      vertikalrokering(j,n,streng);
	      q= q + prosedyre_ned(ut,antall_A,antall_C,antall_G,antall_T,i,j,streng); }
	                 
	    }
	  j=j+1; 
	} return q;
}

int prosedyre_ned(ut,antall_A,antall_C,antall_G,antall_T,i,j,streng)
     int j,i,antall_A,antall_C,antall_G,antall_T;
     char streng[30];
{
  int n,s,q=0;
  s=0; j=j-1;
  if(j==0)
    {
      for(n=0; n<4; n++)
	{
	  vertikalrokering(j,n,streng);
	  q= q + printbase(ut,antall_A,antall_C,antall_G,antall_T,streng,i);
	}
    }
  else
    {
      for(n=0; n<4; n++)
	{
	  vertikalrokering(j,n,streng);
	  q=q + prosedyre_ned1(ut,antall_A,antall_C,antall_G,antall_T,i,j,streng);
	}
    }
   return q;   
}


int prosedyre_ned1(ut,antall_A,antall_C,antall_G,antall_T,i,j,streng)
     int j,i,antall_A,antall_C,antall_G,antall_T;
     char streng[30];
{
  int n,s,q=0;
  s=0; j=j-1;
  if(j==0)
    {
      for(n=0; n<4; n++)
	{
	  vertikalrokering(j,n,streng);
	  q=q + printbase(ut,antall_A,antall_C,antall_G,antall_T,streng,i);
	}
    }
  else
    {
      for(n=0; n<4; n++)
	{
	  vertikalrokering(j,n,streng);
	  q=q + prosedyre_ned(ut,antall_A,antall_C,antall_G,antall_T,i,j,streng);
	}
    }
   return q;
}

