#include <ArduinoHttpClient.h>
#include <WiFiNINA.h>

#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
/////// Wifi Settings ///////
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

const char serverAddress[] = "10.18.247.224";  // server address
int port = 8090;
bool handDetected = false;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

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

  Serial1.println(get());
  delay(3000);
}

String get() {
  String getPath = "/foo";

  //send the GET request
  Serial.println("making GET request");
  client.get(getPath);

  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();
  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);

  int ANIndex = response.indexOf("an");
  int ASIndex = response.indexOf("as");
  int CNIndex = response.indexOf("cn");
  int CSIndex = response.indexOf("cs");
  int FNIndex = response.indexOf("fn");
  int FSIndex = response.indexOf("fs");
  int RSIndex = response.indexOf("rs");
  int RNIndex = response.indexOf("rn");

  String parsedResponse = response.substring(ANIndex + 5, ANIndex + 7) + ",";
  parsedResponse += response.substring(ASIndex + 5, ASIndex + 7) + ",";
  parsedResponse += response.substring(CNIndex + 5, CNIndex + 7) + ",";
  parsedResponse += response.substring(CSIndex + 5, CSIndex + 7) + ",";
  parsedResponse += response.substring(FNIndex + 5, FNIndex + 7) + ",";
  parsedResponse += response.substring(FSIndex + 5, FSIndex + 7) + ",";
  parsedResponse += response.substring(RSIndex + 5, RSIndex + 7) + ",";
  parsedResponse += response.substring(RNIndex + 5, RNIndex + 7);
  
  Serial.println(parsedResponse);
  return parsedResponse;
}
