
void setSysMsg(String msg){
  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0 , 10);
  display.println(msg);
  display.display();
}

void setupDisplay(){
  display.clearDisplay();
  display.drawBitmap(0,5,symbols[dir].bitmap, 64, 64, 1);
   //setDirLabel();
   setDistance();
   setStreet();
   display.display();
}


void setDirLabel(){
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 50);
  char buffer[4];
  itoa(dir, buffer, 10);
  display.println(String(buffer));
//  display.println(F("dir"));
}

void setDistance(){
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(66, 0);
  char buffer[16];
  itoa(dist2, buffer, 10);
  display.println(String(buffer)+"m");
}

void setStreet(){
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(66 , 30);
  display.print(old_street.c_str());
}
