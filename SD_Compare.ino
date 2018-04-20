/*
  SD card read/write

 This example shows how to read and write data to and from an SD card file
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4

 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */
#include <SPI.h>
#include <SD.h>
#define CSpin SS
File myFile;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

// -------------------------------------------------------------
  Serial.print("Initializing SD card...");

  if (!SD.begin(SS)) { // port change
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("database.csv", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    /* Serial.print("Writing to database.csv...");
    myFile.println("database.csv");
    // close the file :*/
    myFile.close();
    // Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening database.csv");
  }

  // re-open the file for reading:
  myFile = SD.open("database.csv");
  String data = "";
  String check = "";
  String input = "Compare----here";
  char temp;
  int count = 0; // count ","
  int info_id = 0;
  int found = 0;
  int pass = 0;
  if (myFile) {
    Serial.println("database.csv");
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      // -------------------------
      // input = "xxxxxxxx"
      
      temp = (char)myFile.read();
      // print information about
     if (temp == ',' && pass != 10) {
          pass++;
     }
     else if (pass == 10) {
          if (temp != ',' && temp >= 32) {
            data += temp; // data == rfid code
          }
          else if (temp == ',') {
              // function compare -------
              if (data == input) {
                Serial.print("Card was Founded!! Card's code is : ");
                Serial.println(data);
                Serial.println("------------------------------");
                data = "";
                // Read the RFID
                while (1) {
                    temp = (char)myFile.read();
                    if (temp == ',') {
                        count++;
                        // there's 9 of ',' before new index of information
                        if (count == 1) {Serial.print("Number : "); Serial.println(data); data = "";}
                        else if (count == 2) {Serial.print("Name : "); Serial.println(data); data = "";}
                        else if (count == 3) {Serial.print("Type : "); Serial.println(data); data = "";}
                        else if (count == 4) {Serial.print("Specie : "); Serial.println(data); data = "";}
                        else if (count == 5) {Serial.print("Gender : "); Serial.println(data); data = "";}
                        else if (count == 6) {Serial.print("Birth of Date : "); Serial.println(data); data = "";}
                        else if (count == 7) {Serial.print("Color : "); Serial.println(data); data = "";}
                        else if (count == 8) {Serial.print("Owner : "); Serial.println(data); data = "";}
                        else if (count == 9) {Serial.print("Contact tel : "); Serial.println(data); data = "";
                            Serial.println("------------------------------");
                            found = 1;
                            break;
                        }
                    }
                    else if (temp != ',') {data += temp;}
                }
              // break; // SHOWING ALL TEXT HERE
              count = 0;
              }
              else if (data != input) {
                pass = 1;
                /*Serial.println(input); // data = rfid card_num
                Serial.println(input.length());
                int i = 0;
                while(i <= 8){
                  Serial.printf("%d\n", input[i]);
                  i++;                  
                }
                Serial.println(data); // data = rfid card_num
                Serial.println(data.length());
                i = 0;
                while(i <= 8){
                  Serial.printf("%d\n", data[i]);
                  i++;                  
                }*/
                data = "";
              }
          }
     }
     if (temp == '#' && found == 0) {
        Serial.println("Not Founded");
     }
    }
    // Serial.println(data);
        // Serial.println((char)myFile.read());
        // data += (char)myFile.read();
      // ----------------------------
    // Serial.print(data);
    
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening database.csv");
  }
}

void loop()
{
  // nothing happens after setup
}

