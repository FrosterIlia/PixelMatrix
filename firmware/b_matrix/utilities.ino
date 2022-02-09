unsigned int get_pixel_number(byte x, byte y) { //возвращает номер пикселя в ленте (если лента намотана зиг-загом)
  if (x > WIDTH - 1 ||  x < 0 ) {
    return 0;
  }
  if (y < 0 || y > HEIGHT - 1) {
    return 0;
  }
  if (y % 2 == 0) {
    return WIDTH * y + x;
  }
  else {
    return WIDTH * y + (WIDTH - x - 1);
  }
}

void pixelRGB(byte x, byte y, byte color[]) { // красит пиксель по заданным координатам в любой цвет rgb
  leds[get_pixel_number(x, y)] = CRGB(color[0], color[1], color[2]);
}
void pixelHSV(byte x, byte y, byte color[]) { // красит пиксель по заданным координатам в любой цвет hsv
  leds[get_pixel_number(x, y)] = CHSV(color[0], color[1], color[2]);
}
//функция заливки всего дисплея цветом
void fill_screen(byte color[]) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(color[0], color[1], color[2]);
  }
}

// как map, только работает с float
/*float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
  {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }*/




//режим музыки, возвращает значение от 0-255 в зависимости от громкости
/*uint8_t music(uint16_t minimum, uint16_t maximum) {
  static uint8_t SAMPLES = 100;
  static uint16_t sound;
  static uint16_t currentValue;
  static int local_max = 200, local_min = 200;
  static float local_maxF, local_minF, fil_lenght;
  static byte counter = 50;
  static uint16_t LEDlength;
  sound = 0;
  for (int i = 0; i < SAMPLES; i++) {
    currentValue = analogRead(PIN_MICRO);
    if (currentValue > sound) sound = currentValue;
  }

  counter++;
  if (counter >= 50) {
    counter = 0;
    local_max = 0;
    local_min = 1023;
  }
  if (sound > local_max) local_max = sound;
  if (sound < local_min) local_min = sound;


  local_maxF += (float) (local_max - local_maxF) * 0.01;
  local_minF += (float) (local_min - local_minF) * 0.01;

  LEDlength = map(sound - local_minF, shum, local_maxF - local_minF, minimum, maximum);
  if (sound > local_maxF - 5) LEDlength = maximum;
  LEDlength = constrain(LEDlength, minimum, maximum);

  // когда только шум - лента не горит
  if (local_maxF - local_minF < shum) LEDlength = 0;

  // сглаживаем все експоненциальным фильтром, дабы лента плавно разгоралась и гасла
  fil_lenght += (float)(LEDlength - fil_lenght) * SMOOTH;
  fil_lenght = constrain(fil_lenght, minimum, maximum);
  return fil_lenght;
  }*/

void analyzeAudio() { // служебная функция для режима с частотами
  static int sum = 0;
  for (int i = 0 ; i < FHT_N ; i++) { // заполняем массив fht_input значениями с микрофона
    sum = 0;
    for (int j = 0; j < 10; j++) { // предварительно фильтруем их через среднее арифметическое
      sum += analogRead(PIN_MICRO);
    }
    fht_input[i] = sum / 10; // put real data into bins
  }
  fht_window();  // window the data for better frequency response
  fht_reorder(); // reorder the data before doing the fht
  fht_run();     // process the data in the fht
  fht_mag_log(); // take the output of the fht
}


// рисует любой текст включая цифры, используя fonts из progmem
void draw_text(int x, byte y, char text[], byte color[]) {
  uint8_t koeef = 0;
  for (uint8_t symbol = 0; symbol < strlen(text); symbol++) {

    if ((byte)text[symbol] > 127) { // для ебучего русского языка (каждый символ русского языка занимает 2 байта в кодировке utf-8)
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

    else { // если английский символ или спецсимвол (короче если хуйня занимает 1 байт)
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
// функция получения цвета пикселя по его номеру


// функция получения цвета пикселя в матрице по его координатам
uint32_t getPixColorXY(int8_t x, int8_t y) {
  return getPixColor(get_pixel_number(x, y));
}
