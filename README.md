***
### AUTO-PRODUCTION TESTS
***
These are the current files to test the Power and UI Boards.<br>
Last updated by: Ryan Nguyen

**Available files:**

### >power_autoprod
- communication_externalESP32
- communication_powerboardside

### >ui_autoprod
- buffers
- mosfet_opto

### to-do:

.Integrate the *ui_autoprod* files together into one single file<br>
.Finish *LTC4151-ProductionTest.ino* along with other production tests

***
### FLASHING TO THE ESP32
***
### this tutorial uses Arduino Version 1.8.19, not Arduino 2.x.x ###

[1] Starting at the default IDE window, navigate to ``file --> preferences``<br>
<br>
![1](https://github.com/UBC-Thunderbots/elec-production-automation/assets/97413336/2b2abd5e-909f-4e34-9299-8577147b2ccb) <br>

[2] Inside the preferences window paste the following text into the board manager URLs:<br>
```https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json```<br>
<br>
![2](https://github.com/UBC-Thunderbots/elec-production-automation/assets/97413336/6747bdb5-1996-48eb-99d6-85c13eee5f79) <br>

[3] Click 'OK', then navigate to ```tools --> board: --> boards manager```<br>
<br>
![3](https://github.com/UBC-Thunderbots/elec-production-automation/assets/97413336/de8411a3-f684-492b-ab11-0b6d4cccf620) <br>

[4] Type into the search 'esp32' and install the one library that pops up (latest should be fine, this one is v2.0.6)<br>
<br>
![4](https://github.com/UBC-Thunderbots/elec-production-automation/assets/97413336/5988ef6a-6e6e-405d-9cc9-596dc63ecc65) <br>

[5] Once the library is installed go back to ```tools --> board: --> (you should now see) ESP32 Arduino --> ESP32 PICO-D4```<br>
<br>
![5](https://github.com/UBC-Thunderbots/elec-production-automation/assets/97413336/60d4de51-0552-4122-8080-a3ebb25d7016) <br>

[6] From here you can change settings within ```tools``` such as BAUD rate, serial port, FLASH erase .. etc.<br>
<br>
![6](https://github.com/UBC-Thunderbots/elec-production-automation/assets/97413336/381fcbbb-9572-4308-82bc-c4bec46640b3) <br>

[7] To start flashing, assuming you have a sketch ready that compiles, make sure that the port in ```tools``` is the same port as the one connected to your ESP32. You can verify by going to the ```device manager``` on your windows machine and looking for a UART BRIDGE device in the PORTS (COM & LPT) drop-down:<br>
<br>
![Screenshot 2023-12-02 131117](https://github.com/UBC-Thunderbots/elec-production-automation/assets/97413336/6305aa45-c928-46c6-ac35-30e50d6e8c2a)

[8] Once that is all done, you can hit the sketch upload button within Arduino IDE. Monitor the terminal, and once you see the ```connecting.......``` message hit the ```BOOT``` button on the ESP32. Another option is holding down the button and releasing it once the connecting message is seen.<br>
<br>
![8](https://github.com/UBC-Thunderbots/elec-production-automation/assets/97413336/d2f9acc9-da3d-4b1d-9f1d-22dcb14aecd4) <br>

And just like that, we should be good to go. I'd recommend flashing this blinky code to test out the ESP32 and get comfy with the flashing process. Happy flashing!<br>

```C
int LED_BUILTIN = 2;


void setup() {

pinMode (LED_BUILTIN, OUTPUT);

}


void loop() {

digitalWrite(LED_BUILTIN, HIGH);

delay(1000);

digitalWrite(LED_BUILTIN, LOW);

delay(1000);

}
```
