#include <FastLED.h>
#define LOG_OUT 1 // use the log output function
#define FHT_N 64 // set to 256 point fht
#include <FHT.h>
#include <EEPROM.h>


#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))


//Пины подключения
#define PIN_MATRIX 4
#define PIN_MICRO A1
#define EMPTY_PIN A7

// эффект "огонь"
#define SPARKLES true        // вылетающие угольки вкл выкл
#define SEGMENTS 1
#define UNIVERSE_FIRE                                  // универсальный огонь 2-в-1 Цветной+Белый

//Настройки матрицы
#define WIDTH 42
#define HEIGHT 8
#define NUM_LEDS  WIDTH * HEIGHT
#define CURRENT_LIMIT 2000 // если 0, то лимит отключен
#define EMPTY_BRIGHT 150

// Настройки количества режимов
#define MODE 12
#define MODE_AMOUNT 17

//Настройки звука
#define LOW_PASS 25 // порог чувствительности микрофона

//Настройки синусоиды
#define SIN_MIN 3
#define SIN_MAX 13

//Настройки текста
#define FONT_HEIGHT 7
#define FONT_WIDTH 5
#define LETTER_DISTANCE 2 // расстояние между буквами в тексте

// Настройки игр
#define GAME_SCORE_TIMEOUT 3000

uint16_t PERIOD =  40;
int8_t button = -1;

struct modeSettings {
  uint8_t effect_speed = 30;
  uint8_t scale = 100;
  uint8_t bright = EMPTY_BRIGHT;
};

String effect_name;
int8_t mode;
CRGB leds[NUM_LEDS];
unsigned long fps;
bool loadingFlag = true;
modeSettings modes[MODE_AMOUNT];

bool eeprom_flag = false;
uint32_t eeprom_timer = 0;

bool turned = true;

//rainbow
uint8_t hue;
uint16_t index = round((255 / WIDTH) * 100);

int counter = 0;

/*DEFINE_GRADIENT_PALETTE( palitra_rainbow ) { // ХУЙ ТЕБЕ, БИБЛИОТЕКА ЕБАНАЯ
  0,  255,  0,  50, // Red
  32,  171, 85,  10, // Orange
  64,  171, 171,  0, // Yellow
  96,    0, 255,  0, // Green
  128,    0, 171, 85, // Aqua
  160,    0,  0, 255, // Blue
  192,   85,  0, 171, // Purple
  224,  200,  0, 100, // PIN_MATRIXk
  255,  255, 0, 100 // and back to Red
  };*/

//CRGBPalette32 myPal = palitra_rainbow;
void drawPixelXY(int16_t x, int16_t y, CRGB color)
{
  if (x < 0 || x > (int16_t)(WIDTH - 1) || y < 0 || y > (int16_t)(HEIGHT - 1)) return;
  uint32_t thisPixel = get_pixel_number((uint8_t)x, (uint8_t)y) * SEGMENTS;
  for (uint8_t i = 0; i < SEGMENTS; i++)
  {
    leds[thisPixel + i] = color;
  }
}

// функция получения цвета пикселя по его номеру
uint32_t getPixColor(uint32_t thisSegm)
{
  uint32_t thisPixel = thisSegm * SEGMENTS;
  if (thisPixel > NUM_LEDS - 1) return 0;
  return (((uint32_t)leds[thisPixel].r << 16) | ((uint32_t)leds[thisPixel].g << 8 ) | (uint32_t)leds[thisPixel].b);
}

// функция получения цвета пикселя в матрице по его координатам
uint32_t getPixColorXY(uint8_t x, uint8_t y)
{
  return getPixColor(get_pixel_number(x, y));
}
unsigned char matrixValue[8][16];
