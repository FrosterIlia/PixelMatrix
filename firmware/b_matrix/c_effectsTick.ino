void loop() {
  if (turned) {
    if (millis() - fps >= PERIOD) {
      fps = millis();

      switch (mode) {
        case 0:  paintball(); break;
        case 1:  runningCube(0); break;
        case 2:  rainbowVert(); break;
        case 3:  rainbowHor(); break;
        case 4:  rainbowDiag(); break;
        case 5:  matrixa(); break;
        case 6:  sinusoid(); break;
        //case 7:  wave(); break;
        case 8:  spectr(); break;
        case 9:  snake(); break;
        case 10: running_text(); break;
        case 11: white_light(); break;
        case 12: peacock(); break;
        case 13: lava(); break;
        case 14: party(); break;
        case 15: runningCube(2500); break;
        case 16: fireRoutine(true); break;

      }
      FastLED.show();
      
      if (mode != 0 && mode != 5 && mode != 15 && mode != 16) { // в этих режимах матрицу очищать не надо
        FastLED.clear();
      }
    }


  }
  else {
    
    FastLED.clear();
    FastLED.show();
  }
   parsing();

  if (eeprom_flag && millis() - eeprom_timer >= 4000) {
    EEPROM.put(1, modes);
    eeprom_flag = false;
  }
}

void yield() { // так как в режиме змейки экран со счетом работает на delay(), то эта фукция позволяет переключать режимы во время экрана счета
   parsing();
}
