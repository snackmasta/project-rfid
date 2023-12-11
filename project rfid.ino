#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

constexpr uint8_t RST_PIN = 8;  // Adjust to your wiring, corresponding to D3
constexpr uint8_t SS_PIN = 10;  // Adjust to your wiring, corresponding to D8
constexpr uint8_t SERVO_PIN = 9;  // Adjust to your wiring

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
Servo myServo;  // Create Servo instance
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

bool servoAtZero = true;  // Variable to store servo position state
char* status = "Closed";
float kolom = 6;

void setup() {
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  myServo.attach(SERVO_PIN,550,2500);
  Serial.println(F("Scan a card..."));
  lcd.init();                      // initialize the lcd 
  lcd.init();

}

void loop() {
  if (! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  if (! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Display UID (same as before)

  // Check if the scanned card is the one we are looking for
  if (mfrc522.uid.uidByte[0] == 0xE3 && mfrc522.uid.uidByte[1] == 0xBA &&
      mfrc522.uid.uidByte[2] == 0x4B && mfrc522.uid.uidByte[3] == 0x0E) {
    Buka();  // Toggle servo position
    layar();
    delay(3000);  // Wait for 5 seconds
    Tutup();
  }

  if (mfrc522.uid.uidByte[0] == 0xC3 && mfrc522.uid.uidByte[1] == 0xD8 &&
      mfrc522.uid.uidByte[2] == 0x88 && mfrc522.uid.uidByte[3] == 0x11) {
    Buka();  // Toggle servo position
    layar();
    delay(3000);  // Wait for 5 seconds
    Tutup();
  }

  if (mfrc522.uid.uidByte[0] != 0xE3 && mfrc522.uid.uidByte[1] != 0xBA &&
      mfrc522.uid.uidByte[2] != 0x4B && mfrc522.uid.uidByte[3] != 0x0E &&
      mfrc522.uid.uidByte[0] != 0xC3 && mfrc522.uid.uidByte[1] != 0xD8 &&
      mfrc522.uid.uidByte[2] != 0x88 && mfrc522.uid.uidByte[3] != 0x11) {
    Tutup();
    kolom = 1;
    status="Akses Ditolak!";
  }
  
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  layar();

}

void Buka() {
    myServo.write(0);  // Move servo to 180 degrees
    kolom = 6;
    status="Open";
}

void Tutup() {
    myServo.write(120);  // Move servo back to 0 degrees
    kolom = 5;
    status="Closed";
}

void layar(){
    // Print a message to the LCD.
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(kolom,0);
  lcd.print(status);
  lcd.setCursor(2,1);

}
