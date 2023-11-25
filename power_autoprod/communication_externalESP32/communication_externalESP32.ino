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
 *  gnd     -->     gnd
 *  tx1     -->     rx1
 *  rx1     -->     tx1
 *  
 * README:
 * - to be used in conjuction with 'communication_powerboardside.ino'
 * - sets up and checks communication between the external esp32 and the power board
 *    - sends a basic[**] signal to the power board esp 
 *      and checks for the received signal from the power board esp
 *    - can be used between two ESP32 PICO V4.1 devkits, just make sure they're wired up properly
 *    - this code assumes that the external ESP32 is SERIAL and the power board is SERIAL 2
 * - ESP_TX is an OUTPUT, sends a signal from the ESP32 to the power board
 * - ESP_RX is an INPUT, receives a signal from the power board to the ESP32
 *  
 * Last updated: 3/02/2023, Ryan Nguyen
 * [**] potentially could be changed to be modular in a future revision
 */

//#include <HardwareSerial.h> 
//HardwareSerial SerialPort(1);   // tells the ESP32 to use UART1

#define ESP_TX 10               // pins 10 and 9 on the PICO V4.1 are available to be used as UART1, (see note at bottom) 
#define ESP_RX 9                // UART0 (dedicated TX0 and RX0 on the board) are reserved for flashing

#define second 1000             // sets the delay to be 1 second when using delay() 

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial.println("Initialized extESP32");
  Serial2.begin(115200, SERIAL_8N1, ESP_RX, ESP_TX);  // Serial port setup format: (baud-rate, protocol, rx pin, tx pin)
}

void loop() {
  // put your main code here, to run repeatedly:

  Serial2.write("extESP to pwb!");
  delay(second);

  //Serial.print("Data received from power board: ");
  //Serial.println(Serial1.readString());               // prints to the external ESP32 serial whatever was sent from the power board
  //delay(second);

  
}

/* An important point to note here is that GPIO pin 9 and pin 10 which are part of UART1 TX and RX pins, 
 * are internally connected to the flash memory. Moreover, these pins are also sometimes not available on the ESP32 boards. 
 * Hence, we will have to reassign the pins for UART1 for serial communication. Luckily, the ESP32 board is 
 * capable to use almost all GPIO pins for serial connections. Here we have reassigned GPIO4 as RX pin and GPIO2 as TX pin.
 */
