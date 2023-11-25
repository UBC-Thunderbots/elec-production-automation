/* 
 * Production test for communication between an external ESP32 MCU 
 * and the ESP32 MC located on the power board.  
 * 
 * Required components:
 * - External ESP32 PICO V4.1 devkit
 * - Power Board
 * 
 * Connections: (refer to definition comments for which pins are tx/rx 1)
 * [ESP32]        [Power Board] (or another ESP32)
 *  gnd     <--     gnd
 *  tx1     <--     rx1
 *  rx1     <--     tx1
 *  
 * README:
 * - to be used in conjuction with 'communication_externalESP32.ino'
 * - sets up and checks communication between the power board and the external ESP32
 *    - will receive a signal from the external ESP32 and sends a return
 *      signal back to the ESP32
 *    - this code assumes that the power board is SERIAL and the ESP32 is SERIAL 2
 * - PWB_TX is an OUTPUT, sends a signal from the power board to the ESP32
 * - PWB_RX is an INPUT, receives a signal from the ESP32 to the power board
 * 
 * Last updated: 3/02/2023, Ryan Nguyen
 */

//#include <HardwareSerial.h>
//HardwareSerial SerialPort(1);    // tells the power board to use UART1

#define PWB_TX 10               // Since we're starting to test this using another ESP32, the TX and RX        
#define PWB_RX 9                // pins will be set to 10 and 9, but these pins might be different on the power board

#define second 1000             // sets the delay to be 1 second when using delay()

void setup() {
  // put your setup code here, to run once:
  // Serial port setup format: (baud-rate, protocol, rx pin, tx pin)
  
  Serial.begin(115200);
  Serial.println("Initialized pwb");
  Serial2.begin(115200, SERIAL_8N1, PWB_RX, PWB_TX);
  String mail = ' ';            // not sure if i even need this...
}

void loop() {
  // put your main code here, to run repeatedly:

   while(Serial2.available())
   {
    String mail = Serial2.read();
    Serial.println(mail);
   }

  //Serial2.write(random(0, 255));
  //delay(second);
  
  //delay(second);
  //Serial.print("Data received from ESP32: ");
}
