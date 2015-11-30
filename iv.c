/* Searches for a kernel of the Burau representation for n=4.
 *
 * This version looks for an arc from the basepoint to a puncture whose lift
 * does not homologically cross the line between the last two punctures.
 * It checks that the crossing numbers describing the arc are coprime.
 */

#include<stdio.h>

#define STARTTOTAL 1300
#define ENDTOTAL 2000
#define HUMPS 3
#define LEVELS 10000
#define STARTLEVEL 5000

int gcd(int i,int j)
{
  while(j)
  {
    i = i-j*(i/j);
    /* Exchange i and j */
    i ^= j;
    j ^= i;
    i ^= j;
  }
  return(i);
}

/*Check to see if a described scc gives a kernel of the Burau representation
 *Return V for victory, 0 for no, or error codes: o for overflow
 */
char example(int* nextcut, int* flcut, int leftend)
{
  int cut, newcut;
  int level = STARTLEVEL;
  int i;
  int lo = STARTLEVEL;
  int hi = STARTLEVEL-1;  /*record hi and lo levels reached*/
  int parity;
  int norm = 0;
  int togo;
  int rightend;
  int poly[LEVELS];

  togo = nextcut[HUMPS-1]/2;
  rightend = leftend + nextcut[HUMPS-1]/2;
  cut = nextcut[0]/2;

  while(togo >= norm)
  {
    /* Check for Error o: overflow */
    if(level<0 || level>=LEVELS) return('o');

    /* Clear space in poly */
    while(lo>level) poly[--lo] = 0;
    while(hi<level) poly[++hi] = 0;

    /* Alter poly and go through */
    if(parity)
    {
      if(poly[level]++ >= 0) norm++;
      else norm--;
    }
    else
    {
      if(poly[level]-- <= 0) norm++;
      else norm--;
    }
    togo--;
    cut = 2*leftend-1-cut;
    
    if(cut<0) cut+= nextcut[HUMPS-1];
    else if(cut>=nextcut[HUMPS-1]) cut-= nextcut[HUMPS-1];

    /* Set i = current hump */
    for(i = 0; nextcut[i] <= cut; i++);

    /* Go over the hump */
    newcut = flcut[i] - cut;
    if(newcut < cut)
    {
      if(i) level--;
      else
      {
        parity = !parity;
        level += HUMPS+1;
      }
      if(newcut < leftend && cut >= leftend) level--;
      if(newcut < rightend && cut >= rightend) level--;
    }
    else if(newcut > cut)
    {
      if(i) level++;
      else
      {
        parity = !parity;
        level -= (HUMPS+1);
      }
      if(newcut >= leftend && cut < leftend) level++;
      if(newcut >= rightend && cut < rightend) level++;
    }
    else
    {
      if(norm) return(0);
      if(togo) return('v');
      return('V');
    }
    cut = newcut;
  }
  return(0);
}

printexamples(int* width,FILE* fp)
{
  int nextcut[HUMPS]; /*1+number of last cut in that hump*/
  int flcut[HUMPS];   /*first cut number + last cut number */
  int i;
  int leftend;
  char error;
  int gcdwidths = 0; /* The gcd of the values of width */

  /* Initialize nextcut, flcut, gcdwidths */
  for(i=0; i<HUMPS; i++)
  {
    gcdwidths = gcd(width[i], gcdwidths);
    nextcut[i] = (i? nextcut[i-1]: 0) + width[i];
    flcut[i] =   (i? nextcut[i-1]: 0) + nextcut[i] - 1;
  }
  for(leftend = nextcut[HUMPS-1]/2; leftend>=0; leftend--)
  {
    if(gcd(gcdwidths,leftend) == 1)
    {
      error = example(nextcut,flcut,leftend);
      if(error) 
      {
        fprintf(fp,"Error %c: widths %d,%d,%d, leftend %d\n",
                            error,width[0],width[1],width[2],leftend);
        fflush(fp);
      }
    }
  }
}

main()
{
  FILE* fp;
  int total;
  int width[HUMPS];
  int leftend;
  char error = 0;

  fp = fopen("ivlog","w");
  for(total = STARTTOTAL; total<=ENDTOTAL; total += 4)
  {
    fprintf(fp,"%d\n",total);
    fflush(fp);
    for(width[0] = 3; width[0]<=total; width[0]+=2)
      for(width[1] = 0; width[0]+width[1] <= total; width[1]+=2)
      {
        width[2] = total - width[0] - width[1];
        printexamples(width,fp);
      }
  }
  fprintf(fp,"Done checking from %d to %d\n",STARTTOTAL,ENDTOTAL);
  fclose(fp);
}

  

