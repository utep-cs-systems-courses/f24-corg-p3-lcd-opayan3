#include <msp430.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "libTimer.h"

#define S1 BIT3   // Button SW1 on P1.3
#define S4 BIT4   // Button SW4 on P1.4
#define LED1 BIT0  // LED 1 on P1.0
#define LED2 BIT6  // LED 2 on P1.6

// Program states
typedef enum { SLEEP, WAKEUP } ProgramState;
volatile ProgramState currentState = SLEEP;
volatile unsigned int timerCount = 0;
volatile int buttonPressed = 0;

void configureButtons();
void configureInterrupts();
void configureTimer();
void SW1_ISR();
void SW4_ISR();
void toggleLEDS();  // Called in assembly

extern void toggleLEDS(void);

void main(void)
{
  WDTCTL = WDTPW | WDTHOLD;  // Stop the Watchdog timer
  configureClocks();
  lcd_init();  // Initialize the LCD
  drawString5x7(10, 10, "LCD Init", COLOR_WHITE, COLOR_BLACK);
  P1DIR |= (LED1 + LED2);   // Set LED pins as output
  P1OUT &= ~(LED1 + LED2);  // Make sure LEDs are initially off
  
  // Set up buttons, interrupts, and timer
  configureButtons();
  configureInterrupts();
  configureTimer();

  while(1)
    {
            
      if (currentState == SLEEP) {
	// Sleep mode - LED 1 OFF, LED 2 OFF
	P1OUT &= ~(LED1 + LED2);
	drawString5x7(10, 10, "Sleep Mode", COLOR_WHITE, COLOR_BLACK);
      }
      else if (currentState == WAKEUP) {
	// Wake-up mode - LED 1 ON, LED 2 OFF
	P1OUT |= LED1;
	P1OUT &= ~LED2;
	drawString5x7(10, 10, "Wake Up Mode", COLOR_WHITE, COLOR_BLACK);
      }

      // Timer-based updates
      if (timerCount >= 1000) {
	// Every 1 second, update graphics
	drawString5x7(10, 20, "1 sec passed", COLOR_WHITE, COLOR_BLACK);
	timerCount = 0;  // Reset timer count
      }
    }
}
void configureButtons()
{
  P1DIR &= ~(S1 + S4);     // Set SW1 and SW4 as input
  P1REN |= (S1 + S4);       // Enable pull-up/down resistors
  P1OUT |= (S1 + S4);       // Set pull-up resistors (SW1 and SW4 should be pulled high)
}
void configureInterrupts()
{
  P1IE |= (S1 + S4);        // Enable interrupts for SW1 and SW4
  P1IES |= (S1 + S4);       // Interrupt on falling edge (button press)
  P1IFG &= ~(S1 + S4);      // Clear interrupt flags for SW1 and SW4
  __bis_SR_register(GIE);     // Enable global interrupts
}
void configureTimer()
{
  // Set Timer A to use ACLK, 1s interrupts
  TA0CCTL0 = CCIE;           // Enable interrupt on CCR0
  TA0CCR0 = 32768 - 1;       // Set CCR0 for 1 second interrupts
  TA0CTL = TASSEL_1 + MC_1;  // ACLK, up mode
}
#pragma vector=PORT1_VECTOR
__interrupt void Port_1_ISR(void)
{
  if (P1IFG & S1) {
    S1_ISR();  // Call the SW1 interrupt service routine
  }

  if (P1IFG & S4) {
    S4_ISR();  // Call the SW4 interrupt service routine
  }
}
void S1_ISR()
{
  if (currentState == SLEEP) {
    currentState = WAKEUP;  // Change state to WAKEUP
    toggleLEDS();            // Call assembly function to toggle LEDs
  } else {
    currentState = SLEEP;   // Change state to SLEEP
    toggleLEDS();            // Call assembly function to toggle LEDs
  }
  buttonPressed = 1;  // Indicate button press
  P1IFG &= ~S1;      // Clear interrupt flag for SW1
}
void S4_ISR()
{
  // Handle SW4 button press
  // (could trigger another state change or action)
  P1IFG &= ~S4;      // Clear interrupt flag for SW4
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
  timerCount++;  // Increment timer count every second
}
