#include <LiquidCrystal.h>
#include <Keypad.h>
#include <SoftwareSerial.h>
#define RX 10
#define TX 11
boolean Home=true;
boolean menu=false;
boolean changinglimit=false;
boolean settingpriority=false;
boolean testing=false;
boolean exitmenu=false;
boolean powersaving=false;
boolean recharge=false;
float ou;
float tu;
float tr3;
float Using=1.00;
float Used=0;
String menuItem[]={"Change Limit","Set Priority","Testing","Exit"};
int Limit=1000;
int Priority=123;
int x=0;
String AP = "chris";       // CHANGE ME
String PASS = "1234567890"; // CHANGE ME
String API = "YOUR_API_KEY";   // CHANGE ME
String HOST = "api.thingspeak.com";
String PORT = "80";
String field = "field1";
String data;
String temp,hum;
String server = "192.168.1.234"; // www.example.com
String uri = "/project/datapost.php";// our example is /esppost.php
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor = 1;
SoftwareSerial esp8266(RX,TX); 

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {22, 23, 24, 25}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {26, 27, 28, 29}; //connect to the column pinouts of the keypad
LiquidCrystal lcd(12, 8, 5, 4, 3, 2);
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
  Serial.begin(115200);
  esp8266.begin(115200);
  pinMode(50,OUTPUT);
  pinMode(52,OUTPUT);
  pinMode(53,OUTPUT);
  digitalWrite(50,0);
  digitalWrite(52,0);
  digitalWrite(53,0);
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  //lcd.print("UNCHAINED(__)");
  //delay(2500);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("loading...");
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  lcd.clear();
  lcd.print("connecting...");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
  lcd.clear();
  
}
 String tv; 
 float Usage=1.00;
void loop(){
  char key = keypad.getKey();
  Usage+=((millis()/36000000));
  Used+= Usage*Using/10000;
  if(Used>=Limit/2){
     //lcd.clear();
     powersaving=true;
     if((Using>(Limit-Used)/10)){digitalWrite(50,1);}else{digitalWrite(50,0);}
     if(Using>(Limit-Used)/5){digitalWrite(52,1);}else{digitalWrite(52,0);}
     if(Using>(Limit-Used)/3){digitalWrite(53,1);}else{digitalWrite(53,0);}
     if(Used>=Limit){Home=false;recharge=true;}else{recharge=false; menu=true;}
   
     
    }else{powersaving=false;}
  if(Home){// check if not on the menu screen
    lcd.setCursor(0,0);// first row and coum of the lcd
    lcd.print("Limit/MTH: N");// print the set limit
    lcd.setCursor(12,0);// first row and coum of the lcd
    lcd.print(String(Limit));// print the set limit
    if(powersaving){
    lcd.setCursor(0,1);//secont row if the lcd
    lcd.print("Used:  N");// print current the usage
    lcd.setCursor(8,1);
    lcd.print(String(Used));
    lcd.setCursor(14,1);
    lcd.print("Ps");
    }
    else{ 
    lcd.setCursor(0,1);//secont row if the lcd
    lcd.print("Used:   N");// print current the usage
    lcd.setCursor(9,1);
    lcd.print(String(Used));
    lcd.setCursor(14,1);
    lcd.print("N");
      }
    
    }
    if(recharge){
       lcd.setCursor(0,0);
       lcd.print("Unchained");
       lcd.setCursor(0,1);
       lcd.print("Budget Exhusted");
      }
    if(menu){
      changinglimit=false;
      lcd.setCursor(0,0);
      lcd.print(menuItem[x]);
      lcd.setCursor(0,1);
      lcd.print("D-OK,B&C-Scroll"); 
      }
     if(changinglimit){
        lcd.setCursor(0,0);
        lcd.print("Type limit, D-ok");
      }
      if(settingpriority){
        lcd.setCursor(0,0);
        lcd.print("set Priority");
        }
      if(testing){
        lcd.setCursor(0,0);
        lcd.print("Assume A load");
        }

  //temp=String(millis()/60000); 
  //hum=String(millis()/1000);
  //temp=String(4890);
  //hum="5677";
  //data = "temperature=" + temp + "&humidity=" + hum;
  //httppost();
 // delay(100);
  
  if (key){
    Serial.println(key);
    //lcd.setCursor(0, 1);
    //lcd.print(key);// to varify if the key is working.
    if(key=='1'||key=='2'||key=='3'||key=='3'||key=='4'||key=='5'||key=='6'||key=='7'||key=='8'||key=='9'||key=='0'){
        if(changinglimit){ lcd.clear(); tv+=key; lcd.setCursor(0,1); lcd.print(tv);}
        if(settingpriority){ lcd.clear(); tv+=key; lcd.setCursor(0,1); lcd.print(tv);}
        if(testing){ lcd.clear(); tv+=key; lcd.setCursor(0,1); lcd.print(tv);}
      }
    if(key=='#'){lcd.clear(); tv="";  lcd.setCursor(0,1); lcd.print(tv);}
    if(key=='A'){
       if(!menu){
       lcd.clear();
       menu=true;Home=false;}
      }//ends if key is equal to A
     else if(key=='B'){
      if(menu){lcd.clear(); x+=1;if(x>=4){x=0;}}
      }
     else if(key=='C'){
      if(menu){lcd.clear(); if(x==0){x=4;} x-=1;}
      }
     else if(key=='D'){
      if(changinglimit)
        {
          Limit=tv.toInt(); 
          changinglimit=false; 
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Limit Set");
          delay(1500);
          lcd.clear();
          tv="";
          Home=true;
        }
       if(settingpriority){
          Priority=tv.toInt(); 
          settingpriority=false; 
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Priority Set");
          delay(1500);
          lcd.clear();
          tv="";
          Home=true;          
        }
        if(testing){
          Using=tv.toFloat();
          testing=false;
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Test Usage set");
          delay(1500);
          lcd.clear();
          tv="";
          Home=true;     
          }
        if(exitmenu){
          menu=false;
          exitmenu=false;
          Home=true;
          }
      if(menu){
        if(x==4){menu=false;}
        else if(x==0){x=0;menu=false; changinglimit=true;  lcd.clear();}
        else if(x==1){x=0;menu=false; settingpriority=true; lcd.clear();}
        else if(x==2){x=0;menu=false; testing=true;  lcd.clear();}
        else if(x==3){x=0;menu=false; exitmenu=true; lcd.clear();}
        }
      }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////




void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OK");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
///////////////////////////////////////////////////////////////////////////////////
 
 void httppost () {

    esp8266.println("AT+CIPSTART=\"TCP\",\"" + server + "\",80");//start a TCP connection.
    
    if( esp8266.find("OK")) {
    
    Serial.println("TCP connection ready");
    
    } 
    delay(200);
    
    String postRequest =
    
    "POST " + uri + " HTTP/1.0\r\n" +
    
    "Host: " + server + "\r\n" +
    
    "Accept: *" + "/" + "*\r\n" +
    
    "Content-Length: " + data.length() + "\r\n" +
    
    "Content-Type: application/x-www-form-urlencoded\r\n" +
    
    "\r\n" + data;
    
    String sendCmd = "AT+CIPSEND=";//determine the number of caracters to be sent.
    
    esp8266.print(sendCmd);
    
    esp8266.println(postRequest.length() );
    
    delay(500);
    
    if(esp8266.find(">")) { Serial.println("Sending.."); esp8266.print(postRequest);
    
    if( esp8266.find("SEND OK")) { Serial.println("Packet sent");
        while (esp8266.available()) {     
          String tmpResp = esp8266.readString();
          Serial.println(tmpResp);
        
        }
      
      // close the connection
      
        esp8266.println("AT+CIPCLOSE");
      
      }
    
    }
  }
