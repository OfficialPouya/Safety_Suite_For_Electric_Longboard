

## Li-ion Output Regulation

### Built-in

[Bluetooth / 802.15.1 Development Tools Adafruit Feather M0 Bluefruit LE](https://www.amazon.com/Bluetooth-802-15-1-Development-Adafruit-Bluefruit/dp/B01E1RESIM)

*Do not use external regulation to 3.3V pin

### Linear Regulator

[ESP8266 Voltage Regulator (LiPo and Li-ion Batteries)](https://randomnerdtutorials.com/esp8266-voltage-regulator-lipo-and-li-ion-batteries/)

MCP1700-3302: [Digikey](https://www.digikey.com/en/products/detail/microchip-technology/MCP1700-3302E-TO/652680)  [Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/MCP1700-Low-Quiescent-Current-LDO-20001826E.pdf) 

Low Dropout (LDO) Voltage - <u>178 mV</u> Typical @ 250 mA for VOUT = 2.8V

3.3V+ 178mV = 3.478V <- Min Li-ion battery voltage

*Can we go lower than 3.3V?*

### SMPS

LTC3440- Buck/Boost

## Bluetooth Integration

### Built-in

[Bluetooth / 802.15.1 Development Tools Adafruit Feather M0 Bluefruit LE](https://www.amazon.com/Bluetooth-802-15-1-Development-Adafruit-Bluefruit/dp/B01E1RESIM)

[Arduino Nano 33 BLE ABX00030](https://www.amazon.com/Arduino-Nano-33-BLE/dp/B07WV59YTZ/ref=sr_1_2?dchild=1&keywords=**Arduino+Nano+33+BLE+%5BABX00030%5D**&qid=1612056793&sr=8-2)



### HC-05 

Already have this,



## Low Power Operation

### [Adafruit Feather M0 Bluefruit LE](https://www.amazon.com/Bluetooth-802-15-1-Development-Adafruit-Bluefruit/dp/B01E1RESIM)

[Datasheet](https://cdn-learn.adafruit.com/downloads/pdf/adafruit-feather-m0-bluefruit-le.pdf)

[Add Power Switch](https://io.adafruit.com/blog/tip/2016/12/14/feather-power-switch/)

CP2104 USB to Serial chip-> remove if here

*need to measure current thru supply*

## Battery Level Indicator

### Coulomb Counter

LTC4150

### Voltage

