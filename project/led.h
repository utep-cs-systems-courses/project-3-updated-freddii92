#ifndef led_included
#define led_included

#include <msp430.h>

#define LED_RED BIT0            // P1.0
#define LED_GREEN BIT6          // P1.6
#define LEDS (BIT0 | BIT6)

extern unsigned char red_on, green_on;
extern unsigned char led_changed;
extern unsigned char leds_changed, green_led_state, red_led_state;  // these are boolean flags

void led_init();                // initialize LEDs
void led_update();              // update LEDs

#endif // included