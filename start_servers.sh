#!/bin/bash

cd /home/pi/Desktop/plantkeeper

# Start the servers
python dashboardServer.py &
python httpCameraServer.py &
#python pumpAutomation.py &