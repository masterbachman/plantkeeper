import requests
import time

# Configuration
LOCAL_HOST = "http://0.0.0.0:5000"  # URL for the local host
GET_SENSOR_STATUS = "/get_sensor_status"  # API endpoint to get the sensor status
PUMP_ON = "/pump_on"  # API endpoint to turn the pump on
PUMP_OFF = "/pump_off"  # API endpoint to turn the pump off
CHECK_INTERVAL = 30  # Check every 30 seconds
THRESHOLD = 15  # Moisture threshold to control the pump

def get_sensor_values():
    # Request sensor values from the API
    response = requests.get(f"{LOCAL_HOST}{GET_SENSOR_STATUS}")
    if response.status_code == 200:
        return response.json()
    else:
        print(f"Error getting sensor values: {response.text}")
        return None

def control_pump(action):
    # Send request to control the pump
    response = requests.get(f"{LOCAL_HOST}{action}")
    if response.status_code != 200:
        print(f"Error controlling pump: {response.text}")

def main():
    while True:
        sensor_values = get_sensor_values()
        if sensor_values:
            # Parse moisture values and convert to integers
            moisture1 = int(sensor_values["moisture1"].strip())
            moisture2 = int(sensor_values["moisture2"].strip())
            moisture3 = int(sensor_values["moisture3"].strip())

            # Calculate the average moisture value
            average_moisture = (moisture1 + moisture2 + moisture3) / 3

            # Print moisture values and average to console for debugging purposes
            print(f"Moisture values: {moisture1}, {moisture2}, {moisture3}")
            print(f"Average moisture: {average_moisture}")

            # Control the pump based on the average moisture value
            if average_moisture < THRESHOLD:
                print("Turning on pump")
                control_pump(PUMP_ON)
            else:
                print("Turning off pump")
                control_pump(PUMP_OFF)

        # Wait for the defined check interval before checking again
        time.sleep(CHECK_INTERVAL)

if __name__ == "__main__":
    main()
