#include "symbols.h"
#include "BLEDevice.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_MOSI   23  //(SDA) 
#define OLED_CLK    18  //(SCL) 
#define OLED_DC     16  //(DC) 
#define OLED_CS     5   //(CS) 
#define OLED_RESET  17  //(RES) 

static BLEUUID serviceUUID("71C1E128-D92F-4FA8-A2B2-0F171DB3436C");
static BLEUUID    charUUID("503DD605-9BCB-4F6E-B235-270A57483026");
const  boolean run_app = true;
static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* kDevice;


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

void IRAM_ATTR ISR() {
   goToSleep();
}

void goToSleep(){
   Serial.println("Going to sleep now");
    delay(1000);
    esp_deep_sleep_start();
}

void print_wakeup_reason(){
  Serial.begin(115200);
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}

/**
   Scan for BLE servers and find the first one that advertises the service we are looking for.
*/
class AdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    /**
        Called for each advertising BLE server.
    */
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      Serial.print("BLE Advertised Device found: ");
      Serial.println(advertisedDevice.toString().c_str());
      // We have found a device, let us now see if it contains the service we are looking for.
      if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {

        BLEDevice::getScan()->stop();
        kDevice = new BLEAdvertisedDevice(advertisedDevice);
        doConnect = true;
        doScan = true;

      }
    }
};

class ClientCallback : public BLEClientCallbacks {

    void onConnect(BLEClient* pclient) {
      Serial.println("onConnect");
    }

    void onDisconnect(BLEClient* pclient) {
      connected = false;
      Serial.println("onDisconnect");
    }
};

bool connectToServer() {
     Serial.print("Forming a connection to ");
    Serial.println(kDevice->getAddress().toString().c_str());
    
    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new ClientCallback());

    // Connect to the remove BLE Server.
    pClient->connect(kDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");
    setSysMsg("Connected to server");

    
    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      setSysMsg("Failed to find our service UUID");
      Serial.println(serviceUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our service");
    setSysMsg("Found our service");


    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      setSysMsg("Failed to find our characteristic UUID");
      Serial.println(charUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our characteristic");
    setSysMsg("Found our characteristic");

    // Read the value of the characteristic.
    if(pRemoteCharacteristic->canRead()) {
      std::string value = pRemoteCharacteristic->readValue();
      Serial.print("The characteristic value was: ");
      Serial.println(value.c_str());
    }

//    if(pRemoteCharacteristic->canNotify())
//      pRemoteCharacteristic->registerForNotify(notifyCallback);

    connected = true;
    return true;
}

void setup() {
  Serial.begin(115200);
  print_wakeup_reason();
  pinMode(GPIO_NUM_33, INPUT_PULLUP);
  pinMode(GPIO_NUM_4, INPUT_PULLUP);
  attachInterrupt(GPIO_NUM_4, ISR, FALLING);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33,0);

  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
      Serial.println(F("SSD1306 allocation failed"));
      for(;;);
  }
  display.clearDisplay();

  Serial.println("Starting BLE Client application...");
  setSysMsg("Starting BLE Client");

   if (run_app){
    Serial.println("Starting BLE Client application...");
    setSysMsg("Starting BLE Client");
    BLEDevice::init("Tonci");
    // Display end
    // Retrieve a Scuranner and set the callback we want to use to be informed when we
    // have detected a new device.  Specify that we want active scanning and start the
    // scan to run for 5 seconds.
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new AdvertisedDeviceCallbacks());
    pBLEScan->setInterval(1349);
    pBLEScan->setWindow(449);
    pBLEScan->setActiveScan(true);
    pBLEScan->start(5, false);
    delay(10000);
  }
}



std::string old_street ;
uint8_t dir;
uint32_t dist2;
long lastUpdate = millis();

void loop() {
  Serial.println(millis() - lastUpdate);
    if((millis() - lastUpdate) > 120*1000){
      goToSleep();
    }
  
   if (run_app){
   if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
      setSysMsg("We are now connected to the BLE Server");
      delay(500);
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
      setSysMsg("We have failed to connect to the server;");
    }
    doConnect = false;
  }

  if (connected) {
     std::string value = pRemoteCharacteristic->readValue();//this crashes sometimes, recieves the whole data packet
     if (value.length() > 4) {
      Serial.println("Update...");
      lastUpdate = millis();
      
     if (value.length() > 8) {
       std::string street = value.substr(9);//this causes abort when there are not at least 9 bytes available

//       if (street != old_street){
        old_street = street;
        Serial.print(" - Street: ");
        Serial.println(street.c_str());
//       }
     }

     std::string direction = value.substr(4, 4);
     uint8_t d = direction[0];

//     if (d != dir){
      dir = d;
      Serial.print(" - Direction: ");
      Serial.println(d);
      delay(10);
//     }
     

     std::string distance = value.substr(5, 8);
     uint32_t dist = distance[0] | distance[1] << 8 | distance[2] << 16 | distance[3] << 24;
//     if (dist2 != dist){
      dist2 = dist;
      Serial.print(" - Distance: ");
      Serial.println(dist);
//     }
     
       setupDisplay();
     }else if (doScan) {
        BLEDevice::getScan()->start(0);  // this is just eample to start scan after disconnect, most likely there is better way to do it in arduino
        connected = false;
        Serial.println("onDisconnect");
        setSysMsg("Disconnect");
    }
  }
  
  delay(1000); // Delay a second between loops.
  }
}
