#include <ESP8266WiFi.h>
#include <RTClib.h>

const char* ssid = "2WIRE347_EXT";
const char* password = "2704414570";

WiFiServer server(80);
String header;

String output14State = "off";
String output12State = "off";

String automatedState = "off";
bool sprinkiled_today = false;

const int output14 = 14;
const int output12 = 12;

unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;

char daysOfTheWeek[7][12] = {"Sunday",   "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
RTC_DS1307 rtc;

bool first12 = false;

String returnTime() {
  DateTime now = rtc.now();
  int hrs = now.hour();
  String result;

  if (hrs == 0 && hrs != 12) {
    hrs = 12;
  } else if (hrs == 12 && hrs != 0) {
    hrs = 12;
  } else if (hrs < 12 && hrs != 0) {
    hrs = hrs;
  } else if (hrs > 12 && hrs != 0) {
    hrs = hrs - 12;
  }

  result += hrs;
  result += ':';
  result += now.minute(), DEC;
  result += ':';
  result += now.second(), DEC;
  result += " ";

  return result;
  result = "";
}

String returnHours(){
 DateTime now = rtc.now();
  int hrs = now.hour();
  String result;

  return result;
}

String returnDay(){
  DateTime now = rtc.now();
  int hrs = now.hour();
  String result;

  result += now.day(), DEC;

  return result;
}

void setup() {
  Serial.begin(115200);

  pinMode(output14, OUTPUT);
  pinMode(output12, OUTPUT);

  digitalWrite(output14, LOW);
  digitalWrite(output12, LOW);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running, Time manually set...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  if (rtc.isrunning() && rtc.begin()) {
    Serial.println("Welcome to Automated sprinklers! It is currently " + returnTime());
  }
}

void loop(){
  WiFiClient client = server.available();   

  if (client) {                             
    Serial.println("New Client.");        
    String currentLine = "";                
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { 
      currentTime = millis();         
      if (client.available()) {           
        char c = client.read();             
        Serial.write(c);                   
        header += c;
        if (c == '\n') { 
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
           
            // turns the GPIOs on and off
            if (header.indexOf("GET /14/on") >= 0) {
              Serial.println("GPIO 14 on");
              output14State = "on";
              output12State = "off";
              sprinkiled_today = true;

              digitalWrite(output14, HIGH);
              delay(200);
              digitalWrite(output14, LOW);
            } else if (header.indexOf("GET /12/on") >= 0) {
              Serial.println("GPIO 12 on");
              output12State = "on";
              output14State = "off";

              digitalWrite(output12, HIGH);
              delay(500);
              digitalWrite(output12, LOW);
            } else if (header.indexOf("GET /a/on") >= 0) {
              automatedState = "on";
            } else if (header.indexOf("GET /a/off") >= 0) {
              automatedState = "off";
            }
            
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> <Title>Remote Sprinkler</Title>");
            client.println("<link rel=\"icon\" href=\"data:,\">");

            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: green; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: red;}</style></head>");
            
            client.println("<body><h1>Remote Sprinkler</h1>");
            client.println("<hr><hr>");
            client.println();

            client.println("<p>Welcome, the time this page is being accessed is roughly <b><u><i>" + returnTime() + "</b></u></i></p>");
            client.println();

            client.println("<p><b><u>Automated Setting</u></b> (Goes off at 7 PM)</p> <p>");
            client.println();

            if(sprinkiled_today == true) 
              client.println("<p>Sprinkled Today: <b style = \"color: green\">TRUE</b></p>");
            else 
              client.println("<p>Sprinkled Today: <b style = \"color: red\">FALSE</b></p>");
            
            
            client.println();

            if (automatedState == "on") {
              client.println("<p><a href=\"/a/off\"><button class=\"button\">ON</button></a></p>");
            }  

            if (automatedState == "off") {
              client.println("<p><a href=\"/a/on\"><button class=\"button button2\">OFF</button></a></p>");
            } 

            client.println("<p><b><u>Current Sprinkler State:</u></b></p>");
            client.println();

            if (output14State == "on") {
              client.println("<p><a href=\"/12/on\"><button class=\"button\">ON</button></a></p>");
            }  

            if (output12State == "on" || output14State == "off") {
              client.println("<p><a href=\"/14/on\"><button class=\"button button2\">OFF</button></a></p>");
            } 

            client.println();
            client.println("<h3 style = \"color: red\"><i><b>Comyar Dehlavi</i></b></h3>");

            client.println("</body></html>");
            client.println();

            break;
          } else { 
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;    
        }
      }
      if(automatedState == "on"){
        if(returnHours() == "7" && first12 == true){
          Serial.println("GPIO 14 on");
          output14State = "on";
          output12State = "off";
          sprinkiled_today = true;
          first12 ==  false;

          digitalWrite(output14, HIGH);
          delay(200);
          digitalWrite(output14, LOW);

          delay(600000);

          digitalWrite(output12, HIGH);
          delay(500);
          digitalWrite(output12, LOW);
        }else{
          first12 == true;
        }
      }
    }

    header = "";

    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }

  if(automatedState == "on"){
    if(returnHours() == "7" && first12 == true){
        Serial.println("GPIO 14 on");
        output14State = "on";
        output12State = "off";
        sprinkiled_today = true;
        first12 ==  false;

        digitalWrite(output14, HIGH);
        delay(200);
        digitalWrite(output14, LOW);

        delay(600000);

        digitalWrite(output12, HIGH);
        delay(500);
        digitalWrite(output12, LOW);
    }else{
      first12 == true;
    }
  }
}