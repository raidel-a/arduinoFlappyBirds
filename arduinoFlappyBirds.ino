#include <Wire.h>
#include <rgb_lcd.h>
#include "notes.h"

rgb_lcd lcd;

#define BUTTON_PIN 4
#define BUZZER_PIN 3
#define GREEN_LED_PIN 2
#define RED_LED_PIN 7

bool hasPlayedDeadChime = false;

void playStartupChime() {
  tone(BUZZER_PIN, 1000);
  delay(100); 
  noTone(BUZZER_PIN);
}

void playCoinChime() {
  tone(BUZZER_PIN, NOTE_B5);
  delay(200);
  noTone(BUZZER_PIN);
  analogWrite(BUZZER_PIN, NOTE_E6);
  delay(400);
  noTone(BUZZER_PIN);
}

void playCollisionChime() {
  tone(BUZZER_PIN, NOTE_A4);
  delay(300);
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, NOTE_A4);
  delay(200);
  noTone(BUZZER_PIN);
}

void playDeadChime() {
  if (!hasPlayedDeadChime){
    tone(BUZZER_PIN, NOTE_B5);
    delay(400);
    noTone(BUZZER_PIN);
    tone(BUZZER_PIN, NOTE_F5);
    delay(500);
    noTone(BUZZER_PIN);
    tone(BUZZER_PIN, NOTE_F5);
    delay(500);
    noTone(BUZZER_PIN);
    tone(BUZZER_PIN, NOTE_E5);
    delay(400);
    noTone(BUZZER_PIN);
    tone(BUZZER_PIN, NOTE_D4);
    delay(300);
    noTone(BUZZER_PIN);
    tone(BUZZER_PIN, NOTE_C4);
    delay(400);
    noTone(BUZZER_PIN);
    tone(BUZZER_PIN, NOTE_E4);
    delay(300);
    noTone(BUZZER_PIN);
    tone(BUZZER_PIN, NOTE_E4);
    delay(200);
    noTone(BUZZER_PIN);
    tone(BUZZER_PIN, NOTE_C4);
    delay(200);
    noTone(BUZZER_PIN);
    hasPlayedDeadChime = true;
  }
}

byte pipeSymbol[8] = {
  0b00000,
  0b11111,
  0b11111,
  0b01110,
  0b01110,
  0b01110,
  0b01110,
  0b01110
};

byte pipeTopSymbol[8] = {
  0b01110,
  0b01110,
  0b01110,
  0b01110,
  0b01110,
  0b11111,
  0b11111,
  0b00000
};

byte birdSymbol[8] = {
  0b00000,
  0b00110,
  0b10111,
  0b11110,
  0b11110,
  0b01100,
  0b00000,
  0b00000
};

byte birdAnimSymbol[8] = {
  0b00000,
  0b00000,
  0b00110,
  0b10111,
  0b11110,
  0b11110,
  0b01100,
  0b01000
};

byte hitSymbol[8] = {
  0b10001,
  0b11011,
  0b01010,
  0b00100,
  0b00100,
  0b01010,
  0b11011,
  0b10001
};

byte heartSymbol[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00100
};

byte coinSymbol[8] = {
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
  lcd.createChar(0, pipeSymbol);
  lcd.createChar(1, birdSymbol);
  lcd.createChar(2, hitSymbol);
  lcd.createChar(3, heartSymbol);
  lcd.createChar(4, birdAnimSymbol);
  lcd.createChar(5, pipeTopSymbol);
  lcd.createChar(6, coinSymbol);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
}

void birdFly() {
  int buttonValue = digitalRead(BUTTON_PIN);
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
     
    digitalWrite(RED_LED_PIN, HIGH);
    delay(100);
    playCollisionChime();
    digitalWrite(RED_LED_PIN, LOW);
    delay(100);
    digitalWrite(RED_LED_PIN, HIGH);
    delay(100);
    digitalWrite(RED_LED_PIN, LOW);
  } else if (coins[birdX] == 1 && birdY != 1) {
    lcd.setCursor(birdX, birdY);
    lcd.write(1);
    coins[birdX] = 0;
    score += 5;
    
    digitalWrite(GREEN_LED_PIN, HIGH);
    delay(100);
    playCoinChime();
    digitalWrite(GREEN_LED_PIN, LOW);
    delay(100);
    digitalWrite(GREEN_LED_PIN, HIGH);
    delay(100);
    digitalWrite(GREEN_LED_PIN, LOW);
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
    playStartupChime();
    if (digitalRead(BUTTON_PIN) == HIGH) {
      gameStarted = true;
    }
  } else if (life > 0) {
    movePipes();
    birdFly();
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
    playDeadChime();
    digitalWrite(RED_LED_PIN, HIGH); 
  }

  delay(speed);
}