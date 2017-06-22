// Baritone: Particle Photon + TSL45315 Light Sensor
#include <spark_wiring_i2c.h>
#include <HttpClient.h>

// TSL45315 I2C address is 0x29(41)
#define ADDR 0x29

int raw_adc = 0;
int luminance;

String device_id = "8ba5f37f7745f8e450b9a9f42534eb983099854a";
String user_secret = "secret";

HttpClient http;
// Headers currently need to be set at init, useful for API keys etc.
http_header_t headers[] = {
    { "Content-Type", "application/json" },
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};

http_request_t request;
http_response_t response;

void setup() {
    // Set variable
    Particle.variable("device", "baritone");
    Particle.variable("luminance", luminance);
    
    request.ip = IPAddress(192,168,0,14);
    request.port = 3000;
    
    // Initialise I2C communication as MASTER
    Wire.begin();
    // Initialise serial communication, set baud rate = 9600
    Serial.begin(9600);
    
    // Start I2C Transmission
    Wire.beginTransmission(ADDR);
    // Select control register
    Wire.write(0x80);
    
    // Normal operation
    Wire.write(0x03);
    // Stop I2C transmission
    Wire.endTransmission();
    
    // Start I2C Transmission
    Wire.beginTransmission(ADDR);
    // Select configuration register
    Wire.write(0x81);
    
    // Multiplier 1x, Tint : 400ms
    Wire.write(0x00);
    // Stop I2C transmission
    Wire.endTransmission();
    delay(300);
}

void loop() {
    delay(1000);
    
    luminance = sense();
    
    // Output data to dashboard
    Particle.publish("Ambient Light luminance: ", String(luminance));
    
    // Send to ChoralAllegro
    request.path = "/";
    request.body = "{\"device_id\":\"" + device_id + "\",\"user_secret\":\"" + user_secret + "\",\"device_data\":{\"luminance\":" + luminance + "},\"device_timestamp\":" + String(Time.now()) + "000}";
    Particle.publish("Application>\tRequest: ", request.body);
    http.post(request, response, headers);
    Particle.publish("Application>\tResponse: ", response.status);
}

int sense() {
    unsigned int data[2];
    // Start I2C Transmission
    Wire.beginTransmission(ADDR);
    // Select data register
    Wire.write(0x84);
    // Stop I2C transmission
    Wire.endTransmission();
    
    // Request 2 bytes of data
    Wire.requestFrom(ADDR, 2);
    // Read 2 bytes of data
    // luminance lsb, luminance msb
    if(Wire.available() == 2) {
      data[0] = Wire.read();
      data[1] = Wire.read();
      delay(300);
    }
    // Convert the data
    return (data[1] * 256) + data[0];
}
