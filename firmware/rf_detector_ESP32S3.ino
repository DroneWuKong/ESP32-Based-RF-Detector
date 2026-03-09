#pragma GCC optimize ("Os")

/*
 * RF DETECTOR - ESP32-S3 Version
 * Hardware: ESP32-S3 N16R8 (16MB Flash, 8MB PSRAM)
 * RF Frontend: Wio-SX1262 LoRa module
 * Features: 75 RF signatures, Web UI, BLE, ESP-NOW, TAK integration
 */

#include <WiFi.h>
#include <WebServer.h>
#include <RadioLib.h>
#include <WiFiUdp.h>
#include <esp_wifi.h>
#include <esp_now.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// ============================================================================
// PIN DEFINITIONS - ESP32-S3 N16R8
// ============================================================================

// ============================================================================
// RADIO MODULE SELECTION - UNCOMMENT ONE
// ============================================================================
// #define USE_SX1262     // Wio-SX1262 module (150-960 MHz, best sensitivity)
#define USE_CC1101     // CC1101 module (300-928 MHz, Flipper Zero compatible)

#ifdef USE_SX1262
  // SX1262 LoRa Module (SPI)
  #define NSS  10           // SPI CS
  #define DIO1 11           // DIO1 interrupt
  #define NRST 12           // Reset
  #define BUSY 13           // Busy signal
  // SPI Pins: MOSI=11, MISO=13, SCK=12 (using HSPI)
#endif

#ifdef USE_CC1101
  // CC1101 Module (SPI) - Flipper Zero Compatible
  #define CC1101_CS   14    // Chip select
  #define CC1101_GDO0 15    // GDO0 interrupt
  #define CC1101_GDO2 16    // GDO2 (optional)
  // SPI Pins: MOSI=11, MISO=13, SCK=12 (using HSPI)
#endif

// Optional future expansion pins (reserved but not used)
#define NRF24_CS    21    // nRF24L01 chip select (future)
#define NRF24_CE    47    // nRF24L01 chip enable (future)
#define RX5808_RSSI 17    // RX5808 RSSI analog input (future)

// Status LED (optional - connect external LED)
#define LED_PIN     48    // Built-in LED on many S3 boards

// ============================================================================
// CONFIGURATION
// ============================================================================

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

String takIP = "";
int takPort = 8087;
bool takOn = false;
String takCall = "RF-DET-01";
float takLat = 0, takLon = 0;

bool f433 = 1, f868 = 1, f915 = 1, fGPS = 1, f24G = 1;
int nFloor[5] = {-120, -120, -120, -120, -120};
bool useNF = 0;
int nMarg = 10;
bool specMode = 0;
int rThresh = -90;
bool scan = 0;

// ============================================================================
// HARDWARE OBJECTS
// ============================================================================

#ifdef USE_SX1262
  SX1262 radio = new Module(NSS, DIO1, NRST, BUSY);
#endif

#ifdef USE_CC1101
  CC1101 radio = new Module(CC1101_CS, CC1101_GDO0, RADIOLIB_NC, CC1101_GDO2);
#endif

WebServer srv(80);
WiFiUDP udp;

#define SVC_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define RX_UUID "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define TX_UUID "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

BLEServer* pSrv = NULL;
BLECharacteristic* pTx = NULL;
bool bleOn = 0;

// Priority frequencies (Russian UAV bands + tactical)
const uint16_t pFreq[] = {868, 870, 915, 912, 918, 924, 980, 1000, 1020, 2400, 2450, 2500};

// ============================================================================
// RF SIGNATURE DATABASE - 75 SIGNATURES
// ============================================================================

struct Sig {
  const char* c, *m, *n;
  uint16_t f;
  uint8_t b, p;
};

const Sig db[] PROGMEM = {
// Tactical MANET (21)
{"Tac","DoodleLabs","MR340-410",375,70,16},{"Tac","DoodleLabs","MR900",915,26,16},{"Tac","DoodleLabs","Mini900",915,26,16},
{"Tac","DoodleLabs","HelixL+S",1975,255,16},{"Tac","DoodleLabs","MRM1-M6",2062,255,16},{"Tac","DoodleLabs","MR2.4G",2437,80,16},
{"Tac","DoodleLabs","Wearable",2437,80,10},{"Tac","DoodleLabs","C-Band",5145,255,40},{"Tac","DoodleLabs","NATO-C",5500,255,40},
{"Tac","Silvus","SC4200EP",2437,80,10},{"Tac","Silvus","SL5200",2437,80,10},{"Tac","Silvus","SM5200",2437,80,20},
{"Tac","Silvus","SC4400E",2437,80,200},{"Tac","Silvus","SC4400X",2437,80,200},{"Tac","DTC","Vanguard",2437,40,10},
{"Tac","DTC","Commander",2437,40,10},{"Tac","Persistent","MPU5",2437,80,10},{"Tac","Persistent","Wave",2437,80,10},
{"Tac","Harris","Falcon",2437,40,50},{"Tac","Rajant","ME4",2437,80,10},{"Tac","Rajant","LX5",5500,160,20},
// Telemetry (7)
{"Tel","RFD","RFD900x",915,26,10},{"Tel","RFD","RFD900+",915,26,10},{"Tel","RFD","RFD868x",868,5,5},
{"Tel","3DR","SiK",915,26,1},{"Tel","MicroHard","P900",915,26,10},{"Tel","HolyBro","SiK433",433,1,1},{"Tel","CUAV","P900",915,26,10},
// RC Systems (12)
{"RC","IRC","Ghost",2442,73,4},{"RC","IRC","GhostAtto",2442,73,1},{"RC","IRC","IronGhost",915,13,10},
{"RC","TBS","Crossfire",915,13,20},{"RC","TBS","CrossfireNano",915,13,2},{"RC","TBS","Tracer",2437,40,1},
{"RC","ELRS","900MHz",915,13,3},{"RC","ELRS","2.4GHz",2437,40,1},{"RC","FrSky","R9M",915,1,10},{"RC","FrSky","ACCST",2437,2,1},
{"RC","HOTRC","DS650",2437,40,1},{"RC","HOTRC","F06A",2437,40,1},
// DJI Drones (20)
{"Drn","DJI","MavicMini",2400,20,1},{"Drn","DJI","MavicMini2",2437,20,1},{"Drn","DJI","MavicAir",2450,20,1},
{"Drn","DJI","MavicAir2",2437,40,1},{"Drn","DJI","MavicAir2S",2437,40,1},{"Drn","DJI","Mavic2Pro",2437,40,1},
{"Drn","DJI","Mavic2Zoom",2437,40,1},{"Drn","DJI","Mavic3",2437,80,1},{"Drn","DJI","Mavic3Cine",2437,80,1},
{"Drn","DJI","PhantomPro",2437,40,1},{"Drn","DJI","Phantom4",2437,40,1},{"Drn","DJI","Phantom4Pro",2437,40,1},
{"Drn","DJI","Phantom4Adv",2437,40,1},{"Drn","DJI","InspireRC",2437,40,1},{"Drn","DJI","Inspire1",2437,40,1},
{"Drn","DJI","Inspire2",2437,40,1},{"Drn","DJI","FPV",2437,80,1},{"Drn","DJI","Mini2",2437,20,1},
{"Drn","DJI","Mini3",2437,40,1},{"Drn","DJI","Mini3Pro",2437,40,1},{"Drn","DJI","Avata",2437,80,1},
// Commercial Drones (8)
{"Drn","Parrot","Anafi",2437,20,1},{"Drn","Parrot","Bebop2",2437,20,1},{"Drn","Parrot","Disco",2437,20,1},
{"Drn","Autel","EvoII",2437,40,1},{"Drn","Autel","EvoLite",2437,40,1},{"Drn","Autel","EvoNano",2437,20,1},
{"Drn","Skydio","S2",2437,80,1},{"Drn","Skydio","X2",2437,80,1},
// Generic (7)
{"Gen","Generic","915RC",915,26,1},{"Gen","Generic","868RC",868,5,1},{"Gen","Generic","433RC",433,1,1},
{"Gen","Generic","ELRS900",915,13,3},{"Gen","Generic","ELRS2.4",2437,40,1},{"Gen","Generic","Crossfire",915,13,20},{"Gen","Generic","Ghost",2442,73,4},
// Russian UAVs (5)
{"Tac","Zala","Lancet",868,5,20},{"Tac","Zala","Lancet",915,13,20},{"Tac","Supercam","S350",900,26,15},
{"Tac","Orlan","10",915,26,20},{"Tac","Orlan","10-new",980,40,20}
};

const int nSig = sizeof(db) / sizeof(Sig);

// ============================================================================
// DETECTION STORAGE
// ============================================================================

struct Det {
  char m[15], n[20];
  uint16_t f;
  int8_t r;
  uint8_t conf;
  uint32_t t;
  bool tac;
  uint16_t brg;
};

Det det[50];
int nDet = 0;

// ============================================================================
// DIRECTION FINDING
// ============================================================================

struct BD {
  int ang, r9, r24;
  uint32_t t;
};

BD brg[36];
int cBrg = 0;

void updBrg(int a) {
  int i = a / 10;
  if (i >= 36) i = 35;
  brg[i].ang = a;
  brg[i].r9 = radio.getRSSI();
  brg[i].r24 = WiFi.status() == WL_CONNECTED ? WiFi.RSSI() : -120;
  brg[i].t = millis();
  
  int mR = -120, pA = 0;
  for (int j = 0; j < 36; j++) {
    if (brg[j].r9 > mR) {
      mR = brg[j].r9;
      pA = brg[j].ang;
    }
  }
  cBrg = pA;
}

void rstBrg() {
  for (int i = 0; i < 36; i++) {
    brg[i].r9 = -120;
    brg[i].r24 = -120;
    brg[i].t = 0;
  }
  cBrg = 0;
}

// ============================================================================
// ESP-NOW MESH NETWORKING
// ============================================================================

typedef struct {
  char id[16], mk[15], md[20];
  uint16_t f;
  int8_t r;
  uint16_t b;
  uint32_t ts;
} ENPkt;

uint8_t bAddr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
bool enOn = 0;

void onSent(const wifi_tx_info_t* m, esp_now_send_status_t s) {}

void onRecv(const esp_now_recv_info* r, const uint8_t* d, int l) {
  ENPkt p;
  memcpy(&p, d, sizeof(p));
  Serial.println("MESH RX: " + String(p.mk) + " " + String(p.md));
}

void sendEN(int i) {
  if (!enOn || i >= nDet) return;
  ENPkt p;
  strncpy(p.id, "RFDET", 15);
  strncpy(p.mk, det[i].m, 14);
  strncpy(p.md, det[i].n, 19);
  p.f = det[i].f;
  p.r = det[i].r;
  p.b = det[i].brg;
  p.ts = millis();
  esp_now_send(bAddr, (uint8_t*)&p, sizeof(p));
}

// ============================================================================
// BLE CONTROL INTERFACE
// ============================================================================

class SC : public BLEServerCallbacks {
  void onConnect(BLEServer* p) { 
    bleOn = 1;
    digitalWrite(LED_PIN, HIGH);
  }
  void onDisconnect(BLEServer* p) {
    bleOn = 0;
    digitalWrite(LED_PIN, LOW);
    p->startAdvertising();
  }
};

class CC : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* p) {
    String r = p->getValue().c_str();
    if (r.length() > 0) {
      r.trim();
      String res = "";
      if (r == "STATUS") res = "Det:" + String(nDet) + "\n";
      else if (r == "LIST") {
        for (int i = 0; i < nDet && i < 10; i++) {
          res += String(det[i].m) + " " + String(det[i].n) + "\n";
        }
      }
      else if (r == "START") { scan = 1; res = "ON\n"; }
      else if (r == "STOP") { scan = 0; res = "OFF\n"; }
      else if (r == "CLEAR") { nDet = 0; res = "CLR\n"; }
      else if (r.startsWith("THRESH ")) { rThresh = r.substring(7).toInt(); res = "OK\n"; }
      else res = "?\n";
      if (res.length() > 0 && pTx) {
        pTx->setValue(res.c_str());
        pTx->notify();
      }
    }
  }
};

// ============================================================================
// TAK SERVER INTEGRATION
// ============================================================================

void sendTAK(String u, String t, String c, float la, float lo, String re) {
  if (!takOn || takIP.length() == 0) return;
  String cot = "<?xml version=\"1.0\"?><event version=\"2.0\" uid=\"" + u + "\" type=\"" + t + "\" time=\"2024-01-01T00:00:00Z\" start=\"2024-01-01T00:00:00Z\" stale=\"2024-01-01T00:05:00Z\" how=\"m-g\">";
  cot += "<point lat=\"" + String(la, 6) + "\" lon=\"" + String(lo, 6) + "\" hae=\"0\" ce=\"10\" le=\"10\"/><detail><contact callsign=\"" + c + "\"/><remarks>" + re + "</remarks></detail></event>";
  udp.beginPacket(takIP.c_str(), takPort);
  udp.write((const uint8_t*)cot.c_str(), cot.length());
  udp.endPacket();
}

// ============================================================================
// DETECTION MANAGEMENT
// ============================================================================

void addDet(int i, uint16_t f, int8_t r, bool tac, uint8_t conf) {
  uint32_t now = millis();
  
  // Check for duplicates
  for (int j = 0; j < nDet; j++) {
    if (strcmp(det[j].m, db[i].m) == 0 && strcmp(det[j].n, db[i].n) == 0) {
      if (now - det[j].t < 30000) {
        det[j].r = r;
        det[j].conf = conf;
        det[j].t = now;
        det[j].brg = cBrg;
        return;
      }
    }
  }
  
  // Add new detection
  if (nDet < 50) {
    strncpy(det[nDet].m, db[i].m, 14);
    strncpy(det[nDet].n, db[i].n, 19);
    det[nDet].f = f;
    det[nDet].r = r;
    det[nDet].conf = conf;
    det[nDet].t = now;
    det[nDet].tac = tac;
    det[nDet].brg = cBrg;
    
    String msg = "[+]" + String(db[i].c) + " " + String(db[i].m) + " " + String(db[i].n) + "@" + String(f) + "MHz," + String(r) + "dBm";
    Serial.println(msg);
    
    if (bleOn && pTx) {
      pTx->setValue((msg + "\n").c_str());
      pTx->notify();
    }
    
    sendEN(nDet);
    
    if (takOn) {
      String uid = "RF-" + String(db[i].m) + "-" + String(f);
      sendTAK(uid, tac ? "a-h-G-E-V-A" : "a-u-G", String(db[i].m) + " " + String(db[i].n), takLat, takLon, String(f) + "MHz@" + String(r) + "dBm");
    }
    
    nDet++;
  }
}

// ============================================================================
// RF SCANNING CORE
// ============================================================================

void scanRF() {
  static uint16_t fi = 0;
  static bool pd = 0;
  
  // Priority scan phase
  if (!pd && fi < 12) {
    uint16_t freq = pFreq[fi];
    if (freq <= 960 && freq >= 150) {
      radio.setFrequency(freq);
      
      #ifdef USE_SX1262
        radio.setBandwidth(40);
        radio.setSpreadingFactor(7);
      #endif
      
      #ifdef USE_CC1101
        radio.setRxBandwidth(812.5);  // CC1101 max bandwidth
      #endif
      
      radio.startReceive();
      delay(50);
      int rssi = radio.getRSSI();
      radio.standby();
      
      for (int i = 0; i < nSig; i++) {
        if (abs((int)db[i].f - (int)freq) < 5 && rssi > rThresh) {
          bool isTac = (strcmp(db[i].c, "Tac") == 0 || strcmp(db[i].c, "Tel") == 0);
          addDet(i, freq, rssi, isTac, 85);
        }
      }
    }
    fi++;
    if (fi >= 12) {
      pd = 1;
      fi = 0;
    }
    return;
  }
  
  // Full signature scan
  for (int i = 0; i < nSig; i++) {
    uint16_t f = db[i].f;
    if (f >= 400 && f <= 450 && !f433) continue;
    if (f >= 850 && f <= 900 && !f868) continue;
    if (f >= 900 && f <= 930 && !f915) continue;
    if (f >= 1560 && f <= 1590 && !fGPS) continue;
    if (f >= 2400 && f <= 2500 && !f24G) continue;
    
    #ifdef USE_SX1262
      if (f > 960 || f < 150) continue;
    #endif
    
    #ifdef USE_CC1101
      if (f > 928 || f < 300) continue;  // CC1101 range: 300-928 MHz
    #endif
    
    if (f >= 930 && f <= 960) continue;  // LTE filter
    
    radio.setFrequency(f);
    
    #ifdef USE_SX1262
      radio.setBandwidth(db[i].b);
      radio.setSpreadingFactor(7);
    #endif
    
    #ifdef USE_CC1101
      // CC1101 bandwidth selection based on signature
      float bw = 812.5;  // Default max bandwidth
      if (db[i].b <= 58) bw = 58.0;
      else if (db[i].b <= 68) bw = 68.0;
      else if (db[i].b <= 81) bw = 81.0;
      else if (db[i].b <= 102) bw = 102.0;
      else if (db[i].b <= 116) bw = 116.0;
      else if (db[i].b <= 135) bw = 135.0;
      else if (db[i].b <= 162) bw = 162.0;
      else if (db[i].b <= 203) bw = 203.0;
      else if (db[i].b <= 232) bw = 232.0;
      else if (db[i].b <= 270) bw = 270.0;
      else if (db[i].b <= 325) bw = 325.0;
      else if (db[i].b <= 406) bw = 406.0;
      else if (db[i].b <= 464) bw = 464.0;
      else if (db[i].b <= 541) bw = 541.0;
      else if (db[i].b <= 650) bw = 650.0;
      else bw = 812.5;
      radio.setRxBandwidth(bw);
    #endif
    
    radio.startReceive();
    delay(50);
    int rssi = radio.getRSSI();
    radio.standby();
    
    if (rssi > rThresh) {
      int bi = 0;
      if (f >= 850 && f <= 900) bi = 1;
      else if (f >= 900 && f <= 930) bi = 2;
      else if (f >= 1560 && f <= 1590) bi = 3;
      
      if (useNF && rssi < nFloor[bi] + nMarg) continue;
      bool isTac = (strcmp(db[i].c, "Tac") == 0 || strcmp(db[i].c, "Tel") == 0);
      addDet(i, f, rssi, isTac, 80);
    }
  }
}

// ============================================================================
// WIFI SCANNING
// ============================================================================

void scanWiFi() {
  esp_wifi_set_promiscuous(specMode);
  WiFi.mode(WIFI_STA);
  int n = WiFi.scanNetworks(false, true, false, 120);
  
  for (int i = 0; i < n; i++) {
    String s = WiFi.SSID(i);
    int rssi = WiFi.RSSI(i);
    uint8_t ch = WiFi.channel(i);
    uint16_t freq = 2407 + (ch * 5);
    
    if (!f24G) continue;
    if (rssi < rThresh) continue;
    
    int bi = 4;
    if (useNF && rssi < nFloor[bi] + nMarg) continue;
    
    for (int j = 0; j < nSig; j++) {
      if (db[j].f < 2400 || db[j].f > 2500) continue;
      
      bool isTac = (strcmp(db[j].c, "Tac") == 0 || strcmp(db[j].c, "Tel") == 0);
      
      if (strcmp(db[j].m, "DJI") == 0) {
        if (s.indexOf("DJI") >= 0 || s.indexOf("Mavic") >= 0 || s.indexOf("Phantom") >= 0 || 
            s.indexOf("Inspire") >= 0 || s.indexOf("Mini") >= 0 || s.indexOf("Air") >= 0 || 
            s.indexOf("Avata") >= 0 || s.indexOf("FPV") >= 0)
          addDet(j, freq, rssi, 0, 90);
      }
      else if (strcmp(db[j].m, "Parrot") == 0) {
        if (s.indexOf("Parrot") >= 0 || s.indexOf("Anafi") >= 0 || s.indexOf("Bebop") >= 0 || s.indexOf("Disco") >= 0)
          addDet(j, freq, rssi, 0, 85);
      }
      else if (strcmp(db[j].m, "Autel") == 0) {
        if (s.indexOf("Autel") >= 0 || s.indexOf("Evo") >= 0)
          addDet(j, freq, rssi, 0, 85);
      }
      else if (strcmp(db[j].m, "Skydio") == 0) {
        if (s.indexOf("Skydio") >= 0 || s.indexOf("S2") >= 0 || s.indexOf("X2") >= 0)
          addDet(j, freq, rssi, 0, 85);
      }
      else if (strcmp(db[j].m, "IRC") == 0) {
        if (s.indexOf("Ghost") >= 0 || s.indexOf("IRC") >= 0)
          addDet(j, freq, rssi, 0, 90);
      }
      else if (strcmp(db[j].m, "TBS") == 0) {
        if (s.indexOf("TBS") >= 0 || s.indexOf("Crossfire") >= 0 || s.indexOf("Tracer") >= 0)
          addDet(j, freq, rssi, 0, 90);
      }
      else if (strcmp(db[j].m, "ELRS") == 0) {
        if (s.indexOf("ELRS") >= 0 || s.indexOf("ExpressLRS") >= 0)
          addDet(j, freq, rssi, 0, 90);
      }
      else if (strcmp(db[j].m, "FrSky") == 0) {
        if (s.indexOf("FrSky") >= 0 || s.indexOf("ACCST") >= 0)
          addDet(j, freq, rssi, 0, 85);
      }
      else if (strcmp(db[j].m, "HOTRC") == 0) {
        if (s.indexOf("HOTRC") >= 0 || s.indexOf("DS650") >= 0 || s.indexOf("F06A") >= 0)
          addDet(j, freq, rssi, 0, 90);
      }
      else {
        addDet(j, freq, rssi, isTac, 75);
      }
    }
  }
  
  WiFi.scanDelete();
  WiFi.mode(WIFI_AP_STA);
  esp_wifi_set_promiscuous(0);
}

// ============================================================================
// WEB INTERFACE - Compact HTML
// ============================================================================

const char H[] PROGMEM = R"HTMLDELIM(<!DOCTYPE html><html><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>RF Detector</title><style>*{margin:0;padding:0;box-sizing:border-box}body{background:#000;color:#0f0;font-family:monospace;padding:10px;font-size:12px}.hdr{text-align:center;padding:15px;border:2px solid #0f0;margin-bottom:15px;background:rgba(0,20,0,0.8)}.hdr h1{font-size:24px;text-shadow:0 0 10px #0f0}.stats{display:grid;grid-template-columns:repeat(4,1fr);gap:8px;margin:10px 0}.stat{padding:8px;border:1px solid #0f0;background:rgba(0,20,0,0.6);text-align:center}.stat-l{font-size:10px;color:#0a0}.stat-v{font-size:16px;font-weight:bold}button{background:#003300;color:#0f0;border:2px solid #0f0;padding:8px 16px;margin:4px;cursor:pointer;font-family:inherit;font-size:12px}button:hover{background:#0f0;color:#000}input[type="range"]{width:200px;background:#002200;border:1px solid #0f0}input[type="text"],input[type="number"]{background:rgba(0,20,0,0.6);color:#0f0;border:1px solid #0a0;padding:6px;font-family:inherit;font-size:11px}input[type="checkbox"]{accent-color:#0f0;margin-right:5px}label{margin:0 8px;cursor:pointer}.panel{border:1px solid #0a0;padding:10px;margin:10px 0;background:rgba(0,15,0,0.6)}.panel-t{font-size:13px;font-weight:bold;margin-bottom:8px;color:#0ff}.ctrl{text-align:center;padding:10px}.thresh{display:flex;align-items:center;justify-content:center;gap:10px;margin:10px 0;padding:8px;background:rgba(0,20,0,0.4)}.flt{display:flex;flex-wrap:wrap;gap:6px;justify-content:center;padding:8px}#spec{border:1px solid #0a0;width:100%;height:120px;position:relative;margin:8px 0;background:rgba(0,10,0,0.4)}#dets{max-height:250px;overflow-y:auto;font-size:12px;line-height:1.6}#dets div{padding:6px;border-left:3px solid #0f0;margin:4px 0;background:rgba(0,20,0,0.5)}#wf{border:1px solid #0a0;background:#000}#log{height:120px;overflow-y:auto;font-size:11px;line-height:1.4;background:rgba(0,8,0,0.6);padding:4px}#log div{padding:3px;border-bottom:1px solid rgba(0,50,0,0.3)}.scan-ind{display:inline-block;width:12px;height:12px;border-radius:50%;margin-left:8px}.scan-act{background:#0f0;animation:p 1s ease-in-out infinite}@keyframes p{0%,100%{opacity:0.4}50%{opacity:1}}.comp{width:180px;height:180px;margin:15px auto;position:relative;border:2px solid #0f0;border-radius:50%;background:radial-gradient(circle,rgba(0,30,0,0.8),rgba(0,10,0,0.4))}.comp-n{position:absolute;top:50%;left:50%;width:3px;height:70px;background:linear-gradient(to top,#f00,#ff0);transform-origin:bottom center;transform:translate(-50%,-100%) rotate(0deg);transition:transform 0.5s}.comp-c{position:absolute;top:50%;left:50%;width:16px;height:16px;background:#0f0;border-radius:50%;transform:translate(-50%,-50%)}.comp-l{position:absolute;font-size:12px;font-weight:bold;color:#0ff}.n{top:8px;left:50%;transform:translateX(-50%)}.e{top:50%;right:8px;transform:translateY(-50%)}.s{bottom:8px;left:50%;transform:translateX(-50%)}.w{top:50%;left:8px;transform:translateY(-50%)}</style></head><body><div class="hdr"><h1>RF DETECTOR</h1><div class="stats"><div class="stat"><div class="stat-l">STATUS</div><div class="stat-v" id="st">OFF</div></div><div class="stat"><div class="stat-l">TOTAL</div><div class="stat-v" id="tot">0</div></div><div class="stat"><div class="stat-l">TAC</div><div class="stat-v" id="tac">0</div></div><div class="stat"><div class="stat-l">DRN</div><div class="stat-v" id="drn">0</div></div></div></div><div class="panel"><div class="panel-t">SPECTRUM</div><div id="spec"></div></div><div class="ctrl"><button onclick="start()">START</button><button onclick="stop()">STOP</button><button onclick="clr()">CLEAR</button><span id="sci" class="scan-ind" style="background:#333"></span></div><div class="thresh"><label>THRESH:</label><input type="range" id="th" min="-110" max="-40" value="-90" step="5" oninput="setTh()"><strong><span id="thv">-90</span>dBm</strong></div><div class="panel"><div class="panel-t">FILTERS</div><div class="flt"><label><input type="checkbox" id="f433" checked onchange="setFlt()">433</label><label><input type="checkbox" id="f868" checked onchange="setFlt()">868</label><label><input type="checkbox" id="f915" checked onchange="setFlt()">915</label><label><input type="checkbox" id="fGPS" checked onchange="setFlt()">GPS</label><label><input type="checkbox" id="f24G" checked onchange="setFlt()">2.4G</label></div></div><div class="panel"><div class="panel-t">MODE</div><div style="text-align:center"><label><input type="radio" name="md" value="sig" checked onchange="setMd()">Signature</label><label><input type="radio" name="md" value="spec" onchange="setMd()">Spectrum</label></div></div><div class="panel" id="nfo"><div class="panel-t">NOISE FILTER</div><div style="padding:8px"><label><input type="checkbox" id="nfn" onchange="setNf()">Enable (<input type="number" id="nfm" value="10" min="5" max="30" style="width:50px" onchange="setNf()">dBm)</label><button onclick="calNf()" style="margin-left:10px">CALIB</button><span id="nfs" style="margin-left:10px;color:#fa0"></span></div></div><div class="panel"><div class="panel-t">DIRECTION</div><div class="comp"><div class="comp-l n">N</div><div class="comp-l e">E</div><div class="comp-l s">S</div><div class="comp-l w">W</div><div class="comp-n" id="ndl"></div><div class="comp-c"></div></div><div style="text-align:center;margin-top:8px"><div style="font-size:28px;font-weight:bold"><span id="bdisp">---</span>deg</div><div style="margin:8px 0"><label>Rotation:<input type="number" id="uang" min="0" max="359" value="0" style="width:60px" onchange="setBrg(this.value)">deg</label><button onclick="rstBrg()">RESET</button></div><div style="font-size:10px;color:#0a0;margin-top:8px">Rotate detector 360deg while scanning</div></div></div><div class="panel"><div class="panel-t">DETECTIONS</div><div id="dets">NO SIGNALS</div></div><div class="panel"><div class="panel-t">WATERFALL</div><canvas id="wf" width="500" height="100" style="width:100%;height:100px"></canvas></div><div class="panel"><div class="panel-t">LOG</div><div id="log"></div></div><div class="panel"><div class="panel-t">TAK</div><div style="display:grid;gap:8px"><div>IP:<input type="text" id="tip" placeholder="IP" size="12"> Port:<input type="number" id="tpt" value="8087" style="width:70px"></div><div>Call:<input type="text" id="tcl" value="RF-DET-01" size="10"> Lat:<input type="number" id="tla" value="0" step="0.000001" style="width:90px"> Lon:<input type="number" id="tlo" value="0" step="0.000001" style="width:90px"></div><div><button onclick="setTAK()">ENABLE</button><button onclick="disTAK()">DISABLE</button><span id="tst" style="margin-left:10px;font-weight:bold">OFF</span></div></div></div><script>let iv;const fB=[{f:433,l:'433'},{f:868,l:'868'},{f:915,l:'915'},{f:1575,l:'GPS'},{f:2437,l:'2.4G'}];let sD={433:0,868:0,915:0,1575:0,2437:0};let wD=[];function start(){fetch('/start');document.getElementById('st').textContent='SCAN';document.getElementById('sci').className='scan-ind scan-act';iv=setInterval(upd,1500);upd();addLog('STARTED');}function stop(){fetch('/stop');clearInterval(iv);document.getElementById('st').textContent='STOP';document.getElementById('sci').className='scan-ind';document.getElementById('sci').style.background='#333';addLog('STOPPED');}function clr(){fetch('/clear');document.getElementById('dets').innerHTML='NO SIGNALS';document.getElementById('tot').textContent='0';document.getElementById('tac').textContent='0';document.getElementById('drn').textContent='0';addLog('CLEARED');}function setTh(){const v=document.getElementById('th').value;document.getElementById('thv').textContent=v;fetch('/thresh?v='+v);}function setFlt(){const f4=document.getElementById('f433').checked?1:0;const f8=document.getElementById('f868').checked?1:0;const f9=document.getElementById('f915').checked?1:0;const fg=document.getElementById('fGPS').checked?1:0;const f2=document.getElementById('f24G').checked?1:0;fetch('/filter?f433='+f4+'&f868='+f8+'&f915='+f9+'&fGPS='+fg+'&f24G='+f2);}function setMd(){const m=document.querySelector('input[name="md"]:checked').value;fetch('/mode?v='+m);document.getElementById('nfo').style.display=m==='sig'?'block':'none';}function setNf(){const e=document.getElementById('nfn').checked?1:0;const m=document.getElementById('nfm').value;fetch('/noise?enabled='+e+'&margin='+m);}function calNf(){document.getElementById('nfs').textContent='Calibrating...';fetch('/calib').then(()=>{document.getElementById('nfs').textContent='OK';addLog('CALIBRATED');setTimeout(()=>{document.getElementById('nfs').textContent='';},3000);});}function setBrg(a){fetch('/bearing?angle='+a);}function rstBrg(){fetch('/bearing?reset=1').then(()=>{document.getElementById('bdisp').textContent='---';document.getElementById('ndl').style.transform='translate(-50%,-100%) rotate(0deg)';addLog('BEARING RESET');});}function setTAK(){const i=document.getElementById('tip').value;const p=document.getElementById('tpt').value;const c=document.getElementById('tcl').value;const la=document.getElementById('tla').value;const lo=document.getElementById('tlo').value;fetch('/tak?ip='+i+'&port='+p+'&call='+c+'&lat='+la+'&lon='+lo).then(()=>{document.getElementById('tst').textContent='ON';document.getElementById('tst').style.color='#0f0';addLog('TAK ON');});}function disTAK(){fetch('/tak?ip=').then(()=>{document.getElementById('tst').textContent='OFF';document.getElementById('tst').style.color='#f00';addLog('TAK OFF');});}async function upd(){try{const d=await(await fetch('/data')).json();document.getElementById('tot').textContent=d.length;let tc=0,dc=0;sD={433:0,868:0,915:0,1575:0,2437:0};let h='';for(let i=0;i<d.length;i++){if(d[i].tac)tc++;else dc++;const f=d[i].f;const r=d[i].r;const p=Math.min(100,Math.max(0,((r+120)/70)*100));if(f>=400&&f<=450)sD[433]=Math.max(sD[433],p);else if(f>=850&&f<=900)sD[868]=Math.max(sD[868],p);else if(f>=900&&f<=930)sD[915]=Math.max(sD[915],p);else if(f>=1560&&f<=1590)sD[1575]=Math.max(sD[1575],p);else if(f>=2400&&f<=2500)sD[2437]=Math.max(sD[2437],p);const b=d[i].brg||'---';h+='<div>['+(d[i].tac?'<span style="color:#f00">T</span>':'<span style="color:#fa0">D</span>')+'] '+d[i].m+' '+d[i].n+' | '+d[i].f+'MHz @ '+d[i].r+'dBm | '+b+'deg</div>';}document.getElementById('tac').textContent=tc;document.getElementById('drn').textContent=dc;if(h==='')h='NO SIGNALS';document.getElementById('dets').innerHTML=h;drawSpec();wD.unshift(Object.assign({},sD));if(wD.length>20)wD.pop();drawWF();const br=await(await fetch('/bearing')).text();if(br&&br!=='---'){const b=parseInt(br);document.getElementById('bdisp').textContent=b;document.getElementById('ndl').style.transform='translate(-50%,-100%) rotate('+b+'deg)';}}catch(e){console.error(e)}}function drawSpec(){const sp=document.getElementById('spec');sp.innerHTML='';fB.forEach((b,i)=>{const lb=document.createElement('div');lb.style.position='absolute';lb.style.bottom='2px';lb.style.left=(i*20)+'%';lb.style.fontSize='9px';lb.style.color='#0f0';lb.style.fontWeight='bold';lb.textContent=b.l;sp.appendChild(lb);const s=sD[b.f]||0;if(s>0){const br=document.createElement('div');br.style.position='absolute';br.style.bottom='0';br.style.left=(i*20)+'%';br.style.height=Math.min(100,s)+'%';br.style.width='18%';br.style.opacity='0.6';br.style.background=s>70?'#f00':s>40?'#fa0':'#0f0';sp.appendChild(br);}});}function drawWF(){const c=document.getElementById('wf');const x=c.getContext('2d');const w=c.width;const h=c.height;x.clearRect(0,0,w,h);if(wD.length===0)return;const rH=h/20;for(let r=0;r<wD.length;r++){const dt=wD[r];const y=r*rH;for(let i=0;i<fB.length;i++){const xx=(i/fB.length)*w;const bW=w/fB.length;const v=dt[fB[i].f]||0;x.fillStyle=v>80?'#f00':v>60?'#f60':v>40?'#fa0':v>20?'#8f0':v>5?'#0f0':'#011';x.fillRect(xx,y,bW,rH+1);}}}function addLog(m){const lg=document.getElementById('log');const d=new Date();const ts=d.getHours().toString().padStart(2,'0')+':'+d.getMinutes().toString().padStart(2,'0')+':'+d.getSeconds().toString().padStart(2,'0');lg.innerHTML='<div>['+ts+'] '+m+'</div>'+lg.innerHTML;const dv=lg.querySelectorAll('div');if(dv.length>50){for(let i=50;i<dv.length;i++)dv[i].remove();}}</script></body></html>)HTMLDELIM";

// ============================================================================
// SETUP
// ============================================================================

void setup() {
  // ESP32-S3 USB CDC fix
  Serial.begin(115200);
  while (!Serial && millis() < 5000) {
    delay(10);  // Wait for serial connection (max 5 seconds)
  }
  delay(1000);
  
  Serial.println("\n\n========================================");
  Serial.println("RF DETECTOR - ESP32-S3 Edition");
  Serial.println("========================================");
  Serial.println("Flash: 16MB | PSRAM: 8MB");
  Serial.println("Signatures: 75");
  Serial.println("========================================\n");
  
  // LED setup
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  // SPI for Radio Module
  SPI.begin(12, 13, 11, -1);  // SCK, MISO, MOSI, SS (SS handled by module)
  
  #ifdef USE_SX1262
    Serial.print("SX1262 Init...");
    if (radio.begin(915.0) == RADIOLIB_ERR_NONE) {
      Serial.println("OK");
      radio.setSpreadingFactor(7);
      radio.setBandwidth(125.0);
    } else {
      Serial.println("FAIL - Check wiring!");
    }
  #endif
  
  #ifdef USE_CC1101
    Serial.print("CC1101 Init...");
    if (radio.begin(915.0) == RADIOLIB_ERR_NONE) {
      Serial.println("OK");
      radio.setRxBandwidth(812.5);  // CC1101 bandwidth
      radio.disableAddressFiltering();
      radio.disableSyncWordFiltering(false);
    } else {
      Serial.println("FAIL - Check wiring!");
    }
  #endif
  
  // WiFi Setup
  Serial.print("WiFi...");
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
  int att = 0;
  while (WiFi.status() != WL_CONNECTED && att < 20) {
    delay(500);
    Serial.print(".");
    att++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nAP Mode");
    WiFi.softAP("RF_Detector", "rf123456");
    Serial.print("AP IP: ");
    Serial.println(WiFi.softAPIP());
  }
  
  // ESP-NOW Setup
  Serial.print("ESP-NOW...");
  if (esp_now_init() == ESP_OK) {
    Serial.println("OK");
    esp_now_register_send_cb(onSent);
    esp_now_register_recv_cb(onRecv);
    
    // Get WiFi channel for ESP-NOW peer
    uint8_t channel = WiFi.channel();
    if (channel == 0) channel = 1;  // Default to channel 1 if not connected
    
    esp_now_peer_info_t pi;
    memset(&pi, 0, sizeof(pi));  // Clear structure
    memcpy(pi.peer_addr, bAddr, 6);
    pi.channel = channel;
    pi.ifidx = WIFI_IF_AP;  // Use AP interface
    pi.encrypt = false;
    
    if (esp_now_add_peer(&pi) == ESP_OK) {
      enOn = 1;
      Serial.println("Mesh Ready");
    } else {
      Serial.println("Mesh peer failed (normal if not using mesh)");
    }
  } else {
    Serial.println("FAIL");
  }
  
  // BLE Setup
  Serial.print("BLE...");
  BLEDevice::init("RF_Detector");
  pSrv = BLEDevice::createServer();
  pSrv->setCallbacks(new SC());
  BLEService* pSvc = pSrv->createService(SVC_UUID);
  pTx = pSvc->createCharacteristic(TX_UUID, BLECharacteristic::PROPERTY_NOTIFY);
  pTx->addDescriptor(new BLE2902());
  BLECharacteristic* pRx = pSvc->createCharacteristic(RX_UUID, BLECharacteristic::PROPERTY_WRITE);
  pRx->setCallbacks(new CC());
  pSvc->start();
  pSrv->getAdvertising()->start();
  Serial.println("OK");
  
  // Web Server Routes
  srv.on("/", []() { srv.send_P(200, "text/html", H); });
  srv.on("/start", []() { scan = 1; srv.send(200, "text/plain", "OK"); });
  srv.on("/stop", []() { scan = 0; srv.send(200, "text/plain", "OK"); });
  srv.on("/clear", []() { nDet = 0; srv.send(200, "text/plain", "OK"); });
  
  srv.on("/data", []() {
    String j = "[";
    int c = 0;
    for (int i = 0; i < nDet; i++) {
      if (c > 0) j += ",";
      j += "{\"m\":\"" + String(det[i].m) + "\",\"n\":\"" + String(det[i].n) + "\",\"f\":" + String(det[i].f) + ",\"r\":" + String(det[i].r) + ",\"conf\":" + String(det[i].conf) + ",\"tac\":" + (det[i].tac ? "true" : "false") + ",\"brg\":" + String(det[i].brg) + "}";
      c++;
    }
    j += "]";
    srv.send(200, "application/json", j);
  });
  
  srv.on("/thresh", []() { if (srv.hasArg("v")) rThresh = srv.arg("v").toInt(); srv.send(200, "text/plain", "OK"); });
  srv.on("/filter", []() { f433 = srv.arg("f433") == "1"; f868 = srv.arg("f868") == "1"; f915 = srv.arg("f915") == "1"; fGPS = srv.arg("fGPS") == "1"; f24G = srv.arg("f24G") == "1"; srv.send(200, "text/plain", "OK"); });
  srv.on("/tak", []() { takIP = srv.arg("ip"); if (takIP.length() > 0) { takPort = srv.arg("port").toInt(); takCall = srv.arg("call"); takLat = srv.arg("lat").toFloat(); takLon = srv.arg("lon").toFloat(); takOn = 1; } else { takOn = 0; } srv.send(200, "text/plain", "OK"); });
  srv.on("/mode", []() { String m = srv.arg("v"); specMode = (m == "spec"); srv.send(200, "text/plain", "OK"); });
  srv.on("/noise", []() { useNF = srv.arg("enabled") == "1"; nMarg = srv.arg("margin").toInt(); srv.send(200, "text/plain", "OK"); });
  
  srv.on("/calib", []() {
    for (int b = 0; b < 5; b++) nFloor[b] = -120;
    uint16_t tF[] = {433, 868, 915, 1575, 2437};
    for (int i = 0; i < 4; i++) {
      if (tF[i] <= 960) {
        radio.setFrequency(tF[i]);
        radio.setBandwidth(40);
        radio.startReceive();
        delay(100);
        nFloor[i] = radio.getRSSI();
        radio.standby();
      }
    }
    WiFi.mode(WIFI_STA);
    int n = WiFi.scanNetworks();
    int minR = 0;
    for (int i = 0; i < n; i++) {
      int r = WiFi.RSSI(i);
      if (r < minR) minR = r;
    }
    nFloor[4] = minR;
    WiFi.scanDelete();
    WiFi.mode(WIFI_AP_STA);
    srv.send(200, "text/plain", "OK");
  });
  
  srv.on("/bearing", []() {
    if (srv.hasArg("reset")) {
      rstBrg();
      srv.send(200, "text/plain", "---");
    } else if (srv.hasArg("angle")) {
      int a = srv.arg("angle").toInt();
      updBrg(a);
      srv.send(200, "text/plain", String(cBrg));
    } else {
      srv.send(200, "text/plain", String(cBrg));
    }
  });
  
  srv.begin();
  
  Serial.println("\n========================================");
  Serial.println("SYSTEM READY");
  Serial.println("========================================");
  Serial.print("Web UI: http://");
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(WiFi.localIP());
  } else {
    Serial.println(WiFi.softAPIP());
  }
  Serial.println("BLE: RF_Detector");
  Serial.println("========================================\n");
}

// ============================================================================
// MAIN LOOP
// ============================================================================

unsigned long lastRF = 0, lastW = 0;

void loop() {
  srv.handleClient();
  
  if (scan) {
    uint32_t now = millis();
    
    if (now - lastRF > 150) {
      scanRF();
      lastRF = now;
    }
    
    if (now - lastW > 5000) {
      scanWiFi();
      lastW = now;
    }
  }
}
