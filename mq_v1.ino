#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SD.h>
#include <RTClib.h>

File myFile;
File myFile1;
String temp;
String buff;
//int flag=0;
const char* ssid;
const char* password;

const char *mqtt_server;
const char *mqtt_user;
const char *mqtt_pass;
char *port;
int port1;
String id="";
int cs;
int light;
String date;
String info1;
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];

int value = 0;
int count = 0;                                          // count = 0
//char input[12];                                       // character array of size 12 
boolean flag = 0; 

int status = 0;
int out = 0;
RTC_Millis rtc;

void time_date()
{
    DateTime now = rtc.now();
    date= "";
    date.concat(String(now.year(), DEC));
    date.concat("-");
    date.concat(String(now.month(), DEC));
    date.concat("-");
    date.concat(String(now.day(), DEC));
    date.concat("T");
    date.concat(String(now.hour(), DEC));
    date.concat(":");
    date.concat(String(now.minute(), DEC));
    date.concat(":");
    date.concat(String(now.second(), DEC));
}
char* get_reader()
{
  char* reader="Topic/Reader";
  return reader;
}
char* get_buffer()
{
  char* buffer_id="Topic/Buffer";
  return buffer_id;
}
void setup_wifi_server() {
   

    myFile = SD.open("setup.properties");
    
  if (myFile) {
    //Serial.println("wifi.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      
  temp.concat((myFile.readStringUntil('\n')));
      
    }
 
    //Serial.println(temp);
    myFile.close();
  } 
   
  char charBuf[200];
   temp.toCharArray(charBuf, 200); 
  //Serial.println(charBuf);
    char *p = charBuf;
    char *str;
    char *ss="abc";
    char *pass="abc";
    char *ser="abc";
    char *port="abc";
    char *user="abc";
    char *pass_ser="abc";
    char *reader_id="abc";
    char *cs1="abc";
    char *light1="abc";
    while ((str = strtok_r(p, "=", &p)) != NULL) // delimiter is the colon
      
      {
        while ((str = strtok_r(p, ";", &p)) != NULL)
        { 
        if(ss=="abc")
      
      {
        ss=str;
        //Serial.println(ss);
        break;
      }
      else if(pass=="abc"){
        pass=str;
        //Serial.println(pass);
        break;
      }
       else if(ser=="abc")
      
      {
        ser=str;
        //Serial.println(ser);
        break;
      }
       else if(user=="abc"){
        user=str;
        //Serial.println(user);
        break;
      }
      else if(pass_ser=="abc"){
        pass_ser=str;
        //Serial.println(pass_ser);
        break;
      }
      else if(port=="abc"){
        port=str;
        //Serial.println(port);
        break;
      }
     else if(reader_id=="abc"){
        reader_id=str;
        //Serial.println(reader_id);
        break;
      }
      else if(cs1=="abc"){
        cs1=str;
        //Serial.println(cs1);
        break;
      }
      else if(light1=="abc"){
        light1=str;
        //Serial.println(light1);
        break;
      }
      else{
        //Serial.println(str);
}
        } }
        
  ssid=ss;
  password =pass;
  mqtt_server=ser;
  mqtt_user=user;
  mqtt_pass=pass_ser;
  port1 = atoi(port);
  id=reader_id;
  cs = atoi(cs1);
  light = atoi(light1);
 
  //Serial.print("Connecting to ");
 
  //Serial.println(ssid);
 // Serial.println(password);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  //Serial.println("");
  //Serial.println("WiFi connected");
  //Serial.println("IP address: ");
  //Serial.println(WiFi.localIP());
 
    //Serial.print("Connecting to ");
  client.setServer(mqtt_server, port1);
  client.setCallback(callback);
  reconnect();

  
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    //Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
      myFile1 = SD.open("logger.log", FILE_WRITE);
      info1="";
  time_date();
  info1.concat(date);
  info1.concat(" ");
  info1.concat("|");
  info1.concat("INFO");
  info1.concat("|");
  info1.concat("Device is connected to SSID : ");
  info1.concat(ssid);
  info1.concat(" ");
  info1.concat("and ip address : ");
 IPAddress ip = WiFi.localIP();
   String s="";
  for (int i=0; i<4; i++) {
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  }
  info1.concat(s);
   if (myFile1) {
   
    myFile1.println(info1);
   
    myFile1.close();  // close the file:
    
    
   }
      myFile1 = SD.open("logger.log", FILE_WRITE);
   info1="";
  time_date();
  info1.concat(date);
  info1.concat(" ");
  info1.concat("|");
  info1.concat("INFO");
  info1.concat("|");
  info1.concat("device connected to broker:"); 
  info1.concat(mqtt_server);
   if (myFile1) {
   
    myFile1.println(info1);
   
    myFile1.close();  // close the file:
    
    
   }
      
      Serial.println("connected");
      buff="";
        myFile = SD.open("buffer.bck");
    char charBuf1[50];
  if (myFile) {
    //Serial.println("wifi.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      
  buff=myFile.readStringUntil('\n');
  Serial.println(buff);
      buff.toCharArray(charBuf1, 100);
      client.publish(get_buffer(), charBuf1);
      myFile1 = SD.open("logger.log", FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile1) {
   
    myFile1.println(charBuf1);
   
    myFile1.close();  // close the file:
    
    
   }
    }
 
    //Serial.println(buff);
    myFile.close();
  }
  SD.remove("buffer.bck");
  myFile = SD.open("buffer.bck");
  digitalWrite(light, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(light, LOW); 
  delay(100); 
  digitalWrite(light, HIGH);
  delay(500);
  digitalWrite(light, LOW);

      client.subscribe("inTopic");
    } else {
      if(flag==0)
      {
         myFile1 = SD.open("logger.log", FILE_WRITE);
   info1="";
  time_date();
  info1.concat(date);
  info1.concat(" ");
  info1.concat("|");
  info1.concat("FATAL");
  info1.concat("|");
  info1.concat("device disconnected from SSID : ");
  info1.concat(ssid);
   if (myFile1) {
   
    myFile1.println(info1);
   
    myFile1.close();  // close the file:
    
    
   }
     
      }
       String content= "";
      flag=1;
    if(Serial.available())
   {
    count = 0;
      while(Serial.available())          // Read 12 characters and store them in input array
      {
          int reader = Serial.read();
         content.concat(String(reader,DEC));
         count++;
         delay(5);
      }
 
   time_date();
  
StaticJsonBuffer<300> JSONbuffer;
JsonObject& JSONencoder = JSONbuffer.createObject();
JSONencoder["tagid"] = content;
JSONencoder["readerid"] = id;
JSONencoder["readAt"] = date;
char JSONmessageBuffer[120];
JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
Serial.println(JSONmessageBuffer);
//client.publish("Sensor Data", JSONmessageBuffer);

  myFile1 = SD.open("buffer.bck", FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile1) {
   
    myFile1.println(JSONmessageBuffer);
   
    myFile1.close();  // close the file:
    
    
   }
   delay(1000);
   }
     // Serial.print(client.state());
//      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      //delay(5000);
    }
  }
}

void setup() {
  pinMode(light, OUTPUT);
  rtc.begin(DateTime(F(__DATE__), F(__TIME__)));
  SPI.begin(); // open SPI connection
  Serial.begin(9600);   // Initiate a serial communication
   Serial.println();
  delay(100);
 while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  if (!SD.begin(cs)) {
    Serial.println("initialization failed!");
    return;
  }
  myFile1 = SD.open("logger.log", FILE_WRITE);
   info1="";
  time_date();
  info1.concat(date);
  info1.concat(" ");
  info1.concat("|");
  info1.concat("INFO");
  info1.concat("|");
  info1.concat("device booted up. ");
   if (myFile1) {
   
    myFile1.println(info1);
   
    myFile1.close();  // close the file:
    
    
   }
  setup_wifi_server();   
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }

  client.loop();
  delay(100);
  String content= "";
  
 if(Serial.available())
   {
    count = 0;
      while(Serial.available() && count<12)          // Read 12 characters and store them in input array
      {
         int reader = Serial.read();
         content.concat(String(reader,DEC));
         count++;
         delay(5);
      }
 
   time_date();  
StaticJsonBuffer<300> JSONbuffer;
JsonObject& JSONencoder = JSONbuffer.createObject();
JSONencoder["tagid"] = content;
JSONencoder["readerid"] = id;
JSONencoder["readAt"] = date;
char JSONmessageBuffer[120];
JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
Serial.println(JSONmessageBuffer);
client.publish(get_reader(), JSONmessageBuffer);

  myFile1 = SD.open("logger.log", FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile1) {
   
    myFile1.println(JSONmessageBuffer);
   
    myFile1.close();  // close the file:
    
    
   }
   delay(1000);
   }
     
}
