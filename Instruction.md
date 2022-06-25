CabinetSecurity.ino is the program that you can upload to your ESP8266
Blynk app folder contains image of application interface
Circuit contains image of every state of the circuit

# IoT-based-room-security-using-RFID-and-Blynk-app
This is an IoT based room security using RFID and ESP8266 Wemos D1 R2 mini and Blynk app. Regardless of the UID, the ESP8266 will send UID scanned on the RFID reader to Blynk app using Blynk cloud

What you need is:
1. ESP8266 microcontroller in order to connect to wifi and send the data to Blynk app (for this, I use ESP8266 Wemos D1 R2 mini)
2. LCD 20x4 with I2C interface to display some text to guest
3. Servo motor that act as the lock
4. Bread board
5. Jumper cable
6. RFID card and tag
7. RC522 RFID reader to scan the RFID card
8. Arduino IDE
9. Blynk IoT app in your mobile phone

Connect RFID reader to ESP8266 using jumper cable where the pin is
RFID  | ESP8266 Wemos
------------------------
SDA   to SS/ D8
SCK   to SCK/ D5
MOSI  to MOSI/ D7
MISO  to MISO/ D6
GND   to GND 
VCC   to 3.3V
RST   to D3

RFID can communicate to ESP8266 in three different ways. Depending on that, 
The MISO pin of RFID will act as Master In Slave Out (MOSI) if SPI interface is enabled, will act as Serial CLock (SCL) if I2C interface is enabled, and will act as serial data output (Tx) if UART is enabled
The SDA pin of RFID will act as Slave Select (SS) if SPI interface is enabled, will act as Serial Data (SDA) if I2C interface is enabled, and will act as serial data input (Rx) if UART is enabled.

For this project, SPI communication is used, so be sure to #include <SPI.h> library in your IDE
be sure to #include <MFRC522.h> too for the RFID made by https://github.com/miguelbalboa/rfid.git

Connect servo motor to ESP8266
Servo             | ESP8266
---------------------
VCC (Red)         to 5V
GND (Dark brown)  to GND
Signal (Yellow)   to any digital pin (D4 in my case)

Be sure to #include <servo.h> to allow ESP8266 to control the servo motor

Connect LCD with I2C to ESP8266
I2C    | ESP8266
------------------
VCC    to 5V
GND    to GND
SCL    to SCL
SDA    to SDA

Be sure to #include <LiquidCrystal_I2C.h> in order to print messages on the LCD using I2C interface

To make a Blynk device, create a new template and name it. And then, set its HARDWARE to ESP8266 and CONNECTION TYPE to Wi-Fi

Then, be sure to set up your Blynk app interface. The widgets needed are as follow:
1. LCD with Virtual pin (V2) with String data type (you can set this up in Blynk.cloud from your PC)
2. Button. Label it with "Accept" and set its dataStream to virtual pin (V0) with integer data type
3. Button. Label it with "Deny" and set its dataStream to virtual pin (V1) with integer data type

As we are using ESP8266, be sure to #include <BlynkSimpleEsp8266.h> in order to enable the communication between ESP8266 and blynk app.

Create a new device base on your template and be sure to remember the Auth token, device name, and template ID.

Go to your IDE and program the ESP8266

