Plant Monitoring System
=======================

This repository contains the code for a plant monitoring system that can be used to monitor the moisture, temperature, humidity, and light levels in a plant environment. The system uses an Arduino board with sensors connected to it and a Python Flask web application to display the sensor data in real-time.

Photo
-----
![IMG_2091](https://user-images.githubusercontent.com/20326910/236612451-cef785e5-bb2e-4f10-9279-13cb09384d4c.png)

Video
-----
https://user-images.githubusercontent.com/20326910/236614716-82780312-fc47-4eba-8d34-e2aec6b0808f.mp4

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

`

Getting started
---------------

1.  Clone this repository to your local RPI.
2.  Connect the Arduino board with sensors to your computer.
3.  SSH into RPI 
4.  Install the required Python libraries by running the following command in the terminal:

These can be installed via pip:

```
pip install flask 
pip install pyserial
```

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
