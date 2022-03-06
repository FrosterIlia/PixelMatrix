void setup() {
  //top_button.setTickMode(AUTO);
  //down_button.setTickMode(AUTO);
  //top_button.setTimeout(600);
  EEPROM.begin(512);

  Serial.begin(115200);
  Serial.setTimeout(1000);
  FastLED.addLeds <WS2812, PIN_MATRIX, GRB>(leds, NUM_LEDS);
  if (CURRENT_LIMIT > 0) FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT); //ограничиваем ток
  FastLED.show();
  FastLED.setBrightness(EMPTY_BRIGHT);

  if (EEPROM.read(0) != 'w') {
    EEPROM.put(0, 'w');
    EEPROM.put(1, modes);
    EEPROM.commit();
  }
  else {
    EEPROM.get(1, modes);
  }


  if (WIFI_ENABLED) {
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      if (millis() > 15000) ESP.restart();
    }
    Serial.println("Connected");

    bot.attach(message_handler);

    
  }
  
}
