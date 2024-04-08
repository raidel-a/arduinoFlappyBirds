#include <Wire.h>
#include <LiquidCrystal_I2C.h>  

LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize the LCD object

#define buttonPin  8 // Define the button pin

byte pipe[8] = { // Define the pipe character
  0b11111,
  0b11111,
  0b01110,
  0b01110,
  0b01110,
  0b01110,
  0b01110,
  0b01110
};

byte bird[8] = { // Define the bird character
  0b00000,
  0b00110,
  0b10111,
  0b11110,
  0b11110,
  0b01100,
  0b00000,
  0b00000
};

byte hit[8] = { // Define the hit character
  0b00000,
  0b11011,
  0b01110,
  0b00100,
  0b00100,
  0b01110,
  0b11011,
  0b00000
};

byte heart[8] = { // Define the heart character
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000,
  0b00000
};

int pipes[16]; // Array to store the pipe positions
int birdY = 2; // Initial bird position on the Y-axis
int birdX = 7; // Initial bird position on the X-axis
int score = 0; // Current score
int speed = 400; // Speed of the game

int life = 5; // Number of lives

void setup() {
  lcd.backlight(); // Turn on the LCD backlight
  Serial.begin (9600); // Initialize the serial communication
  
  randomSeed(analogRead(0)); // Seed the random number generator with an analog input
  
  pinMode(buttonPin, INPUT_PULLUP); // Set the button pin as input with internal pull-up resistor
  
  int i = 0;
  for (i = 0; i<16; i++) {
    pipes[i] = 0; // Initialize the pipes array with zeros
  }
 
  lcd.begin(16,2); // Initialize the LCD with 16 columns and 2 rows

  lcd.createChar(0, pipe); // Create custom characters on the LCD
  lcd.createChar(1, bird);
  lcd.createChar(2, hit);
  lcd.createChar(3, heart);
}

void birdfly() {
  int buttonValue = digitalRead(buttonPin); // Read the button state
  int charId = 0;
  
  if (buttonValue == LOW) { // If the button is pressed
    birdY = 0; // Move the bird up
    charId = 1; // Set the character ID to display the bird
  } else {
    birdY = 1; // Move the bird down
    charId = 1; // Set the character ID to display the bird
    
    if (pipes[birdX] == 1) { // If the bird hits a pipe
      charId = 2; // Set the character ID to display the hit character
      life--; // Decrease the number of lives
      score--; // Decrease the score
    }
  }
  
  lcd.setCursor(birdX, birdY); // Set the cursor position on the LCD
  lcd.write((byte)charId); // Write the character to the LCD
}

void movePipes() {
  int dist = random(0, 2); // Generate a random number to determine the distance between pipes
  // Move the pipes along the screen
  int i;
  for (i=1; i<16; i++) {
    pipes[i-1] = pipes[i];
  }
  
  if (dist == 1 && (pipes[16] == 2 || pipes[13] == 1)) {
    pipes[15] = 0; // If the distance is 1 and there is a pipe in the last two positions, remove the pipe
  } else {
    pipes[15] = random(0, 2); // Generate a random number to determine if a pipe should be added
  }
    
  for (i = 0; i<16; i++) {
    if (pipes[i] != 0) { // If there is a pipe at the current position
      lcd.setCursor(i, 1); // Set the cursor position on the LCD
      lcd.write((byte)0); // Write the pipe character to the LCD
    }
  }
}

void showLives() {
  int i;
  for (i=0; i<life; i++) { // Loop through the number of lives
    lcd.setCursor(i, 0); // Set the cursor position on the LCD
    lcd.write((byte)3); // Write the heart character to the LCD
  }
}

void showScore() {
  lcd.setCursor(13,0); // Set the cursor position on the LCD
  lcd.print(score); // Print the score on the LCD
}

void loop() {
  lcd.clear(); // Clear the LCD screen
  
  if (life > 0) { // If there are remaining lives
    movePipes(); // Move the pipes
    birdfly(); // Make the bird fly
    showLives(); // Show the remaining lives
    showScore(); // Show the score
    score++; // Increase the score
    speed--; // Decrease the speed of the game
  } else {
    lcd.setCursor(3, 0); // Set the cursor position on the LCD
    lcd.print("GAME OVER"); // Print "GAME OVER" on the LCD
    lcd.setCursor(3, 1); // Set the cursor position on the LCD
    lcd.print("Score: "); // Print "Score: " on the LCD
    lcd.print(score); // Print the final score on the LCD
  }

  delay(speed); // Delay the execution to control the speed of the game
}
