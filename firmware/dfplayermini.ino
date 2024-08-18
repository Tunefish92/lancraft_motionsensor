#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// DFPlayer Mini
SoftwareSerial mySoftwareSerial(10, 11);  // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

// Bewegungsmelder
int Bewegungsstatus = 0;
int Bewegungsmelder = 7;

bool connected = false;

void setup() {
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

  Serial.println();
  Serial.println(F("Lancraft Motionsensor with audio!"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  delay(3000);

  while (!connected) {
    if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
      Serial.println(F("Error..."));
      delay(1000);
    } else {
      connected = true;
      Serial.println(F("DFPlayer Mini online!"));
      myDFPlayer.volume(15);  //Set volume value. From 0 to 30
      myDFPlayer.play(2);
      delay(3000);
    }
  }


  pinMode(Bewegungsmelder, INPUT);
}
void loop() {
  Bewegungsstatus = digitalRead(Bewegungsmelder);

  if (Bewegungsstatus == HIGH) {
    Serial.println(("Motionsensor active"));
    myDFPlayer.play(1);
    delay(7000);
  }

  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read());  //Print the detail message from DFPlayer to handle different errors and states.
  }
}

void printDetail(uint8_t type, int value) {
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!\n"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!\n"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!\n"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!\n"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!\n"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!\n");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!\n");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!\n"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:\n"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found\n"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping\n"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack\n"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match\n"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound\n"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File\n"));
          break;
        case Advertise:
          Serial.println(F("In Advertise\n"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
