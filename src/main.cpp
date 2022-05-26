#include <Arduino.h>
#include <String>
/*****************OLED*******************/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128    // Ширина OLED дисплея 
#define SCREEN_HEIGHT 64    // Высота OLED дисплея
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     LED_BUILTIN
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/*****************MQTT*******************/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
const char* ssid = "DIR-615";    //"Nova"; 
const char* password = "29660438"; //"vbnmvbnm"; //
const char* mqtt_server = "217.71.129.139";
int mqttPort = 4019;
const char *mqtt_user = "_____________"; // Логин от сервер
const char *mqtt_pass = "_____________"; // Пароль от сервера
const char* topicToSub = "iot/morozov/toNode";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int  value = 0;

bool find = false;
String labelToFind = "";

//настройки чипосчета
int  counterid = 1;
byte mode = 1;

extern bool MYprotocol(void);


/*****************RFID*******************/
#include <MFRC522.h>
#define RST_PIN         0          
#define SS_PIN          2          
MFRC522 mfrc522(SS_PIN, RST_PIN);  

/************serial + display************/
void myprint(String text) ;
void myprintln(String text);

/**************BUTTON**************/
#define BUTTON 15 //D8(gpio15)
bool buttonState = false;
int lastEnter=0, lastConnect = 0, lastDisp = 0;



void setup_wifi() {
  display.setCursor(0,0);
  delay(10);
  //Подключаемся к wi-fi
  Serial.println();
  myprint("Connecting to ");
  myprintln((String)ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    MYprotocol(); //на случай, если нужно задать сеть вручную
    myprint("=>");
    display.display();
  }

  randomSeed(micros());

  myprintln("");
  myprintln("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  myprintln("ID = 1");
  myprintln("I`m ready");
  display.display();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  //Serial.println(payload[9]);
  
  if ( payload[9] == counterid + 48 ) 
  {
    find = true; 
    //String to Find
    labelToFind = (char)payload[21]; labelToFind += (char)payload[22];
    labelToFind += (char)payload[23]; labelToFind += (char)payload[24];
    labelToFind += (char)payload[25]; labelToFind += (char)payload[26];
    labelToFind += (char)payload[27]; labelToFind += (char)payload[28];
    display.clearDisplay();
    display.setCursor(0,0);
    myprintln("Command to find:");
    myprintln(labelToFind);
    display.display();
  }
  // Отмена поиска, только если он был запущен
  if ( payload[9] == 48 && find) 
  {
    find = false; 
    myprintln("Find stop.");
    display.display();
    delay(1000);
  }

}

void reconnect() {
  // Loop until we're reconnected
  
  while (!client.connected()) {
    String clientId = "ESP8266Client-morozov";
    // Попытка соединения
    if (client.connect(clientId.c_str())) {
      //myprintln("connected");
      //client.publish("iot/morozov/def1", "\"id\":0, \"count\":0, \"label\":\"0\"");
      client.subscribe(topicToSub);

      display.display();
    } else {
      //Сообщаем только о повторных попытка подключиться
      display.setCursor(0,0);
      display.clearDisplay();
      myprint("Attempting MQTT connection...");
      myprint("failed, rc=");
      myprint((String)client.state()); 
      myprintln(" try again in 1 seconds");
      client.subscribe(topicToSub);
      display.display();
      delay(1000);
    }
  }
}

void myprint(String text) {
  Serial.print(text);
  display.print(text);
}
void myprintln(String text) {
  Serial.println(text);
  display.println(text);
}


void setup() {
  Serial.begin(9600);
  pinMode(BUTTON, INPUT); // push button 
  pinMode(16, OUTPUT); //светодиод
  /*****************OLED*******************/
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  //(0x3C for 32-pixel-tall displays, 0x3D for all others)
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  // Clear the buffer
  display.clearDisplay();
  display.setTextSize(1);             // 
  display.setTextColor(WHITE);        // 
  display.setCursor(0,0);
  display.display();
 /*****************MQTT*******************/
  setup_wifi();
  client.setServer(mqtt_server, mqttPort);
  client.setCallback(callback);
  client.subscribe(topicToSub);
  //client.loop();
/*****************RFID*******************/
  SPI.begin();      
  mfrc522.PCD_Init();

}

int light = 0, count = 0, fill_count = 0, threshold = 20,timecount = 0;
float analogValue;
#define adc0      A0

void loop() {

long now = millis();
//**************SMD COUNT******************
analogValue = analogRead(adc0);
//Serial.println(analogValue);
if (analogValue < threshold)
  fill_count ++;
//for debug
//Serial.println(analogValue);
timecount++;

if (analogValue > threshold && fill_count > 1)
{
  fill_count = 0;
  count ++;
}

MYprotocol();
 
  //обновление дисплея
  if (now - lastDisp > 300 )
  {
    //проверка соединения
    if (client.connected()) 
      client.loop();
    else
      {    
        reconnect();  
        client.loop();
      }
    // если не в режиме поиска, отображаем число насчитанных компонент
    if (!find)
    {
      lastDisp  = now;
      //ВЫВОД ЧИСЛА КОМПОНЕНТ
      display.setTextSize(3);
      display.clearDisplay(); 
      display.setCursor(0,0);
      display.println(count);    
      display.display();
      display.setTextSize(1);
    }
  }
 
  // КНОПКА СБРОСА
  if (digitalRead(BUTTON))
  {
    if (now - lastEnter > 200)
    {
      buttonState = true;
      count = 0;
      lastEnter = now;
      find = false; 
      //ВЫВОД ЧИСЛА КОМПОНЕНТ
      display.clearDisplay(); 
      display.setTextSize(3);
      display.setCursor(0,0);
      display.print(count);    
      display.display();
      display.setTextSize(1);
    }
  }
  
  if (now - lastMsg > 1000 && mode == 1) {
    lastMsg = now;    
    if (mfrc522.PICC_IsNewCardPresent()) 
    {
      display.clearDisplay();
      if ( mfrc522.PICC_ReadCardSerial()) {
        for (byte i = 0; i < mfrc522.uid.size; i++) 
        {
          //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
          //Serial.println(mfrc522.uid.uidByte[i], HEX);
        }
        
        display.setCursor(0,0);

        if (!find)
        {
          //проверка соединения
          if (!client.connected()) {    reconnect(); }
          
          //form JSON message
          String message = "{\"id\":1, \"count\":";
          message += count;
          message += ", \"label\":\"";
          message += String(mfrc522.uid.uidByte[0], HEX);
          message += String(mfrc522.uid.uidByte[1], HEX);
          message += String(mfrc522.uid.uidByte[2], HEX);
          message += String(mfrc522.uid.uidByte[3], HEX);
          message += "\"}";
          Serial.print("Publish message: ");
          Serial.println(message);
          display.println(message);  
          client.publish("iot/morozov/fromNode", message.c_str()); 
          myprintln("Sent to the site");
          display.display();
          delay(2000);
        }
        String INmessage = String(mfrc522.uid.uidByte[0], HEX);
        INmessage += String(mfrc522.uid.uidByte[1], HEX);
        INmessage += String(mfrc522.uid.uidByte[2], HEX);
        INmessage += String(mfrc522.uid.uidByte[3], HEX);
        if (find && (INmessage == labelToFind)) 
        {
          myprintln(INmessage);
          myprintln("FIND!");
          display.display();
          delay(2000);
          find = false;
        }
        if (find && (INmessage != labelToFind))
        {
          myprintln(INmessage);
          myprintln(labelToFind + " not FIND!");          
        }
        display.display();
        count = 0;
      }
    }
  }   


}//close main