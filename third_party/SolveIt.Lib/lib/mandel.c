/*-----------------------------------------------
 * Berlin Brown
 * bigbinc@hotmail.com
 * August 6, 2000
 * Generate a MandelBrot fractal
 *------------------------------------------------*/

#include <stdlib.h>
#include <math.h>
#include <vga.h>
#include <time.h>      /* time functions, */

#include "testing.h" 
#include "graphics.h"     /* our own timer functions */

/*------------------------------------------------
 * MAIN
 *================================================*/
int main(void)
{
  int index =0;
  
  vga_init();
  vga_setmode(G640x480x256);

  generatelookup(); // cos/sin lookup table

  // set the palette, nothing fancy
  for (index=0; index < 256; index++)
    {
      vga_setpalette(index, index, index, 0);
    } /* end of the for */
  
  #if 0
  startclock();
  circletest(1500);
  stopclock();
  
  startclock();
  fractaltest();
  stopclock();
  
  g(4);

  #endif
  
  startclock();
  for (index=0; index < 30000; index++)
  { 
   drawline(rand()%getscreenwidth(),
	    rand()%getscreenheight(), 
	    rand()%getscreenwidth(), 
	    rand()%getscreenheight(), rand()%256);
  } // end of the for 
  stopclock();

  // Return to Linux Console
  getchar();  // wait for keypress

  vga_setmode(TEXT);
  return 0;
} /* end of main */
