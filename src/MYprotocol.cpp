#include <Arduino.h>

extern const char* ssid; 
extern const char* password; 
extern bool find;
extern bool mode;
extern String labelToFind;
extern int  counterid, count ;
extern void setup_wifi();

//ПРОТОКОЛ
bool MYprotocol(void);
#define SOH "%"
#define EOT ";"
#define TER " "

bool  MYprotocol(void) {

  if (Serial.available() > 0) {         //если доступно
    String buf = Serial.readString();     // читаем в строку

    //если соответствует протоколу 
    if (buf.startsWith(SOH) && buf.endsWith(EOT)) {
    
      String command  = buf.substring( 1, buf.indexOf(TER) );
      String value =    buf.substring( buf.indexOf(TER) + 1, buf.length() - 1 );
      //Serial.println(command);
      //Serial.println(value);
      //СМЕНА ИМЕНИ ВАЙ-ФАЙ 
      if (command == "login") 
      {
        ssid = value.c_str();  
        Serial.print("Wi-Fi login changed to "); Serial.println(value);      
        setup_wifi();
      }
      //СМЕНА пароля ВАЙ-ФАЙ 
      else if (command == "pass") 
        {
          password = value.c_str(); 
          Serial.print("Wi-Fi password changed to "); Serial.println(value);  
          setup_wifi();
        }
      //КОНТРОЛЬ ПОИСКА
      else if (command == "find") 
        {
          if ( value != "0" )
          {
            find = true; 
            labelToFind = value.c_str(); 
            Serial.print("I`m Find "); Serial.println(value);  
          }
          else 
            {
              find = false;
              Serial.println("Поиск отменен");
            }
        }
      //СМЕНА id устройства, от 1 до 9 
      else if (command == "counterid") 
        {
          counterid = value.toInt();
          Serial.print("Counter Id = "); Serial.println(counterid); 
        }
      //mode 1 = only count 
      else if (command == "mode") 
        {
          mode = (mode + 1) % 2;
          Serial.println("Mode switched"); 
        }
      //change count 
      else if (command == "count") 
        {
          count = value.toInt();
          Serial.print("Change count to "); Serial.println(value); 
        }
      else Serial.println("Нет такой команды");
      
    }
  }
  return true;
} //end of protocol