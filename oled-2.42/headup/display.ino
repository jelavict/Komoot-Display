void setupDisplay(){
  display.clearDisplay();
   setDirection(dir);
   setDistance(dist2);
   setStreet(old_street.c_str());
   setBatteryStatus();
   display.display();
}



void setDirection(int dir){
  display.drawBitmap(0,5,symbols[dir].bitmap, 64, 64, 1);
}

void setDistance(int meters){
  display.setTextSize(1);
  display.setCursor(66,15);
  display.setTextColor(SSD1306_WHITE);
  display.println("D:"+String(meters)+" m");
}


void setStreet(String street){
  int i = 0;
  String word = "";
  for(auto c : street) {
   word = word+c;
   i++;
   if(i>8){
    word = word+"|";
    i = 0;
   }
  }

  char delimiter = '|';
  char inputCharArray[word.length() + 1];
  word.toCharArray(inputCharArray, sizeof(inputCharArray));
  //splitString(inputCharArray,'|');

  int lineCnt = 0;
  int pos = 35;
  char* token = strtok(inputCharArray, &delimiter);
  while (token != NULL) {
    if (lineCnt < 3){
      display.setTextSize(1);
      display.setCursor(66,pos);
      display.setTextColor(SSD1306_WHITE);
      display.println(token);
      pos = pos + 10;
    }
     
    token = strtok(NULL, &delimiter);
    lineCnt++;
  }
}

void setBatteryStatus(){
  int adcValue = analogRead(GPIO_NUM_32);
  uint8_t percentage = 100;
  float voltage = ((float)adcValue / (4095)*3.3)+1.5;

  if (voltage > 1 ) { // Only display if there is a valid reading
    percentage = 2836.9625 * pow(voltage, 4) - 43987.4889 * pow(voltage, 3) + 255233.8134 * pow(voltage, 2) - 656689.7123 * voltage + 632041.7303;
    if (voltage >= 4.18) percentage = 100;
    if (voltage <= 3.33) percentage = 0;
    Serial.println("Voltage = " + String(voltage));
    Serial.println("Percentage = " + String(percentage));
    int xPos=83;
    int yPos=0;
    int batW=16;
    int batH=8;
    
    display.drawRect(xPos, yPos, batW, batH, SSD1306_WHITE);
    display.fillRect(xPos+batW, yPos+1, 3, batH-2, SSD1306_WHITE);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(xPos+batW+4,0);
    display.println(String(percentage)+ "%");
    display.fillRect(xPos+2, yPos+2, percentage*0.12, batH-4, SSD1306_WHITE);
  }
}

void setSysMsg(String msg){
  display.clearDisplay();
  setBatteryStatus();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0 , 10);
  display.println(msg);
  display.display();
}
