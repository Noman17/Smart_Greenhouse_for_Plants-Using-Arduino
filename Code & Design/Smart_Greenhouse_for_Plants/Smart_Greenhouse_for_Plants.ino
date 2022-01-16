#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>
#define NOTE_C4 262
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_B4 494
#define NOTE_C5 523

LiquidCrystal lcd1(A4, A5, A3, A2, A1, A0);
LiquidCrystal lcd2(A4, A6, A3, A2, A1, A0);
LiquidCrystal lcd3(A4, A9, A3, A2, A1, A0);
Servo myservo;

const byte rows=4; //four rows
const byte cols=3; //three columns
char key[rows][cols]={
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[rows]={21,20,19,18}; //connect to the row pinouts of the keypad
byte colPins[cols]={17,16,15}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad(makeKeymap(key),rowPins,colPins,rows,cols);
char* password = "0821";
int currentposition = 0;
int wr_count = 0;
int buzzer = 14; //buzzer to arduino pin 14
int buzz_led = 2; //connect Led to arduino pin 2
int value = 0;
float millivolts, cel;
int lm35 = A7; // connect lm35 sensor to arduino pin A7
int pump = 3; //motor to arduino pin 3
int ldr = A8;
int val = 0;
int pos = 0; // position of servo motor

void setup() {
  // put your setup code here, to run once:
  displayscreen();
  lcd1.begin(16,2);
  lcd2.begin(16,2);
  lcd3.begin(16,2);
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 14 as an output
  pinMode(buzz_led, OUTPUT);
  pinMode(pump, OUTPUT);
  myservo.attach(9); //Servo motor connection
}

void loop() {
  // put your main code here, to run repeatedly:
  value = analogRead(lm35);
  millivolts = (value / 1024.0) * 5 * 1000;
  cel = millivolts / 10;

  val = analogRead(ldr);
  
  if(currentposition == 0)
  {
    displayscreen(); 
  }
  int l ;
  char code = keypad.getKey();
  if(code != NO_KEY)
  {
    lcd1.clear();
    lcd1.setCursor(0,0);
    lcd1.print("PASSWORD:");
    lcd1.setCursor(7,1);
    lcd1.print(" ");
    lcd1.setCursor(7,1);
    
    for(l=0;l<=currentposition;++l)
    {    
      lcd1.print("*"); //keypress();
    }
   
    if (code == password[currentposition])
    {
      ++currentposition;
      if(currentposition == 4)
      {  
        digitalWrite(buzz_led, LOW);
        unlocker();
      }
    }
    else
    {
      wr_count++;
      incorrect();
      currentposition=0;
    }
  }
}

//------------Function 1 - DISPLAY FUNCTION --------------------//
void displayscreen()
{  
  lcd1.setCursor(0,0);
  lcd1.println("*ENTER THE CODE*");
  lcd1.setCursor(0,1); 
  lcd1.println("TO ENABLE SYSTEM!!"); 
}

//------------------ Function 2 - UNLOCK THE SYSTEM --------------//
void unlocker()
{
  lcd1.setCursor(0,0);
  lcd1.println(" ");
  lcd1.setCursor(1,0);
  lcd1.print("Access Granted");
  lcd1.setCursor(4,1);
  lcd1.println("WELCOME!!");
  lcd1.setCursor(15,1);
  lcd1.println(" ");
  lcd1.setCursor(16,1);
  lcd1.println(" ");
  lcd1.setCursor(14,1);
  lcd1.println(" ");
  lcd1.setCursor(13,1);
  lcd1.println(" ");

  lcd2.clear();
  lcd2.setCursor(0,0);
  lcd2.print("Temp in Garden!!");
  lcd2.setCursor(0,1);
  lcd2.print("Temperature:");
  lcd2.setCursor(12, 1);
  lcd2.print(cel);
  lcd2.setCursor(14, 1);
  lcd2.print("*C");
  
  if(cel >= 35)
  {
    digitalWrite(pump, HIGH);
  }
  else
  {
    digitalWrite(pump, LOW);
  }

  lcd3.clear();
  lcd3.setCursor(0,0);
  lcd3.print("Intensity of LDR");
  lcd3.setCursor(0,1);
  lcd3.print("Intensity: ");
  lcd3.setCursor(12, 1);
  lcd3.print(val);

  if(val >= 10 && val <= 300)
  {
    for(pos = 0; pos <= 8; pos +=2) // open the window
    { 
      myservo.write(pos); 
      delay(205);     
    }
  }
  else if(val >= 301 && val <= 600)
  {
    for(pos = 0; pos <= 68; pos +=5) // open the window
    { 
      myservo.write(pos); 
      delay(205);     
    }
  }
  else if(val >= 601)
  {
    for(pos = 0; pos <= 81; pos +=5) // open the window
    { 
      myservo.write(pos); 
      delay(205);     
    }
  }
}

//--------------------Function 3 - Wrong code --------------//
void incorrect()
{
  if(wr_count<=10)
  {
    if(wr_count<=2)
    {
      lcd1.clear();
      lcd1.setCursor(1,0);
      lcd1.print("Wrong password!");
      delay(1000);
      lcd1.clear();
      displayscreen();
    }
    else
    {
      lcd1.clear();
      lcd1.setCursor(1,0);
      lcd1.print("Intruder Alert!");
      delay(1000);
      digitalWrite(buzz_led, HIGH);
      tone(buzzer, NOTE_C4, 500);
      delay(500);
      tone(buzzer, NOTE_D4, 500);
      delay(500);
      tone(buzzer, NOTE_E4, 500);
      delay(500);
      tone(buzzer, NOTE_F4, 500);
      delay(500);
      tone(buzzer, NOTE_G4, 500);
      delay(500);
      lcd1.clear();
      displayscreen();
    }
  }
}
