READMEC for zufall random number package (C version)
------- --- ------ ------ ------ -------
This package contains a portable random number generator set
for: uniform (u in [0,1)), normal (<g> = 0, <g^2> = 1), and
Poisson distributions. The basic module, the uniform generator,
uses a lagged Fibonacci series generator:

              t    = u(n-273) + u(n-607)
              u(n) = t - float(int(t))

where each number generated, u(k), is floating point. Since
the numbers are floating point, the left end boundary of the
range contains zero. This package is portable except that
the test package contains some machine dependent timing data.
These are cycle times (in seconds) for NEC SX-3, Fujitsu VP2200, 
Cray Y-MP, and Sun-4. Select your favorite and comment out the 
others. There are also vectorization directives for Cray Y-MP
machines in the form of "pragma _CRI", which should be ignored
although perhaps complained about by other compilers. Otherwise 
the package is portable and returns the same set of floating 
point numbers up to word precision on any machine. 

External documentation, "Lagged Fibonacci Random Number Generators
for the NEC SX-3," is to be published in the International
Journal of High Speed Computing (1994). Otherwise, ask the
author: 

         W. P. Petersen 
         IPS, RZ F-5
         ETHZ
         CH 8092, Zurich
         Switzerland

e-mail:  wpp@ips.ethz.ch.

The package contains the following routines:

------------------------------------------------------
UNIFORM generator routines:

      int zufalli_(seed)
      int seed;
/* initializes struct containing seeds. if seed=0,
   the default value is 1802. */

      int zufall_(n,u)
      int n;
      double u[n];
/*  returns set of n uniforms u[0], ..., u[n-1]. */

      int zufallsv_(zusave)
      double zusave[608];
/*  saves buffer and pointer in zusave, for later restarts */

      int zufallrs_(zusave)
      double zusave[608];
/*  restores seed buffer and pointer from zusave */
------------------------------------------------------

NORMAL generator routines:

      int normalen_(n,g)
      int n;
      double g[n];
/*  returns set of n normals g[0], ..., g[n-1] such that
    mean <g> = 0, and variance <g**2> = 1. */

      int normalsv_(normsv)
      double normsv[1634];
/*  saves zufall seed buffer and pointer in normsv
    buffer/pointer for normalen restart also in normsv */

      int normalrs_(normsv)
      double normsv[1634];
/*  restores zufall seed buffer/pointer and 
    buffer/pointer for normalen restart from normsv */
------------------------------------------------------

POISSON generator routine:

      int fische_(n,mu,q)
      integer n,q[n];
      double mu;
/*  returns set of n integers q, with poisson
    distribution, density p(q,mu) = exp(-mu) mu**q/q!
    
    USE zufallsv and zufallrs for stop/restart sequence */

----------------- END READMEC ------------------------
------------------------------------------------------
