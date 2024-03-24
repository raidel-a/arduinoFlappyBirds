#include <Wire.h>
#include <rgb_lcd.h>
#include "pitches.h"

rgb_lcd lcd;

#define buttonPin 4
#define BUZZER_PIN 8

bool hasPlayedDeadChime = false;
void startupChime() {
  tone(BUZZER_PIN, 1000); // Play a 1kHz tone
  delay(100); // Wait for 100ms
  noTone(BUZZER_PIN); // Stop the tone
}
void coinChime() {
  tone(BUZZER_PIN, NOTE_B5);
  delay(200);
  noTone(BUZZER_PIN);
  analogWrite(BUZZER_PIN, NOTE_E6);
  delay(400);
  noTone(BUZZER_PIN);
}
void collisionChime() {
  tone(BUZZER_PIN, NOTE_A4);
  delay(300);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, NOTE_A4);
  delay(200);
  noTone(BUZZER_PIN);
}
void deadChime() {
  if (!hasPlayedDeadChime){
  tone(BUZZER_PIN, NOTE_B5);
  delay(300);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, NOTE_F5);
  delay(300);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, NOTE_F5);
  delay(300);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, NOTE_E5);
  delay(300);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, NOTE_D4);
  delay(300);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, NOTE_C4);
  delay(300);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, NOTE_E4);
  delay(300);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, NOTE_E4);
  delay(300);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, NOTE_C4);
  delay(300);
  noTone(BUZZER_PIN);
  hasPlayedDeadChime = true;
}}

byte pipe[8] = {
  0b00000,
  0b11111,
  0b11111,
  0b01110,
  0b01110,
  0b01110,
  0b01110,
  0b01110
};

byte pipeTop[8] = {
  0b01110,
  0b01110,
  0b01110,
  0b01110,
  0b01110,
  0b11111,
  0b11111,
  0b00000
};

byte bird[8] = {
  0b00000,
  0b00110,
  0b10111,
  0b11110,
  0b11110,
  0b01100,
  0b00000,
  0b00000
};

byte birdAnim[8] = {
  0b00000,
  0b00000,
  0b00110,
  0b10111,
  0b11110,
  0b11110,
  0b01100,
  0b01000
};

byte hit[8] = {
  0b10001,
  0b11011,
  0b01010,
  0b00100,
  0b00100,
  0b01010,
  0b11011,
  0b10001
};

byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00100
};

byte coin[8] = {
  0b00000,
  0b01110,
  0b11011,
  0b11111,
  0b11011,
  0b11011,
  0b01110,
  0b00000
};

int pipes[16];
int topPipes[16] = { 0 };
int coins[16] = { 0 };
int birdY = 2;
int birdX = 7;
int score = 0;
int speed = 400;
int life = 3;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.createChar(0, pipe);
  lcd.createChar(1, bird);
  lcd.createChar(2, hit);
  lcd.createChar(3, heart);
  lcd.createChar(4, birdAnim);
  lcd.createChar(5, pipeTop);
  lcd.createChar(6, coin);
  pinMode(buttonPin, INPUT_PULLUP);
}

void birdfly() {
  int buttonValue = digitalRead(buttonPin);
  int charId = 0;

  if (buttonValue == HIGH) {
    birdY = 0;
    charId = 1;
  } else {
    birdY = 1;
    charId = 1;
  }

  if (birdY == 0) {
    lcd.setCursor(birdX, birdY);
    lcd.write(1);
  } else {
    lcd.setCursor(birdX, birdY);
    lcd.print(' ');
    birdY = 1;
    lcd.setCursor(birdX, birdY);
    lcd.write(1);
  }
}

void checkCollision() {
  if ((pipes[birdX] == 1 && birdY != 0) || (topPipes[birdX] == 1 && birdY == 0)) {
    lcd.setCursor(birdX, birdY);
    lcd.write(2);
    life--;
    score -= 5;
    collisionChime();
  } else if (coins[birdX] == 1 && birdY != 1) {
    lcd.setCursor(birdX, birdY);
    lcd.write(1);
    coins[birdX] = 0;
    score += 5;
    coinChime();
  }
}

void movePipes() {
  for (int i = 1; i < 16; i++) {
    pipes[i - 1] = pipes[i];
    topPipes[i - 1] = topPipes[i];
    coins[i - 1] = coins[i];
  }

  if (pipes[14] == 0 && topPipes[14] == 0) {
    if (random(2) == 0) {
      pipes[15] = random(0, 2);
      topPipes[15] = 0;
    } else {
      topPipes[15] = random(0, 2);
      pipes[15] = 0;
    }
    if (pipes[15] == 0 && topPipes[15] == 0) {
      coins[15] = random(0, 2);
    } else {
      coins[15] = 0;
    }
  } else {
    pipes[15] = 0;
    topPipes[15] = 0;
    coins[15] = 0;
  }

  for (int i = 0; i < 16; i++) {
    lcd.setCursor(i, 0);
    if (topPipes[i] == 1) {
      lcd.write((byte)5);
    } else if (coins[i] == 1) {
      lcd.write((byte)6);
    } else {
      lcd.write(' ');
    }

    lcd.setCursor(i, 1);
    if (pipes[i] == 1) {
      lcd.write((byte)0);
    } else {
      lcd.write(' ');
    }
  }
}

void showLives() {
  int i;
  for (i = 0; i < life; i++) {
    lcd.setCursor(i, 0);
    lcd.write((byte)3);
  }
}

void showScore() {
  lcd.setCursor(3, 0);
  lcd.print(score);
}

bool gameStarted = false;

void loop() {
  lcd.clear();

  if (!gameStarted) {
    lcd.setCursor(1, 0);
    lcd.write(4);
    delay(300);

    lcd.setCursor(3, 0);
    lcd.print("Flappy Bird");
    lcd.setCursor(0, 1);
    lcd.print("Press the button");

    lcd.setCursor(1, 0);
    lcd.write(1);
    delay(100);
    startupChime();
    if (digitalRead(buttonPin) == HIGH) {
      gameStarted = true;
    }
  } else if (life > 0) {
    movePipes();
    birdfly();
    showLives();
    showScore();
    checkCollision();
    score++;
    speed--;
  } else {
    lcd.setCursor(3, 0);
    lcd.print("GAME OVER");
    lcd.setCursor(3, 1);
    lcd.print("Score: ");
    lcd.print(score);
    deadChime();
  }

  delay(speed);
}