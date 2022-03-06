char last_callback[20];

void message_handler(FB_msg& msg) {

  char message_text[msg.text.length() + 1];
  msg.text.toCharArray(message_text, msg.text.length() + 1);


  if (equal_strings(last_callback, "")) {
    bot.showMenu(F("Choose mode\nSet brightness\tSet speed\tSet scale\nSend text"), msg.chatID);
  }
  else if (equal_strings(last_callback, "Choose mode")) {
    mode = find_effect_number(message_text);
    bot.sendMessage(F("Режим установлен"), msg.chatID);

  }
  else if (equal_strings(last_callback, "Set brightness")) {
    modes[mode].bright = atoi(message_text);
    bot.sendMessage(F("Новое значение яркости установлено"), msg.chatID);
    eeprom_flag = true;
    eeprom_timer = millis();
  }
  else if (equal_strings(last_callback, "Set speed")) {
    modes[mode].effect_speed = atoi(message_text);
    bot.sendMessage(F("Новое значение скорости установлено"), msg.chatID);
    eeprom_flag = true;
    eeprom_timer = millis();
  }
  else if (equal_strings(last_callback, "Set scale")) {
    modes[mode].scale = atoi(message_text);
    bot.sendMessage(F("Новое значение масшатаба установлено"), msg.chatID);
    eeprom_flag = true;
    eeprom_timer = millis();
  }
  else if (equal_strings(last_callback, "Send text")) {
    running_string = String(message_text);
    running_string = convertUnicode(running_string);
    
    bot.sendMessage(F("Новый текст установлен"), msg.chatID);
    bot.sendMessage(running_string, msg.chatID);
  }

  if (equal_strings(message_text, "Choose mode")) {
    bot.inlineMenu(F("Choose mode"), F("Matrix\nRainbowHor\nRainbowVert\nRainbowDiag\nSinusoid\nRunningText\nWhiteLight\nPeacock\nLava\nParty\nPaintBall\nRunningCube\nFire"), msg.chatID);

  }
  else if (equal_strings(message_text, "Set brightness")) {
    bot.sendMessage(F("Введите яркость (0-255)"), msg.chatID);
  }
  else if (equal_strings(message_text, "Set speed")) {
    bot.sendMessage(F("Введите скорость (0-255)"), msg.chatID);
  }
  else if (equal_strings(message_text, "Set scale")) {
    bot.sendMessage(F("Введите масштаб (0-255)"), msg.chatID);
  }
  else if (equal_strings(message_text, "Send text")) {
    bot.sendMessage(F("Введите текст"), msg.chatID);
  }

  strcpy(last_callback, message_text);

}

uint8_t find_effect_number(char *text) {
  bool flag = false;
  uint8_t res = 0;
  for (uint8_t i = 0; i < 13; i++) {
    if (strlen_P(effect_names[i]) == strlen(text)) {
      for (uint8_t j = 0; j < strlen(text); j++) {
        if ((byte)pgm_read_byte(&effect_names[i][j]) == (byte)text[j]) {
          flag = true;
        }
        else {
          flag = false;
          break;
        }
      }
      if (flag) res = i;
    }

  }
  return res;
}
