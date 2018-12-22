#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
int x=0;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
  display.clearDisplay();
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  x++;
  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(1, 0);
  display.println("HOLA MUNDO");
  display.setCursor(100, 0);
  display.println(x);
  display.display();      // Show initial text
  delay(1000);
}
