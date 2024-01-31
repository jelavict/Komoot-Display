#include "symbols.h"
#include "BLEDevice.h"
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define OLED_WIDTH 128
#define OLED_HEIGHT 64

#define OLED_ADDR 0x3C
int demodelay=2000;

// The remote service we wish to connect to.
static BLEUUID serviceUUID("71C1E128-D92F-4FA8-A2B2-0F171DB3436C");
// The characteristic of the remote service we are interested in.
static BLEUUID    charUUID("503DD605-9BCB-4F6E-B235-270A57483026");
const  boolean run_app = true;
static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* kDevice;

Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT);

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

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
  Serial.print("Notify callback for characteristic ");
  Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
//  Serial.print(" of data length ");
//  Serial.println(length);
//  Serial.print("- data: ");
//  
//  for (int i = 0; i < length; i++){
//    Serial.print((char)pData[i]);
//  }
//  
//  Serial.println("");
//  String str = (char*)pData;
//  Serial.println(str);
//  String new_str = str.substring(4, 4);
//  Serial.println(new_str);
}

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
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);

  
  if (run_app){
    Serial.println("Starting Arduino BLE Client application...");
    setSysMsg("Starting Arduino BLE Client");
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


void loop() {
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
    }
  }
  
  delay(1000); // Delay a second between loops.
  }
 }
