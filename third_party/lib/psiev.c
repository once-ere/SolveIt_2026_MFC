/***************************

 * Psi Evolver Version 0.9 *

 *      by Alvin Goh       *

 *      February 1996      *

 ***************************/



#include <mathlink.h>

#include <stdio.h>

#include <stdlib.h>

#include <math.h>

#include <string.h>



#if !WINDOWS_MATHLINK

#define TXDOUBLE(dn) MLPutDouble(stdlink,dn)

#else

#define TXDOUBLE(dn) dn==0?MLPutInteger(stdlink,dn):MLPutDouble(stdlink,dn)

#endif



#define  ROUND(dn)   floor(dn+0.5)



#define  NCHECK   100



/* Default simulation parameter values */

#define   JP_0   1000

#define   LAMBDA_0   1

#define   W_0   1

#define  TIME_0 0



/* Parameters and global variables */

unsigned int JP;

double Lambda, W, Time, Epsilon, EpsilonSqr, Delta;



/* Psi */

typedef struct {

   double Re;

   double Im;

} TCplx;

TCplx *Psi=NULL;



/* Potential */

double *V=NULL;



/*

 * Return the modulus-squared of

 * the given complex number.

 */

double AbSqr(TCplx *Z){

   return Z->Re*Z->Re+Z->Im*Z->Im;

}



/*

 * Send error message and $Failed

 * to Mathematica.

 */

void error(char *errmsg){

   char msg[100];

   sprintf(msg, "Message[%s]", errmsg);

   MLEvaluate(stdlink, msg);

   MLNextPacket(stdlink);

   MLNewPacket(stdlink);

   MLPutSymbol(stdlink, "$Failed");

}



/*

 * Set up other global variables

 * based on JP, Lambda and W.

 */

void setup(void){

   Epsilon=W/JP;

   EpsilonSqr=Epsilon*Epsilon;

   Delta=2*EpsilonSqr/Lambda;

   free(Psi);

   free(V);

   Psi=calloc(JP+1,sizeof(TCplx));

   V=calloc(JP+1,sizeof(double));

}



/*

 * Transmit the parameter list

 * {JP, Lambda, W and Time}

 * to Mathematica.

 */

void txparms(void){

   MLPutFunction(stdlink, "List", 4);

   TXDOUBLE((double)JP);

   TXDOUBLE(Lambda);

   TXDOUBLE(W);

   TXDOUBLE(Time);

}



/*

 * Transmit j+1 points of

 * Psi to Mathematica.

 */

void txpsi(unsigned int jm){

   unsigned int i,jlo,jhi;

   double jfactor,j;

   jfactor=(double)JP/(double)jm;

   MLPutFunction(stdlink, "List", jm+1);

   for(i=0;i<=jm;i++){

      MLPutFunction(stdlink, "Complex", 2);

      {

         TCplx tmp;

//         tmp=Psi[i*jfactor];

         j=i*jfactor;

         if((jlo=floor(j))<0)

            jlo=0;

         if((jhi=ceil(j))>JP)

            jhi=JP;

         if(jlo==jhi)

            tmp=Psi[(int)(i*jfactor)];

         else{

            tmp.Re=(Psi[jlo].Re*(jhi-j)+Psi[jhi].Re*(j-jlo))/(jhi-jlo);

            tmp.Im=(Psi[jlo].Im*(jhi-j)+Psi[jhi].Im*(j-jlo))/(jhi-jlo);

         }

         TXDOUBLE(tmp.Re);

         TXDOUBLE(tmp.Im);

      }

   }

}



/*

 * Transmit j+1 points of

 * V to Mathematica.

 */

void txpotential(unsigned int jm){

   unsigned int i,jlo,jhi;

   double jfactor,j;

   jfactor=(double)JP/(double)jm;

   MLPutFunction(stdlink, "List", jm+1);

   for(i=0;i<=jm;i++){

      {

         double tmp;

//         tmp=V[i*jfactor];

         j=i*jfactor;

         if((jlo=floor(j))<0)

            jlo=0;

         if((jhi=ceil(j))>JP)

            jhi=JP;

         if(jhi==jlo)

            tmp=V[(int)j];

         else

            tmp=(V[jlo]*(jhi-j)+V[jhi]*(j-jlo))/(jhi-jlo);

         TXDOUBLE(tmp);

      }

   }

}





double integrateabpsisq(double x1, double x2){

   unsigned int j, J1, J2;

   double res=0;

   if(x1<0) x1=0;

   if(x1>W) x1=W;

   if(x2<0) x2=0;

   if(x2>W) x2=W;

   if(x1>x2){

      double tmp;

      tmp=x1;

      x1=x2;

      x2=tmp;

   }

   J1=ROUND(x1/Epsilon);

   J2=ROUND(x2/Epsilon);

   if(J1!=J2){

      for(j=J1+1;j<=J2-1;j++)

         res+=AbSqr(Psi+j);

      res+=(AbSqr(Psi+J1)+AbSqr(Psi+J2))/2;

      res*=Epsilon;

   }

   return res;

}



/*

 * Initialize parameters and global

 * variables to defaults.

 */

void init(void){

   JP=JP_0;

   Lambda=LAMBDA_0;

   W=W_0;

   Time=TIME_0;



   setup();

   if((Psi==NULL)||(V==NULL))

      error("Init::memerr");

   else

      txparms();

}



/* Free memory in advance of program termination. */

void done(void){

   free(Psi);

   free(V);

   MLPutSymbol(stdlink, "$Okay");

}



/*

 * Set parameters and global variables

 * based on given arguments.

 */

void setparms(jp0, lambda0, w0, t0)

unsigned int jp0;

double lambda0, w0, t0;

{

   if((jp0==0)||(lambda0<=0)||(w0<=0)){

      error("SetParms::rngerr");

      return;

   }

   JP=jp0;

   Lambda=lambda0;

   W=w0;

   Time=t0;

   setup();

   if((Psi==NULL)||(V==NULL))

      error("SetParms::memerr");

   else

      txparms();

}



/*

void setpsi(void){

   unsigned int i;

   long n;

   char *fname;

   TCplx *tmppsi=NULL;

   MLGetFunction(stdlink, &fname, &n);

   MLDisownSymbol(stdlink, fname);

   MLGetFunction(stdlink, &fname, &n);

   printf("fname=%s, n=%d",fname, n);

   if((int)(--n)!=JP){

      MLNewPacket(stdlink);

      MLDisownSymbol(stdlink, fname);

      error("SetPsi::lenerr");

      return;

   }

   if(strcmp(fname,"List")){

      MLNewPacket(stdlink);

      MLDisownSymbol(stdlink, fname);

      error("SetPsi::notlist");

      return;

   }

   MLDisownSymbol(stdlink, fname);

   tmppsi=calloc(JP, sizeof(TCplx));

   if(tmppsi==NULL){

      MLNewPacket(stdlink);

      error("SetPsi::noalloc");

      return;

   }

   for(i=0;i<=JP;i++){

      MLGetFunction(stdlink, &fname, &n);

      if(strcmp(fname,"Complex")||(n!=2)){

         MLNewPacket(stdlink);

         MLDisownSymbol(stdlink, fname);

         error("SetPsi::notcplx");

      } else {

         int k,z;

         for(z=0;z<2;z++){

            switch(MLGetType(stdlink)){

               case MLTKREAL:MLGetDouble(stdlink,&tmppsi[i].Re);

               case MLTKINT:{MLGetInteger(stdlink,&k); tmppsi[i].Re=k;

                            }

               default:{MLNewPacket(stdlink);error("SetPsi::notnum");}

            }

            switch(MLGetType(stdlink)){

               case MLTKREAL:MLGetDouble(stdlink,&tmppsi[i].Im);

               case MLTKINT:{MLGetInteger(stdlink,&k); tmppsi[i].Im=k;

                            }

               default:{MLNewPacket(stdlink);error("SetPsi::notnum");}

            }

         }

      }

      free(Psi);

      Psi=tmppsi;

   }

}

*/



/*

 * Set real part of Psi

 * based on given RealList.

 */

void setpsireal(double *psireal, long listlen){

   unsigned int i,jhi,jlo;

   double jfactor,j;

   jfactor=(double)(listlen-1)/(double)JP;

   for(i=0;i<=JP;i++)

//      Psi[i].Re=psireal[i*jfactor];

   {

      j=i*jfactor;

      if((jlo=floor(j))<0)

         jlo=0;

      if((jhi=ceil(j))>(listlen-1))

         jhi=listlen-1;

      if(jlo==jhi)

         Psi[i].Re=psireal[(int)(i*jfactor)];

      else

         Psi[i].Re=(psireal[jlo]*(jhi-j)+psireal[jhi]*(j-jlo))/(jhi-jlo);

   }

   txpsi(JP);

}



/*

 * Set imaginary part of Psi

 * based on given RealList.

 */

void setpsiimag(double *psiimag, long listlen){

   unsigned int i,jlo,jhi;

   double jfactor,j;

   jfactor=(double)(listlen-1)/(double)JP;

   for(i=0;i<=JP;i++)

//      Psi[i].Im=psiimag[i*jfactor];

   {

      j=i*jfactor;

      if((jlo=floor(j))<0)

         jlo=0;

      if((jhi=ceil(j))>(listlen-1))

         jhi=listlen-1;

      if(jlo==jhi)

         Psi[i].Im=psiimag[(int)(i*jfactor)];

      else

         Psi[i].Im=(psiimag[jlo]*(jhi-j)+psiimag[jhi]*(j-jlo))/(jhi-jlo);

   }

   txpsi(JP);

}



/*

 * Set potential function

 * based on given RealList.

 */

void setpotential(double *tmparray, long listlen){

   unsigned int i,jlo,jhi;

   double jfactor,j;

   jfactor=(double)(listlen-1)/(double)JP;

   for(i=0;i<=JP;i++)

//      V[i]=tmparray[i*jfactor];

   {

      j=i*jfactor;

      if((jlo=floor(j))<0)

         jlo=0;

      if((jhi=ceil(j))>(listlen-1))

         jhi=listlen-1;

      if(jlo==jhi)

         V[i]=tmparray[(int)(i*jfactor)];

      else

         V[i]=(tmparray[jlo]*(jhi-j)+tmparray[jhi]*(j-jlo))/(jhi-jlo);

   }

   txpotential(JP);

}



/*

 * Set Psi to be a Gaussian

 * with mean position x0 and

 * std. dev. sigmaX0 and

 * mean momentum p0.

 */

void setgaussian(double x0, double sigmaX0, double p0){

   unsigned int i;

   double   x,a,y;



   x=-x0;

   a=1/sqrt(sqrt(2*M_PI)*sigmaX0);

   for(i=1;i<=JP-1;i++){

      x+=Epsilon;

      y=x/sigmaX0;

      y=a*exp(-y*y/4);

      Psi[i].Re=cos(p0*x)*y;

      Psi[i].Im=sin(p0*x)*y;

   }

   txpsi(JP);

}



/* Normalize Psi */

void normalizepsi(void){

   unsigned int i;

   double c;

   if((c=sqrt(integrateabpsisq(0, W)))!=0)

      for(i=0;i<=JP;i++){

         Psi[i].Re/=c;

         Psi[i].Im/=c;

      }

   txpsi(JP);

}



/*

 * Set V to be a square potential based

 * on the given RealList, e.g.

 * SetSquarePotential[{V0, x1, V1, x2, V2 ...}]

 * gives the potential function

 * V(x)=V0,  0<=x<x1,

 *     =V1, x1<=x<x2,

 *     =.......

 */

void setsquarepotential(double *tmparray, long listlen){

   if((listlen%2)!=0){

      unsigned int l,i,jboundary=0;

      for(l=0;l<(listlen-1);l+=2)

         for(i=jboundary;i<(jboundary=tmparray[l+1]/Epsilon);i++)

            V[i]=tmparray[l];

      for(i=jboundary;i<=JP;i++)

         V[i]=tmparray[l];

   }

   txpotential(JP);

}



/*

 * Set V to be a step potential, i.e.

 * V(x) = v1,     0<= x <xstep

 *      = v2, xstep<= x <=W

 */

void setsteppotential(double v1, double xstep, double v2){

   unsigned int i,jstep;

   if(xstep<0) xstep=0;

   else if(xstep>W) xstep=W;

   jstep=xstep/Epsilon;

   for(i=0;i<jstep;i++)

      V[i]=v1;

   for(i=jstep;i<=JP;i++)

      V[i]=v2;

   txpotential(JP);

}



void settime(double t0){

   Time=t0;

   TXDOUBLE(Time);

}



/* Send jm+1 points of Psi to Mathematica. */

void getpsi(unsigned int jm){

   txpsi(jm);

}



/* Send jm+1 points of V to Mathematica. */

void getpotential(unsigned int jm){

   txpotential(jm);

}



/*

 * Send jm+1 points of the

 * modulus-squared of Psi

 * to Mathematica.

 */

void getprobdensity(unsigned int jm){

   unsigned int i,jlo,jhi;

   double jfactor,j;

   jfactor=(double)JP/(double)jm;

   MLPutFunction(stdlink, "List", jm+1);

   for(i=0;i<=jm;i++){

      {

         double tmp;

         j=i*jfactor;

         if((jlo=floor(j))<0)

            jlo=0;

         if((jhi=ceil(j))>JP)

            jhi=JP;

         if(jlo==jhi)

            tmp=AbSqr(Psi+(int)(i*jfactor));

         else

            tmp=(AbSqr(Psi+jlo)*(jhi-j)+AbSqr(Psi+jhi)*(j-jlo))/(jhi-jlo);

         TXDOUBLE(tmp);

      }

   }

}



/*

 * Send the value of the integral of

 * |Psi|^2 between x1 and x2 to

 * Mathematica.

 */

void getprobability(x1, x2)

double x1, x2;

{

   TXDOUBLE(integrateabpsisq(x1,x2));

}



/* Send the current time to Mathematica. */

void gettime(void){

   TXDOUBLE(Time);

}



/* Send the value of Delta,

 * the time-increment,

 * to Mathematica.

 */

double getdelta(void){

   return Delta;

}





/* Send the current parameter

 * settings to Mathematica.

 */

void getparms(void){

   txparms();

}



/* Evolve the wave function Psi

 * forward by a time deltat.

 * Based on Goldberg A, Schey HM, Schwartz JL.

 * Computer-Generated Motion Pictures of

 * One-Dimensional Quantum

 * Mechanical Transmission and

 * Reflection Phenomena.

 * Am J Phys 1967; 35 (3): 177-86.

 */

void evolve(double deltat){

   TCplx *E,*F;

   E=calloc(JP+1, sizeof(TCplx));

   F=calloc(JP+1, sizeof(TCplx));

   if((E!=NULL)&&(F!=NULL)){

      unsigned int j,n,ntotal;

      double AbEPrevSqr;

      ntotal=deltat/Delta;

      for(n=0; (n<ntotal) && !MLAbort; n++){

         E[1].Re=2+EpsilonSqr*V[1];

         E[1].Im=-Lambda;

         F[1].Re=-Psi[2].Re+(2+EpsilonSqr*V[1])*Psi[1].Re-Lambda*Psi[1].Im;

         F[1].Im=-Psi[2].Im+(2+EpsilonSqr*V[1])*Psi[1].Im+Lambda*Psi[1].Re;

         for(j=2;j<JP;j++){

            AbEPrevSqr=AbSqr(E+j-1);

            E[j].Re=2+EpsilonSqr*V[j]-E[j-1].Re/AbEPrevSqr;

            E[j].Im=-Lambda+E[j-1].Im/AbEPrevSqr;

            F[j].Re=-Psi[j+1].Re-Psi[j-1].Re+(2+EpsilonSqr*V[j])*Psi[j].Re

               -Lambda*Psi[j].Im+(F[j-1].Re*E[j-1].Re+F[j-1].Im*E[j-1].Im)

               /AbEPrevSqr;

            F[j].Im=-Psi[j+1].Im-Psi[j-1].Im+(2+EpsilonSqr*V[j])*Psi[j].Im

               +Lambda*Psi[j].Re+(F[j-1].Im*E[j-1].Re-F[j-1].Re*E[j-1].Im)

               /AbEPrevSqr;

         }

         AbEPrevSqr=AbSqr(E+(j=JP-1));

         Psi[j].Re=(-F[j].Re*E[j].Re-F[j].Im*E[j].Im)/AbEPrevSqr;

         Psi[j].Im=(-F[j].Im*E[j].Re+F[j].Re*E[j].Im)/AbEPrevSqr;

         for(j=JP-2;j>0;j--){

            AbEPrevSqr=AbSqr(E+j);

            Psi[j].Re=((Psi[j+1].Re-F[j].Re)*E[j].Re+

               (Psi[j+1].Im-F[j].Im)*E[j].Im)/AbEPrevSqr;

            Psi[j].Im=((Psi[j+1].Im-F[j].Im)*E[j].Re-

               (Psi[j+1].Re-F[j].Re)*E[j].Im)/AbEPrevSqr;

         }

         Time+=Delta;

//         if((n%NCHECK)==0)

//            MLCallYieldFunction(MLYieldFunction(stdlink), stdlink,

//               (MLYieldParameters)0 );

      }

//      if(MLAbort) {

//         MLPutFunction(stdlink, "Abort", 0);

//      } else

         TXDOUBLE(Time);

   } else

      error("Evolve::memerr");

   free(E);

   free(F);

}





