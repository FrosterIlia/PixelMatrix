// **************** НАСТРОЙКА МАТРИЦЫ ****************
#if (CONNECTION_ANGLE == 0 && STRIP_DIRECTION == 0)
#define _WIDTH WIDTH
#define THIS_X x
#define THIS_Y y

#elif (CONNECTION_ANGLE == 0 && STRIP_DIRECTION == 1)
#define _WIDTH HEIGHT
#define THIS_X y
#define THIS_Y x

#elif (CONNECTION_ANGLE == 1 && STRIP_DIRECTION == 0)
#define _WIDTH WIDTH
#define THIS_X x
#define THIS_Y (HEIGHT - y - 1)

#elif (CONNECTION_ANGLE == 1 && STRIP_DIRECTION == 3)
#define _WIDTH HEIGHT
#define THIS_X (HEIGHT - y - 1)
#define THIS_Y x

#elif (CONNECTION_ANGLE == 2 && STRIP_DIRECTION == 2)
#define _WIDTH WIDTH
#define THIS_X (WIDTH - x - 1)
#define THIS_Y (HEIGHT - y - 1)

#elif (CONNECTION_ANGLE == 2 && STRIP_DIRECTION == 3)
#define _WIDTH HEIGHT
#define THIS_X (HEIGHT - y - 1)
#define THIS_Y (WIDTH - x - 1)

#elif (CONNECTION_ANGLE == 3 && STRIP_DIRECTION == 2)
#define _WIDTH WIDTH
#define THIS_X (WIDTH - x - 1)
#define THIS_Y y

#elif (CONNECTION_ANGLE == 3 && STRIP_DIRECTION == 1)
#define _WIDTH HEIGHT
#define THIS_X y
#define THIS_Y (WIDTH - x - 1)

#else
#define _WIDTH WIDTH
#define THIS_X x
#define THIS_Y y
#pragma message "Wrong matrix parameters! Set to default"

#endif

// получить номер пикселя в ленте по координатам
uint16_t get_pixel_number(int8_t x, int8_t y) {
  if ((THIS_Y % 2 == 0) ) {               // если чётная строка
    return (THIS_Y * _WIDTH + THIS_X);
  } else {                                              // если нечётная строка
    return (THIS_Y * _WIDTH + _WIDTH - THIS_X - 1);
  }
}
void pixelRGB(byte x, byte y, byte color[]) {
  // красит пиксель по заданным координатам в любой цвет rgb
  leds[get_pixel_number(x, y)] = CRGB(color[0], color[1], color[2]);
}
void pixelHSV(byte x, byte y, byte color[]) {
  // красит пиксель по заданным координатам в любой цвет hsv
  leds[get_pixel_number(x, y)] = CHSV(color[0], color[1], color[2]);
}

void draw_text(int x, byte y, String text, byte color[]) {
  uint8_t koeef = 0;
  for (uint8_t symbol = 0; symbol < text.length(); symbol++) {

    if ((byte)text[symbol] > 127) { // для русского языка (каждый символ русского языка занимает 2 байта в кодировке utf-8)
      uint8_t rus_symbol[2];
      rus_symbol[0] = text[symbol];
      rus_symbol[1] = text[symbol + 1];


      symbol += 1;
      for (uint8_t i = 0; i < pgm_read_byte(&alfavit_length_rus); i++) { // находим индекс символа в массиве
        if (pgm_read_byte(&alfavit_rus[i][0]) == rus_symbol[0] && pgm_read_byte(&alfavit_rus[i][1]) == rus_symbol[1]) {
          koeef = i;
          break;
        }
      }
      for (uint8_t i = 0; i < FONT_HEIGHT; i++) { // рисуем символ
        for (uint8_t j = 0; j < FONT_WIDTH; j++) {
          if (pgm_read_byte(&letters_rus[koeef][i][j]) &&
              x + j < WIDTH &&
              x + j >= 0 &&
              y - i >= 0 &&
              y - i < HEIGHT) {
            pixelHSV(x + j, y - i, color);
          }
        }
      }
      x += FONT_WIDTH + LETTER_DISTANCE;
      continue;
    }

    else { // если английский символ или спецсимвол (короче если символ занимает 1 байт)
      if (text[symbol] == ' ') { // отрабатываем пробел
        x += FONT_WIDTH;
        continue;
      }
      for (uint8_t i = 0; i < pgm_read_byte(&alfavit_length); i++) { // находим индекс символа в массиве
        if ((char)pgm_read_byte(&alfavit[i]) == (char)text[symbol]) {
          koeef = i;
          break;
        }
      }
      for (uint8_t i = 0; i < FONT_HEIGHT; i++) { // рисуем символ
        for (uint8_t j = 0; j < FONT_WIDTH; j++) {
          if (pgm_read_byte(&letters[koeef][i][j]) &&
              x + j < WIDTH &&
              x + j >= 0 &&
              y - i >= 0 &&
              y - i < HEIGHT) {
            pixelHSV(x + j, y - i, color);
          }
        }
      }
      x += FONT_WIDTH + LETTER_DISTANCE;
    }
  }
}

boolean equal_strings(char *text1, char *text2) {
  bool flag = false;
  if (strlen(text1) == strlen(text2)) {
    for (int j = 0; j < strlen(text2); j++) {
      if (text1[j] == text2[j]) {
        flag = true;

      }
      else {
        flag = false;
        break;
      }
    }
  }
  if (flag) return true;
  return false;

}
String convertUnicode(String unicodeStr){
  
  
  String out = "";
  int len = unicodeStr.length();
  char iChar;
  char* error;
  for (int i = 0; i < len; i++){
     iChar = unicodeStr[i];
     if(iChar == '\\'){ // got escape char
       iChar = unicodeStr[++i];
       if(iChar == 'u'){ // got unicode hex
         char unicode[7];
         unicode[0] = '0';
         unicode[1] = 'x';
         for (int j = 0; j < 4; j++){
           iChar = unicodeStr[++i];
           unicode[j + 2] = iChar;
         }
         unicode[6] = NULL;
         int unicodeVal;
         sscanf(unicode,"%x", &unicodeVal);
         unicodeVal -= 1040;
         
         out += (char)pgm_read_byte(&alfavit_rus[unicodeVal][0]);
         out += (char)pgm_read_byte(&alfavit_rus[unicodeVal][1]); 
       } else if(iChar == '/'){
         out += iChar;
       } else if(iChar == 'n'){
         out += '\n';
       }
     } else {
       out += iChar;
     }
  }
  return out;
}
