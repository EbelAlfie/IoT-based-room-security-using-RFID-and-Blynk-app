#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLRx121DKE" //your ID template
#define BLYNK_DEVICE_NAME "Locker Cabinet Security" //your device name

#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>

char auth[] = "av4q7Z9wr7ikc5wX7GttU7nOYCp20RaG"; //your auth token

/*for WiFI connection*/
char ssid[] = "yourwifi" ; //your Wifi
char pass[] = "" ; //your Wifi password

/*LCD I2C, Servo motor, blynk virtual LCD, blynk timer*/
LiquidCrystal_I2C lcd(0x27, 20, 4);
Servo cabinetServo ;
WidgetLCD blynkLcd(V2) ;
BlynkTimer timer ;

/*RFID reader*/
#define SS_PIN D8 
#define RST_PIN D3
MFRC522 mfrc522(SS_PIN, RST_PIN);  

bool awaitingCardAproval = false ;
bool printed = false ; 
int pos =0;
void setup()
{ 
  Serial.begin(9600); // Debug console
  while (!Serial);
  SPI.begin();  
  lcd.begin();
  lcd.clear();
  lcd.backlight();
  mfrc522.PCD_Init();
  cabinetServo.attach(D4);
  cabinetServo.write(0);
  Blynk.begin(auth, ssid, pass);
  blynkLcd.clear() ;
  Serial.println("Put your card to the reader...");
}

/*Button reject*/
BLYNK_WRITE(V1){
  int a = param.asInt() ;
  if (a == 1 && awaitingCardAproval == true) {
    Serial.println("Admin decided to deny access") ;
    blynkLcd.clear() ;
    blynkLcd.print(0, 0, "Cabinet");
    blynkLcd.print(0, 1, "Is kept close");
    lcd.clear() ;
    lcd.setCursor(0, 0);
    lcd.print("Admin denied");
    lcd.setCursor(0, 1);
    lcd.print("Your access");
    awaitingCardAproval = false;
    delay(1000);
    blynkLcd.clear() ;
    lcd.clear();
    printed = false;
  }
}

/*Button Accept*/
BLYNK_WRITE(V0){
  int a = param.asInt() ;
  if (a == 1 && awaitingCardAproval == true) {
    Serial.println("Admin decided to give access.") ;
    Serial.println("Door is opening...") ;
    blynkLcd.clear();
    blynkLcd.print(0, 0, "Cabinet");
    blynkLcd.print(0, 1, "is opened");
    bukaPintu() ;    
    awaitingCardAproval = false;
    blynkLcd.clear() ;
    lcd.clear();
    printed = false;
  }
}

void loop()
{  
  if(awaitingCardAproval == false && printed == false){
    Serial.println("No one at the moment");
    lcd.setCursor(0, 1);
    lcd.print("Put your card");
    lcd.setCursor(0, 2);
    lcd.print("On RFID reader");
    blynkLcd.print(0, 0, "No one");
    blynkLcd.print(0, 1, "At the moment");
    printed = true ;
  }
  if ( ! mfrc522.PICC_IsNewCardPresent() && awaitingCardAproval == false) 
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial() && awaitingCardAproval == false) 
  {
    return;
  }
  if(awaitingCardAproval == false){
    printed = false;
  }
  /*Print the UID tag*/
  Serial.print("UID tag :");
  awaitingCardAproval = true ;  

  String content= "";
  byte letter;  
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ")); 
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "86 E5 E6 57") //Change "86 E5 E6 57" to your prefered UID
  {
    if(printed == false){
      Serial.println("ID is valid, waiting for users approval");
      Serial.println();
      blynkLcd.clear();
      blynkLcd.print(0,0, "ID " + content.substring(1) );
      blynkLcd.print(0,1, "OPEN CABINET?");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Valid ID,");
      lcd.setCursor(0, 1);
      lcd.print("Awaiting admin's");
      lcd.setCursor(0,2);
      lcd.print("permission");
      printed = true;
    }    
  }else{
    blynkLcd.clear();
    blynkLcd.print(0, 0, "ID: " + content.substring(1));
    blynkLcd.print(0, 1, "Have no access");
    Serial.println("ID is invalid");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Invalid ID");
    lcd.setCursor(0, 1);
    lcd.print("Access denied");
    awaitingCardAproval = false; 
    delay(2000);
    lcd.clear();
    blynkLcd.clear();
    printed = false;
  }
  Blynk.run();
  timer.run();
}

void bukaPintu(){
  Serial.println("");
  lcd.clear() ;
  lcd.setCursor(0, 0);
  lcd.print("Access");
  lcd.setCursor(0, 1);
  lcd.print("Permitted");
  cabinetServo.write(150);
  delay(5000);
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
          cabinetServo.write(pos);     // tell servo to go to position in variable 'pos'
  }
}
