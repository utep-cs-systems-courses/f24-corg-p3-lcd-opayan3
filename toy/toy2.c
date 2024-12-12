#include "msp430.h"
#include <stdio.h>
#include <string.h>
#include "lcdutils.h"
#include "libTimer.h"

void setup();
void sleep_mode();
void wake_up();
void start_timer();
void stop_timer();
void display_text(const char *text, int x, int y);
void reaction_time_calc_asm();
void show_result(int reaction_time);

volatile int game_state = 0;
volatile int reaction_time = 0;
volatile int timer_count = 0;

int main(void){
  WDTCTL = WDTPW | WDTHOLD;
  configureClocks();
  setup();

  while(1){
    switch(game_state){
    case 0:
      sleep_mode();
      break;

    case 1:
      wake_up();
      display_text("Ready...",10,10);
      start_timer();
      game_state = 2;
      break;

    case 2:
      break;

    case 3:
      stop_timer();
      reaction_time_calc_asm();
      game_state = 4;
      break;

    case 4:
      show_result(reaction_time);
      game_state = 0;
      break;
      
    }
  }
}

void setup(){
  P1DIR |= BIT0;
  P1OUT &= ~BIT0;

  P1DIR &= ~BIT1;
  P2DIR &= ~BIT4;
  P1REN |= BIT1;
  P2REN |= BIT4;
  P1OUT |= BIT1;
  P2OUT |= BIT4;
  P1IE |= BIT1;
  P2IE |= BIT4;
  P1IFG &= ~BIT1;
  P2IFG &= ~BIT4;

  TA0CTL = TASSEL_1 | MC_0 | TACLR;
  TA0CCTL0 = CCIE;
  TA0CCR0 = 250;

  lcd_init();
}

void sleep_mode(){
  P1OUT &= ~BIT0;
  __bis_SR_register(LPM3_bits + GIE);
}

void wake_up(){
  P1OUT |= BIT0;
}

void start_timer(){
  TA0CCR0 = 5000 + (rand()% 15000);
  TA0CTL |= TASSEL_1 | MC_1;
  timer_count = 0;
}

void stop_timer(){
  TA0CTL &= ~MC_3;
  reaction_time = timer_count;
}

void display_text(const char *text, int x, int y){
  u_char i, j;
  u_char character;

  // Set the area on the LCD screen to draw the text
  lcd_setArea(x, y, x + 5 * strlen(text) - 1, y + 7); // Adjusting for font size

  // Loop through each character in the text string
  while (*text) {
    character = *text - 32;  // ASCII 32 (space) is the first printable character
    for (i = 0; i < 5; i++) {  // 5 columns per character in 5x7 font
      u_char row = font_5x7[character][i]; // Get character data from font
      for (j = 0; j < 7; j++) {  // 7 rows per character in 5x7 font
	if (row & (1 << j)) {
	  lcd_writeColor(COLOR_WHITE);  // If bit is set, write color (text color)
	} else {
	  lcd_writeColor(COLOR_BLACK);  // If bit is clear, write background color
	}
      }
    }
    text++;  // Move to the next character
    x += 5;  // Move to the next character's position
  }
}

void show_result(int reaction_time){
  if(reaction_time > 0){
    display_text("Reaction Time:", 10, 20);
  }else{
    display_text("Too slow!", 10, 20);
  }
}

#pragma vector = PORT1_VECTOR
__interrupt void PORT_1_ISR(){
  if(P1IFG & BIT1){
    game_state = 1;
  }
  P1IFG &= ~BIT1;
}

#pragma vector = PORT2_VECTOR
__interrupt void PORT_2_ISR(){
  if(P2IFG & BIT4){
    if(game_state == 3){
      stop_timer();
      game_state = 4;
    }
  }
  P2IFG &= ~BIT4;
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A_ISR(){
  timer_count++;
}
