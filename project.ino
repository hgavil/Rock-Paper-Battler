// Hannah Gavilan
// An Arduino rock-paper-scissors game. You are a student at a magic school, and your professor is testing your abilities in combat.

// Code snippets from:
// - https://arduinogetstarted.com/tutorials/arduino-button-long-press-short-press

#include <LiquidCrystal.h>

// initialize LCD
const int rs = 5, en = 4, d4 = 3, d5 = 2, d6 = 1, d7 = 0;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// set pin numbers for components
const int fireLed = 13;
const int fireButton = 12;
const int waterLed = 11;
const int waterButton = 10;
const int earthLed = 9;
const int earthButton = 8;
const int powerUp = 7;

// button setup
int firePrev, fireState = 0;
int waterPrev, waterState = 0;
int earthPrev, earthState = 0;
int powerUpPrev, powerUpState = 0;
const int maxHold = 500;
unsigned long pressed, released = 0;
long duration = 0;
bool fireTriggered, waterTriggered, earthTriggered, powerUpTriggered = 0;

int fireInput, waterInput, earthInput, powerUpInput = 0;

// game variables
int HP = 3;
String HPString = "[OOO]";
int wins = 0;
int enemy = 0;
int remainingPowerUps = 3;
int highScore = 0;


// reset the enemy and display information to the serial monitor
void newRound() {
  if (HP == 0) {
    Serial.println("=============================================");
    Serial.print("Class has ended. You beat the professor ");
    Serial.print(wins);
    Serial.println(" times! Press the Observe button to restart.");
    Serial.println("=============================================");
    
    if (wins > highScore) highScore = wins;
  }

  else {
    enemy = random(1,4);

    Serial.println("---");
    Serial.println("The professor is preparing his next attack! What will you do?");
    
    // for testing: show the enemy's attack
    Serial.print(" [DEBUGGING] enemy is: ");
    Serial.println(enemy); 
  
    // display the current information
    Serial.print("  Wins: ");
    Serial.println(wins);
    Serial.print("  HP: ");
    Serial.println(HPString);
    Serial.print("  Observations remaining: ");
    Serial.println(remainingPowerUps);
  }
}

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(fireButton, INPUT);
  pinMode(waterButton, INPUT);
  pinMode(earthButton, INPUT);
  pinMode(powerUp, INPUT);
  
  pinMode(fireLed, OUTPUT);
  pinMode(waterLed, OUTPUT);
  pinMode(earthLed, OUTPUT);

  enemy = random(1,4);

  Serial.println("Welcome to (game title)!");
  Serial.println("The professor will choose an attack of either Fire, Water, or Earth."); delay(2000);
  Serial.println("You must choose your own attack and counter his."); delay(2000);
  Serial.println("Fire will beat Earth, Water will beat Fire, and Earth will beat Water."); delay(2000);
  Serial.println("You may Observe the professor up to 3 times to see which attack he WON'T use.");
  Serial.println("Beating the professor adds to your score. Losing will lower your HP. Once your HP reaches 0, the game ends."); delay(2000);

  Serial.println("=================================");
  Serial.println("[GAME START] Class is in session. ");
  Serial.println("=================================");

  // for testing: show the enemy's attack
  Serial.print(" [DEBUGGING] enemy is: ");
  Serial.println(enemy); 

  // display the current information
  Serial.println("The professor is preparing his attack. What will you do?");
  Serial.print("  Wins: ");
  Serial.println(wins);
  Serial.print("  HP: ");
  Serial.println(HPString);
  Serial.print("  Observations remaining: ");
  Serial.println(remainingPowerUps);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Wins: ");
  lcd.setCursor(6,0);
  lcd.print(wins);
  lcd.setCursor(0,1);
  lcd.print(HPString);
  
  delay(3000);

}

void loop() {
  // read the button inputs
  fireState = digitalRead(fireButton);
  waterState = digitalRead(waterButton);
  earthState = digitalRead(earthButton);
  powerUpState = digitalRead(powerUp);


  // reset the leds
  digitalWrite(fireLed, LOW);
  digitalWrite(waterLed, LOW);
  digitalWrite(earthLed, LOW);
  
  if (HP == 0) {
    // RESTART THE GAME
    if (powerUpPrev == HIGH && powerUpState == LOW) { // button was pressed
      pressed = millis();
      delay(100);
    } else if (powerUpPrev == LOW && powerUpState == HIGH) { // button was released
      released = millis();
      duration = released - pressed; // check how long the button was pressed for
      
      if (duration < maxHold) {
        Serial.println("=================================");
        Serial.println("[GAME START] Class is in session.");
        Serial.print("  High score: ");
        Serial.println(highScore);
        Serial.println("=================================");

        // reset everything
        wins = 0;
        HP = 8;
        HPString = "[OOO]";
        remainingPowerUps = 3;

        newRound();
      }
    }
  }
  else {    
    lcd.setCursor(0, 0);
    lcd.print("Wins: ");
    lcd.setCursor(6, 0);
    lcd.print(wins);
    lcd.setCursor(0, 1);
    lcd.print(HP); /* For the sake of testing, HP is printed as a digit -- HP bar is not implemented yet */
  
    // POWER UP BUTTON PRESSED
    if (powerUpPrev == HIGH && powerUpState == LOW) { // button was pressed
      pressed = millis();
      delay(100);
    } else if (powerUpPrev == LOW && powerUpState == HIGH) { // button was released
      released = millis();
      duration = released - pressed; // check how long the button was pressed for
      
      if (duration < maxHold) {
        if (remainingPowerUps > 0) {
          remainingPowerUps--;
          // if the enemy isn't using fire, tell the player that
          if (enemy != 1) Serial.println("It doesn't look like they're going to use fire...");
          
          // otherwise, if they are using fire, then the player needs to be told that they arent using either water or earth
          else {
            int hint = random(2); // "randomly" select which hint to display
            if (hint == 0) Serial.println("It doesn't look like they're going to use water...");
            else Serial.println("It doesn't look like they're going to use earth...");
            }
          Serial.print("  Observations remaining: ");
          Serial.println(remainingPowerUps);
          }
        else {
          Serial.println("No power ups left...");
        }
      }
    }
  
    // FIRE BUTTON PRESSED
    if (firePrev == HIGH && fireState == LOW) { // button was pressed
      pressed = millis();
      delay(100);
    } else if (firePrev == LOW && fireState == HIGH) { // button was released
      released = millis();
      duration = released - pressed; // check how long the button was pressed for
  
      // GAME LOGIC
      if (duration < maxHold) {
        digitalWrite(fireLed, HIGH);
        
        switch(enemy) {
         case 1: // ENEMY CHOSE FIRE
          // results in a tie
          Serial.println("The professor chose fire. It's a tie!");
          delay(3000);
          break;
  
        case 2: // ENEMY CHOSE WATER
          // results in a loss
          Serial.println("The professor chose water. You lost!");
          delay(3000);
          // decrement the hp and change the enemy's attack
          HP--;
          HPString.remove(HP+1,1);
          break;
  
        case 3: // ENEMY CHOSE EARTH
          // results in a win
          Serial.println("The professor chose earth. You won!");
          delay(3000);
          // increment the wins
          wins++;
          break;
        }
  
        newRound();
      }
    }
  
    // WATER BUTTON PRESSED
    if (waterPrev == HIGH && waterState == LOW) { // button was pressed
      pressed = millis();
      delay(100);
    } else if (waterPrev == LOW && waterState == HIGH) { // button was released
      released = millis();
      duration = released - pressed; // check how long the button was pressed for
  
        // GAME LOGIC
        if (duration < maxHold) {
          digitalWrite(waterLed, HIGH);
          
          switch(enemy) {
           case 1: // ENEMY CHOSE FIRE
            // results in a win
            Serial.println("The professor chose fire. You won!");
            delay(3000);
            // increment the wins
            wins++;
            break;
    
          case 2: // ENEMY CHOSE WATER
            // results in a tie
            Serial.println("The professor chose water. It's a tie!");
            delay(3000);
            break;
    
          case 3: // ENEMY CHOSE EARTH
            // results in a loss
            Serial.println("The professor chose earth. You lost!");
            delay(3000);
            // decrement the hp and change the enemy's attack
            HP--;
            HPString.remove(HP+1,1);
            break;
          }
    
          newRound();
        }
      }
  
    // EARTH BUTTON PRESSED
    if (earthPrev == HIGH && earthState == LOW) { // button was pressed
      pressed = millis();
      delay(100);
    } else if (earthPrev == LOW && earthState == HIGH) { // button was released
      released = millis();
      duration = released - pressed; // check how long the button was pressed for
  
        // GAME LOGIC
        if (duration < maxHold) {
          digitalWrite(earthLed, HIGH);
          
          switch(enemy) {
           case 1: // ENEMY CHOSE FIRE
            // results in a loss
            Serial.println("The professor chose fire. You lost!");
            delay(3000);
            // decrement the hp and change the enemy's attack
            HP--;
            HPString.remove(HP+1,1);
            break;
            
          case 2: // ENEMY CHOSE WATER
            // results in a win
            Serial.println("The professor chose water. You won!");
            delay(3000);
            // increment the wins
            wins++;
            break;
    
          case 3: // ENEMY CHOSE EARTH
            // results in a tie
            Serial.println("The professor chose earth. It's a tie!");
            delay(3000);
            break;
          }
    
          newRound();
        }
    }
  

  }
    firePrev = fireState;
    waterPrev = waterState;
    earthPrev = earthState;
    powerUpPrev = powerUpState;
}
