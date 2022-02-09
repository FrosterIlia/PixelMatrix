void loop() {

  // опрашиваем кнопки
  right_button.tick();
  left_button.tick();

  //переключаем режимы
  if (right_button.isClick()) {
    if (++mode >= MODE_AMOUNT) {
      mode = 0;

      FastLED.clear();
      FastLED.show();
    }
    loadingFlag = true;
  }
  if (left_button.isClick()) {
    if (--mode < 0) {
      mode = MODE_AMOUNT - 1;

      FastLED.clear();
      FastLED.show();
    }
    loadingFlag = true;

  }
  //Настройка масштаба и скорости
  if (top_button.isClick()) {
    if (down_button.state()) modes[mode].scale = constrain(modes[mode].scale - 5, 0, 255);
    else modes[mode].scale = constrain(modes[mode].scale + 5, 0, 255);
    eeprom_flag = true;
    eeprom_timer = millis();

  }

  if (down_button.isClick()) {
    if (top_button.state()) modes[mode].effect_speed = constrain(modes[mode].effect_speed - 5, 0, 255);
    else modes[mode].effect_speed = constrain(modes[mode].effect_speed + 5, 0, 255);
    eeprom_flag = true;
    eeprom_timer = millis();
  }
  if (top_button.isHolded() && down_button.state()) turned = !turned;
  if (turned) {
    if (main_timer.isReady()) {
      switch (mode) {
        case 0: matrixa(); break;
        case 1: rainbowHor(); break;
        case 2: rainbowVert(); break;
        case 3: rainbowDiag(); break;
        case 4: sinusoid(); break;
        case 5: running_text(); break;
        case 6: white_light(); break;
        case 7: peacock(); break;
        case 8: lava(); break;
        case 9: party(); break;
        case 10: paintball(); break;
        case 11: runningCube(2500); break;
        case 12: fireRoutine(true); break;
        case 13: computer_pic(); break;
      }


      if (mode != 14) {
        FastLED.show();
      }


      if (mode != 0 && mode != 10 && mode != 11 && mode != 12 && mode != 13 && mode != 14) {
        FastLED.clear();
      }

    }
  }
  else {
    FastLED.clear();
    FastLED.show();
  }

  if (eeprom_flag && millis() - eeprom_timer >= 4000) {
    EEPROM.put(1, modes);
    eeprom_flag = false;
    EEPROM.commit();

  }

}
