#include "U8glib.h"
#include <FastLED.h>
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);

#define LED_PIN     5
#define NUM_LEDS    8
CRGB leds[NUM_LEDS];
uint8_t max_bright=100;

int gsr_sensor = A0;
int force_sensor = A1;
int gsr_val;
int force_val;
void setup()
{
  Serial.begin(9600);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(max_bright);  
  


}

void loop()
{
  gsr_val = get_gsr_val();     //get GSR sensor data
  force_val = get_force_val();;     //get force sensor data
  
  if(gsr_val < 460)     //set min gsr value to 460 after testing it myself
  {
    gsr_val = 460;
  }
  if(gsr_val > 540)     //set max gsr value to 540 after testing it myself
  {
    gsr_val = 540;
  }
  if(force_val > 951)     //set max force value to 951 after testing it myself
  {
    force_val = 951;
  }
   
  int gsr_map = map(gsr_val,460,540,0,100);      //match gsr value to 0-100
  int force_map = map(force_val,0,951,0,100);    //match force value to 0-100

  
  
  int abs_data = abs(gsr_map - force_map);      //get the absolute value of the difference between the gsr value and the force value
  Serial.print("abs_data = ");
  Serial.println(abs_data);


  //when emotion fluctuation has similar level with the force, green light
  if(abs_data < 50) 
  {
    
    u8g.firstPage();
    do {
    display1();
    } while (u8g.nextPage() );
    
    for(int i=0;i<NUM_LEDS;i++)      //light fade in and out from number 1-8
    {
     leds[i] = CRGB (255,0,0);
     delay(100);
     FastLED.show(); 
    }
    
  }


 //when emotion fluctuation has less difference with the force, blue light
  else if(abs_data >= 50 && abs_data < 80)  
  {
    u8g.firstPage();
    do {
    display2();
    } while (u8g.nextPage() );
  
    for(int i=0;i<NUM_LEDS;i++)      //light fade in and out from number 1-8
    {
     leds[i] = CRGB (0,0,255);
     delay(100);
     FastLED.show(); 
    }
    
  }

  //when emotion fluctuation has large difference with the force, red light
  else if(abs_data >= 80) 
  {
    u8g.firstPage();
    do {
    display3();
    } while (u8g.nextPage() );
  
    for(int i=0;i<NUM_LEDS;i++)      //light fade in and out from number 1-8
    {
     leds[i] = CRGB (0,255,0);
     delay(100);
     FastLED.show(); 
    }
    
  }
  
}

int get_gsr_val()
{
  int sensor_val = analogRead(gsr_sensor);
  Serial.print("gsr_val = ");
  Serial.println(sensor_val);
  return sensor_val;
}

int get_force_val()
{
  int sensor_val = analogRead(force_sensor);
  Serial.print("force_val = ");
  Serial.println(sensor_val);
  return sensor_val;
}
void display1()
{
  u8g.setFont(u8g_font_profont12);
  u8g.setPrintPos(0, 10);
  u8g.print("Express your feeling ???");
}

void display2()
{
  u8g.setFont(u8g_font_profont12);
  u8g.setPrintPos(0, 10);
  u8g.print("Take a rest ???");
}

void display3()
{
  u8g.setFont(u8g_font_profont12);
  u8g.setPrintPos(0, 10);
  u8g.print("Stay real to yourself ????");
}
