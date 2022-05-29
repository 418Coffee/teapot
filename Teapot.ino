#include "env.h"
#include <WiFiWebServer.h>
#include "uptime.h"
#if ENV_DEBUG
#include <LibPrintf.h>
#else
#define printf(format, ...)
#endif

const char ssid[] = ENV_SSID;
const char pass[] = ENV_PASS;
const int port = ENV_PORT;
int status = WL_IDLE_STATUS;
unsigned long reqCount = 0;

WiFiWebServer server(port);

void setup()
{
#if ENV_DEBUG
  Serial.begin(9600);
  while (!Serial); // wait for serial communication
#endif

  WiFi.setHostname("Teapot");
  printf("Attempting to connect to network named: %s\n", ssid);
  while (status != WL_CONNECTED) {
    status = WiFi.begin(ssid, pass);
    if (status != WL_CONNECTED) {
      printf("Failed to connect: %d - retrying...", WiFi.reasonCode());
    }
    // wait 10 seconds for connection
    delay(10000);
  }

  server.on(F("/"), handleRoot);
  server.onNotFound(handleNotFound);
  const char *headerKeys[] = {"Cookie"};
  size_t headerKeysSize = sizeof(headerKeys) / sizeof(char*);
  server.collectHeaders(headerKeys, headerKeysSize);
  server.begin();
#if ENV_DEBUG
  printWiFiStatus();
#endif
}

void loop() {
  server.handleClient();
  updateTime();
}

void handleRoot() {
#define BUFFER_SIZE     512
  char temp[BUFFER_SIZE];
  bool visited = false;
  uptime::calculateUptime();
  if (server.hasHeader(F("Cookie"))) {
    String cookie = server.header(F("Cookie"));
    if (cookie.indexOf(F("visited=1")) != -1) {
      visited = true;
    }
  }
  if (!visited) {
    reqCount++;
    server.sendHeader(F("Set-Cookie"), F("visited=1; expires=Tue, 19 Jan 2038 03:14:07 GMT; comment=Y2K38"));
  }
  const char* pluralYears = uptime::getYears() == 1 ? "year" : "years";
  const char* pluralDays = uptime::getDays() == 1 ? "day" : "days";
  const char* pluralHours = uptime::getHours() == 1 ? "hour" : "hours";
  const char* pluralMinutes = uptime::getMinutes() == 1 ? "minute" : "minutes";
  const char* pluralSeconds = uptime::getSeconds() == 1 ? "second" : "seconds";
  snprintf(temp, BUFFER_SIZE - 1,
  "<html><head>\
  <meta http-equiv='refresh' content='10'/>\
  <title>Teapot</title>\
  <style>\
  body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #105d61; }\
  </style>\
  </head>\
  <body>\
  <h1>I'm a teapot</h1>\
  <h3>View count: %d</h3>\
  <p>Uptime: %d %s %d %s %d %s %d %s %d %s</p>\
  <img src='https://downtremendous.com/teapot_small.jpg' alt='teapot'>\
  </body></html>",
           reqCount,
           uptime::getYears(), pluralYears,
           uptime::getDays(), pluralDays,
           uptime::getHours(), pluralHours,
           uptime::getMinutes(), pluralMinutes,
           uptime::getSeconds(), pluralSeconds);
  server.send(418, F("text/html"), temp);
}

void handleNotFound() {
  String message = F("Not Found\n\n");
  message += F("URI: ");
  message += server.uri();
  message += F(" was not found\n\n");
  message += F("Method: ");
  message += (server.method() == HTTP_GET) ? F("GET\n\n") : F("POST\n\n");
  message += "I'm still teapot...";
  server.send(418, F("text/plain"), message);
}

const char *rssi2str(long dBm) {
  if (dBm <= -90) {
    return "Unusable";
  } else if (dBm <= -80) {
    return "Not Good";
  }  else if (dBm <= -70) {
    return "Okay";
  }  else if (dBm <= -67) {
    return "Very Good";
  }
  return "Amazing";
}

void printWiFiStatus() {
  IPAddress ip = WiFi.localIP();
  long rssi = WiFi.RSSI();
  printf("Connected to: %s\n", WiFi.SSID());
  printf("Local IP Address: %d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
  printf("Signal strength (RSSI): %d dBm - %s\n", rssi, rssi2str(rssi));
  printf("Server available on: http://%d.%d.%d.%d:%d\n", ip[0], ip[1], ip[2], ip[3], port);
}

void updateTime()
{
  static unsigned long updateTimeTimeout = 0;

#define STATUS_CHECK_INTERVAL     86400000L // 86400 * 1000 ms = 86400 s = 24 h = 1 day

  if ((millis() > updateTimeTimeout) || (updateTimeTimeout == 0))
  {
    updateTimeTimeout = millis() + STATUS_CHECK_INTERVAL;
    uptime::calculateUptime();
  }
}
