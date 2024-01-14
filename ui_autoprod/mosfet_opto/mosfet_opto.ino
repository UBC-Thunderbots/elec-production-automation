/* 
 * Production test for measuring the current output for the APC-817D1 optocouplers on the UI Board 
 * 
 * Required components:
 * - External ESP32 PICO V4.1 devkit
 * - UI Board
 * 
 * Connections:
 * [ESP32]            [UI Board]
 *  gnd         -->     gnd
 *  3.3V        -->     Jetson 3.3V
 *  opto1_in    <--     tp17 (boot test point)
 *  opto2_in    <--     tp18 (reset test point)
 *  control_out -->     pb_boot, pb_reset ()
 *  
 * datasheets:
 * {https://www.americanbrightled.com/pdffiles/ir-uv/ir/APC-817.pdf}
 * {https://www.mccsemi.com/pdf/Products/MMSS8050(SOT-23).pdf}
 * {https://docs.espressif.com/projects/esp-idf/en/latest/esp32/hw-reference/esp32/get-started-pico-kit.html}
 * 
 * {https://randomnerdtutorials.com/esp32-adc-analog-read-arduino-ide/} (1)
 * 
 * 
 * README:
 * - sets up and measures the voltage on the output pin (3) on the APC-817D1 optocoupler
 * - sends a digital signal on control_out to the src_boot, src_reset pins on the UI Board, pulls boot/reset low
 * - if all goes well, a reading of ~0.3V should be read on the output pin
 *  
 * Last updated: 13/01/2024, Ryan Nguyen
 * - some minor bug fixes
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

int control_out = 9;      // responsible for turning on the optocoupler and pulls the reset/boot lines to ~0.3V
int opto1_in = 35;        // reads the analog voltage on pin 35 for the optocoupler responsible for 'boot'
int opto2_in = 34;        // reads the analog voltage on pin 34 for the optocoupler responsible for 'reset'
char text_buf[100];

void setup()
{
  pinMode(control_out, OUTPUT);
  pinMode(opto1_in, INPUT);
  pinMode(opto2_in, INPUT);

  Serial.begin(115200);

  Serial.println();
  Serial.println("Initialized optocoupler test");
}

bool test_opto(int in)
{
  bool test;
  double val;
  // send a signal to turn on the optocoupler
  digitalWrite(control_out, HIGH);
  delay(100);
  val = analogRead(in);
  //Serial.print(val);
  if(val > 350 && val < 400)      // according to (1) the ADC on the ESP32 interprets 0V as ~0 and 3.3V as ~4095, a ratio of ~1241
  {
    sprintf(text_buf, "pin[%d] | voltage reading on the output pin of the 817D1: %.1fV\n", in, val/1241);
    Serial.print(text_buf);
    test = true;
  }
  else
  {
    sprintf(text_buf, "test fail :(\n");
    Serial.print(text_buf);
    test = false;
  }
  digitalWrite(control_out, LOW);

  return test;
}

void loop()
{
  test_opto(opto1_in);
  delay(1000);
  test_opto(opto2_in);

  while(1);     // stop test
}
