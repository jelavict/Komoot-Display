#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define OLED_WIDTH 128
#define OLED_HEIGHT 64

#define OLED_ADDR 0x3C
int demodelay=2000;

Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT);

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
}

void loop() {

// Clear the OLED screen
  display.clearDisplay();
  
  // ------------------------------------------------------------------------------------------------
  // drawPixel(x, y, color)
  // Draw one single tiny pixel in the center of the display ( 128x32 pixels )
  // ------------------------------------------------------------------------------------------------
  display.drawPixel(64, 16, WHITE);
  
  // Show the display buffer on the OLED display this is required to see the changes of draw functions
  display.display();
  
  //  Pauses the program for number of millisecounds defined in the variable demodelay 
  delay(demodelay);

  display.clearDisplay();
  // ------------------------------------------------------------------------------------------------
  // drawLine(x start, y start,x end, y end, color)
  // Draw line from x,y start to x,y end
  // ------------------------------------------------------------------------------------------------
  display.drawLine(5, 15, 100, 30, WHITE);
  display.display();
  delay(demodelay);

  display.clearDisplay();
  // ------------------------------------------------------------------------------------------------
  // drawFastVLine(x, y, length, color)
  // Draw a verticle line starting at x,y for the length of pixels
  // ------------------------------------------------------------------------------------------------
  display.drawFastVLine(64, 16, 15, WHITE);
  display.display();
  delay(demodelay);
  
  display.clearDisplay();
  // ------------------------------------------------------------------------------------------------
  // drawFastHLine(x, y, length, color)
  // Draw a horizontal line starting at x,y for the length of pixels
  // ------------------------------------------------------------------------------------------------
  display.drawFastHLine(64, 16, 15, WHITE);
  display.display();
  delay(demodelay);

  display.clearDisplay();
  // ------------------------------------------------------------------------------------------------
  // drawTriangle(x1, y1, x2, y2, x3, y3, color)
  // Draws a triangle from corner x,y point 1 to point 2 and then 3
  // ------------------------------------------------------------------------------------------------
  display.drawTriangle(5,15, 100,10, 64,30, WHITE);
  display.display();
  delay(demodelay);
  
  display.clearDisplay();
  // ------------------------------------------------------------------------------------------------
  // fillTriangle(x1, y1, x2, y2, x3, y3, color)
  // Draws and fills a triangle from corner x,y point 1 to point 2 and then 3
  // ------------------------------------------------------------------------------------------------
  display.fillTriangle(5,15, 100,10, 64,30, WHITE);
  display.display();
  delay(demodelay);
  
  display.clearDisplay();
  // ------------------------------------------------------------------------------------------------
  // drawRect(x corner, y corner, width, height, color)
  // Draws a rectangle from corner x,y with the width and height
  // ------------------------------------------------------------------------------------------------
  display.drawRect(64, 16, 30,10, WHITE);
  display.display();
  delay(demodelay);

  display.clearDisplay();
  // ------------------------------------------------------------------------------------------------
  // fillRect(x corner, y corner, width, height, color)
  // Draws and fills rectangle from corner x,y with the width and height
  // ------------------------------------------------------------------------------------------------
  display.fillRect(64, 16, 30,10, WHITE);
  display.display();
  delay(demodelay);

  display.clearDisplay();
  // ------------------------------------------------------------------------------------------------
  // drawCircle(x center, y center, radius, color)
  // Draws a circle x,y are the center of circle and radius length controls the size
  // ------------------------------------------------------------------------------------------------
  display.drawCircle(64,16, 15, WHITE);
  display.display();
  delay(demodelay);

  display.clearDisplay();
  // ------------------------------------------------------------------------------------------------
  // fillCircle(x center, y center, radius, color)
  // Draws and fills a circle x,y are the center of circle and radius length controls the size
  // ------------------------------------------------------------------------------------------------
  display.fillCircle(64,16, 15, WHITE);
  display.display();
  delay(demodelay);

}
