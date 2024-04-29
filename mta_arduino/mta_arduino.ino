#include <ArduinoHttpClient.h>
#include <WiFiNINA.h>

#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
/////// Wifi Settings ///////
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

const char serverAddress[] = "10.18.182.246";  // server address
int port = 8090;
bool handDetected = false;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(9600);

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);  // print the network name (SSID);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
  }

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

}

void loop() {
  if (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);  // print the network name (SSID);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
  }

  get();
  delay(5000);
}

bool get() {
  // assemble the path for the GET message:
  String getPath = "/foo";

  // send the GET request
  // Serial.println("making GET request");
  client.get(getPath);

  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();
  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);

  // int labelStart = response.indexOf("content\":");
  // // find the first { after "content":
  // int contentStart = response.indexOf("{", labelStart);
  // // find the following } and get what's between the braces:
  // int contentEnd = response.indexOf("}", labelStart);
  // String content = response.substring(contentStart + 1, contentEnd);
  // Serial.println(content);

  // // now get the value after the colon, and convert to an int:
  // int valueStart = content.indexOf(":");
  // String valueString = content.substring(valueStart + 1);
  // int number = valueString.toInt();
  // bool result = number == 1 ? true : false;
  return true;
}
