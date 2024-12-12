#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

#define SW1 1  // Button SW1 on P1.3
#define SW4 8  // Button SW4 on P1.4
#define LED1 BIT6  // LED 1 on P1.6

typedef enum { SLEEP, WAKEUP } ProgramState;
volatile ProgramState currentState = SLEEP;
volatile unsigned int timerCount = 0;
volatile int buttonPressedFlag = 0;

unsigned short swordColors[] = {COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_YELLOW};
unsigned char currentColorIndex = 0; 

void configureButtons();
void configureInterrupts();
void configureTimer();
void SW1_ISR();
void SW4_ISR();
void toggleLEDS();  // Called in assembly

extern void toggleLEDS(void);

void drawSword(int col, int row, unsigned short color) {
  fillRectangle(col, row, 2, 5, color);        // Sword handle
  fillRectangle(col + 1, row - 5, 1, 6, color);  // Sword blade
}

void main(void) {
  WDTCTL = WDTPW | WDTHOLD;  // Stop the Watchdog timer
  configureClocks();
  lcd_init();  // Initialize the LCD
  drawString5x7(10, 10, "LCD Init", COLOR_WHITE, COLOR_BLACK);
  fillRectangle(0, 0, 10, 10, COLOR_RED);
  
  P1DIR |= LED1;  // Set LED1 as output
  P1OUT &= ~LED1; // Make sure LED is off initially

  // Set up buttons, interrupts, and timer
  configureButtons();
  configureInterrupts();
  configureTimer();

  while (1) {
    if (buttonPressedFlag) {
      // Draw a message on LCD indicating button press
      drawString5x7(10, 30, "Button Pressed", COLOR_WHITE, COLOR_BLACK);
      buttonPressedFlag = 0;  // Reset the flag
    }
    fillRectangle(0, 10, 100, 20, COLOR_BLACK);
    
    if (currentState == SLEEP) {
      // Sleep mode - LED 1 OFF
      P1OUT &= ~LED1;
      drawString5x7(10, 10, "Sleep Mode", COLOR_WHITE, COLOR_BLACK);
    } else if (currentState == WAKEUP) {
      // Wake-up mode - LED 1 ON
      P1OUT |= LED1;
      drawString5x7(10, 10, "Wake Up Mode", COLOR_WHITE, COLOR_BLACK);
      drawSword(50, 40,swordColors[currentColorIndex]);  // Draw sword when awake
    }
    if (timerCount >= 1000) {  // Every 1 second
      fillRectangle(0, 20, 100, 20, COLOR_BLACK);
      drawString5x7(10, 20, "1 sec passed", COLOR_WHITE, COLOR_BLACK);
      timerCount = 0;  // Reset timer count
    }
  }
}
void configureButtons() {
  P1DIR &= ~(SW1 + SW4);     // Set SW1 and SW4 as input
  P1REN |= (SW1 + SW4);       // Enable pull-up/down resistors
  P1OUT |= (SW1 + SW4);       // Set pull-up resistors
}
void configureInterrupts() {
  P1IE |= (SW1 + SW4);        // Enable interrupts for SW1 and SW4
  P1IES |= (SW1 + SW4);       // Interrupt on falling edge (button press)
  P1IFG &= ~(SW1 + SW4);      // Clear interrupt flags for SW1 and SW4
  __bis_SR_register(GIE);   // Enable global interrupts
}
void configureTimer() {
  // Set Timer A to use ACLK, 1s interrupts
  TA0CCTL0 = CCIE;           // Enable interrupt on CCR0
  TA0CCR0 = 32768 - 1;       // Set CCR0 for 1 second interrupts
  TA0CTL = TASSEL_1 + MC_1;  // ACLK, up mode
}
#pragma vector=PORT1_VECTOR
__interrupt void Port_1_ISR(void) {
  if (P1IFG & SW1) {
    SW1_ISR();  // Call the S1 interrupt service routine
  }
  if (P1IFG & SW4) {
    SW4_ISR();  // Call the S4 interrupt service routine
  }
}
void SW1_ISR() {
  if (currentState == SLEEP) {
    currentState = WAKEUP;  // Change state to WAKEUP
    drawString5x7(10, 20, "State: WAKEUP", COLOR_WHITE, COLOR_BLACK);
    toggleLEDS();            // Call assembly function to toggle LEDs
  } else {
    currentState = SLEEP;   // Change state to SLEEP
    drawString5x7(10, 20, "State: SLEEP", COLOR_WHITE, COLOR_BLACK); 
    toggleLEDS();            // Call assembly function to toggle LEDs
  }
  buttonPressedFlag = 1;  // Indicate button press
  P1IFG &= ~SW1;      // Clear interrupt flag for S1
}
void SW4_ISR() {
  currentColorIndex = (currentColorIndex + 1 ) % 4;
  P1IFG &= ~SW4;      // Clear interrupt flag for S4
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void) {
  timerCount++;  // Increment timer count every second
}
