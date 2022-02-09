class Rect {
  public:
    byte x, y;
    int dx, dy;
    uint8_t width, height;
    CHSV color;
    byte hue;

    Rect(byte x_s = 0, byte y_s = 0, uint8_t width_s = 3, uint8_t height_s = 3, CHSV color_s = CHSV(0, 255, 255)) {
      x = x_s;
      y = y_s;
      dx = 1;
      dy = 1;
      width = width_s;
      height = height_s;
      color = color_s;
      hue = color.hue;
    };

    
    unsigned int get_pixel_number(byte x, byte y) {
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



    void draw() {
    color.hue = hue;
      for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
          leds[get_pixel_number(i + x, y - j)] = color;
        }
      }
     
    }
};
