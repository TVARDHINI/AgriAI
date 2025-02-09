# AgriAI
This IoT-based agricultural monitoring system uses sensors to track temperature, humidity, soil moisture, rainfall, and gas levels. The data is sent to the ThingSpeak cloud platform for analysis and displayed on an LCD. Artificial Neural Networks (ANN) are utilized for data insights and AI-driven analysis.

# IoT-Based Agricultural Monitoring System

This project implements an IoT-based agricultural monitoring system that utilizes various sensors and Artificial Neural Networks (ANN) to monitor environmental conditions and provide valuable insights for farmers. The system collects data on temperature, humidity, soil moisture, rainfall, and gas levels, transmitting this data to the ThingSpeak cloud platform. The data is displayed on an LCD and sent to the ThingSpeak server for artificial intelligence and data analysis. The results are displayed through the ThingSpeak platform.

## Hardware Required

- **ESP32 Development Board**: The microcontroller responsible for collecting and transmitting sensor data.
- **DHT11 Sensor**: For measuring temperature and humidity.
- **MQ-2 Gas Sensor**: For detecting combustible gases (e.g., smoke).
- **Rain Sensor**: To detect rainfall.
- **Soil Moisture Sensor**: Measures the moisture level in the soil.
- **PIR Motion Sensor**: Detects movement.
- **LCD (16x2) with I2C Interface**: For local display of sensor readings.
- **Connecting Wires**: To connect the sensors to the ESP32.

## Software Required

- **Arduino IDE**: For programming the ESP32 microcontroller.
- **ThingSpeak Account**: For cloud-based data storage, visualization, and analysis. (Sign up [here](https://thingspeak.com/)).
- **Arduino Libraries**:
    - `DHT sensor library`
    - `LiquidCrystal_I2C`
    - `WiFi`
    - `HTTPClient`
    - `Wire`
    - `SPI`
    - `NeuralNetwork`

## Setup Instructions

### 1. Install Arduino IDE
Download and install the Arduino IDE from the official website:  
[Arduino IDE Download](https://www.arduino.cc/en/software)

### 2. Install ESP32 Board Support
- Open Arduino IDE and go to **File** > **Preferences**.
- Add the following URL to "Additional Boards Manager URLs":  
  `https://dl.espressif.com/dl/package_esp32_index.json`
- Go to **Tools** > **Board** > **Boards Manager**.
- Search for "ESP32" and install the **ESP32 by Espressif Systems** board.
- Select your ESP32 board (e.g., "ESP32 Dev Module") from **Tools** > **Board**.

### 3. Install Required Libraries
- Go to **Sketch** > **Include Library** > **Manage Libraries**.
- Search for and install the following libraries:
  - `DHT sensor library by Adafruit`
  - `LiquidCrystal_I2C by Frank de Brabander`
  - `NeuralNetwork by BrainyThing`

### 4. Create a ThingSpeak Account and Channel
- Sign up for a free account at [ThingSpeak](https://thingspeak.com/).
- Create a new channel to store the sensor data.
- Configure the channel fields according to the data you want to send (e.g., Temperature, Humidity, Soil Moisture, Gas Level).
- Note down the API Key for your ThingSpeak channel.

### 5. Hardware Connections

- **DHT11**: Data pin to ESP32 pin 23
- **MQ-2**: 
  - Analog output (AOUT) to ESP32 pin 39
  - Digital output (DOUT) to ESP32 pin 27
- **Rain Sensor**: Digital output to ESP32 pin 35
- **Soil Moisture Sensor**: Analog output to ESP32 pin 36
- **PIR Sensor**: Digital output to ESP32 pin 12
- **LCD**: 
  - SDA to ESP32 SDA pin (usually 21)
  - SCL to ESP32 SCL pin (usually 22)

Connect VCC and GND pins of all the sensors to the appropriate 5V and GND pins on the ESP32.

### 6. Configure the Code

- Open the Arduino sketch (`.ino` file) in the Arduino IDE.
- **WiFi Configuration**: 
  - Replace `xxxx` with your WiFi network SSID (name).
  - Replace `xxxxx` with your WiFi password. Make sure they are in lowercase.
- **ThingSpeak API Key**: 
  - Replace `xxxxxxxxxxxxxx` in `String serverName = "https://api.thingspeak.com/update?api_key=xxxxxxxxxxxxxx";` with your ThingSpeak API key.
  - Replace `xxxxxxxxxxxxxx` in `String serverNameAI = "https://api.thingspeak.com/update?api_key=xxxxxxxxxxxxxx";` with your ThingSpeak API key.

### 7. Upload the Code to ESP32

- Connect the ESP32 to your computer via USB.
- In the Arduino IDE, select the correct port under **Tools** > **Port**.
- Click the **Upload** button to compile and upload the code to the ESP32.

## Running the Code

### 1. Monitor Serial Output
- After uploading the code, open the **Serial Monitor** in the Arduino IDE (**Tools** > **Serial Monitor**).
- Set the baud rate to 9600.
- You should see the ESP32 connecting to your WiFi network and sensor readings being printed.

### 2. Check ThingSpeak Channel
- Log in to your ThingSpeak account and go to the channel you created.
- You should see the sensor data being updated in real-time on the channel's charts.

### 3. LCD Display
- The LCD should display the temperature, humidity, and other sensor readings.

## Code Explanation

### 1. Includes
The code includes necessary libraries for DHT11 sensor, LCD, WiFi, HTTP communication, and more.

### 2. Definitions
- Pin assignments for sensors, WiFi credentials, and ThingSpeak API key.

### 3. `setup()` Function
- Initializes serial communication, sensor pins, LCD, and WiFi connection.
- Connects to the specified WiFi network.
- Initializes the DHT sensor and LCD.

### 4. `loop()` Function
- Reads sensor values (temperature, humidity, gas level, rain, soil moisture).
- Displays the sensor readings on the LCD.
- Sends the sensor data to the ThingSpeak channel using HTTP GET requests.
- Delays for a specified interval before the next reading.

### 5. `sendData()` Function
- Constructs the ThingSpeak API URL with sensor data.
- Sends an HTTP GET request to ThingSpeak to update the channel with the new data.

## Neural Network Explanation

The neural network is designed with three layers:
- An input layer with 2 neurons
- A hidden layer with 4 neurons
- An output layer with 1 neuron

The `NeuralNetwork` class handles the feedforward and backpropagation processes. The network is trained using sample inputs (`inputs`) and expected outputs (`expectedOutput`) to minimize the mean squared error (MSE). After training, the network can predict outputs based on new inputs.

## Modifications and Enhancements

- **More Sensors**: Add more sensors to monitor additional environmental parameters (e.g., wind speed, light intensity).
- **Data Analysis**: Implement more advanced data analysis techniques to generate insights and predictions.
- **Mobile App**: Develop a mobile app for farmers to easily access sensor data and receive alerts.
- **Alert System**: Implement an alert system that sends SMS or email notifications to farmers when certain thresholds are reached (e.g., low soil moisture, high gas level).
- **Solar Power**: Integrate a solar panel and battery to make the system self-sufficient.

## Troubleshooting

### 1. WiFi Connection Issues
- Double-check the WiFi SSID and password in the code.
- Ensure your ESP32 board is compatible with your WiFi network (2.4GHz).

### 2. ThingSpeak Not Updating
- Verify your ThingSpeak API key in the code.
- Check your ThingSpeak channel settings (field names, data format).
- Make sure your internet connection is stable.

### 3. Sensor Readings Incorrect
- Check the sensor connections to the ESP32.
- Make sure you have installed the correct sensor libraries.
- Verify the sensor readings with a known reliable source.

### 4. LCD Not Displaying
- Ensure you have connected the LCD to ESP32 properly.
- Check the I2C address on the serial monitor (usually `0x27` or `0x39`) and declare it accordingly.

## Contributing

Feel free to contribute to this project by submitting bug reports, feature requests, or pull requests.

## License

This project is open-source and available under the MIT license.
