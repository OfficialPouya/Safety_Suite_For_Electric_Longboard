General Links:

Adafruit Feather M0 Bluefruit LE- [Amazon](https://www.amazon.com/Bluetooth-802-15-1-Development-Adafruit-Bluefruit/dp/B01E1RESIM) [Datasheet](https://cdn-learn.adafruit.com/downloads/pdf/adafruit-feather-m0-bluefruit-le.pdf)

MCP1700-3302 LDO- [Digikey](https://www.digikey.com/en/products/detail/microchip-technology/MCP1700-3302E-TO/652680)  [Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/MCP1700-Low-Quiescent-Current-LDO-20001826E.pdf) 

Arduino Nano 33 BLE- [Amazon](https://www.amazon.com/Arduino-Nano-33-BLE/dp/B07WV59YTZ/ref=sr_1_2?dchild=1&keywords=**Arduino+Nano+33+BLE+%5BABX00030%5D**&qid=1612056793&sr=8-2)

LTC4150- [Datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/4150fc.pdf)



# Li-ion Output Regulation

## Built-in

- Adafruit Feather M0 Bluefruit LE

> Do not use external regulation to 3.3V pin

## Linear Regulator

[Youtube: ESP8266 Voltage Regulator (LiPo and Li-ion Batteries)](https://randomnerdtutorials.com/esp8266-voltage-regulator-lipo-and-li-ion-batteries/)

Use MCP1700-3302

Low Dropout (LDO) Voltage - <u>178 mV</u> Typical @ 250 mA for VOUT = 2.8V

3.3V+ 178mV = 3.478V <- Min Li-ion battery voltage

> Can we go lower than 3.3V?

## SMPS

LTC3440- Buck/Boost

# Bluetooth Integration

## Built-in

- Adafruit Feather M0 Bluefruit LE

- Arduino Nano 33 BLE

## HC-05 

> (Already have this)

# Low Power Operation

## Adafruit Feather M0 Bluefruit LE

[Add Power Switch](https://io.adafruit.com/blog/tip/2016/12/14/feather-power-switch/)

![img](https://i.gyazo.com/a261e3e320189de6ec2b54a52ddaa677.png)

>CP2104 USB to Serial chip-> remove if here

> Need to measure current thru supply

Other case: Attiny85 https://www.youtube.com/watch?v=VlUq2COFjj4

# Battery Level Indicator

## Coulomb Counter

LTC4150

> If you turn off the controller, it will turn MCU off, so will lose previous information?

Alternate: Shift register?

## Voltage Level

## Display:

https://www.youtube.com/watch?v=F_KYZncSiPg



