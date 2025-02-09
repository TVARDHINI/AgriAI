#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h> 
#include <SPI.h> 
#include <math.h>
/**************************************/
#define NumberOf(arg) ((unsigned int) (sizeof (arg) / sizeof (arg [0]))) // calculates the number of layers (in this case 3)

#define SELU 
#include <NeuralNetwork.h>
          NeuralNetwork *NN;

const unsigned int layers[] = {2,4,1}; //3 layers (1st)layer with 2 input neurons (2nd)layer with 4 hidden neurons and (3rd)layer with 1 output neuron
float *output1; // 3rd layer's output(s)
float *output2;
float *output3;
float *output4;
float *output5;
float *output6;

//Default Inputs [for Training only]
const float inputs[4][2] = {
  {0, 0}, // = 0
  {0, 1}, // = 1
  {1, 0}, // = 1
  {1, 1}  // = 0
};
const float expectedOutput[4][1] = {{0},{1},{1},{0}}; // Values that we are expecting to get from the 3rd/(output)layer of Neural-network, in other words something like a feedback to the Neural-network.

/********** config ****/
int i, j, p, q;
int ReportEvery1000;
float input[8];
/************************************/
#define DHT_SENSOR_PIN  23 // ESP32 pin GIOP23 connected to DHT11 sensor
#define DHT_SENSOR_TYPE DHT11
const int mq2a = 39;
const int mq2d = 27;
const int pir = 12;
const int rain = 35;
const int moisture = 36;
LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address 0x3F, 16 column and 2 rows
DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

const char* ssid = "xxxx";//add ssid of wifi/hotspot is small letters only
const char* password = "xxxxx";//add password for wifi/hotspot is small letters only
 
String serverName = "https://api.thingspeak.com/update?api_key=xxxxxxxxxxxxxx"; // Replace xxxxxxxxxxxxxx with your API key
String serverNameAI = "https://api.thingspeak.com/update?api_key=xxxxxxxxxxxxxx"; // Replace xxxxxxxxxxxxxx with your API key
unsigned long lastTime = 0;
unsigned long timerDelay = 15000;
 
int Rain,Pir,Moisture,Mqd,Ai,r;
float tempC,humi,Mqa;
void setup() {
  Serial.begin(9600);
  pinMode(mq2d,INPUT);
  pinMode(mq2a,INPUT);
  pinMode(moisture,INPUT);
  pinMode(pir,INPUT);
  pinMode(rain,INPUT);
  Serial.println("WELCOME TO ENVIRENMENT");
  dht_sensor.begin(); // initialize the DHT sensor
  lcd.begin();         // initialize the lcd
  lcd.backlight();    // open the backlight
  WiFi.begin(ssid, password); // Attempt to connect to wifi with our password
  Serial.println("Connecting"); // Print our status to the serial monitor
  // Wait for wifi to connect
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  /*************************************/
  randomSeed(millis());
  
  NN = new NeuralNetwork(layers,NumberOf(layers)); //Initialization of NeuralNetwork object
 
  do{
    for (int j = 0; j < NumberOf(inputs); j++) // Epoch
    {
      NN->FeedForward(inputs[j]);      // FeedForwards the input arrays through the NN | stores the output array internally
      NN->BackProp(expectedOutput[j]); // "Tells" to the NN if the output was the-expected-correct one | then, "teaches" it
    }
    
    // Prints the MSError.
    Serial.print("MSE: "); 
    Serial.println(NN->MeanSqrdError,6);

    // Loops through each epoch Until MSE goes  < 0.003
  }while(NN->getMeanSqrdError(NumberOf(inputs)) > 0.003);
  
  NN->print(); // Prints the weights and biases of each layer
  /*****************************************/
}
void loop() {
  humi  = dht_sensor.readHumidity();    // read humidity
  tempC = dht_sensor.readTemperature(); // read temperature
  Rain = digitalRead(rain);
  Pir = digitalRead(pir);
  Mqd = digitalRead(mq2d);
  Mqa = analogRead(mq2a);
  Moisture = analogRead(moisture);
  Serial.print("GAS=");
  Serial.println(Mqa);
  Mqa = map(Mqa,0,4095,0,100);
  Moisture = map(Moisture,4095,0,0,100);
  lcd.clear();
  // check whether the reading is successful or not
  if (isnan(tempC) || isnan(humi)) {
    lcd.setCursor(0, 0);
    lcd.print("Failed");
  } else {
    Serial.print("TEMP:");
    Serial.println(tempC);
    Serial.print("Humidity:");
    Serial.println(humi);
    lcd.setCursor(0, 0);  // display position
    lcd.print("T: ");
    lcd.print(tempC);     // display the temperature
    lcd.print("°C");

    lcd.setCursor(8, 0);  // display position
    lcd.print("H: ");
    lcd.print(humi);      // display the humidity
    lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print("R:");
    if(Rain == LOW)
    {
    lcd.print("Y "); 
    r = 1; 
    }
    else
    {
    lcd.print("N ");
    r = 0;
    }
    if(Pir == LOW)
    {
     lcd.print("P:N"); 
    }
    else
    {
    lcd.print("P:Y");   
    }
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("MS:");
    lcd.print(Moisture);
    lcd.setCursor(0, 1);
    lcd.print("GAS:");
    lcd.print(Mqa);
    lcd.print(" G:");
    if(Mqd == LOW)
    {
    lcd.print(" Y ");  
    }
    else
    {
    lcd.print("N ");
    }
    delay(1000);
  }
    if(WiFi.status()== WL_CONNECTED){ // Check to make sure wifi is still connected
      sendData(tempC, humi,Moisture,Mqa,r,Pir); // Call the sendData function defined below
      input[0] = random(2); // ... lets say input from a       button/Switch | random(2) = 0 or 1
      input[1] = random(2); // ... lets say input from another button/Switch | random(2) = 0 or 1
      input[2] = random(2); // ... lets say input from a       button/Switch | random(2) = 0 or 1
      input[3] = random(2); // ... lets say input from another button/Switch | random(2) = 0 or 1
      input[4] = random(2); // ... lets say input from a       button/Switch | random(2) = 0 or 1
      input[5] = random(2); // ... lets say input from another button/Switch | random(2) = 0 or 1
     output1 = NN->FeedForward(input); // FeedForwards the input-array through the NN | returns the predicted output(s)
      sendDataAI(input[0], input[1],input[2],input[3],input[4],input[5]); // Call the sendData function defined below
  }
}
void sendData(float temp, float HUMI,float ms,float gs,int r,int p){
  HTTPClient http; // Initialize our HTTP client
 
 
  String url = serverName + "&field1=" + temp + "&field2=" + HUMI + "&field3=" + ms + "&field4=" + gs + "&field5=" + r + "&field6=" + p; // Define our entire url
       
  http.begin(url.c_str()); // Initialize our HTTP request
       
  int httpResponseCode = http.GET(); // Send HTTP request
       
  if (httpResponseCode > 0){ // Check for good HTTP status code
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  }else{
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}
void sendDataAI(float temp, float HUMI,float ms,float gs,float r,float p){
  HTTPClient http; // Initialize our HTTP client
 
 
  String url = serverNameAI + "&field1=" + temp + "&field2=" + HUMI + "&field3=" + ms + "&field4=" + gs + "&field5=" + r + "&field6=" + p; // Define our entire url
       
  http.begin(url.c_str()); // Initialize our HTTP request
       
  int httpResponseCode = http.GET(); // Send HTTP request
       
  if (httpResponseCode > 0){ // Check for good HTTP status code
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  }else{
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}
