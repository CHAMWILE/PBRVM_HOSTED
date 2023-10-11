#include "esp_system.h"
#include "esp_log.h"
#include "esp_types.h"
#include "driver/touch_pad.h"

void disable_all_touch_pads()
{
    for (int i = 0; i < TOUCH_PAD_MAX; i++) {
        touch_pad_t touch_pad_num = (touch_pad_t)i;
        touch_pad_config(touch_pad_num, 0);
    }
}   

#include <Servo.h>
Servo myservo;  // create servo object to control a servo,can get to eight
int pos = 0;    // variable to store the servo position


#include <HX711.h>
HX711 scale; // Create HX711 object
float calibration_factor = 474; // Replace with calibration factor of your load cell
 float weightmeasured;          
float weightload;
float previousweight=0;
#include <WiFi.h>
#include <HTTPClient.h>
#include <string>
#include <string.h>
const char* ssid = "HUAWEI";
const char* password1 = "987654321";

int chupa;
int simamisha;
String phoneNumber;
String password;
String Address="192.168.43.224";
#include <HardwareSerial.h>

HardwareSerial mySerial(2);
int amount1;
String amount2;
int check;

#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
const int ROW_NUM = 4; 
const int COLUMN_NUM = 4; 

int capacitive = 15;
int photo = 0;

char keys[ROW_NUM][COLUMN_NUM] = {
{'1', '2', '3', 'A'},
{'4', '5', '6', 'B'},
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};


byte pin_rows[ROW_NUM] = {23,01,3,19};
byte pin_column[COLUMN_NUM] = {18,5,4,2};

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

String inputString;
unsigned long previous;
int y;
int locklogin=0;
char ficha = '*';
String ficha2;
float amount;
float weight;
  
LiquidCrystal_I2C lcd(0x27,20,4);

#include "SR04.h"
#define TRIG_PIN 26
#define ECHO_PIN 25
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long a;

void setup() {
  Wire.begin(14,27);
  pinMode(capacitive,INPUT);
pinMode(photo,INPUT);
Serial.begin(115200);
mySerial.begin(115200);
    myservo.attach(22); 
    myservo.write(30);
    scale.begin(13,12); // Initialize HX711 with channel A0 as data and A1 as clocks
  scale.set_scale(calibration_factor); // Setcalibration factor
     scale.tare(); // Tare weight
     disable_all_touch_pads();
  WiFi.begin(ssid, password1);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
    lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("   PLASTIC BOTTLE ");
  lcd.setCursor(0,1);
  lcd.print("  REVERSE VENDING ");
  lcd.setCursor(0,2);
  lcd.print("      MAcHINE     ");
  delay(3000);
  lcd.clear();
  
  
inputString.reserve(10); 
  
}
void prepaid();
void weightl()
{
  
weightload = scale.get_units(); // Get weight in units
weightmeasured=weightload-previousweight;
  previousweight=weightload;
  
Serial.print("Weight: ");
Serial.print(weightmeasured,2); // Print weight with 2 decimal places
Serial.println(" units");
 lcd.setCursor(0,0);
 lcd.print("Uzito: ");
 lcd.print(weightmeasured,2);
 lcd.print(" g");
 lcd.setCursor(0,1);
 lcd.print("Sawa na Sh: ");
    float hela = weightmeasured * 0.4;
 lcd.print(hela,2);
 delay(3000);
 lcd.clear();
   
 
String weightStr = String(weightmeasured);

  HTTPClient http;
  http.begin("http://"+Address+"/PBRM/pbrvmupdata.php?phone_number="+phoneNumber+"&weight="+weightStr);
  int httpCode = http.GET();
  if (httpCode > 0) {
    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.println("Error on HTTP request");
  }
  http.end();
  delay(1000);
   locklogin = 4;
   prepaid();

  
}
 void kuona()
{
while(mySerial.available()){
Serial.write(mySerial.read());
      
}
}
 void updateweight()
 {
         int amount4 = amount2.toInt();
  float weight = amount4/0.4;
  String weightString = String(weight);

  HTTPClient http;
  http.begin("http://"+Address+"/PBRM/pbrvmdelete.php?phone_number="+phoneNumber+"&weight="+weightString);
  int httpCode = http.GET();
  if (httpCode > 0) {
    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.println("Error on HTTP");
  }
  http.end();
  delay(500);
 }


 
void checkmuamala()
{
  String message="";
  mySerial.println("AT+CMGF=1"); // set SMS mode to text
delay(500);
mySerial.println("AT+CNMI=1,2,0,0,0"); //enable new message notification
delay(10000);
while(mySerial.available()){
 message += mySerial.readString(); 
}
Serial.println(message);
//message.indexOf("Umetuma")==0
if(true){ //check if first word is "umetuma"
  Serial.println("Transaction complete!");
       updateweight();
         lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("     HOngera");
      lcd.setCursor(0,1);
    lcd.print("Muamala umekamilika!");
    delay(2000);
    
    lcd.clear();
    phoneNumber="";
    password="";
    amount2="";
    message="";
    locklogin = 0;
     loop();
}
else 
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Muamala ");
  lcd.setCursor(0,1);
  lcd.print(" Haujakamilika! ");
  delay(1000);
  lcd.clear();
   amount2="";
    message="";
      locklogin=4;
      prepaid();
}

}




void gsm()
{                // code za kuweka kiasi

                String amount3 = String(amount2);
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("  Subiri Pesa Yako ");
                lcd.setCursor(0,1);
                lcd.print(".");  
mySerial.println("AT+CUSD=1,\"*150*01#\""); 
delay(5000);
kuona();

 lcd.print("...");

mySerial.println("AT+CUSD=1,\"1\""); 
delay(5000);
kuona();
 lcd.print("..");

mySerial.println("AT+CUSD=1,\"1\""); 
delay(5000);
kuona();
 lcd.print("...");

mySerial.println("AT+CUSD=1,\"" + phoneNumber + "\"");  
delay(5000);                                                                    
kuona();
 lcd.print("..");

mySerial.println("AT+CUSD=1,\"" + amount2 + "\"");  
delay(5000);                          
kuona();
 lcd.print("...");

mySerial.println("AT+CUSD=1,\"2255\"");  
delay(5000);
kuona();
 lcd.print("..");
 
       checkmuamala();
       
      
}

void fetch()
{
    HTTPClient http;
  http.begin("http://"+Address+"/PBRM/pbrvmread.php?phone_number="+phoneNumber);
  int httpCode = http.GET();
  if (httpCode > 0) {
    String response = http.getString();
    // parse the response string to extract the weight and amount
    int weightStartIndex = response.indexOf("Weight: ") + 8;
    int weightEndIndex = response.indexOf(" Amount: ");
    int amountStartIndex = response.indexOf("Amount: ") + 8;
    weight = response.substring(weightStartIndex, weightEndIndex).toFloat();
    amount = response.substring(amountStartIndex).toFloat();
    // do something with the weight and amount
    Serial.println("Weight: " + String(weight) + " Amount: " + String(amount));
      amount1= (int) amount;
      lcd.clear();
     } else {
    Serial.println("Error on HTTP request");
  }

   http.end();
   delay(50);
}


void transaction()
{

    fetch();
        
          previous=millis();       
   while(locklogin==6)
  {    


    lcd.setCursor(0,0);
    lcd.print("Kiasi Kilichopo:");
    lcd.print(amount1);
      lcd.setCursor(0,1);
    lcd.print("Weka kiasi ");
       lcd.setCursor(0,2);
             lcd.print("Kiasi: ");
              lcd.cursor();
     char key = keypad.getKey();  
      
  if (key) {
      
    Serial.println(key);

    if (key >= '0' && key <= '9') {     
      amount2 += key;               
           lcd.setCursor(0,2);
             lcd.print("Kiasi: ");
          lcd.print(amount2);
           lcd.cursor();
          
       }
       if (key == 'C')
       {
        if(amount2.length()> 0)
        {
         amount2.remove(amount2.length() - 1);
           lcd.print("Kiasi: ");
          lcd.print(amount2);
           lcd.cursor();
       }
       }
       if(key == 'D')
       {
        locklogin=4;
        amount2="";
        lcd.clear();
        prepaid();
       }
      if (key == 'A')
          {
              check = amount2.toInt();
//           check = (int)amount2;
             if(check <= amount1 && check>=100)
             {
              gsm();
             }
             else
             {
              lcd.clear();
              lcd.setCursor(0,1);
              lcd.print("Weka kiasi sahihi!");
              delay(3000);
              check=0;
              amount2="";
              lcd.clear();
              
                 }
             }
        
         }
if ((millis () - previous) >= 60000)
 {
        locklogin=0;
        phoneNumber="";
        password="";
        amount2="";
        lcd.clear();
        loop();
 }

}
}

void tumasms2()
{
         String phoneNumber1 ="0659829835";
     mySerial.println("AT+CMGF=1"); //
  delay(100);
 mySerial.println("AT+CMGS=\"" + phoneNumber1 + "\"");
  delay(100);
  mySerial.println("Mashine ya kituo cha UDOM CIVE Imejaa Chupa");
   delay(100);
  mySerial.println((char)26);
  delay(5000);
}
void tumasms1()
{
      String phoneNumber1 ="0659829835";
  mySerial.println("AT+CMGF=1"); //
  delay(100);
  mySerial.println("AT+CMGS=\"" + phoneNumber1 + "\"");
  delay(100);
  mySerial.println("Mashine ya kituo cha UDOM CIVE inakaribia kujaa,Toa Chupa kwenye Mashine");
  delay(100);
  mySerial.println((char)26);
  delay(5000);
}

void kujaa()
 {
  a=sr04.Distance();
       
if ( a<=50)
{
  tumasms1();
}
 else if (a<=20)
 {
   tumasms2();
 }
 }


 void kuscan()
{
  // code za photoelectric and capacitance
  int kataa=0;
  simamisha=1;
  do{  
      if(chupa<=15)
     {   
   a=sr04.Distance();
   Serial.print(a);
   Serial.println("cm");
   lcd.setCursor(0,0);
   lcd.print("  Weka Chupa");
if ( a<=6)
{    
         lcd.setCursor(0,0);
    lcd.print("Chupa Inakaguliwa   ");
  int valuecapa = digitalRead(capacitive);
  int valuephoto = digitalRead(photo);
      delay(2000);
 if(valuecapa == 0 &&  valuephoto == 1 )
 {  
      lcd.setCursor(0,0);
    lcd.print("Chupa Imekubaliwa   ");
   
    myservo.write(180); 
    delay(1000);
    myservo.write(30);
     delay(1000);
     chupa=chupa + 1; 
    lcd.clear();   
 }
 else 
 {
    kataa=kataa + 1;
    if (kataa<=5)
    {
  lcd.setCursor(0,0);
    lcd.print("Chupa imekataliwa!  ");
  lcd.setCursor(0,1); 
    lcd.print("Ondoa chupa ");
      lcd.setCursor(0,2); 
    lcd.print("Weka chupa nyingine");
    delay(2000);
    lcd.clear();
    }
    else 
    {
      lcd.setCursor(0,0);
    lcd.print("Anza mwanzo upya  ");
      delay(2000);
      lcd.clear();
      simamisha=0;
    }
 }
     
  }


   char stopi = keypad.getKey();
   if (stopi) {
     
  if (stopi == 'A')
  {
    simamisha=0;
    lcd.clear();
    
        
  }
  
}
  }
  else 
  {
     lcd.setCursor(0,0);
     lcd.print("Mashine imejaaa");
     lcd.setCursor(0,1);
     lcd.print("Huwezi weka chupa");
       tumasms2(); 
     delay(2000);
     lcd.clear();
     simamisha=0;
  }
  }while(simamisha);

weightl();

}
 
void prepaid()
{
    fetch();
         
          previous=millis();
  while(locklogin==4)
  {    
     lcd.setCursor(0,0);
    lcd.print("Kiasi: ");
    lcd.print(amount1);
    lcd.setCursor(0,1);
    lcd.print("Kuweka Chupa:A ");
     lcd.setCursor(0,2);
    lcd.print("  Kutoa Pesa:B ");
    lcd.setCursor(0,3);
    lcd.print("      Kutoka:D ");
      char key = keypad.getKey();   
  if (key) {
     
  if (key == 'A')
  {  
     if (chupa<=10)
     {
    locklogin = 5;
    lcd.clear();
    kuscan();
     }
     else
     {
        lcd.clear();
      lcd.setCursor(0,0);
    lcd.print("Mshine imejaa ");
     lcd.setCursor(0,1);
    lcd.print("Fanya huduma nyingine ");
      delay(3000);
      lcd.clear();
     }
        
  }
  
  if (key == 'B')
  {
    locklogin = 6;
    
      lcd.clear();
    transaction();    
    
  }
     if (key == 'D')
  {
    locklogin = 0;
     phoneNumber="";
    password="";
    locklogin = 0;
    lcd.clear(); 
    loop();
  }
  }
  if ((millis () - previous) >= 60000)
 {
        locklogin=0;
        phoneNumber="";
        password="";
        amount2="";
        lcd.clear();
        loop();
 }

  }
}

 void getdata()
 {     
       ficha2="";
      Serial.begin(115200);
      if(y==1)
      {
        lcd.setCursor(0,0);
       lcd.print("  Kujisajili");
      }
      else
      {
        lcd.setCursor(0,0);
       lcd.print("  Kuingia ");
      }
       lcd.setCursor(0,1);
       lcd.print("Nambari:");
       lcd.cursor();
      
          previous=millis();
     while (locklogin==1)   
   {       
        char key = keypad.getKey();   
  if (key) {
      Serial.println( phoneNumber.length());
       Serial.println(key);
    if (key >= '0' && key <= '9') { 
      if(phoneNumber.length()<=10)
      {    
        
      phoneNumber += key;               
           lcd.setCursor(0,1);
             lcd.print("Nambari:");
          lcd.print(phoneNumber);
           lcd.cursor();
         Serial.println(phoneNumber);
     }
    }
   if (key == 'C')
       {
        if(phoneNumber.length()> 0)
        {
         phoneNumber.remove(phoneNumber.length() - 1);
               lcd.clear();
       if(y==1)
      {
        lcd.setCursor(0,0);
       lcd.print("  Kujisajili");
      }
      else
      {
        lcd.setCursor(0,0);
       lcd.print("  Kuingia ");
      }
               lcd.setCursor(0,1);
             lcd.print("Nambari:");
          lcd.print(phoneNumber);
           lcd.cursor();
       }
       }
       if(key == 'D')
       {
         locklogin=0;
        phoneNumber="";
        password="";
        ficha2="";
        lcd.clear();
        loop();
       }
  if (key == 'A')
  {
    locklogin = 2;    
  }
  

   }

if ((millis () - previous) >= 60000)
 {
        locklogin=0;
        phoneNumber="";
        password="";
        ficha2="";
        lcd.clear();
        loop();
 }
 }
        
 
        lcd.setCursor(0,2);
        lcd.print("Pini:"); 
         lcd.cursor();
       
          previous=millis();          
 while (locklogin==2)
 {   
     char key = keypad.getKey();   
  if (key) {
      
    Serial.println(key);

    if (key >= '0' && key <= '9') { 
      if(password.length()<=4)
      {   
        ficha2 +=ficha; 
           password += key;               
//              lcd.setCursor(0,2);
//             lcd.print("Pini:");
 //             lcd.print(password);
             lcd.setCursor(0,2);
             lcd.print("Pini:");
             lcd.print(ficha2);
              lcd.cursor();
         
          Serial.print(password);
          
       }
    }
       
       if (key == 'C')
       {
        if(password.length()> 0)
        {
         password.remove(password.length() - 1);
         ficha2.remove(ficha2.length() - 1 );
              lcd.clear();
       if(y==1)
      {
        lcd.setCursor(0,0);
       lcd.print("  Kujisajili");
      }
      else
      {
        lcd.setCursor(0,0);
       lcd.print("  Kuingia ");
      }
              lcd.setCursor(0,1);
             lcd.print("Nambari:");
          lcd.print(phoneNumber);
//               lcd.setCursor(0,2);
//             lcd.print("Pini:");
//          lcd.print(password);
            lcd.setCursor(0,2);
             lcd.print("Pini:");
          lcd.print(ficha2);
           lcd.cursor();
           Serial.print(password);
       }
       }

         if(key == 'D')
       {
        locklogin=0;
        phoneNumber="";
        password="";
        ficha2="";
        lcd.clear();
        loop();
       }
      if (key == 'A')
          {
           locklogin = 3;
            lcd.noCursor(); 
          } 
              if(key == '#')
       {
               lcd.setCursor(0,2);
             lcd.print("Pini:");
          lcd.print(password);
                delay(1000);
                lcd.setCursor(0,2);
             lcd.print("Pini:");
          lcd.print(ficha2);
        
       }  
       
      }
  if ((millis () - previous) >= 60000)
 {
        locklogin=0;
        phoneNumber="";
        password="";
        ficha2="";
        lcd.clear();
         lcd.noCursor();
        loop();
 }
   }

 }
void signup()
{          
     getdata();  
    
 if (locklogin == 3) 
 {
   HTTPClient http;
   http.begin("http://"+Address+"/PBRM/pbrvm.php?phone_number="+phoneNumber+"&password="+password);
   int httpCode = http.GET();
   if (httpCode > 0) {
    String response = http.getString();
         lcd.clear();
         lcd.setCursor(0,0);
    lcd.print("    Umesajiliwa ");
      delay(500);
      lcd.clear();
     locklogin = 1;
      phoneNumber="";
      password="";
          y=2;
          login();
   } 
    else 
    {
     lcd.clear();
    lcd.print("   Jaribu tena!");
     delay(500);
       lcd.clear();
       phoneNumber="";
      password="";
     locklogin = 1 ;
     signup();
    
    }
     http.end();
     delay(50);


 }
}
void  login()
 {
           
       getdata();

if(locklogin==3)
{
   HTTPClient http;
  http.begin("http://"+Address+"/PBRM/pbrvml.php?phone_number="+phoneNumber+"&password="+password);
  int httpCode = http.GET();
  if (httpCode > 0) {
    String response = http.getString();
    String stri= response;
    String check ="Successful";

    if(stri == check)
    {   
       lcd.clear();
         lcd.setCursor(0,0);
     lcd.print("   Umefanikiwa!");
        delay(500);
       lcd.clear();
      locklogin = 4;
      prepaid();
    }
    else 
    { 
       lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" Nambari au pini");
    lcd.setCursor(0,1);
    lcd.print(" Siyo sahihi!");
        delay(500);
       lcd.clear();
      locklogin = 1;
      password="";
      phoneNumber="";
      login();
    }
  } else {
     lcd.clear();
     lcd.print(" Jaribu tena!");
      delay(500);
      lcd.clear();
      locklogin = 1;
      password="";
      phoneNumber="";
      login();
  }
  
  http.end();
  delay(50);
 
 }
 
 }
void firstacivity()
{   
    Serial.println("Jisajili: A ");
    Serial.println("Ingia   : B ");
    lcd.setCursor(0,0);
    lcd.print("   CIVE PBRVM");
    lcd.setCursor(0,1);
    lcd.print("Jisajili:A ");
    lcd.setCursor(0,2);
    lcd.print("   Ingia:B");
    if (chupa>=10)
    {
      lcd.setCursor(0,3);
      lcd.print("Mashine imejaa chupa");
    }
    else
    {
      lcd.setCursor(0,3);
      lcd.print("");
    }
  char key = keypad.getKey();
     if (key) {
        if(key=='A')
        {
          y=1;
         locklogin = 1 ;
         lcd.clear();
         signup(); 
        }
       if(key=='B')
       {
        y=2;
        locklogin = 1;
        lcd.clear();
        login();
        }
      }
}

void loop() {
  if (locklogin==0)
  {
  firstacivity();
  }
}
