void setup() {
  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  sbi(ADCSRA, ADPS0);

  //уменьшаем всякие таймауты у кнопок, чтобы ускорить их работу
  //пытаемся сделать по-настоящему рандомные числа
  randomSeed(analogRead(EMPTY_PIN));


  mode = MODE;
  Serial.begin(115200);
  Serial.setTimeout(5);
  FastLED.addLeds <WS2812, PIN_MATRIX, GRB>(leds, NUM_LEDS);
  if (CURRENT_LIMIT > 0) FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT); //ограничиваем ток
  FastLED.show();
  FastLED.setBrightness(EMPTY_BRIGHT);
  pinMode(PIN_MICRO, INPUT);

  if (EEPROM.read(0) != 'w') {
    EEPROM.put(0, 'w');
    EEPROM.put(1, modes);
  }
  else {
    EEPROM.get(1, modes);
  }
  send_all_data();
  fps = millis(); //основной счетчик (mainloop)
                                       
}
