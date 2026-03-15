#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo doorLock;

String authorizedUID = "12 34 56 78"; // Replace with your RFID card UID

void setup() {
    Serial.begin(9600);
    SPI.begin();
    mfrc522.PCD_Init();
    doorLock.attach(6);  // Servo connected to pin 6
    doorLock.write(0);   // Locked position
    Serial.println("RFID Door Lock Ready...");
}

void loop() {
    if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
        return;

    String UID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        UID += String(mfrc522.uid.uidByte[i], HEX) + " ";
    }
    Serial.println("Card UID: " + UID);

    if (UID == authorizedUID) {
        Serial.println("✅ Access Granted! Unlocking...");
        doorLock.write(90); // Unlock door
        delay(3000);
        doorLock.write(0);  // Lock door again
    } else {
        Serial.println("❌ Access Denied!");
    }

    mfrc522.PICC_HaltA();
}
