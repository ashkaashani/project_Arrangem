#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //LCD initialization
volatile int processFlag = 0;// //initialize process flag as volatile
int IR = 2; //initialize process flag as volatile

Servo shutter; //140 and 15 approx
Servo servoSort;
Servo servoGate;

int CSS0 = 4; // Colour Sensor
int CSS1 = 5; // Colour Sensor
int CSS2 = 6; // Colour Sensor
int CSS3 = 7; // Colour Sensor
int CSLED = 8; // Colour Sensor
int CSOUT = 3; // Colour Sensor

unsigned int pulseWidth; //variable to get the frequency of colors

int Red = 0; //red component
int Green = 0; //green component
int Blue = 0; //blue component

long RedLong = 0;
long GreenLong = 0;
long BlueLong = 0;
   //count variables
int RCount = 0;
int GCount = 0;
int BCount = 0;
int OCount = 0;
int TCount = 0;
//interrupt routine
void IRInterrupt()
{
processFlag = 1;
}

void setup()
{

Serial.begin(4800); //serial monitor

pinMode(13,OUTPUT); //on board led pin

pinMode(IR,INPUT); // IR

servoSort.attach(9); // servo for sorting on pin 9

servoGate.attach(10); //servo for gate on pin 10

shutter.attach(11);
//  initialize color sensor pins
pinMode(CSS0,OUTPUT);
pinMode(CSS1,OUTPUT);
pinMode(CSS2,OUTPUT);
pinMode(CSS3,OUTPUT);
pinMode(CSOUT,INPUT);
//initialize scaling factor to 2%
digitalWrite(CSS0,LOW);
digitalWrite(CSS1,HIGH);

shutter.write(145);
delay(1000);
shutter.write(15);
delay(1000);
shutter.write(145);
delay(1000);
//make led of color sensor on and then off
pinMode(CSLED,OUTPUT);
digitalWrite(CSLED,HIGH);
delay(500);
digitalWrite(CSLED,LOW);
//onboard led off
digitalWrite(13,LOW);
//initialize servo gate-close,open,close
servoGate.write(120);
delay(1000);
servoGate.write(142);
delay(1000);
servoGate.write(120);
delay(1000);

servoSort.write(100);
delay(1000);
//4 angles of servo sort
for(int i=55;i<=145;i+=30) 
{
  servoSort.write(i);
  delay(1000);
}
servoSort.write(100);
delay(1000);

lcd.begin(16,2);
lcd.backlight();
//LCD   
lcd.setCursor(0,0);
lcd.print("    ARRANGEM    ");
lcd.setCursor(0,1);
lcd.print("    S.B.M.P.    ");
delay(1500);

lcd.setCursor(0,0);
lcd.print("  Ashka Ashani  ");
lcd.setCursor(0,1);
lcd.print("Roll No. 1280002");
delay(1500);

lcd.setCursor(0,0);
lcd.print(" Deesha Nirmal  ");
lcd.setCursor(0,1);
lcd.print("Roll No. 1280037");
delay(1500);

lcd.setCursor(0,0);
lcd.print("  Shaily Sheth  ");
lcd.setCursor(0,1);
lcd.print("Roll No. 1280053");
delay(1500);

lcd.setCursor(0,0);
lcd.print(" R  G  B  O  T  ");
lcd.setCursor(0,1);
lcd.print(" 00 00 00 00 00 ");
//interrupt
interrupts();
attachInterrupt(0,IRInterrupt,RISING); //attach interrupt INT0

}

void loop()
{
if (processFlag == 1)    //if interrupt occurs
{
  delay(1000);                  
  ColourSensorProcess();    //do the further process
  delay(500);
  digitalWrite(13,HIGH);    //make onboard led on and off
  delay(1000);
  digitalWrite(13,LOW);
  processFlag = 0;          //make the process flag 0  processFlag = 0;
  shutter.write(145);
delay(500);
shutter.write(15);
delay(500);
shutter.write(145);
delay(500);
}

}
void ColourSensorProcess()    //Main process(color detection)

{

digitalWrite(CSLED,HIGH);    //make rgb sensor leds on to detect color
delay(250);

                              //for RED component
digitalWrite(CSS2,LOW);
digitalWrite(CSS3,LOW);
delay(250);
pulseWidth = pulseIn(CSOUT,LOW);
RedLong = pulseWidth;
Serial.print("Red : ");
Serial.println(RedLong);

                              //for GREEN component
digitalWrite(CSS2,HIGH);
digitalWrite(CSS3,HIGH);
delay(250);
pulseWidth = pulseIn(CSOUT,LOW);
GreenLong = pulseWidth;
Serial.print("Green : ");
Serial.println(GreenLong);

                              //for BLUE component
digitalWrite(CSS2,LOW);
digitalWrite(CSS3,HIGH);
delay(250);
pulseWidth = pulseIn(CSOUT,LOW);
BlueLong = pulseWidth;
Serial.print("Blue : ");
Serial.println(BlueLong);

digitalWrite(CSLED,LOW);    //make rgb sensor leds off
 if(Red>Blue && Red>Green)
    {
      if(Red>244 && Red<248)
      {
          servoSort.write(85);
          RCount=RCount+1;
          if (RCount >= 100)
            {
              RCount = 0;
            }
  goto Skip;
      }

    }

                                    //PRINT GREEN COMPONENT

    else if(Green>Blue && Green>Red)
    {

          servoSort.write(115);
          GCount=GCount+1;
          if (GCount >= 100)
            {
              GCount = 0;
            }
  goto Skip;
    } 

                                     //PRINT BLUE COMPONENT
    else if(Blue>Red && Blue>Green)
    {
        if(Blue>246)
        {
          servoSort.write(145);
          BCount=BCount+1;
           if (BCount >= 100)
            {
              BCount = 0;
            }
  goto Skip;
        }
    }


          servoSort.write(55);
          OCount=OCount+1;
          if (OCount >= 100)
            {
              OCount = 0;
            }

Skip:
//increment total count
TCount = TCount + 1;
if (TCount >= 100)
{
  TCount = 0;
}


servoGate.write(142); //now OPEN the gate
delay(1000);
servoGate.write(120); //now CLOSE the gate
delay(1000);


//DISPLAY RESULT ON LCD
lcd.setCursor(0,0);
lcd.print(" R  G  B  O  T  ");
lcd.setCursor(0,1);

lcd.print(" ");

if (RCount < 10)  
{
  lcd.print("0");
  lcd.print(RCount);
}
else
{
  lcd.print(RCount);
}

lcd.print(" ");

if (GCount < 10)  
{
  lcd.print("0");
  lcd.print(GCount);
}
else
{
  lcd.print(GCount);
}

lcd.print(" ");

if (BCount < 10)  
{
  lcd.print("0");
  lcd.print(BCount);
}
else
{
  lcd.print(BCount);
}

lcd.print(" ");

if (OCount < 10)  
{
  lcd.print("0");
  lcd.print(OCount);
}
else
{
  lcd.print(OCount);
}

lcd.print(" ");

if (TCount < 10)  
{
  lcd.print("0");
  lcd.print(TCount);
}
else
{
  lcd.print(TCount);
}
lcd.print(" ");
} 

