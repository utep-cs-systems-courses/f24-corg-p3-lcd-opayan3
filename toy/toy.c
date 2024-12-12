#include "msp430.h"
#include <stdlib.h>
#include "lcdutils.h"
#include "main.s"

#define SW1_PIN BIT0
#define SW4_PIN BIT3
#define LED_PIN BIT6 // LED on P1.6

#define LEFT_ARROW "<----"
#define RIGHT_ARROW "---->"

#define MAX_SEQUENCE_LENGTH 10

unsigned char game_sequence[MAX_SEQUENCE_LENGTH];
unsigned char current_index = 0;
unsigned char sequence_length = 1;

typedef enum{
  DISPLAY_SEQUENCE,
  WAIT_FOR_INPUT,
  CHECK_INPUT,
  GAME_OVER,
    WAIT_FOR_RESTART
} GameState;

GameState current_state = DISPLAY_SEQUENCE;

void display_arrow(char *arrow){
  lcd_clear();
  lcd_drawString(arrow);
  delay(500);
}

unsigned char read_button(unsigned char button_pin){
  return (P1IN & button_pin) ? 0 : 1;
}

void generate_new_sequence(){
  game_sequence[sequence_length - 1] = rand() % 2;
  sequence_length++;
  display_sequence();
}

void display_sequence(){
  for(unsigned char i = 0; i < sequence_length; i++){
    if(game_sequence[i] == 0){
      display_arrow(LEFT_ARROW);
    } else {
      display_arrow(RIGHT_ARROW);
    }
  }
}

void check_input(unsigned char player_choice){
  if(player_choice == game_sequence[current_index]){
    current_index++;
    if(current_index == sequence_length){
      sequence_length++;
      current_index = 0;
      current_state = DISPLAY_SEQUENCE;
      generate_new_sequence();
    }
  } else {
    current_state = GAME_OVER;
  }
}

void display_game_over(){
  lcd_clear();
  lcd_drawString("Game Over");
  delay(1000);
  current_state = WAIT_FOR_RESTART;
}

void reset_game(){
  current_index = 0;
  sequence_length = 1;
  current_state = DISPLAY_SEQUENCE;
  generate_new_sequence();
}

id setup_interrupt(){
  P1DIR &= ~(SW1_PIN | SW4_PIN);  // Set SW1 and SW4 as input
  P1REN |= (SW1_PIN | SW4_PIN);   // Enable pull-up resistors
  P1OUT |= (SW1_PIN | SW4_PIN);   // Pull-up on buttons
  P1IE |= (SW1_PIN | SW4_PIN);    // Enable interrupts for buttons
  P1IES |= (SW1_PIN | SW4_PIN);   // Interrupt on high-to-low transition
  P1IFG &= ~(SW1_PIN | SW4_PIN);  // Clear interrupt flags
}

void setup_timer(){
  TA0CCTL0 = CCIE;      // Enable interrupt for timer
  TA0CCR0 = 1000;       // Set timer period
  TA0CTL = TASSEL_2 + MC_1; // Use SMCLK, up mode
}

__interrupt void Port_1(void){
  unsigned char button_pressed = 0;

  if(P1IFG & SW1_PIN){
    button_pressed = 0;
    P1IFG &= ~SW1_PIN;
  }
  if(P1IFG & SW4_PIN){
    button_pressed = 1;
    P1IFG &= ~SW4_PIN;
  }

  if(current_state == WAIT_FOR_INPUT){
    check_input(button_pressed);
  }
  if(current_state == WAIT_FOR_RESTART){
    if(button_pressed == 0 || button_pressed == 1){
      reset_game();
    }
  }
}

__interrupt void Timer_A (void){
  P1OUT ^= LED_PIN;
}

extern void _asmFunction(void); 

int main(void){
  WDTCL = WDPTW | WDTHOLD;  // Stop watchdog timer
  lcd_init();                // Initialize the LCD
  setup_interrupt();         // Setup button interrupts
  setup_timer();             // Setup timer interrupt
  reset_game();              // Start the game

  while(1){
    switch (current_state){
    case DISPLAY_SEQUENCE:
      display_sequence();
      current_state = WAIT_FOR_INPUT;
      break;
    case WAIT_FOR_INPUT:
      __low_power_mode_3();  // Low-power mode while waiting for input
      break;
    case GAME_OVER:
      display_game_over();
      break;
    case WAIT_FOR_RESTART:
      __bis_SR_register(LPM4_bits + GIE);  // Wait for restart in low-power mode
      break;
    }
  }
  return 0;
}

void delay(unsigned int count){
  volatile unsigned int i;
  for(i = 0; i < count; i++);
}
