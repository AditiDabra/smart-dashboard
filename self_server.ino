/*
* VVM312 & Hall Sensor - WEB SERVER + API PUSH EDITION
*/

#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h> 
const int MACHINE_ID = 1; 

// 2. Server Configuration
// REPLACE WITH YOUR PC'S IP ADDRESS
const char* serverUrl = "http://172.26.111.251:3001/api/readings"; //<---change ip for the host system on network

// 3. WiFi Credentials
const char* ssid = "IITRPR";    //<------change wifiname and pass   
const char* password = "password"; //<---|

// 4. Simulation Toggle
const int FORCE_ZERO_VOLTAGE = 0; 

WebServer server(80); 

#define SENSOR_BAUD_RATE 9600
#define INVERT_LOGIC false
#define RXD2 16
#define TXD2 17

const float IPN = 100.0; 
const int ADC_PIN = 34;
const float R1 = 15.0; 
const float R2 = 22.0; 

const byte numChars = 64;
char receivedChars[numChars];
boolean newData = false;

volatile float latest_vvm_voltage = 0.0; 
volatile float latest_avg_current = 0.0; 

float zero_voltage_mv = 0.0;
const float SENSOR_V_AT_IPN = 5.0;
const float VOLTAGE_DIVIDER_RATIO = R2 / (R1 + R2);

unsigned long lastDataLogTime = 0;
const unsigned long dataLogInterval = 500; // Push data every 500ms
volatile double current_sum_amps = 0.0;
volatile int current_reading_count = 0;

void sendDataToAPI(float voltage, float current) {
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    // Construct JSON Payload manually
    String json = "{";
    json += "\"machineId\":" + String(MACHINE_ID) + ",";
    json += "\"voltage\":" + String(voltage, 2) + ",";
    json += "\"current\":" + String(current, 2) + ",";
    json += "\"temperature\": 0"; 
    json += "}";

    int httpResponseCode = http.POST(json);

    if(httpResponseCode > 0){
      String response = http.getString();
      // Serial.println("API Success: " + response); 
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
}

// --- WEB SERVER HANDLER (Kept for local debugging) ---
void handleRoot() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta http-equiv='refresh' content='0.5'>";
  html += "<style>body{font-family: sans-serif; text-align: center; margin-top: 50px;}";
  html += "h1{color: #333;} .data{font-size: 2.5rem; color: #007BFF;}</style></head>";
  html += "<body>";
  html += "<h1>Welding Monitor</h1>";
  html += "<p>Voltage:</p><div class='data'>" + String(latest_vvm_voltage, 2) + " V</div>";
  html += "<p>Current:</p><div class='data'>" + String(latest_avg_current, 2) + " A</div>";
  html += "<p style='font-size:12px; color:grey'>Pushing to API...</p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  
  // Connect to WiFi
  Serial.println("\nConnecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected. IP: ");
  Serial.println(WiFi.localIP()); 

  server.on("/", handleRoot);
  server.begin();

  Serial2.begin(SENSOR_BAUD_RATE, SERIAL_8N1, RXD2, TXD2, INVERT_LOGIC);
  delay(2000); 
  
  if (FORCE_ZERO_VOLTAGE == 0) {
      Serial2.print("SERIALON"); 
  }
  delay(1000);
  
  // Calibrate Hall Sensor
  Serial.println("Calibrating Hall sensor...");
  analogSetPinAttenuation(ADC_PIN, ADC_11db);
  long total_mv = 0;
  for (int i = 0; i < 500; i++) {
    total_mv += analogReadMilliVolts(ADC_PIN);
    delay(2);
  }
  zero_voltage_mv = (float)total_mv / 500.0;
  Serial.println("Calibration complete.");
}

void recvWithEndMarker() {
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
    while (Serial2.available() > 0 && newData == false) {
        rc = Serial2.read();
        if (rc == '\r') { continue; }
        if (rc != endMarker) {
            if (ndx < numChars - 1) {
                receivedChars[ndx] = rc;
                ndx++;
            }
        } else {
            receivedChars[ndx] = '\0';
            ndx = 0;
            newData = true;
        }
    }
}

void parseData() {
  String sensorData(receivedChars);
  sensorData.trim();
  if (sensorData.length() == 0) return;

  int v_index = sensorData.indexOf('V');
  int dc_index = sensorData.indexOf("DC");
  int d_index_end = sensorData.lastIndexOf('D');

  if (v_index == 0 && dc_index > 1 && d_index_end > dc_index + 2) {
    String voltageString = sensorData.substring(v_index + 1, dc_index);
    latest_vvm_voltage = voltageString.toFloat(); 
  } 
}

void loop() {
  server.handleClient();

  // VVM Sensor Logic
  if (FORCE_ZERO_VOLTAGE == 1) {
    static unsigned long lastFakeUpdate = 0;
    static unsigned long nextFakeInterval = 2000; 

    if (millis() - lastFakeUpdate > nextFakeInterval) {
        lastFakeUpdate = millis();
        if (random(0, 10) < 4) {
            latest_vvm_voltage = 0.00;
        } else {
            latest_vvm_voltage = random(1, 6) / 100.0; 
        }
        nextFakeInterval = random(1000, 4001); 
    }
    while(Serial2.available() > 0) { Serial2.read(); }
  } 
  else {
    recvWithEndMarker();
    if (newData) {
      parseData();
      newData = false;
    }
  }

  // Hall Sensor Integration
  float v_adc_mv = analogReadMilliVolts(ADC_PIN);
  float v_sensor_mv = (v_adc_mv - zero_voltage_mv) / VOLTAGE_DIVIDER_RATIO;
  if (v_sensor_mv < 0) v_sensor_mv = 0;
  
  float current_amps = ( 2 * (v_sensor_mv / 1000.0) * IPN) / SENSOR_V_AT_IPN;
  current_sum_amps += current_amps;
  current_reading_count++;

  // --- API UPDATE TIMER ---
  unsigned long currentTime = millis();
  if (currentTime - lastDataLogTime >= dataLogInterval) {
    lastDataLogTime = currentTime; 

    if (current_reading_count > 0) {
      latest_avg_current = current_sum_amps / current_reading_count;
    } else {
      latest_avg_current = 0.0;
    }

    // DEBUG PRINT
    Serial.print("API Push -> V: ");
    Serial.print(latest_vvm_voltage);
    Serial.print(" | I: ");
    Serial.println(latest_avg_current);

    // CALL THE API
    sendDataToAPI(latest_vvm_voltage, latest_avg_current);

    current_sum_amps = 0.0;
    current_reading_count = 0;
  }
}
