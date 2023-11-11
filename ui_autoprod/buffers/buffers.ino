/* 
 * Production test for testing the TXU0304 buffers on the UI board.
 * 
 * Required components:
 * - External ESP32 PICO V4.1 devkit
 * - UI Board
 * 
 * Connections:
 * [ESP32]            [UI Board]
 *  gnd         -->     gnd
 *  3.3V        -->     Jetson 3.3V
 *  signal_out  -->     sck
 *  signal_in   <--     tp8 (sck test point)
 *  
 * datasheets:
 * {https://www.ti.com/lit/ds/symlink/txu0304.pdf?ts=1680909156782&ref_url=https%253A%252F%252Fwww.google.com%252F}
 * {https://docs.espressif.com/projects/esp-idf/en/latest/esp32/hw-reference/esp32/get-started-pico-kit.html}
 * 
 *  **The TXU0304 buffers can only handle <20mA
 *  WAIT UNTIL I DO SOME PHYSICAL TESTING TO FIND OUT THE REST OF THE WIRING SCHEME
 *  RIGHT NOW IT'S GOING TO BE 5 WIRES IN PARALLEL WITH A SINGLE OUTPUT PIN  
 *  
 * README:
 * - sets up and checks signal clarity between the external esp32 and the ui board
 *    - sends a signal with variable frequency from signal_out to sck,
 *      and checks for the same received signal from the ui board esp
 * - signal_out is an OUTPUT, sends a signal from the ESP32 to the ui board
 * - signal_in is an INPUT, receives a signal from the ui board to the ESP32
 *  
 * Last updated: 07/04/2023, Ryan Nguyen
 * - added functionality with all 5 buffers, just need to test this by pinning up
 *
 *                                                                   ESP32 PICO V4 pinout
 *                                                                       ___________
 *                                                                      |           |
 *                                 EMAC_CLK_OUT/U2RXD/HS1_DATA4/GPIO16 -| CS     D1 |- GPIO8/SD_DATA1/SPID/HS1_DATA1/U2CTS
 *                             EMAC_CLK_OUT_180/U2TXD/HS1_DATA5/GPIO17 -| D0     D3 |- GPIO7/SD_DATA0/SPIQ/HS1_DATA0/U2RTS
 *                                  U1RTS/HS1_CMD/SPICS0/SD_CMD/GPIO11 -| D2     CLK|- GPIO6/SD_CLK/SPICLK/HS1_CLK/U1CTS
 *                                           RTC_GPIO0/ADC1_CH0/GPIO36 -| VP     21 |- GPIO21/VSPIHD/EMAC_TX_EN
 *                                           RTC_GPIO3/ADC1_CH3/GPIO39 -| VN     22 |- GPIO22/VSPIWP/U0RTS/EMAC_TXD1
 *                           EMAC_RXD0/RTC_GPIO6/ADC2_CH8/DAC_1/GPIO25 -| 25     19 |- GPIO19/VSPIQ/U0CTS/EMAC_TXD0
 *                           EMAC_RXD1/RTC_GPIO7/ADC2_CH9/DAC_2/GPIO26 -| 26     23 |- GPIO23/VSPID/HS1_STORE
 *                                    RTC_GPIO9/TOUCH9/ADC1_CH4/32K_XP -| 32     18 |- GPIO18/VSPICLK/HS1_DATA7
 *                                    RTC_GPIO8/TOUCH8/ADC1_CH5/32K_XN -| 33      5 |- GPIO5/VSPICS0/HS1_DATA6/EMAC_RX_CLK
 *                                EMAC_RX_DV/RTC_GPIO7/TOUCH7/ADC2_CH7 -| 27     10 |- GPIO10/SD_DATA3/SPIWP/HS1_DATA3/U1TXD
 *    EMAC_TXD2/SD_CLK/HS2_CLK/HSPICLK/MTMS/RTC_GPIO16/TOUCH6/ADC2_CH6 -| 14      9 |- GPIO9/SD_DATA2/SPIHD/HS1_DATA2/U1RXD
 *  EMAC_TXD3/SD_DATA2/HS2_DATA2/HSPIQ/MTDI/RTC_GPIO15/TOUCH5/ADC2_CH5 -| 12     RX0|- GPIO3/U0RXD/CLK_OUT2
 * EMAC_RX_ER/SD_DATA3/HS2_DATA3/HSPID/MTCK/RTC_GPIO14/TOUCH4/ADC2_CH4 -| 13     TX0|- GPIO1/U0TXD/CLK_OUT3/EMAC_RXD2
 *    EMAC_RXD3/SD_CMD/HS2_CMD/HSPICS0/MTDO/RTC_GPIO13/TOUCH3/ADC2_CH3 -| 15     35 |- ADC1_CH7/RTC_GPIO5
 *                SD_DATA0/HS2_DATA0/HSPIWP/RTC_GPIO12/TOUCH2/ADC2_CH2 -| 2      34 |- ADC1_CH6/RTC_GPIO4
 *    EMAC_RTX_ER/SD_DATA1/HS2_DATA1/HSPIHD/RTC_GPIO10/TOUCH0/ADC2_CH0 -| 4      38 |- GPIO38/ADC1_CH2/RTC_GPIO2
 *                     EMAC_TX_CLK/CLK_OUT1/RTC_GPIO11/TOUCH1/ADC2_CH1 -| 0      37 |- GPIO37/ADC1_CH1/RTC_GPIO1
 *                                                                 3V3 -|3V3     EN |- CHIP_PU
 *                                                                 GND -|GND     GND|- GND
 *                                                                 5V0 -| 5V     3V3|- 3V3
 *                                                                      |___________|
 * 
 * 
 */

#include <arduino.h>
#include <stdlib.h>
#include <stdio.h>

int signal_out = 38;  // the pin responsible for sending a small signal through the buffers
int buf1_in = 9;      // checks pin 9 for the output of buffer 1
int buf2_in = 10;     // checks pin 10 for the output of buffer 2
int buf3_in = 5;      // checks pin 5 for the output of buffer 3
int buf4_in = 18;     // checks pin 18 for the output of buffer 4
int buf5_in = 23;     // checks pin 23 for the ouptput of buffer 5
char text_buf[100];
double frequency = 1000; // frequency for the small pulse (in Hz)

void setup() 
{
  pinMode (signal_out, OUTPUT);
  pinMode (buf1_in, INPUT);
  pinMode (buf2_in, INPUT);
  pinMode (buf3_in, INPUT);
  pinMode (buf4_in, INPUT);
  pinMode (buf5_in, INPUT);
  
  Serial.begin(115200);

  Serial.println();
  Serial.println("Initialized buffer test");
  Serial.print("frequency: ");
  Serial.println(frequency);
}

bool check_pulse(int frequency, int in) // in: response from buffer; out: signal sent to buffer
{ 
  bool test_h = false;          // default case = false
  bool test_l = false;

  digitalWrite(signal_out, HIGH);
  // check signal high
  if (digitalRead(in))        // if 'in' is 1
  {
    //Serial.println("pass: response is HIGH");
    test_h = true;
  }
  else
  {
    //Serial.println("fail: response is not as expected!");
    test_h = false;
  }

  delay( 1/(frequency*2) );   // keeps the signal high for half the period
  digitalWrite(signal_out, LOW);
  // check signal low
  if (!digitalRead(in))       // if 'in' is 0
  {
    //Serial.println("pass: response is LOW");
    test_l = true;
  }
  else
  {
    //Serial.println("fail: response is HIGH (expected: LOW)");
    test_l = false;
  }
  
  delay( 1/(frequency*2) );   // keeps the signal low for half the period 
  Serial.println("..........");
  if (test_h && test_l)
  {
    sprintf(text_buf, "[pass] input on pin %d detected\n", in);
    Serial.print(text_buf);
    return true;
  }

  return false;
}

void loop()
{
  check_pulse(frequency, buf1_in);
  delay(500);
  check_pulse(frequency, buf2_in);
  delay(500);
  check_pulse(frequency, buf3_in);
  delay(500);
  check_pulse(frequency, buf4_in);
  delay(500);
  check_pulse(frequency, buf5_in);

  while(1);                           // stop test
}