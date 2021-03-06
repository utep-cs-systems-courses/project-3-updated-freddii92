#include <msp430.h>
#include "switches.h"
#include "lcd.h"
#include "led.h"
#include "stateMachine.h"
#include "buzzer.h"
#include "lcdutils.h"
#include "lcddraw.h"

char switch_state_down, switch_state_changed;         // effectively boolean
char speed;

static char switch_update_interrupt_sense()
{
  char p2val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);                        // if switch up, sense down
  P2IES &= (p2val | ~SWITCHES);                       // if switch down, sense up
  return p2val;
}

void switch_init()                                    // setup switch
{
  P2REN |= SWITCHES;                                  // enables resistors for switches
  P2IE |= SWITCHES;                                   // enables interrupts from switches
  P2OUT |= SWITCHES;                                  // pull-ups for switches
  P2DIR &= ~SWITCHES;                                 // set switches' bits for input
  switch_update_interrupt_sense();
  led_update();
}

void switch_interrupt_handler()
{
  char p2val = switch_update_interrupt_sense();

  if (p2val & SW1 && p2val & SW2 && p2val & SW3 && p2val & SW4) {
    idle_state();
  }
  else if (!(p2val & SW1)) {                               // if switch S1 is pushed
    speed = 50;
    left_siren = COLOR_RED;
    right_siren = COLOR_BLUE;
    siren();
    switch_state_down = 1;
  }
  else if (!(p2val & SW2)) {                          // if switch S2 is pushed                    
    speed = 80;
    twinkle_advance();
    switch_state_down = 1;
  }
  else if (!(p2val & SW3)) {                          // if switch S3 is pushed
    speed = 4;
    char one = dimmer_advance(1);
    switch_state_down = one;
  }
  else if (!(p2val & SW4)) {                          // is switch S4 is pushed
    speed = 80;
    happy_birthday();
    switch_state_down = 1;
  }
  
  switch_state_changed = 1;
  led_update();
}
