#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 lcd(128, 64, &Wire, -1);

//Calibration step
float calibration_value = 21.34+0.21;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;

void setup() {
  Serial.begin(9600);

  //LCD Code
  // initialize OLED display with address 0x3C for 128x64
  if (!lcd.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  delay(2000);         // wait for initializing
  lcd.clearDisplay(); // clear display

  lcd.setTextSize(1);          // text size
  lcd.setTextColor(WHITE);     // text color
  lcd.setCursor(50, 10);
  lcd.print("Hello");
  lcd.setCursor(10,20);
  lcd.println("Please insert probe");
  lcd.setCursor(40, 30);
  lcd.println("in liquid");
  lcd.display();
}

void loop() {
  //Reading the analog inputs
  for(int i=0;i<10;i++)
   {
   buffer_arr[i]=analogRead(A0);
   Serial.println(buffer_arr[i]);
   delay(30);
   }
  Serial.println("Done");
  //Sorting the readings
  for(int i=0;i<9;i++)
   {
   for(int j=i+1;j<10;j++)
     {
     if(buffer_arr[i]>buffer_arr[j])
       {
       temp=buffer_arr[i];
       buffer_arr[i]=buffer_arr[j];
       buffer_arr[j]=temp;
       }
     }
   }

   //Getting the pH value
   avgval=0;
   for(int i=2;i<8;i++)
     {
     avgval+=buffer_arr[i];
     }
   //avgval = 2765; //For testing purposes
   Serial.println(avgval);
   float volt=(float)avgval*5.0/1024/6;
   float ph_act = (-5.70 * volt) + calibration_value;

   //Displaying the pH value 
   delay(5000);  
   lcd.clearDisplay();
   lcd.setCursor(30, 10);
   lcd.println("pH Val:");
   lcd.setCursor(75, 10);
   lcd.println(ph_act);

   //For too high or too low pH values
   if (ph_act>8.50)
    {
    lcd.setCursor(32,32);
    lcd.println("pH too high!");
    }
   else if(ph_act>7.6)
    {
    float diff = (ph_act - 7.5)*100;
    float change = (diff-10)*100/diff;
    lcd.setCursor(10,20);
    lcd.println(change);
    lcd.setCursor(40,20);
    lcd.println("% water change");
    }
   else if(ph_act>6.5)
    {
    lcd.setCursor(20,32);
    lcd.println("Perfect pH levels");
    }
   else
    {
    lcd.setCursor(32,32);
    lcd.println("pH too low!");
    }
   lcd.display();
   delay(1000);

}
