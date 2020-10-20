# ESP8266-Dallas-Solar-syslog
Running the ESP8266 on the solar panel with a single  Li-Ion 14500 battery and reporting data to syslog server. 

The idea came when I bought very simple solar powered LED light with PIR sensor. 
I used the solar charged battery to connect the ESP with a Dallas temperature sesnor. The data goes over WiFi to my syslog server 
(UDP port 1514 - which is not a standard UDP Syslog server)

## How to - hardware:

   You need to connect the D0 pin with RST to wake up the ESP8266. BUT! 
   You need to disconnect the D0 -- RST pin to be able to upload the program to ESP   ! 
   So after the upload you need to connect the D0 to RST again. 
 
 ### Temperature sensor:
  
Standard Dallas DS18B20 sensor. The data pin (middle pin) of Dallas Temperature Sensor is connected to D2 (aka GPIO4)
<DallasTemperature.h> library is used. 

### Syslog connectivity:
  
  
   The program uses https://github.com/arcao/Syslog library, (though the
    syslog protocol is rather easy and you only need to prepare single UDP datagram) 

