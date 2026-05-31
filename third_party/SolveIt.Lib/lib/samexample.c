/*-***************************************************************************
 *
 *  File Name: samexample.c
 *
 *
 *  Description:  It implements Square and Multiply algorithm using the 
 *  GNU MP Library version 2.0.2
 *
 *  Functions:
 *
 *
 *  Notes:
 *
 *
 *  Revision History:
 *  Person                  Date              Comment
 * _____________         _______________      _________________________________
 * Jorge Guajardo        Sun March 14 1999    Creation Date
 * 
 ***************************************************************************-*/
#include <stdio.h>
#include <stdlib.h>
#include "gmp.h"         /* All MP programs need to include gmp.h */



main ()
{
  mpz_t exp, base, mod, res;
  int size_exp;
  mp_limb_t exponent_word;  /*mp_limb_t is the type of each word that 
			      retpresents the long number */
  mp_limb_t MASK;
  mp_limb_t var_mask;
  int indx;

  /*Initialize variables*/
  mpz_init (exp);
  mpz_init (base);
  mpz_init (mod);
  mpz_init (res);

  /* Assign exp, base, mod from base 16 strings */
  mpz_set_str (mod, "830BD41A71825C6AEAF89AC06FEA5B51E65601E6E1273AC30BE6644F3F80F766B94BC3C2888452C0192B2562E1EC386A7C9B22F92BF597A6694CFB65146A6180BF567357707CBB4F222057266A1891AF416AF4814AEC2F495A4A16D7173857C64BE0FD530D80D98730D7CB5574ACB2FCFA41215A93C1675D17E11EA7743E913B", 16);
  mpz_set_str (base, "130CD41A71925C6AEAF84AC06FEA5B52E65601E6E127FAC30BE6644F3F80F766B94BC3C2888452C01A2B2562E1EC386A7C9B22F92BF597D6694CFB65146A6180B4567357707CBB4F222057266A1891AF416AF4814AEC2F495A4A16D7173857C64BE0FD530D80D98730D7CB5574ACB2FCFA41215A93C1675D17E11EA7743E9132", 16);
  mpz_set_str (exp, "A459FBD2EE8C449D7106D9E0E93FC73BA2CD04AB97087E80FC7FC62F6C8A80141AFB07AB8EE9D9613EC3C7C2CF6409C359817D9C45664D88E833E8E847D13EF9B01583EA392B90F091AFE93207431D20A432B306D5797E742879169102F2E26C9A3BC0AEE9FA63FF5E6D9FBBF266BD55FAE4E46DF1A323BCB4ADEFEB7B9640E1", 16);
  
  printf("the number of bits in each word of the longnumber is %d\n", mp_bits_per_limb);

  /*IMPORTANT !!!!!!!!!!!!!!!!!!!!!!!!
    This is the number of words used in exp 
    Whenever you use the size of the longnumber MAKE SURE THAT You make the 
    size POSITIVE because this variable can be positive or negative.*/
  size_exp = exp->_mp_size;
  if (size_exp < 0)
    size_exp = -size_exp;

  printf("The number of %d bit words in exp is %d\n", mp_bits_per_limb, size_exp);


  /*Create the MASK.  This is just a 1 followed by mp_bits_per_limb -1 zeros*/
  MASK = 1;
  MASK = MASK << (mp_bits_per_limb -1) ;
  printf("MASK is %lx \n", MASK);


  /*BEGIN OF S-A-M */
  /*Set exponent_word to the most significant word in exp*/
  exponent_word = exp->_mp_d[size_exp -1];
  
  /*Search the bits in exp until we find the MSB */
  var_mask = MASK;
  while(!(exponent_word & var_mask))
    {
      var_mask = var_mask >> 1;
    }
  var_mask = var_mask >> 1;

  /*Set res to base */
  mpz_set (res, base); 
  for (indx = (size_exp -1); indx >= 0; indx--)
    {
      exponent_word = exp->_mp_d[indx];
      while(var_mask != 0)
	{
	  /*Square and Mod reduction*/
	  mpz_mul (res, res, res); 
	  mpz_mod (res, res, mod);
	  
	  /*if the bit is equal to 1 then multiply*/
	  if (exponent_word & var_mask) 
	    {
	      mpz_mul (res, res, base); 
	      mpz_mod (res, res, mod);
	    }
	  var_mask = var_mask >>1;
	}
      var_mask = MASK;
    }
  
  
  /*print out the variable*/
  mpz_out_str (stdout, 16, res);
  fputc ('\n', stdout);
  
  /* Clear out variables */
  mpz_clear (res);
  mpz_clear (base);
  mpz_clear (exp);
  mpz_clear (mod);
  exit(0);

}


