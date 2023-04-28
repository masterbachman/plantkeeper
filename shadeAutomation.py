import requests
import json
import time

LOCAL_HOST = "http://0.0.0.0:5000"
SENSOR_STATUS_ENDPOINT = "/get_sensor_status"
SHADE_ENDPOINTS = {
    "down": ["/shade_1_down", "/shade_2_down", "/shade_3_down"],
    "up": ["/shade_1_up", "/shade_2_up", "/shade_3_up"]
}
REED_SWITCHES = ["reed1", "reed2", "reed3", "reed4", "reed5", "reed6"]
INTERVAL = 60  # Time interval in seconds

def get_sensor_status():
    response = requests.get(LOCAL_HOST + SENSOR_STATUS_ENDPOINT)
    if response.status_code == 200:
        return json.loads(response.text)
    return None

def control_shades(direction):
    for endpoint in SHADE_ENDPOINTS[direction]:
        requests.get(LOCAL_HOST + endpoint)

def check_conditions(sensor_data):
    temperature = int(sensor_data["temperature"].strip())
    humidity = float(sensor_data["humidity"].strip())
    light = int(sensor_data["light"].strip())

    if temperature == 77 and humidity == 60.0 and light == 3000:
        control_shades("down")
    else:
        control_shades("up")

def check_reed_switches(sensor_data):
    for switch in REED_SWITCHES:
        if int(sensor_data[switch].strip()) == 1:
            if switch in ["reed1", "reed3", "reed5"]:
                control_shades("up")
            else:
                control_shades("down")
            break

def main():
    while True:
        sensor_data = get_sensor_status()
        if sensor_data:
            check_conditions(sensor_data)
            check_reed_switches(sensor_data)
        else:
            print("Error: Unable to fetch sensor data")
        time.sleep(INTERVAL)

if __name__ == "__main__":
    main()
