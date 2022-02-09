void matrixa() {
  FastLED.setBrightness(modes[mode].bright);
  main_timer.setPeriod(constrain(map(modes[mode].effect_speed, 0, 255, 1, 200), 1, 200));
  uint8_t scale = constrain(map(modes[mode].scale, 0, 255, 1, 5), 1, 5);
  for (int i = 0; i < NUM_LEDS - WIDTH; i++) {
    if (i / WIDTH % 2 == 0) {

      leds[i] = leds[get_pixel_number(i - WIDTH * (i / WIDTH), (i / WIDTH) + 1)];
      leds[get_pixel_number(i - WIDTH * (i / WIDTH), (i / WIDTH) + 1)].nscale8(random(3000, 10000));
    }
    else {
      leds[i] = leds[get_pixel_number(WIDTH - 1 - i + (WIDTH * (i / WIDTH)), (i / WIDTH) + 1)];
      leds[get_pixel_number(WIDTH - 1 - i + (WIDTH * (i / WIDTH)), (i / WIDTH) + 1)].nscale8(random(3000, 10000));
    }

  }

  if (!random(2)) {
    for (int i = 0; i < scale; i++) {
      leds[random(NUM_LEDS - WIDTH, NUM_LEDS)] = CRGB(0, 255, 0);
    }
  }

}

void rainbowVert() {
  FastLED.setBrightness(modes[mode].bright);
  uint8_t effect_speed = constrain(map(modes[mode].effect_speed, 0, 255, 1, 50), 1, 50);
  uint8_t scale = constrain(map(modes[mode].scale, 0, 255, 10, 70), 1, 50);

  for (uint16_t i = 0; i < WIDTH; i++) {
    for (uint16_t j = 0; j < HEIGHT; j++) {

      leds[get_pixel_number(i, j)] = ColorFromPalette(RainbowColors_p  , (i * (float)n_index / 100)  - hue);
    }
    n_index = scale * 100;
  }
  hue += effect_speed;
}

void rainbowHor() {
  FastLED.setBrightness(modes[mode].bright);
  uint8_t effect_speed = constrain(map(modes[mode].effect_speed, 0, 255, 1, 50), 1, 50);
  uint8_t scale = constrain(map(modes[mode].scale, 0, 255, 10, 70), 1, 50);

  for (uint16_t i = 0; i < WIDTH; i++) {
    for (uint16_t j = 0; j < HEIGHT; j++) {

      leds[get_pixel_number(i, j)] = ColorFromPalette(RainbowColors_p  , (j * (float)n_index / 100)  - hue);
    }
    n_index = scale * 100;
  }
  hue += effect_speed;
}

void rainbowDiag() {
  FastLED.setBrightness(modes[mode].bright);
  uint8_t effect_speed = constrain(map(modes[mode].effect_speed, 0, 255, 1, 50), 1, 50);
  uint8_t scale = constrain(map(modes[mode].scale, 0, 255, 10, 70), 1, 50);

  uint8_t x;
  uint8_t y;

  uint8_t rainbow_counter = 0;
  int hue_index = 0;
  y = 0;
  for (x = 0; x < WIDTH; x++) {
    rainbow_counter = 0;
    while (x - rainbow_counter >= 0 && y + rainbow_counter < HEIGHT) {
      leds[get_pixel_number(x - rainbow_counter, y + rainbow_counter)] = ColorFromPalette(RainbowColors_p  , (hue_index * (float)n_index / 100)  - hue);
      rainbow_counter++;

    }
    n_index = scale * 100;
    hue_index++;
  }
  //hue += modes[mode].effect_speed;
  x = WIDTH - 1;
  for (y = 1; y < HEIGHT; y++) {

    rainbow_counter = 0;
    while (x - rainbow_counter >= 0 && y + rainbow_counter < HEIGHT) {
      leds[get_pixel_number(x - rainbow_counter, y + rainbow_counter)] = ColorFromPalette(RainbowColors_p  , (hue_index * (float)n_index / 100)  - hue);
      rainbow_counter++;

    }
    n_index = scale * 100;
    hue_index++;
  }
  hue += effect_speed;
}

void sinusoid() {
  FastLED.setBrightness(modes[mode].bright);
  main_timer.setPeriod(constrain(map(modes[mode].effect_speed, 0, 255, 1, 100), 1, 100));
  static int8_t prev;

  for (int i = 0; i < WIDTH; i++) {

    uint8_t row = round((float)(HEIGHT - 1) / 2 + sin((millis() - PERIOD * i) * (float)modes[mode].scale / 255 * DEG_TO_RAD) * ((float)(HEIGHT - 1) / 2));
    if (i != 0) {
      if (prev > row) {
        for (int j = prev; row <= j; j--) {

          leds[get_pixel_number(i, j)] = CRGB(255, 0, 0);
        }
      }
      else {
        for (int j = prev; row >= j; j++) {

          leds[get_pixel_number(i, j)] = CRGB(255, 0, 0);
        }
      }
    } else {
      leds[get_pixel_number(0, row)] = CRGB(255, 0, 0);
    }

    prev = row;

  }
}

/*void wave() {
  FastLED.setBrightness(modes[mode].bright);
  main_timer.setPeriod(constrain(map(modes[mode].effect_speed, 0, 255, 1, 100), 1, 100));
  static byte FATNESS = 2;
  static uint16_t filt_sinus;
  static uint8_t ONpixels[16];
  byte hsv[] = {modes[mode].scale, 255, modes[mode].bright};
  float sinus = sin((float)millis() * DEG_TO_RAD * (float)modes[mode].effect_speed / 255);
  for (int i = 0; i < WIDTH; i++) {
    if (i == SIN_MAX + 1) {
      ONpixels[i] = filt_sinus;
    }
    else {
      ONpixels[i] = ONpixels[i + 1];
    }
    for (int j = 0; j < FATNESS; j++) {
      pixelHSV(i, ONpixels[i] - j, hsv);
      pixelHSV(i, ONpixels[i] - j, hsv);
    }

  }
  for (int j = 0; j < FATNESS; j++) {
    pixelHSV(15, filt_sinus - j, hsv);
    pixelHSV(15, filt_sinus - j, hsv);
  }

  ONpixels[15] = filt_sinus;


  filt_sinus = (uint16_t)map((uint16_t)(sinus * 1000), -1000, 1000, SIN_MIN * 1000, SIN_MAX * 1000) / 1000;
  }*/

void white_light() {
  FastLED.setBrightness(modes[mode].bright);
  byte hsv[] = {0, 0, modes[mode].bright};
  if (HEIGHT % 2 == 0) {
    for (int i = HEIGHT / 2; i > 0; i--) {
      if ((constrain(map(modes[mode].scale, 0, 255, 0, HEIGHT), 0, HEIGHT) / 2) < i) {
        for (uint8_t j = 0; j < WIDTH; j++) {
          pixelHSV(j, i - 1, hsv);
          pixelHSV(j, HEIGHT - i, hsv);
        }
      }

    }
  }
  else {
    for (uint8_t j = 0; j < WIDTH; j++) { // убогий костыль "лишь бы работало" (без этого не светиться центральная строка)
      pixelHSV(j, HEIGHT / 2, hsv);
    }
    for (int i = HEIGHT / 2; i > 0; i--) {
      if ((constrain(map(modes[mode].scale, 0, 255, 0, HEIGHT), 0, HEIGHT) / 2) < i) {
        for (uint8_t j = 0; j < WIDTH; j++) {
          pixelHSV(j, i - 1, hsv);
          pixelHSV(j, HEIGHT - i, hsv);
        }
      }

    }
  }
}

void peacock() //павлин
{
  static int counter = 0;
  if (loadingFlag) {
    counter = 0;
    loadingFlag = false;
  }
  FastLED.setBrightness(modes[mode].bright);
  uint8_t scale = constrain(map(modes[mode].scale, 0, 255, 10, 70), 1, 50);
  main_timer.setPeriod(modes[mode].effect_speed);

  for (uint16_t i = 0; i < WIDTH; i++) {
    for (uint16_t j = 0; j < HEIGHT; j++) {

      leds[get_pixel_number(i, j)] = ColorFromPalette(RainbowStripeColors_p , inoise8(i * scale, j * scale, counter) - hue);
    }
  }
  counter += 20;

}

void lava() {
  static int counter = 0;
  if (loadingFlag) {
    counter = 0;
    loadingFlag = false;
  }
  FastLED.setBrightness(modes[mode].bright);
  uint8_t scale = constrain(map(modes[mode].scale, 0, 255, 10, 70), 1, 50);
  main_timer.setPeriod(modes[mode].effect_speed);

  for (uint16_t i = 0; i < WIDTH; i++) {
    for (uint16_t j = 0; j < HEIGHT; j++) {

      leds[get_pixel_number(i, j)] = ColorFromPalette(LavaColors_p , inoise8(i * scale, j * scale, counter) - hue);
    }
  }
  counter += 20;
}

void party() {
  static int counter = 0;
  if (loadingFlag) {
    counter = 0;
    loadingFlag = false;
  }
  FastLED.setBrightness(modes[mode].bright);
  uint8_t scale = constrain(map(modes[mode].scale, 0, 255, 10, 70), 1, 50);
  main_timer.setPeriod(modes[mode].effect_speed);

  for (uint16_t i = 0; i < WIDTH; i++) {
    for (uint16_t j = 0; j < HEIGHT; j++) {

      leds[get_pixel_number(i, j)] = ColorFromPalette(RainbowColors_p , inoise8(i * scale, j * scale, counter) - hue);
    }
  }
  counter += 20;
}

#define running_string "Лiцей iнформацiйних технологiй"
void running_text() {
  static int counter = 0;
  main_timer.setPeriod(modes[mode].effect_speed);
  static int x;
  if (loadingFlag) {
    x = WIDTH;
    loadingFlag = false;
  }
  byte hsv[] = {modes[mode].scale, 255, modes[mode].bright};
  draw_text(x, (HEIGHT + FONT_HEIGHT) / 2, running_string, hsv);
  x--;
  counter = 0;
  for (int i = 0; i < strlen(running_string); i++) {
    if ((byte)running_string[i] > 127) i++; // если русский символ, то он занимает 2 байта
    counter++;
  }
  if (x + counter * (FONT_WIDTH + LETTER_DISTANCE) < 0) x = WIDTH;
}

void paintball() {

  FastLED.setBrightness(modes[mode].bright);
  main_timer.setPeriod(constrain(map(modes[mode].effect_speed, 0, 255, 1, 100), 1, 100));
  uint8_t scale = constrain(map(modes[mode].scale, 0, 255, 1, 5), 1, 5);
  static Rect rects[3];

  if (loadingFlag) { // сгенерировать кубы
    for (int i = 0; i < 3; i++) {
      rects[i] = Rect(random(0, WIDTH), random(0, HEIGHT), scale, scale, CHSV(50 * i, 255, modes[mode].bright));
    }
    loadingFlag = false;
  }
  for (int i = 0; i < NUM_LEDS; i++) { // плавно тушим все светодиоды
    leds[i].nscale8(250);
  }
  if (scale != rects[0].width) {
    for (int i = 0; i < 3; i++) {
      rects[i].width = scale;
      rects[i].height = scale;
    }
  }

  for (int i = 0; i < 3; i++) {
    if (rects[i].x + (int)(rects[i].dx) + rects[i].width - 1 < WIDTH && rects[i].x + (int)(rects[i].dx)  >= 0) rects[i].x += (int)(rects[i].dx);
    else {
      rects[i].hue += 10;
      rects[i].dx = -rects[i].dx;
      if (rects[i].x + (int)(rects[i].dx) + rects[i].width - 1 < WIDTH && rects[i].x + (int)(rects[i].dx)  >= 0) rects[i].x += (int)(rects[i].dx);
    }

    if (rects[i].y + (int)(rects[i].dy) < HEIGHT && rects[i].y + (int)(rects[i].dy) - rects[i].height + 1 >= 0) rects[i].y += (int)(rects[i].dy);
    else {
      rects[i].hue += 10;
      rects[i].dy = -rects[i].dy;
      if (rects[i].y + (int)(rects[i].dy) < HEIGHT && rects[i].y + (int)(rects[i].dy) - rects[i].height + 1 >= 0) rects[i].y += (int)(rects[i].dy);
    }

    rects[i].dx += random(-3, 3);
    rects[i].dy += random(-3, 3);
    if (rects[i].dx > 5) {
      rects[i].dx -= 5;
    }
    else if (rects[i].dx < -5) {
      rects[i].dx += 5;
    }
    if (rects[i].dy > 5) {
      rects[i].dy -= 5;
    }
    else if (rects[i].dy < -5) {
      rects[i].dy += 5;
    }
    rects[i].draw();
  }


}

void runningCube(uint16_t nscale) {
  FastLED.setBrightness(modes[mode].bright);
  if (nscale > 0) {
    for (int i = 0; i < NUM_LEDS; i++) { // плавно тушим все светодиоды
      leds[i].nscale8(nscale);
    }
  }
  uint8_t scale = constrain(map(modes[mode].scale, 0, 255, 1, 8), 1, 8);
  main_timer.setPeriod(constrain(map(modes[mode].effect_speed, 0, 255, 1, 100), 1, 100));
  static Rect rect;
  if (loadingFlag) { // сгенерировать куб
    rect = Rect(4, 4, scale, scale, CHSV(random(0, 255), 255, modes[mode].bright));
    rect.dx = 2;
    rect.dy = 1;
    loadingFlag = false;
  }
  if (scale != rect.width) {
    for (int i = 0; i < 3; i++) {
      rect.width = scale;
      rect.height = scale;
    }
  }
  if (rect.x + (int)( rect.dx) + rect.width - 1 < WIDTH && rect.x + (int)( rect.dx)  >= 0) rect.x += (int)( rect.dx);
  else {
    if (!random(8)) { // меняем траекторию
      if (rect.dx == 2) {
        rect.dx = 1;
        rect.dy = 2;
      }
      else {
        rect.dx = 2;
        rect.dy = 1;
      }
    }
    rect.hue += 10;
    rect.dx = -rect.dx;
    if (rect.x + (int)(rect.dx) + rect.width - 1 < WIDTH && rect.x + (int)( rect.dx)  >= 0) rect.x += (int)( rect.dx);
  }

  if (rect.y + (int)(rect.dy) < HEIGHT && rect.y + (int)(rect.dy) - rect.height + 1 >= 0) rect.y += (int)(rect.dy);
  else {
    rect.hue += 10;
    rect.dy = -rect.dy;
    if (rect.y + (int)(rect.dy) < HEIGHT && rect.y + (int)(rect.dy) - rect.height + 1 >= 0) rect.y += (int)(rect.dy);
  }
  rect.draw();
}

uint8_t line[WIDTH];
uint8_t pcnt = 0U;
uint8_t deltaHue = 0;                                // текущее смещение пламени (hueMask)
uint8_t shiftHue[HEIGHT];                              // массив дороожки горизонтального смещения пламени (hueMask)
uint8_t deltaValue = 16U;                              // текущее смещение пламени (hueValue)
uint8_t shiftValue[HEIGHT];                            // массив дороожки горизонтального смещения пламени (hueValue)

//these values are substracetd from the generated values to give a shape to the animation
const unsigned char valueMask[8][16] PROGMEM =
{
  {128 , 0  , 0  , 0  , 0  , 0  , 0  , 128 , 128 , 128  , 128  , 32  , 32  , 32  , 32  , 128 },
  {150 , 0  , 0  , 0  , 0  , 0  , 0  , 150 , 128 , 128  , 150  , 32  , 32  , 32  , 32  , 150 },
  {200 , 0  , 0  , 0  , 0  , 0  , 0  , 200 , 200 , 200 , 200 , 64  , 64  , 64 , 128 , 200 },
  {210, 0  , 0  , 0  , 0  , 0  , 0  , 200, 200, 200 , 200 , 128 , 128 , 128 , 128 , 210},
  {255, 96 , 0  , 0  , 0  , 0  , 96 , 210, 210, 210 , 200 , 180 , 180 , 180 , 120 , 255},
  {255, 128, 96 , 0  , 0  , 96 , 128, 220, 220, 220, 200 , 200 , 200 , 200 , 128, 255},
  {255, 160, 128, 96 , 96 , 128, 160, 255, 255, 255, 220, 220 , 220 , 220, 220, 255},
  {255, 192, 160, 128, 128, 160, 192, 255, 255, 255, 255, 255, 255, 255, 255, 255}
};

//these are the hues for the fire,
//should be between 0 (red) to about 25 (yellow)
static const uint8_t hueMask[8][16] PROGMEM =
{
  {25, 22, 11, 1 , 1 , 11, 19, 25, 25, 22, 11, 1 , 1 , 11, 19, 25 },
  {25, 19, 8 , 1 , 1 , 8 , 13, 19, 25, 19, 8 , 1 , 1 , 8 , 13, 19 },
  {19, 16, 8 , 1 , 1 , 8 , 13, 16, 19, 16, 8 , 1 , 1 , 8 , 13, 16 },
  {13, 13, 5 , 1 , 1 , 5 , 11, 13, 13, 13, 5 , 1 , 1 , 5 , 11, 13 },
  {11, 11, 5 , 1 , 1 , 5 , 11, 11, 11, 11, 5 , 1 , 1 , 5 , 11, 11 },
  {8 , 5 , 1 , 0 , 0 , 1 , 5 , 8 , 8 , 5 , 1 , 0 , 0 , 1 , 5 , 8  },
  {5 , 1 , 0 , 0 , 0 , 0 , 1 , 5 , 5 , 1 , 0 , 0 , 0 , 0 , 1 , 5  },
  {1 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 1  }
};

void fireRoutine(bool isColored)
{
  main_timer.setPeriod(constrain(map(modes[mode].effect_speed, 0, 255, 1, 100), 1, 100));

  FastLED.setBrightness(modes[mode].bright);
  if (loadingFlag) {
    loadingFlag = false;
    //FastLED.clear();
    generateLine();
    //memset(matrixValue, 0, sizeof(matrixValue)); без очистки
  }
  if (pcnt >= 30) {                                         // внутренний делитель кадров для поднимающегося пламени
    shiftUp();                                              // смещение кадра вверх
    generateLine();                                         // перерисовать новую нижнюю линию случайным образом
    pcnt = 0;
  }
  //  drawFrame(pcnt, (strcmp(isColored, "C") == 0));           // прорисовка экрана
  drawFrame(pcnt, isColored);                              // для прошивки где стоит логический параметр
  pcnt += 25;  // делитель кадров: задает скорость подъема пламени 25/100 = 1/4
}

// Randomly generate the next line (matrix row)
void generateLine() {
  for (uint8_t x = 0U; x < WIDTH; x++) {
    line[x] = random(64, 255);                             // заполнение случайным образом нижней линии (127, 255) - менее контрастное, (64, 255) - оригинал
  }
}

void shiftUp() {                                            //подъем кадра
  for (uint8_t y = HEIGHT - 1U; y > 0U; y--) {
    for (uint8_t x = 0U; x < WIDTH; x++) {
      uint8_t newX = x % 16U;                               // сократил формулу без доп. проверок
      if (y > 7U) continue;
      matrixValue[y][newX] = matrixValue[y - 1U][newX];     //смещение пламени (только для зоны очага)
    }
  }

  for (uint8_t x = 0U; x < WIDTH; x++) {                    // прорисовка новой нижней линии
    uint8_t newX = x % 16U;                                 // сократил формулу без доп. проверок
    matrixValue[0U][newX] = line[newX];
  }
}

// draw a frame, interpolating between 2 "key frames"
// @param pcnt percentage of interpolation

void drawFrame(uint8_t pcnt, bool isColored) {                  // прорисовка нового кадра
  uint8_t scale = constrain(map(modes[mode].scale, 0, 255, 2, 230), 2, 230);
  int32_t nextv;
#ifdef UNIVERSE_FIRE                                            // если определен универсальный огонь  
  //  uint8_t baseHue = (float)modes[currentMode].Scale * 2.57;
  uint8_t baseHue = (float)(scale - 1U) * 2.6;
#else
  uint8_t baseHue = isColored ? 255U : 0U;
#endif
  uint8_t baseSat = 255;  // вычисление базового оттенка


  //first row interpolates with the "next" line
  deltaHue = random(0U, 2U) ? constrain (shiftHue[0] + random(0U, 2U) - random(0U, 2U), 15U, 17U) : shiftHue[0]; // random(0U, 2U)= скорость смещения языков чем больше 2U - тем медленнее
  // 15U, 17U - амплитуда качания -1...+1 относительно 16U
  // высчитываем плавную дорожку смещения всполохов для нижней строки
  // так как в последствии координаты точки будут исчисляться из остатка, то за базу можем принять кратную ширину матрицы hueMask
  // ширина матрицы hueMask = 16, поэтому нам нужно получить диапазон чисел от 15 до 17
  // далее к предыдущему значению прибавляем случайную 1 и отнимаем случайную 1 - это позволит плавным образом менять значение смещения
  shiftHue[0] = deltaHue;                                   // заносим это значение в стэк

  deltaValue = random(0U, 3U) ? constrain (shiftValue[0] + random(0U, 2U) - random(0U, 2U), 15U, 17U) : shiftValue[0]; // random(0U, 3U)= скорость смещения очага чем больше 3U - тем медленнее
  // 15U, 17U - амплитуда качания -1...+1 относительно 16U
  shiftValue[0] = deltaValue;


  for (uint8_t x = 0U; x < WIDTH; x++) {                                          // прорисовка нижней строки (сначала делаем ее, так как потом будем пользоваться ее значением смещения)
    uint8_t newX = x % 16;                                                        // сократил формулу без доп. проверок
    nextv =                                                               // расчет значения яркости относительно valueMask и нижерасположенной строки.
      (((100.0 - pcnt) * matrixValue[0][newX] + pcnt * line[newX]) / 100.0)
      - pgm_read_byte(&valueMask[0][(x + deltaValue) % 16U]);
    CRGB color = CHSV(                                                            // вычисление цвета и яркости пикселя
                   baseHue + pgm_read_byte(&hueMask[0][(x + deltaHue) % 16U]),    // H - смещение всполохов
                   baseSat,                                                       // S - когда колесо масштаба =100 - белый огонь (экономим на 1 эффекте)
                   (uint8_t)max(0, nextv)                                         // V
                 );
    leds[get_pixel_number(x, 0)] = color;                                            // прорисовка цвета очага
  }

  //each row interpolates with the one before it
  for (uint8_t y = HEIGHT - 1U; y > 0U; y--) {                                      // прорисовка остальных строк с учетом значения низлежащих
    deltaHue = shiftHue[y];                                                         // извлекаем положение
    shiftHue[y] = shiftHue[y - 1];                                                  // подготавлеваем значение смешения для следующего кадра основываясь на предыдущем
    deltaValue = shiftValue[y];                                                     // извлекаем положение
    shiftValue[y] = shiftValue[y - 1];                                              // подготавлеваем значение смешения для следующего кадра основываясь на предыдущем


    if (y > 8U) {                                                                   // цикл стирания текущей строоки для искр
      for (uint8_t _x = 0U; _x < WIDTH; _x++) {                                     // стираем строчку с искрами (очень не оптимально)
        drawPixelXY(_x, y, 0U);
      }
    }
    for (uint8_t x = 0U; x < WIDTH; x++) {                                          // пересчет координаты x для текущей строки
      uint8_t newX = x % 16U;                                                       // функция поиска позиции значения яркости для матрицы valueMask
      if (y < 8U) {                                                                 // если строка представляет очаг
        nextv =                                                                     // расчет значения яркости относительно valueMask и нижерасположенной строки.
          (((100.0 - pcnt) * matrixValue[y][newX]
            + pcnt * matrixValue[y - 1][newX]) / 100.0)
          - pgm_read_byte(&valueMask[y][(x + deltaValue) % 16U]);

        CRGB color = CHSV(                                                                  // определение цвета пикселя
                       baseHue + pgm_read_byte(&hueMask[y][(x + deltaHue) % 16U ]),         // H - смещение всполохов
                       baseSat,                                                             // S - когда колесо масштаба =100 - белый огонь (экономим на 1 эффекте)
                       (uint8_t)max(0, nextv)                                               // V
                     );
        leds[get_pixel_number(x, y)] = color;
      }
      else if (y == 8U && SPARKLES) {                                               // если это самая нижняя строка искр - формитуем искорку из пламени
        if (random(0, 20) == 0 && getPixColorXY(x, y - 1U) != 0U) drawPixelXY(x, y, getPixColorXY(x, y - 2U));  // 20 = обратная величина количества искр
        else drawPixelXY(x, y, 0U);
      }
      else if (SPARKLES) {                                                          // если это не самая нижняя строка искр - перемещаем искорку выше
        // старая версия для яркости
        newX = (random(0, 4)) ? x : (x + WIDTH + random(0U, 2U) - random(0U, 2U)) % WIDTH ;   // с вероятностью 1/3 смещаем искорку влево или вправо
        if (getPixColorXY(x, y - 1U) > 0U) drawPixelXY(newX, y, getPixColorXY(x, y - 1U));    // рисуем искорку на новой строчке
      }
    }
  }
}

void computer_pic() {

  if (loadingFlag) {
    Serial.println("cleared");
    FastLED.clear();
    FastLED.show();
    loadingFlag = false;
  }
  main_timer.setPeriod(0);
  static int counter = 0;
  FastLED.setBrightness(modes[mode].bright);

  if (Serial.available()) {
    char symbol = Serial.read();
    if (symbol == 'p') {
      FastLED.clear();
      counter = 0;
      symbol = Serial.read();
    }
    else if (symbol == 'f') {
      FastLED.show();
    }



    char one_color[30];
    if (symbol == 's') {

      int amount = Serial.readBytesUntil(';', one_color, 30);
      one_color[amount] = NULL;
      char* offset = one_color;
      int count = 0;
      int data[5];
      while (true) {
        data[count++] = atoi(offset);
        offset = strchr(offset, ',');
        if (offset) offset++;
        else break;
      }

      leds[get_pixel_number(data[0], data[1])] = CRGB(data[2], data[3], data[4]);
      counter++;
    }
  }

}
