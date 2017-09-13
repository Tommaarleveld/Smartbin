#include <SPI.h>
#include <WiFi.h>
#include "Hx711.h"
// Hx711.DOUT - pin #A2
// Hx711.SCK - pin #A3

// EDIT: Change the 'ssid' and 'password' to match your network
char ssid[] = "TimLand WiFi";  // wireless network name
char password[] = "appeltaart"; // wireless password
int status = WL_IDLE_STATUS;
WiFiClient client;
  
// EDIT: 'Server' address to match your domain
char server[] = "www.timfalken.com"; // This could also be 192.168.1.18/~me if you are running a server on your computer on a local network.

// This is the data that will be passed into your POST and matches your mysql column
String yourdata;

int Senval=0;

int Senpin=A0;

void setup() {
  Serial.begin(9600);
  
  connectWifi();

  // You're connected now, so print out the status
  printWifiStatus();

  
  
}

void loop() {
  Serial.print(scale.getGram() * (-4.6), 1);
  Serial.println(" g");
  delay(200);
  
  Senval=analogRead(Senpin);
  Serial.println(Senval);
  if (Senval >= 100) {
    postData();
  }
}

void connectWifi() {
  // Attempt to connect to wifi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, password);
    // Wait 10 seconds for connection
    delay(5000);
  }
}

void printWifiStatus() {
  // Print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // Print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // Print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

// This method makes a HTTP connection to the server and POSTs data
void postData() {
  // Combine yourdatacolumn header (yourdata=) with the data recorded from your arduino
  // (yourarduinodata) and package them into the String yourdata which is what will be
  // sent in your POST request
  yourdata = "binId=8&newWeight=3&token=45f17b19ad5527e8bd6a0b749bf412ac";

  // If there's a successful connection, send the HTTP POST request
  if (client.connect(server, 80)) {
    Serial.println("connecting...");

    // EDIT: The POST 'URL' to the location of your insert_mysql.php on your web-host
    client.println("POST /hr/internetfornature/updateBinWeight.php HTTP/1.1");

    // EDIT: 'Host' to match your domain
    client.println("Host: www.timfalken.com");
    client.println("User-Agent: Arduino/1.0");
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded;");
    client.print("Content-Length: ");
    client.println(yourdata.length());
    client.println();
    client.println(yourdata); 
  } 
  else {
    // If you couldn't make a connection:
    Serial.println("Connection failed");
    Serial.println("Disconnecting.");
    client.stop();
  }
}
//Einde chatgesprek

