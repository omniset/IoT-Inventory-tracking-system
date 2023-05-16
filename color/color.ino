#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>
#include <Servo.h>
#define SS_PIN 4  //D2
#define RST_PIN 5 //D1
#include <SPI.h>
#include <MFRC522.h>

const char * myWriteAPIKey = "E5HT8NGK54DWY134";
unsigned long myChannelNumber = 2141311;
const char *ssid =  "Calistu Ionut Home";    
const char *pass =  "MeRiCaNut2714"; 
WiFiClient client;
Servo dropServo;
const int s0 = D4;
const int s1 = D5;
const int s2 = D6;  
const int s3 = D7;  
const int out = D8; 
  
int red = 0;  
int green = 0;  
int blue = 0; 
int color=0;
int redcolor = 0;
int greencolor = 0;
int orangecolor = 0;
int yellowcolor = 0;

int pos = 0;

MFRC522 mfrc522(SS_PIN, RST_PIN);   
int variable = 0;

void setup()   
{  
  Serial.begin(9600); 
  pinMode(s0, OUTPUT);  
  pinMode(s1, OUTPUT);  
  pinMode(s2, OUTPUT);  
  pinMode(s3, OUTPUT);  
  pinMode(out, INPUT);  
  digitalWrite(s0, HIGH);  
  digitalWrite(s1, HIGH);  
  dropServo.attach(D1);
  
  Serial.begin(9600);   
  SPI.begin();      
  mfrc522.PCD_Init();  
  
  ThingSpeak.begin(client);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) 
     {
            delay(550);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
}  
void loop() 
{  
  digitalWrite(s2, LOW);  
  digitalWrite(s3, LOW);  
  //count OUT, pRed, RED  
  red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
  digitalWrite(s3, HIGH);  
  //count OUT, pBLUE, BLUE  
  blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
  digitalWrite(s2, HIGH);  
  //count OUT, pGreen, GREEN  
  green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH); 
  Serial.print("R Intensity:");  
  Serial.print(red, DEC);  
  Serial.print(" G Intensity: ");  
  Serial.print(green, DEC);  
  Serial.print(" B Intensity : ");  
  Serial.print(blue, DEC);  
    if(red<85 & red>75 & green<80 & green>73 &blue<81 & blue>76){
      dropServo.write(73);
      redcolor++;
      Serial.print("Red");
      ThingSpeak.writeField(myChannelNumber, 1,redcolor, myWriteAPIKey);
    }
    if(green<45 & green>40 & blue<55 &blue>48){
      dropServo.write(107);
      orangecolor++;
      Serial.print("Orange");
      ThingSpeak.writeField(myChannelNumber, 2,orangecolor, myWriteAPIKey);
    }
    if(red<112 & red>100 & green<80 & green>63){
      dropServo.write(132);
      greencolor++;
      Serial.print("Green");
      ThingSpeak.writeField(myChannelNumber, 3,greencolor, myWriteAPIKey);
    }
    if(red<35 & red>22 & green<33 & green>28 & blue<35 & blue>28){
      dropServo.write(162);
      yellowcolor++;
      Serial.print("Yellow");
      ThingSpeak.writeField(myChannelNumber, 4,yellowcolor, myWriteAPIKey);
    }
  Serial.println();
  delay(7000);

  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //nampilin tag di serial monitor
  Serial.print(" UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();
  if (content.substring(1) == "C1 9A 15 44") 
  {
    Serial.println(" KOTAK BUAH APEL ");
    Serial.println(" WARNA:MERAH ");
    delay(300);
  }
  
   else if (content.substring(1) == "83 F5 82 1D")
    {
      Serial.println(" KOTAK BUAH APEL ");
      Serial.println(" WARNA:YELLOW ");
      delay(300);
    }
   else if (content.substring(1) == "")
    {
      Serial.println(" KOTAK BUAH APEL ");
      Serial.println(" WARNA:ORANGE ");
      delay(300);
    }
   else if (content.substring(1) == "") 
    {
      Serial.println(" KOTAK BUAH APEL ");
      Serial.println(" WARNA:GREEN ");
      delay(300);
    }
}
