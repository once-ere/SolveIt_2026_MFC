#include <stdio.h>
#include "gmp.h"         /* All MP programs need to include gmp.h */

main (void)
{
  mpz_t a, b, p;
  
  /* Initialize variables */
  mpz_init (a);
  mpz_init (b);
  mpz_init (p);
  
  /* Assign a and b from base 10 strings in argv */
  mpz_set_str (a, "98365871231256752134", 10);
  mpz_set_str (b, "319378318340103345227", 10);
  
  /* Multiply a and b and put the result in p */
  mpz_mul (p, a, b);
  
  /* Print p in base 10 */
  mpz_out_str (stdout, 10, p);
  fputc ('\n', stdout);
  
  /* Clear out variables */
  mpz_clear (a);
  mpz_clear (b);
  mpz_clear (p);
  /*exit (0);*/
}

