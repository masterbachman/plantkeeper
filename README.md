Plant Monitoring System
=======================

This repository contains the code for a plant monitoring system that can be used to monitor the moisture, temperature, humidity, and light levels in a plant environment. The system uses an Arduino board with sensors connected to it and a Python Flask web application to display the sensor data in real-time.

Requirements
------------

To run this application, you will need:

-   An Arduino board with sensors connected to it.
-   Python 3.6 or higher.
-   Flask and PySerial Python libraries installed.

Dependencies
------------

The following libraries are required:

-   `serial`
-   `time`
-   `threading`
-   `flask`
-   `collections`

These can be installed via pip:

Copy code

`pip install pyserial
pip install flask`

Getting started
---------------

1.  Clone this repository to your local machine.
2.  Connect the Arduino board with sensors to your computer.
3.  Install the required Python libraries by running the following command in the terminal:

Copy code

`pip install flask pyserial`

1.  Open the `dashboardServer.py` file and update the serial port and baud rate to match your Arduino board settings:

pythonCopy code

`# define serial port and baud rate
ser = serial.Serial('/dev/ttyACM0', 9600)`

1.  Run the application by running the following command in the terminal:

Copy code

```
python dashboardServer.py
python shadeAutomation.py
python pumpAutomation.py
python httpCameraServer.py

```

1.  Open a web browser and go to `http://localhost:5000` to access the plant monitoring system.



Features
--------

-   The system can turn on/off the pump and hose used to water the plants.
-   The system can control the shades to regulate the amount of light plants receive.
-   The system can retrieve the sensor data, including moisture, temperature, humidity, and light levels, and display it in real-time in the web application.

Routes
------

The following routes are available in the application:

-   `/`: Displays the dashboard with buttons to control the pump, hose, and shades.
-   `/pump_on`: Turns on the pump.
-   `/pump_off`: Turns off the pump.
-   `/hose_on`: Turns on the hose.
-   `/hose_off`: Turns off the hose.
-   `/shade_1_up`: Raises shade 1.
-   `/shade_1_down`: Lowers shade 1.
-   `/shade_2_up`: Raises shade 2.
-   `/shade_2_down`: Lowers shade 2.
-   `/shade_3_up`: Raises shade 3.
-   `/shade_3_down`: Lowers shade 3.
-   `/shade_1_stop`: Stops shade 1.
-   `/shade_2_stop`: Stops shade 2.
-   `/shade_3_stop`: Stops shade 3.
-   `/get_sensor_status`: Retrieves the sensor data and displays it in real-time.
