// Alto: Particle Photon + ADC121C021_PIZO Vibration Sensor
#include <spark_wiring_i2c.h>
#include <HttpClient.h>

#define ADDR 0x50

int raw_adc = 0;
int knocks = 0;
unsigned long knocksTimeout = 1000;
bool knocking = false;
unsigned long knocksStart = 0;
unsigned long disableStart = 0;
unsigned long disableTimeout = 200;
int peakArray[4] ={0,0,0,0};
int peakArrayIndex = 0;
int threshold = 12;

String device_id = "3df97c177d96280618348dbc7723d33870034e4e";
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
    Particle.variable("device", "alto");
    Particle.variable("knocks", knocks);
    
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
    if (millis() > knocksStart + disableTimeout) {
        peakArray[peakArrayIndex] = sense();
        int total = 0;
        for (int i = 0; i < 4; i++) {
            total += peakArray[i];
        }
        if (total >= threshold) {
            knocking = true;
            knocks++;
            knocksStart = millis();
            for (int i = 0; i < 4; i++) {
                peakArray[i] = 0;
            }
        }
        if (peakArrayIndex == 3) {
            peakArrayIndex = 0;
            
        } else {
            peakArrayIndex++;
        }
    }

    if (knocking && millis() >= knocksStart + knocksTimeout) {
        //Publish event and print to serial console
        char buffer[50];
        sprintf(buffer, "%i-knocks", knocks);
        Particle.publish("knocks", buffer);
        Serial.printf("Number of Knocks: %i \n", knocks);

        // Send to ChoralAllegro
        request.path = "/";
        request.body = "{\"device_id\":\"" + device_id + "\",\"user_secret\":\"" + user_secret + "\",\"device_data\":{\"knocks\":" + knocks + "},\"device_timestamp\":" + String(Time.now()) + "000}";
        Particle.publish("Application>\tRequest: ", request.body);
        http.post(request, response, headers);
        Particle.publish("Application>\tResponse: ", response.status);
            
        //Reset Variables
        knocks = 0;
        knocking = false;
    }
}

int sense() {
    unsigned int data[2];
    // Start I2C transmission
    Wire.beginTransmission(ADDR); 
    // Calling conversion result register, 0x00(0)
    Wire.write(0x00);
    // Stop I2C transmission
    Wire.endTransmission();

    // Request 2 bytes
    Wire.requestFrom(ADDR, 2);
    
    // Read 2 bytes of data, raw_adc msb, raw_adc lsb
    if(Wire.available() == 2)
    {  
        data[0] = Wire.read();
        data[1] = Wire.read();
    }
    
    // Convert the data to 12 bits
    return ((data[0] * 256) + data[1]) & 0x0FFF;
}
