// Hannah Gavilan
// A basic Arduino rock-paper-scissors game that tracks wins and HP and displays it on the LCD

#include <LiquidCrystal.h>

// initialize LCD
const int rs = 7, en = 6, d4 = 4, d5 = 3, d6 = 2, d7 = 1;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// set pin numbers for components
const int rockButton = 12;
const int paperButton = 11;
const int scissorsButton = 10;

// button setup
int rockPrev, rockState = 0;
int paperPrev, paperState = 0;
int scissorsPrev, scissorsState = 0;
const int maxHold = 500;
unsigned long pressed, released = 0;
long duration = 0;

int rockInput, paperInput, scissorsInput = 0;

// game variables
int HP = 8;
//String HPstring = "[■■■■■■■■]";
int wins = 0;
int enemy = 1;

void setup() {
  lcd.begin(16, 2);
  pinMode(rockButton, INPUT);
  pinMode(paperButton, INPUT);
  pinMode(scissorsButton, INPUT);
  
  lcd.print("R-P-S");
  delay(3000);

}

// track whenever the buttons are pressed, prevents button holding
int buttonPress(int prev, int state) {
  if (prev == HIGH && state == LOW) { // button was pressed
    pressed = millis();
    delay(100);
  } else if (prev == LOW && state == HIGH) { // button was released
    released = millis();
    duration = released - pressed; // check how long the button was pressed for

    // paper button was successfully pressed
    if (pressed < maxHold) {
      return 1;
    } else return 0;
  }
}


void loop() {
  rockState = digitalRead(rockButton);
  paperState = digitalRead(paperButton);
  scissorsState = digitalRead(scissorsButton);

  rockInput = buttonPress(rockPrev, rockState);
  paperInput = buttonPress(paperPrev, paperState);
  scissorsInput = buttonPress(scissorsPrev, scissorsState);
  
  lcd.setCursor(0, 0);
  lcd.print("Wins: ");
  lcd.setCursor(6, 0);
  lcd.print(wins);
  lcd.setCursor(0, 1);
  lcd.print(HP); /* For the sake of testing, HP is printed as a digit -- HP bar is not implemented yet */

  /* For the sake of testing, the enemy is always rock -- random enemy selection is not implemented yet */
  
  switch(enemy) {
    case 1: // enemy chose rock -- rock = tie / paper = win / scissors = lose
      // rock button was pressed
      if (rockInput == 1) {
        lcd.setCursor(0, 0);
        lcd.clear();
        lcd.print("Tie!");
        delay(3000);
      }
      // paper button was pressed:
      else if (paperInput == 1) {
        lcd.setCursor(0, 0);
        lcd.clear();
        lcd.print("Win!");
        delay(3000);
        wins++;
      }
      // scissors button was pressed:
      else if (scissorsInput == 1) {
        lcd.setCursor(0, 0);
        lcd.clear();
        lcd.print("Lose!");
        delay(3000);
        HP--;
      }
      break;

    case 2: // enemy chose paper -- rock = lose / paper = tie / scissors = win
      // rock button was pressed
      if (rockInput == 1) {
        lcd.setCursor(0, 0);
        lcd.clear();
        lcd.print("Lose!");
        delay(3000);
        HP--;
      }
      // paper button was pressed:
      else if (paperInput == 1) {
        lcd.setCursor(0, 0);
        lcd.clear();
        lcd.print("Tie!");
        delay(3000);
      }
      // scissors button was pressed:
      else if (scissorsInput == 1) {
        lcd.setCursor(0, 0);
        lcd.clear();
        lcd.print("Win!");
        delay(3000);
        wins++;
      }
      break;

    case 3: // enemy chose scissors -- rock = win / paper = lose / scissors = tie
      // rock button was pressed
      if (rockInput == 1) {
        lcd.setCursor(0, 0);
        lcd.clear();
        lcd.print("Win!");
        delay(3000);
        wins++;
      }
      // paper button was pressed:
      else if (paperInput == 1) {
        lcd.setCursor(0, 0);
        lcd.clear();
        lcd.print("Lose!");
        delay(3000);
        HP--;
      }
      // scissors button was pressed:
      else if (scissorsInput == 1) {
        lcd.setCursor(0, 0);
        lcd.clear();
        lcd.print("Tie!");
        delay(3000);
      }
      break;
  }

  rockPrev = rockState;
  paperPrev = paperState;
  scissorsPrev = scissorsState;
  
}
