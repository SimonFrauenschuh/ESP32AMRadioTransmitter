#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <ArduinoJson.h>

const char* ssid = "Apple WLAN";
const char* password = "Schulschluss4.";

WebServer server(80);

void handleGet() {
  server.send(200, "text/plain", "hello from esp32!");
}

void handlePost() {
  String body = server.arg("plain");

  if (sizeof(body) > 100000) {
    Serial.println("HIER2");
    Serial.println(body);
    server.send(413, "application/json", "Body Payload over 100000 elements. Short the music file");
  } else {
    Serial.println("HIER");
    Serial.println(body);
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", "{}");
  }
}

void setup(void) {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleGet);
  server.on("/data", HTTP_POST, handlePost);

  server.enableCORS();
  server.enableCrossOrigin();
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
}
