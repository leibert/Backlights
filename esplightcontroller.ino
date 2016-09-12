#include <ESP8266WiFi.h>

const char* ssid     = "66ADAMS";
const char* password = "password";
//IPAddress ip(192, 168, 0, 41);
int greenBANK = 5;
int blueBANK = 4;
int redBANK = 2;


//int tmr;
int mSec = 0;
int seconds = 0;
int minutes = 0;

bool watchingCoffee = false;

WiFiServer server(80);





void initLamp() {

  digitalWrite(redBANK, LOW);
  digitalWrite(greenBANK, LOW);
  digitalWrite(blueBANK, LOW);
  delay(3000);
  digitalWrite(redBANK, HIGH);
  digitalWrite(greenBANK, HIGH);
  digitalWrite(blueBANK, HIGH);
  delay(3000);
  analogWrite(blueBANK, 2000);
}






void setup() {
  Serial.begin(115200);

  delay(10);
//  analogWriteResolution(8); 
  //  pinMode(ledPin, OUTPUT);
  //  digitalWrite(ledPin, LOW);
  pinMode(redBANK,OUTPUT);
  pinMode(greenBANK,OUTPUT);
  pinMode(blueBANK,OUTPUT);
  digitalWrite(redBANK, HIGH);
  digitalWrite(greenBANK, HIGH);
  digitalWrite(blueBANK, HIGH);
  initLamp();

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

}


void loop() {

//  delay(100);
//  mSec = mSec + 100;
//  Serial.println(mSec);



  //  Serial.println("current time:");
  //  Serial.println("mSec:");
  //  Serial.print(mSec);
  //  Serial.println("seconds:");
  //  Serial.print(seconds);
  //  Serial.println("minutes:");
  //  Serial.print(minutes);

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
  Serial.println(request.indexOf("LED=1"));

  if (request.indexOf("/LED=1") != -1) {
        Serial.println("color1 trigger");
        digitalWrite(redBANK, !digitalRead(redBANK));
        
  }
  else if (request.indexOf("/LED=2") != -1) {
        Serial.println("color2 trigger");
        digitalWrite(greenBANK, !digitalRead(greenBANK));
        
  }
  else if (request.indexOf("/LED=3") != -1) {
        Serial.println("color3 trigger");
        digitalWrite(blueBANK, !digitalRead(blueBANK));
        
  }
  else if (request.indexOf("/INIT") != -1) {
        initLamp();
        
  }
  else if (request.indexOf("/PWMtest1") != -1) {
        while(1){
          digitalWrite(blueBANK, HIGH);
          delayMicroseconds(100); // Approximately 10% duty cycle @ 1KHz
          digitalWrite(blueBANK, LOW);
          delayMicroseconds(1000 - 100);
        }
  }
    
  else if (request.indexOf("/PWMtest2") != -1) {
        for(int i=1; i<1000;i++){
          digitalWrite(blueBANK, LOW);
          delayMicroseconds(100); // Approximately 90% duty cycle @ 1KHz
          digitalWrite(blueBANK, HIGH);
          delayMicroseconds(1000 - 100);
        }
        Serial.println("done");
  }
    

  
  else if (request.indexOf("/PWM3") != -1) {
        Serial.println("pwm3 trigger");
        for(int i=1; i<1000;i++){
          analogWrite(blueBANK, i);
          Serial.println(i);
          delay(100);
        }
        
        delay(1000);
        analogWrite(redBANK, 100);
//        analogWrite(blueBANK, 100);
        
//        for (int i=0;i<255;i++){
//          analogWrite(blueBANK, (255-i));
//          Serial.println(i);
//          delay(100);
//        }
          
        digitalWrite(blueBANK, !digitalRead(blueBANK));
        
  }


  // Match the request

  //  int value = LOW;
  //  if (request.indexOf("/LED=ON") != -1) {
  //    digitalWrite(ledPin, HIGH);
  //    value = HIGH;
  //  }
  //if (request.indexOf("/LED=OFF") != -1) {
  //digitalWrite(ledPin, LOW);
  //value = LOW;
  //}

  // Set ledPin according to the request
  //digitalWrite(ledPin, value);

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); // do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

//  if (watchingCoffee == true) {
//
//
//    if (minutes < goodCoffee) {
//      client.print("Coffee Fresh");
//
//    }
//    else if (minutes < mehCoffee) {
//      client.print("Coffee Meh");
//
//    }
//    else {
//      client.print("Coffee Bad");
//    }
//  }
//  else {
//    client.print("NOT WATCHING COFFEE");
//  }


  client.println("<br><br>");
  client.println("Click <a href=\"/LED=1\">here</a> turn the LED on pin 2 ON<br>");
  client.println("Click <a href=\"/LED=2\">here</a> turn the LED on pin 2 OFF<br>");
  client.println("Click <a href=\"/LED=3\">here</a> turn the LED on pin 3 OFF<br>");
  client.println("Click <a href=\"/PWM3\">here</a> PWM testy<br>");
  client.println("</html>");

  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");

}

