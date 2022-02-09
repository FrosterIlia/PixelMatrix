//протокол
// a - яркость
// b - скорость
// c - масштаб
// d - эффект назад
// e - эффект вперед
// f - кнопка выключения
// g - кнопка вниз
// h - кнопка влево
// i - копка вверх
// j - кнопка вправо
// k - переход к режимам


void parsing() {
  static byte prev_buf = 0;
  static uint32_t tmr = 0;
  byte buf = Serial.available();
  if (prev_buf != buf) {
    prev_buf = buf;
    tmr = millis();
  }
  if (Serial.available() > 1) {
    char key = Serial.read();
    int value = Serial.parseInt();

    switch (key) {
      case 'a':
        modes[mode].bright = value;
        eeprom_flag = true;
        eeprom_timer = millis();
        break;
      case 'b':
        modes[mode].effect_speed = value;
        eeprom_flag = true;
        eeprom_timer = millis();
        break;
      case 'c':
        modes[mode].scale = value;
        eeprom_flag = true;
        eeprom_timer = millis();
        break;
      case 'd':
        if (--mode < 0) mode = MODE_AMOUNT - 1;
        loadingFlag = true;
        PERIOD = 40;
        send_all_data();
        break;
      case 'e':
        if (++mode >= MODE_AMOUNT) mode = 0;
        loadingFlag = true;
        PERIOD = 40;
        send_all_data();
        break;
      case 'f':
        turned = !turned;
        break;
      case 'i': // вверх
        button = 0;
        break;
      case 'j': // вправо
        button = 1;
        break;
      case 'g': // вниз
        button = 2;
        break;
      case 'h': //влево
        button = 3;
        break;
      case 'k':
        send_all_data();
        break;
    }
  }
}

void end_command() {
  for (uint8_t i = 0; i < 3; i++) {
    Serial.write(0xff);
  }
}

void sendInt(String attr, int value) {
  Serial.print(attr);
  Serial.print('=');
  Serial.print(value);
  end_command();
}

void sendStr(String attr, String value) {
  Serial.print(attr);
  Serial.print('=');
  Serial.print("\"" + value + "\"");
  end_command();
}

void get_effect_name() {
  effect_name = "";
  for (byte i = 0; i < strlen_P(effect_names[mode]); i++) {
    effect_name += (char)pgm_read_byte(&(effect_names[mode][i]));
  }

}

void send_all_data() {
  sendInt(F("h0.val"), modes[mode].bright);
  sendInt(F("h1.val"), modes[mode].scale);
  sendInt(F("h2.val"), modes[mode].effect_speed);
  get_effect_name();
  sendStr(F("page0.t3.txt"), effect_name);
}
