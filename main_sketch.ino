#include <BH1750.h>
#include <dht.h>
#include <Wire.h>

// Pin Definition for the Shift Register (165)
const uint8_t ISRDataPin = 7;   // connected to 74HC165 QH (9) pin
const uint8_t ISRLatchPin = 8;  // connected to 74HC165 SH/LD (1) pin
const uint8_t ISRClockPin = 9;  // connected to 74HC165 CLK (2) pin

// Pin definition for the Shift Register (595)
const uint8_t OSRDataPin = 10;   // connected to 74HC595 SER (14) pin
const uint8_t OSRLatchPin = 11;  // connected to 74HC595 RCLK (12) pin
const uint8_t OSRClockPin = 12;  // connected to 74HC595 SRCLK (11) pin

//Relay Connections
const int PUMP_PIN = 5;
const int HOSE_PIN = 6;

//Sensor Connections
const int SOIL_MOISTURE_1_PIN = A0;  
const int SOIL_MOISTURE_2_PIN = A1;
const int SOIL_MOISTURE_3_PIN = A2;
const int VOLTAGE_PIN = A3;
const int TEMPERATURE_HUMIDITY_PIN = 2;

const int WATER_LEVEL_LOW_PIN = 3;
const int WATER_LEVEL_HIGH_PIN = 4;

const uint8_t MOTOR1_SW_UP_PIN = 0;
const uint8_t MOTOR1_SW_DN_PIN = 1;

const uint8_t MOTOR2_SW_UP_PIN = 2;
const uint8_t MOTOR2_SW_DN_PIN = 3;

const uint8_t MOTOR3_SW_UP_PIN = 4;
const uint8_t MOTOR3_SW_DN_PIN = 5;

//Variable Declarations
const int DRY_VALUE = 595;  // value for dry sensor
const int WET_VALUE = 239;  // value for wet sensor
dht DHT;              // Creates a DHT object
BH1750 lightMeter(0x23);
int offset =20;

//Motors Inputs
uint8_t motor1_input = 0;
uint8_t motor2_input = 0;
uint8_t motor3_input = 0;

// Define values for the shift registers
const uint8_t shiftReg1Value = 0; // (165N)
const uint8_t shiftReg2Value = 0; // (595N)

// Buffer for incoming serial commands
const int SERIAL_BUFFER_SIZE = 32;
char serialBuffer[SERIAL_BUFFER_SIZE];
int serialBufferIndex = 0;

// Last time sensors were read
unsigned long lastSensorReadTime = 0;
const unsigned long SENSOR_READ_INTERVAL = 1000;  // 1 second

void setup() {
  Serial.begin(9600);

  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  Wire.begin();
  lightMeter.begin();

  //Pin modes for relay and sensors
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(HOSE_PIN, OUTPUT);
  pinMode(TEMPERATURE_HUMIDITY_PIN, INPUT);
  pinMode(WATER_LEVEL_LOW_PIN, INPUT);
  pinMode(WATER_LEVEL_HIGH_PIN, INPUT);
  pinMode(VOLTAGE_PIN, INPUT);

  // 74HC165 shift register
  setupISR();
  
  // 74HC595 shift register
  setupOSR();

  // Turn off all motors initially
  osrWriteRegister(0);

  // Turn off hose and pump - Initial state
  digitalWrite(PUMP_PIN, LOW);
  digitalWrite(HOSE_PIN, LOW);

}

void loop() {
  // Read incoming serial commands
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n') {
      serialBuffer[serialBufferIndex] = '\0';  // Terminate string
      handleSerialCommand(serialBuffer);
      serialBufferIndex = 0;  // Reset buffer
    } else if (serialBufferIndex < SERIAL_BUFFER_SIZE - 1) {
      serialBuffer[serialBufferIndex++] = c;  // Add to buffer
    }
  }

  // Read sensors periodically
  unsigned long currentTime = millis();
  if (currentTime - lastSensorReadTime >= SENSOR_READ_INTERVAL) {
    lastSensorReadTime = currentTime;
  }
}

//---------------------------------------------------------------------------------
//----------------- Reed(Magnetic Switch) Functions -------------------------------
//---------------------------------------------------------------------------------
void setupISR(){
  pinMode(ISRDataPin, INPUT);
  pinMode(ISRLatchPin, OUTPUT);
  pinMode(ISRClockPin, OUTPUT);
}

uint8_t isrReadRegister() {
   uint8_t inputs = 0;
   digitalWrite(ISRClockPin, HIGH);  // preset clock to retrieve first bit
   digitalWrite(ISRLatchPin, HIGH);  // disable input latching and enable shifting
   inputs = shiftIn(ISRDataPin, ISRClockPin, MSBFIRST);  // capture input values
   digitalWrite(ISRLatchPin, LOW);  // disable shifting and enable input latching
   return inputs;
}

int isrDigitalRead(uint8_t pin) {
   return bitRead(isrReadRegister(), pin);
}

void readInputsWithDigitalRead() {
   // Read and print individual inputs
   Serial.print("MOTOR1_SW_UP = ");  Serial.println(isrDigitalRead(MOTOR1_SW_UP_PIN) ? "HIGH" : "LOW");
   Serial.print("MOTOR1_SW_DN = ");  Serial.println(isrDigitalRead(MOTOR1_SW_DN_PIN) ? "HIGH" : "LOW");
   Serial.print("MOTOR2_SW_UP = ");  Serial.println(isrDigitalRead(MOTOR2_SW_UP_PIN) ? "HIGH" : "LOW");
   Serial.print("MOTOR2_SW_DN = ");  Serial.println(isrDigitalRead(MOTOR2_SW_DN_PIN) ? "HIGH" : "LOW");
   Serial.print("MOTOR3_SW_UP = ");  Serial.println(isrDigitalRead(MOTOR3_SW_UP_PIN) ? "HIGH" : "LOW");
   Serial.print("MOTOR3_SW_DN = ");  Serial.println(isrDigitalRead(MOTOR3_SW_DN_PIN) ? "HIGH" : "LOW");

   Serial.println();
}


//---------------------------------------------------------------------------------
//----------------- Motor Functions -----------------------------------------------
//---------------------------------------------------------------------------------

void setupOSR(){
  pinMode(OSRDataPin, OUTPUT);
  pinMode(OSRLatchPin, OUTPUT);
  pinMode(OSRClockPin, OUTPUT);
}

void osrWriteRegister(uint8_t outputs) {
  // Initiate latching process, next HIGH latches data
  digitalWrite(OSRLatchPin, LOW);
  delayMicroseconds(50); // <-- Increase this delay
  // Shift output data into the shift register, most significant bit first
  shiftOut(OSRDataPin, OSRClockPin, MSBFIRST, outputs);
  // Latch outputs into the shift register
  digitalWrite(OSRLatchPin, HIGH);
  delayMicroseconds(50); // <-- Add this delay
}


void osrDigitalWrite(uint8_t pin, uint8_t value) {
  static uint8_t outputs = 0;  // retains shift register output values

  if (value == HIGH) bitSet(outputs, pin);  // set output pin to HIGH
  else if (value == LOW) bitClear(outputs, pin);  // set output pin to LOW
  osrWriteRegister(outputs);  // write all outputs to shift register
}

void motorControl(uint8_t inputvalue1, uint8_t inputvalue2, uint8_t inputvalue3) {
  // Output pin definitions
  const uint8_t MOTOR1_IN1 = 0;
  const uint8_t MOTOR1_IN2 = 1;
  const uint8_t MOTOR2_IN1 = 2;
  const uint8_t MOTOR2_IN2 = 3;
  const uint8_t MOTOR3_IN1 = 4;
  const uint8_t MOTOR3_IN2 = 5;

  uint8_t motorOutputs = 0;

  // Motor 1 control
  if (inputvalue1 == 0) { // motor stays off
    bitClear(motorOutputs, MOTOR1_IN1);
    bitClear(motorOutputs, MOTOR1_IN2);
  } else if (inputvalue1 == 1) { // motor moves up
    bitClear(motorOutputs, MOTOR1_IN1);
    bitSet(motorOutputs, MOTOR1_IN2);
  } else if (inputvalue1 == 2) { // motor moves down
    bitSet(motorOutputs, MOTOR1_IN1);
    bitClear(motorOutputs, MOTOR1_IN2);
  }

  // Motor 2 control
  if (inputvalue2 == 0) { // motor stays off
    bitClear(motorOutputs, MOTOR2_IN1);
    bitClear(motorOutputs, MOTOR2_IN2);
  } else if (inputvalue2 == 1) { // motor moves up
    bitClear(motorOutputs, MOTOR2_IN1);
    bitSet(motorOutputs, MOTOR2_IN2);
  } else if (inputvalue2 == 2) { // motor moves down
    bitSet(motorOutputs, MOTOR2_IN1);
    bitClear(motorOutputs, MOTOR2_IN2);
  }

  // Motor 3 control
  if (inputvalue3 == 0) { // motor stays off
    bitClear(motorOutputs, MOTOR3_IN1);
    bitClear(motorOutputs, MOTOR3_IN2);
  } else if (inputvalue3 == 1) { // motor moves up
    bitClear(motorOutputs, MOTOR3_IN1);
    bitSet(motorOutputs, MOTOR3_IN2);
  } else if (inputvalue3 == 2) { // motor moves down
    bitSet(motorOutputs, MOTOR3_IN1);
    bitClear(motorOutputs, MOTOR3_IN2);
  }

  osrWriteRegister(motorOutputs);
  delay(1000);  // Add a 1-second delay to give the motors time to move
}

//---------------------------------------------------------------------------------
//----------------- Serial Command Functions --------------------------------------
//---------------------------------------------------------------------------------
void handleSerialCommand(const char* command) {
  if (strcmp(command, "pump_on") == 0) {
    digitalWrite(PUMP_PIN, HIGH);
  } else if (strcmp(command, "pump_off") == 0) {
    digitalWrite(PUMP_PIN, LOW);
  } else if (strcmp(command, "hose_on") == 0) {
    digitalWrite(HOSE_PIN, HIGH);
  } else if (strcmp(command, "hose_off") == 0) {
    digitalWrite(HOSE_PIN, LOW);

  // Motor 1 commands
  }  else if (strcmp(command, "shade_1_up") == 0) {
    motor1_input = 1;
    motorControl(motor1_input, motor2_input, motor3_input);
  } else if (strcmp(command, "shade_1_down") == 0) {
    motor1_input = 2;
    motorControl(motor1_input, motor2_input, motor3_input);
  } else if (strcmp(command, "shade_1_stop") == 0) {
    motor1_input = 0;
    motorControl(motor1_input, motor2_input, motor3_input);

  // Motor 2 commands
  } else if (strcmp(command, "shade_2_up") == 0) {
    motor2_input = 2;
    motorControl(motor1_input, motor2_input, motor3_input);
  } else if (strcmp(command, "shade_2_down") == 0) {
    motor2_input = 1;
    motorControl(motor1_input, motor2_input, motor3_input);
  } else if (strcmp(command, "shade_2_stop") == 0) {
    motor2_input = 0;
    motorControl(motor1_input, motor2_input, motor3_input);

  // Motor 3 commands
  } else if (strcmp(command, "shade_3_up") == 0) {
    motor3_input = 1;
    motorControl(motor1_input, motor2_input, motor3_input);
  } else if (strcmp(command, "shade_3_down") == 0) {
    motor3_input = 2;
    motorControl(motor1_input, motor2_input, motor3_input);
  } else if (strcmp(command, "shade_3_stop") == 0) {
    motor3_input = 0;
    motorControl(motor1_input, motor2_input, motor3_input);
  
  } else if (strcmp(command, "get_sensor_status") == 0) {
    printSensorData();
  }
}

void printSensorData() {
  int soilMoisture1 = analogRead(SOIL_MOISTURE_1_PIN);
  soilMoisture1 = map(soilMoisture1, DRY_VALUE, WET_VALUE, 0, 100);

  int soilMoisture2 = analogRead(SOIL_MOISTURE_2_PIN);
  soilMoisture2 = map(soilMoisture2, DRY_VALUE, WET_VALUE, 0, 100);

  int soilMoisture3 = analogRead(SOIL_MOISTURE_3_PIN);
  soilMoisture3 = map(soilMoisture3, DRY_VALUE, WET_VALUE, 0, 100);

  int readData = DHT.read11(TEMPERATURE_HUMIDITY_PIN);
  int temperature = ((DHT.temperature*9.0)/5.0+32.0); //Converts Celsius to Farenheit and stores in temp
  int humidity = DHT.humidity;

  int waterLevelLow = digitalRead(WATER_LEVEL_LOW_PIN);
  int waterLevelHigh = digitalRead(WATER_LEVEL_HIGH_PIN);

  float light = lightMeter.readLightLevel();
  light = map(light, 0, 700, 0, 100);

  double batteryPercentage = analogRead(VOLTAGE_PIN);
  batteryPercentage = map(batteryPercentage,0,1023, 0, 2500) + offset;// map 0-1023 to 0-2500 and add correction offset
  batteryPercentage /=12.8;// divide by 100 to get the decimal values

  bool reed1 = isrDigitalRead(MOTOR1_SW_UP_PIN);
  bool reed2 = isrDigitalRead(MOTOR1_SW_DN_PIN);

  bool reed3 = isrDigitalRead(MOTOR2_SW_UP_PIN);
  bool reed4 = isrDigitalRead(MOTOR2_SW_DN_PIN);

  bool reed5 = isrDigitalRead(MOTOR3_SW_UP_PIN);
  bool reed6 = isrDigitalRead(MOTOR3_SW_DN_PIN);

  //Soil Moisture Sensor 1, Soil Moisture Sensor 2, Soil Moisture Sensor 3, Temperature, Humidity, Water Level
  Serial.print(soilMoisture1);
  Serial.print(", ");
  Serial.print(soilMoisture2);
  Serial.print(", ");
  Serial.print(soilMoisture3);
  Serial.print(", ");
  Serial.print(temperature);
  Serial.print(", ");
  Serial.print(humidity);
  Serial.print(", ");
  Serial.print(waterLevelLow);
  Serial.print(", ");
  Serial.print(waterLevelHigh);
  Serial.print(", ");
  Serial.print(light);
  Serial.print(", ");
  Serial.print(batteryPercentage);
  Serial.print(", ");
  Serial.print(reed1);
  Serial.print(", ");
  Serial.print(reed2);
  Serial.print(", ");
  Serial.print(reed3);
  Serial.print(", ");
  Serial.print(reed4);
  Serial.print(", ");
  Serial.print(reed5);
  Serial.print(", ");
  Serial.print(reed6);
  Serial.print("\n");
}
