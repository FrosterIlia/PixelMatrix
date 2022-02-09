#include <FastLED.h>
#include "Timer.h"
#include "GyverButton.h"
#include <EEPROM.h>

#define WIDTH 16
#define HEIGHT 16

#define CONNECTION_ANGLE 0
#define STRIP_DIRECTION 0

#define NUM_LEDS WIDTH * HEIGHT

#define PIN_MATRIX 2 //D4
#define PIN_TOP_BUTTON 5 //D8
#define PIN_RIGHT_BUTTON 12 //D6
#define PIN_DOWN_BUTTON 4 //D2
#define PIN_LEFT_BUTTON 14 //D5

#define CURRENT_LIMIT 2000 // если 0, то лимит отключен

#define MODE 0
#define MODE_AMOUNT 14

#define EMPTY_BRIGHT 50

CRGB leds[NUM_LEDS];

uint16_t PERIOD =  40;
int8_t mode = MODE;

struct modeSettings {
  uint8_t effect_speed = 40;
  uint8_t scale = 100;
  uint8_t bright = EMPTY_BRIGHT;
};

modeSettings modes[MODE_AMOUNT];

bool loadingFlag = true;

Timer main_timer(PERIOD);

uint8_t hue;
uint16_t n_index = round((255 / WIDTH) * 100);


bool eeprom_flag = false;
uint32_t eeprom_timer = 0;

bool turned = true;


GButton top_button(PIN_TOP_BUTTON);
GButton down_button(PIN_DOWN_BUTTON);
GButton right_button(PIN_RIGHT_BUTTON);

GButton left_button(PIN_LEFT_BUTTON);

//ФУНКЦИИ ДЛЯ ОГНЯ
void drawPixelXY(int16_t x, int16_t y, CRGB color)
{
  if (x < 0 || x > (int16_t)(WIDTH - 1) || y < 0 || y > (int16_t)(HEIGHT - 1)) return;
  uint32_t thisPixel = get_pixel_number((uint8_t)x, (uint8_t)y);

  leds[thisPixel] = color;

}

// функция получения цвета пикселя по его номеру
uint32_t getPixColor(uint32_t thisSegm)
{
  uint32_t thisPixel = thisSegm;
  if (thisPixel > NUM_LEDS - 1) return 0;
  return (((uint32_t)leds[thisPixel].r << 16) | ((uint32_t)leds[thisPixel].g << 8 ) | (uint32_t)leds[thisPixel].b);
}

// функция получения цвета пикселя в матрице по его координатам
uint32_t getPixColorXY(uint8_t x, uint8_t y)
{
  return getPixColor(get_pixel_number(x, y));
}
unsigned char matrixValue[8][16];
