//
//
// Stepper test program
//
// Adapted from python source & gertboard examples
//   needs: gb_common.h from gertboard sw.
//
// Peter Mansvelder &ltpman mac.com="mac.com"&gt      27/09/2012

#include "gb_common.h"

//      Stepper motor connections, based on the 
//    uln2003/28BJY-48 board commonly found on eBay:
//    Inp1 (P1-18) GPIO 24
//    Inp2 (P1-22) GPIO 25
//    Inp3 (P1-24) GPIO 8
//    Inp4 (P1-26) GPIO 7

#define ALL_INPUTS (I1|I2|I3|I4)

// Stepper test GPIO mapping:
//         Function            Mode

// GPIO7=  Stepper input 1     Output
// GPIO8=  Stepper input 1     Output
// GPIO24= Stepper input 1     Output
// GPIO25= Stepper input 1     Output

void setup_gpio(void)
{
  INP_GPIO(7);  OUT_GPIO(7);
  INP_GPIO(8);  OUT_GPIO(8);
  INP_GPIO(24);  OUT_GPIO(24);
  INP_GPIO(25);  OUT_GPIO(25);
} // setup_gpio

//

static int numberofsteps = 512;

// patterns to make the stepper motor work; 
// patterns 2 and 3 are the 'official' way to run the 28BJY-48 
// patterns 0 and 1 are faster, but give less torque

static int pattern0[] = 
  {I1,
   I2,
   I3,
   I4,
    -1 };
static int pattern1[] = 
  {I4,
   I3,
   I2,
   I1,
    -1 };
static int pattern2[] = 
  {I1,
   I1|I2,
   I2,
   I2|I3,
   I3,
   I3|I4,
   I4,
   I1|I4,
   -1 };
static int pattern3[] = 
  {I1|I4,
   I4,
   I3|I4,
   I3,
   I2|I3,
   I2,
   I1|I2,
   I1,
   -1 };

// Local (to this file) variables
static int *pattern = pattern0;  // current pattern
static int step = 0;  // which pattern element we are showing

void set_STEPPER(int value)
{
  // first turn off all inputs - GPIO_CLR0 selects which output pins
  // will be set up 0
  GPIO_CLR0 = ALL_INPUTS;
  // now light up the ones for this value - GPIO_SET0 selects which 
  // output pins will be set up 1
  GPIO_SET0 = value;
} // set_pattern

void stepper_off(void)
{
  GPIO_CLR0 = ALL_INPUTS;
}

//
// Start anew with one of the available patterns
//
void start_new_pattern(int p)
{
   switch (p)
   {
   case 0 : pattern = pattern0; break;
   case 1 : pattern = pattern1; break;
   case 2 : pattern = pattern2; break;
   case 3 : pattern = pattern3; break;
   default: return;
   }
   step = 0;
   // set the stepper inputs for the first item in the new pattern
   set_STEPPER(pattern[step]);
} // start_new_pattern

int stepper_step()
{
   step++;
   if (pattern[step]==-1) // we're at end of this pattern, start over
      step=0;
   set_STEPPER(pattern[step]);
   return pattern[step+1]== -1 ? 1 : 0; // are we at last value?
} // stepper_step

//
//
int main( int argc, char *argv[] )
{ int p,r,last;

  if (argc>1) { 
                numberofsteps = atoi(argv[1]);
                }
  if (numberofsteps&lt0) {
        numberofsteps *= -1;
        p = 0;
        }
  else
    p = 1;

  // Map the I/O sections
  setup_io();

  // Set 12 GPIO pins to output mode
  setup_gpio();

  /* for testing purposes...
  GPIO_SET0 = 0x180;
  (void) getchar();
  GPIO_CLR0 = 0x100;
  (void) getchar();
  */

    start_new_pattern(p);
    for (r=0; r<numberofsteps; r++)
    { do {
        last = stepper_step();
        usleep(5000);
      } while (!last);
    } 
    stepper_off();

  stepper_off();
  restore_io();
} // main

