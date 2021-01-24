#include <GyverEncoder.h>     // библиотека дял энкодера
#include <OLED_I2C.h>         // библиотека для OLED дисплея
#include "FastLED.h"          // библиотека для работы с лентой

#define KEY 8                  //подключения энкодера
#define S2 9
#define S1 10

#define LED_DT 7              // пин, куда подключен DIN ленты
#define LED_COUNT 60          //число светодиодов в ленте

Encoder encoder(S2, S1, KEY);    //CLK - S2, DT - S1, SW - KEY Подключение потенциометра (энкодера)

OLED  myOLED(SDA, SCL, 8);
extern uint8_t SmallFont[];
extern uint8_t BigNumbers[];

struct CRGB leds[LED_COUNT];

int MAX_INTENSITY = 255;
int rgb[] = {0, 0, 0};
int current_color = 0;

void show_color() 
{  
  for (int i = 0 ; i < LED_COUNT; i++ )
    leds[i].setRGB(rgb[0], rgb[1], rgb[2]);
  LEDS.show();
  
}

void setup() {
  myOLED.begin();
  oled_display();
  
  Serial.begin(9600);
  encoder.setType(TYPE2);  
  LEDS.setBrightness(MAX_INTENSITY);  // ограничить максимальную яркость
  show_color();          // погасить все светодиоды
  LEDS.addLeds<WS2811, LED_DT, GRB>(leds, LED_COUNT);  // настрйоки для нашей ленты (ленты на WS2811, WS2812, WS2812B)
}

void loop() {
  encoder.tick();

  if (encoder.isRight()) 
    change_intensity(rgb[current_color], 1);         // если был поворот
  if (encoder.isLeft()) 
    change_intensity(rgb[current_color], -1);
  
  if (encoder.isRightH()) 
    change_intensity(rgb[current_color], MAX_INTENSITY); // если было удержание + поворот
  if (encoder.isLeftH()) 
    change_intensity(rgb[current_color], -MAX_INTENSITY);
  
  //if (encoder.isPress()) change_current_color();         // нажатие на кнопку (+ дебаунс)
  if (encoder.isRelease()) 
    change_current_color();     // отпускание кнопки (+ дебаунс)
  if (encoder.isHolded()) 
    equal();       // если была удержана и энк не поворачивался
  //if (encoder.isHold()) Serial.println("Hold");         // возвращает состояние кнопки

}

void change_intensity(int &intensity, int stp)
{   
  intensity += stp;
  if (intensity < 0)
    intensity = 0;
  else if (intensity > MAX_INTENSITY)
    intensity = MAX_INTENSITY;
  show_color();
  oled_display();
  
}  

void change_current_color()
{
  current_color += 1;
  if (current_color >= 3)
    current_color = 0;
  oled_display();
}

void equal()
{
  rgb[0] = rgb[current_color];
  rgb[1] = rgb[current_color];
  rgb[2] = rgb[current_color];
  show_color();
  leds[0].setRGB(255, 255, 255);
  oled_display();
}


void oled_display()
{ 
  myOLED.clrScr();
  myOLED.invert(false);
  myOLED.setFont(SmallFont);
  myOLED.print(String("RED:   "), LEFT, 10);
  myOLED.print(String("GREEN: "), CENTER, 10);
  myOLED.print(String("BLUE: "), RIGHT, 10);

  myOLED.setFont(BigNumbers);
  myOLED.printNumI(rgb[0], LEFT, 30);
  myOLED.printNumI(rgb[1], CENTER, 30);
  myOLED.printNumI(rgb[2], RIGHT, 30);

  myOLED.setFont(SmallFont);
  if (current_color == 0)
    myOLED.print(String("*"), 3, 20);
    
  else if (current_color == 1)
    myOLED.print(String("*"), 60, 20);

  else
    myOLED.print(String("*"), 118, 20);

  myOLED.update();
}
