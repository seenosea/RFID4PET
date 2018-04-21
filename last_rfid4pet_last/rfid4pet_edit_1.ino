#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include "config.h"

String Value = "";

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

// Init array that will store new NUID 
byte nuidPICC[4];

void setup() { 
  Serial.begin(baud);
  Serial.println(WiFi.localIP());
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting\n");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(250);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_KEY);
  //Firebase.set("f255dd49/NAME", "JOB");
  
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  Serial.println(F("This code scan the MIFARE Classsic NUID."));
  Serial.print(F("Using the following key:"));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
}
 
void loop() {
  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

    Serial.println(F("A new card has been detected."));

    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }
   
    Serial.println(F("The NUID tag is:"));
    Serial.print(F("In hex: "));
    sentValue(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
    Serial.print(F("In dec: "));
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
    Serial.println(Value);
    // Start Firebase
    FirebaseObject data = Firebase.get(Value);
    if(data.isNullString()){
      // If null
      Serial.println("Card wasn't found");
      /*Serial.println("Enter");
      while (Serial.available() > 0) {
        Firebase.set(Value+"/NAME", Serial.read());
    
      }*/
    }else{
      // If have data
      Serial.println("--------------------------------------------------");
      
      Serial.print("RFID code : ");
      Serial.println(data.getString("ID"));
      
      Serial.print("Number : ");
      Serial.println(data.getString("Number"));

      Serial.print("Pet's Name : ");
      Serial.println(data.getString("Name"));
      
      Serial.print("Kind : ");
      Serial.println(data.getString("Kind"));

      Serial.print("Specie : ");
      Serial.println(data.getString("Specie"));

      Serial.print("Gender : ");
      Serial.println(data.getString("Gender"));

      Serial.print("Date of Birth : ");
      Serial.println(data.getString("Date of Birth"));

      Serial.print("Colors : ");
      Serial.println(data.getString("Colors"));

      Serial.print("Owner : ");
      Serial.println(data.getString("Owner"));

      Serial.print("Contact tel : ");
      Serial.println(data.getString("Contact"));

      Serial.println("------------------------------------------------");
    }

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
  Value = "";
}


/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}

void sentValue(byte *buffer, byte bufferSize) {
  
  for (byte i = 0; i < bufferSize; i++) {
    buffer[i] < 0x10 ? " 0" : " ";
    Value += String(buffer[i], HEX);
  }
  Serial.print(String(Value));
  Serial.println("Sent successful");
}
