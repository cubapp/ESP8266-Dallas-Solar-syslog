/**
   

   Deep Sleep ESP example with Dallas temperature sensor and data feed to syslog server 
   You need to connect the D0 pin with RST to wake up the ESP8266
   You need to disconnect the D0 -- RST pin to be able to upload the program to ESP   ! 
   Data pin of Dallas Temperature Sensor is connected to D2 (aka GPIO4)
   The program uses https://github.com/arcao/Syslog library though the
    syslog protocol is rather easy. 
*/

#include <ESP8266WiFi.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <WiFiUdp.h>
#include <Syslog.h>

// Syslog server connection info
// IP address and UDP port of the syslog server
#define SYSLOG_SERVER "11.22.33.44" 
#define SYSLOG_PORT 1514

// This device info
// name it so you can grep if from syslog feed easily
// I used both hostname and app_name for simple search 
// grep ESP8266-SOLAR /var/log/syslog
// Oct 20 19:11:04 ESP8266-SOLAR DALLAS-001 ESP temperature: 5

#define DEVICE_HOSTNAME "ESP8266-SOLAR"
#define APP_NAME "DALLAS-001"

// GPIO 4 == D2 on phys. board 
// for Dallas temperature sensor
#define ONE_WIRE_BUS 4   

// WiFi credentials.
const char* WIFI_SSID = "secretSSID";
const char* WIFI_PASS = "EvenMoreSecretPassword2YourWiFi";

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire); 
WiFiClientSecure wifiClient;
double temperature; 

void connect() {
  // Connect to Wifi.
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // WiFi fix: https://github.com/esp8266/Arduino/issues/2186
  WiFi.persistent(false);
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  unsigned long wifiConnectStart = millis();

  while (WiFi.status() != WL_CONNECTED) {
    // Check to see if
    if (WiFi.status() == WL_CONNECT_FAILED) {
      Serial.println("Failed to connect to WiFi. Please verify credentials: ");
      delay(10000);
    }

    delay(500);
    Serial.println("...");
    // Only try for 5 seconds.
    if (millis() - wifiConnectStart > 15000) {
      Serial.println("Failed to connect to WiFi");
      return;
    }
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Connected!");
  Serial.println("This device is now ready for use!");
 
}



void setup() {
  Serial.begin(9600);
  Serial.setTimeout(2000);
  sensors.begin();

  // Wait for serial to initialize.
  while (!Serial) { }
  connect();

  // A UDP instance to let us send and receive packets over UDP
  WiFiUDP udpClient;

  // Create a new syslog instance with LOG_KERN facility
  Syslog syslog(udpClient, SYSLOG_SERVER, SYSLOG_PORT, DEVICE_HOSTNAME, APP_NAME, LOG_KERN);

  sensors.requestTemperatures();  
  // Send the command to get temperatures  
  temperature = sensors.getTempCByIndex(0);
  Serial.println("Temperature is: ");
  Serial.println(temperature); 
  
  syslog.logf(LOG_INFO, "ESP temperature: %d", int(temperature));
  delay(1000); 

  Serial.println("Going into deep sleep for 60 seconds");
  ESP.deepSleep(60e6); // 60e6 is 20 microseconds
}

void loop() {
}
