#define LED_PIN D4
#define LDR_PIN A0
#define THRESHOLD 49
#define PERIOD 15

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

//WIFI
#define WIFI_SSID "JioFiber-11-2.4G"
#define WIFI_PASSWORD "01012020"

bool previous_state;
bool current_state;
int i = 0;
int flag = 0;
int flag_main = 0;
char loc[12];
char location[14]; 
void setup()
{
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LDR_PIN, INPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);        //set up WIFI
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected to wifi: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  if (WiFi.status() == WL_CONNECTED) {
//    Serial.println("checking");
    WiFiClient client;
    HTTPClient http;
    http.setTimeout(2000);
    if (flag_main == 0) {
      current_state = get_ldr();
      if (!current_state && previous_state)
      {
        print_byte(get_byte());
      }
      previous_state = current_state;
    }
    if (flag_main == 1) {
      Serial.println(loc);
      strcpy(location,loc);
      location[14] = '\0';
      send_backend();
      delay(1000000); //inf
    }
  }
}

bool get_ldr()
{
  int voltage = analogRead(LDR_PIN);
  //  Serial.println(voltage);
  return voltage > THRESHOLD ? true : false;
}

char get_byte()
{
  char ret = 0;
  delay(PERIOD * 1.5);
  for (int i = 0; i < 8; i++)
  {
    ret = ret | get_ldr() << i;
    delay(PERIOD);
  }
  return ret;
}

void print_byte(char my_byte)
{
  char buff[2];
  sprintf(buff, "%c", my_byte);
    Serial.print(buff);
  if ( my_byte == 'S')
    flag = 1;
  if (flag == 1) {
    loc[i] = my_byte;
    i++;
    if (i == 12) {
      flag = 0;
      flag_main = 1;
    }
  }
    Serial.println(loc);
}

void send_backend()
{
  WiFiClient client;
  HTTPClient http;
  http.setTimeout(5000);
  http.begin(client, "http://still-dawn-71437.herokuapp.com/"); //SERVER
//  http.setTimeout(5000);
  http.addHeader("Content-Type", "application/json");
  String x = String(location);
  Serial.print(x);
  /*start connection and send HTTP header and body*/
  int httpCode = http.POST("{\"location\" : \" " + x+ "\"}");
  Serial.printf("[HTTP] POST... code: %d\n", httpCode);

  /*when http is connected*/
  if (httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    String payload = http.getString();   //Get the request response payload
    Serial.println(payload);

    // when file found at server
    Serial.print(HTTP_CODE_OK);
    if (httpCode == HTTP_CODE_OK) {
      const String& payload = http.getString();
      Serial.println("received payload:\n<<");
      Serial.println(payload);
      Serial.println(">>");
    }
  }
  /*when http is not connected (negative http on error)*/
  else {
    Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
}
