#include <Wire.h>
#include <LiquidCrystal.h>
// #include <LiquidCrystal_I2C.h>

// LiquidCrystal_I2C lcd(0x27, 16, 2);

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // Initialize the LCD display

#define buttonPin 8 // Define the button pin, tells the arduino what pin the button is connected to

// The pipe object is defined as an array of bytes.
// It represents a custom character that will be displayed on an LCD screen.
// The pipe array is initialized with 8 elements, each representing a row of the character.
// Each row is defined using a binary number that represents the pattern of pixels in that row.
// The binary numbers are written in the form of 0bXXXXXXXX, where each X represents a pixel.
// For example, the first row of the pipe character is defined as 0b11111, which means that all 5 pixels in that row are turned on.
// The second row is also 0b11111, indicating that all 5 pixels in that row are turned on as well.
// The following rows have a similar pattern, forming the shape of a pipe.
// By defining the pipe character in this way, it can be easily displayed on the LCD screen by sending the corresponding byte values to the LCD library.
// The LCD library will interpret these byte values and display the custom character accordingly.

byte pipe[8] = { // Define the pipe obstacle
    0b00000,
    0b11111,
    0b11111,
    0b01110,
    0b01110,
    0b01110,
    0b01110,
    0b01110};

byte pipeTop[8] = {
    0b01110,
    0b01110,
    0b01110,
    0b01110,
    0b01110,
    0b11111,
    0b11111,
    0b00000};

byte bird[8] = { // Define the bird character
    0b00000,
    0b00110,
    0b10111,
    0b11110,
    0b11110,
    0b01100,
    0b00000,
    0b00000};

byte birdAnim[8] = { // Bird animation/shifted y -2
    0b00000,
    0b00000,
    0b00110,
    0b10111,
    0b11110,
    0b11110,
    0b01100,
    0b01000};

byte hit[8] = { // Define the hit marker
    0b10001,
    0b11011,
    0b01010,
    0b00100,
    0b00100,
    0b01010,
    0b11011,
    0b10001};

byte heart[8] = { // Define the heart symbol
    0b00000,
    0b01010,
    0b11111,
    0b11111,
    0b11111,
    0b01110,
    0b00100,
    0b00100};

byte coin[8] = { // Define the coin symbol
    0b00000,
    0b01110,
    0b11011,
    0b11111,
    0b11011,
    0b11011,
    0b01110,
    0b00000};

int pipes[16];          // Array to store the pipe positions
int topPipes[16] = {0}; // Array to store the Top pipe positions
int coins[16] = {0};    // Array to store the coin positions
int birdY = 2;          // Initial bird position on the Y-axis
int birdX = 7;          // Initial bird position on the X-axis
int score = 0;          // Current score
int speed = 600;        // Speed of pipes
int life = 3;           // Number of lives

void setup()
{
  // lcd.backlight();  only needed for I2C use
  Serial.begin(9600); // Start the serial communication, used for debugging,
                      // not needed for the game,
                      // but can be useful to see the score in the serial monitor

  lcd.begin(16, 2); // Initialize the LCD display with 16 columns and 2 rows

  lcd.createChar(0, pipe); // Create the pipe character

  lcd.createChar(1, bird);     // Create the bird character
  lcd.createChar(2, hit);      // Create the hit character
  lcd.createChar(3, heart);    // Create the heart character
  lcd.createChar(4, birdAnim); // Create the bird animation character
  lcd.createChar(5, pipeTop);  // Create the Top pipe character
  lcd.createChar(6, coin);     // Create the coin character

  pinMode(buttonPin, INPUT_PULLUP); // Set the button pin as an input and enable the internal pull-up resistor
}

void birdfly()
{
  int buttonValue = digitalRead(buttonPin); // Read the button value
  int charId = 0;

  if (buttonValue == LOW)
  {             // If the button is pressed
    birdY = 0;  // Move the bird up
    charId = 1; // Set the character ID to display the bird
  }
  else
  {
    birdY = 1;  // Move the bird down
    charId = 1; // Set the character ID to display the bird
  }

  if (birdY == 0)
  {
    lcd.setCursor(birdX, birdY);
    lcd.write(1); // Display the bird
  }
  else
  {
    lcd.setCursor(birdX, birdY);
    lcd.print(' '); // Clear the previous bird position
    birdY = 1;
    lcd.setCursor(birdX, birdY);
    lcd.write(1); // Display the bird
  }
}

void checkCollision()
{
  if ((pipes[birdX] == 1 && birdY != 0) || (topPipes[birdX] == 1 && birdY == 0))
  { // if the bird has hit a pipe, 
    // either a regular one when it's not at the bottom, 
    // or a top one when it is at the bottom.
    lcd.setCursor(birdX, birdY);
    lcd.write(2); // Display the hit marker
    life--;       // Decrease the number of lives
    score -= 5;   // Decrease the score by 5
  }
  else if (coins[birdX] == 1)
  { // If the bird hits a coin
    lcd.setCursor(birdX, birdY);
    lcd.write(1);     // Display the bird
    coins[birdX] = 0; // Remove the coin
    score += 5;       // Increase the score by 2
  }
}

void movePipes()
{
  // Move the pipes, top pipes, and coins to the left
  for (int i = 1; i < 16; i++)
  {
    pipes[i - 1] = pipes[i];
    topPipes[i - 1] = topPipes[i];
    coins[i - 1] = coins[i];
  }

  // Generate new pipes, top pipes, and possibly a coin at the right side of the screen
  // Ensure there is at least one space between the regular and top pipes
  if (pipes[14] == 0 && topPipes[14] == 0)
  {
    if(random(2) == 0) {
      pipes[15] = random(0, 2); // 50% chance to generate a regular pipe
      topPipes[15] = 0;
    } else {
      topPipes[15] = random(0, 2); // 50% chance to generate a top pipe
      pipes[15] = 0;
    }
    // Generate a coin only if there is no pipe or top pipe at the same position
    if(pipes[15] == 0 && topPipes[15] == 0) {
      coins[15] = random(0, 2); // 50% chance to generate a coin
    } else {
      coins[15] = 0;
    }
  }
  else
  {
    pipes[15] = 0;
    topPipes[15] = 0;
    coins[15] = 0;
  }

  // Draw the pipes, top pipes, and coins
  for (int i = 0; i < 16; i++)
  {
    lcd.setCursor(i, 0);
    if (topPipes[i] == 1) {
      lcd.write((byte)5); // Display the top pipe
    } else if (coins[i] == 1) {
      lcd.write((byte)6); // Display the coin
    } else {
      lcd.write(' '); // No top pipe or coin
    }

    lcd.setCursor(i, 1);
    if (pipes[i] == 1) {
      lcd.write((byte)0); // Display the pipe
    } else {
      lcd.write(' '); // No pipe
    }
  }
}

void showLives()
{
  int i;
  for (i = 0; i < life; i++)
  {
    lcd.setCursor(i, 0); // Set the cursor position on the LCD display
    lcd.write((byte)3);  // Display the heart character
  }
}

void showScore()
{
  lcd.setCursor(3, 0); // Set the cursor position on the LCD display
  lcd.print(score);    // Display the score
}

bool gameStarted = false;

void loop()
{
  lcd.clear(); // Clear the LCD display

  if (!gameStarted)
  {
    // Display the start screen

    lcd.setCursor(1, 0);
    lcd.write(4); // Display the bird
    delay(300);   // in milliseconds

    lcd.setCursor(3, 0);           // Set the cursor position on the LCD display
    lcd.print("Flappy Bird");      // Display "Flappy Bird"
    lcd.setCursor(0, 1);           // Set the cursor position on the LCD display
    lcd.print("Press the button"); // Display "Press the button"

    lcd.setCursor(1, 0);
    lcd.write(1); // Display the bird animation
    delay(100);

    // Check if the button is pressed
    if (digitalRead(buttonPin) == LOW)
    {
      // If the button is pressed, start the game
      gameStarted = true;
    }
  }
  else if (life > 0)
  { // If there are lives remaining
    movePipes();
    birdfly();
    showLives();
    showScore();
    checkCollision();
    score++;
    speed--; // Decrease the speed level
  }
  else
  {
    lcd.setCursor(3, 0); // Set the cursor position on the LCD display
    lcd.print("GAME OVER");
    lcd.setCursor(3, 1); // Set the cursor position on the LCD display
    lcd.print("Score: ");
    lcd.print(score); // Display the final score
  }

  delay(speed); // Delay for the specified speed level
}
