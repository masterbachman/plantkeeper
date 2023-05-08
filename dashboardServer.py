import serial
import time
import threading
from flask import Flask, render_template, request, jsonify
from collections import OrderedDict

# create a Flask app
app = Flask(__name__)

# define serial port and baud rate
ser = serial.Serial('/dev/ttyACM0', 9600)

# define routes
@app.route('/')
def index():
    # render the template with the buttons
    return render_template('dashboard.html')

@app.route('/pump_on')
def pump_on():
    # send "pump_on" command over serial
    ser.write(b'pump_on\n')
    return 'Pump On'

@app.route('/pump_off')
def pump_off():
    # send "pump_off" command over serial
    ser.write(b'pump_off\n')
    return 'Pump Off'

@app.route('/hose_on')
def hose_on():
    # send "hose_on" command over serial
    ser.write(b'hose_on\n')
    return 'Hose On'

@app.route('/hose_off')
def hose_off():
    # send "hose_off" command over serial
    ser.write(b'hose_off\n')
    return 'Hose Off'

@app.route('/shade_1_up')
def shade_1_up():
    # send "shade_1_up" command over serial
    ser.write(b'shade_1_up\n')
    return 'Shade 1 Up'

@app.route('/shade_1_down')
def shade_1_down():
    # send "shade_1_down" command over serial
    ser.write(b'shade_1_down\n')
    return 'Shade 1 Down'

@app.route('/shade_2_up')
def shade_2_up():
    # send "shade_2_up" command over serial
    ser.write(b'shade_2_up\n')
    return 'Shade 2 Up'

@app.route('/shade_2_down')
def shade_2_down():
    # send "shade_2_down" command over serial
    ser.write(b'shade_2_down\n')
    return 'Shade 2 Down'

@app.route('/shade_3_up')
def shade_3_up():
    # send "shade_3_up" command over serial
    ser.write(b'shade_3_up\n')
    return 'Shade 3 Up'

@app.route('/shade_3_down')
def shade_3_down():
    # send "shade_3_down" command over serial
    ser.write(b'shade_3_down\n')
    return 'Shade 3 Down'

@app.route('/shade_1_stop')
def shade_1_stop():
    # send "shade_1_stop" command over serial
    ser.write(b'shade_1_stop\n')
    return 'Shade 1 Stop'

@app.route('/shade_2_stop')
def shade_2_stop():
    # send "shade_2_stop" command over serial
    ser.write(b'shade_2_stop\n')
    return 'Shade 2 Stop'

@app.route('/shade_3_stop')
def shade_3_stop():
    # send "shade_3_stop" command over serial
    ser.write(b'shade_3_stop\n')
    return 'Shade 3 Stop'

@app.route('/get_sensor_status')
def get_sensor_status():
    # send "get_sensor_status" command over serial
    ser.write(b'get_sensor_status\n')
    # read serial data
    data = ser.readline().decode('utf-8').strip()
    # log the data to console
    print(data)
    # split the data into a list
    data_list = data.split(',')
    # create a dictionary to store the data
    data_dict = {
        'moisture1': data_list[0],
        'moisture2': data_list[1],
        'moisture3': data_list[2],
        'temperature': data_list[3],
        'humidity': data_list[4],
        'waterLevelLow': data_list[5],
        'waterLevelHigh': data_list[6],
        'light': data_list[7],
        'batteryPercentage': data_list[8],
        'reed1': data_list[9],
        'reed2': data_list[10],
        'reed3': data_list[11],
        'reed4': data_list[12],
        'reed5': data_list[13],
        'reed6': data_list[14],
    }
    # return the data as a JSON object
    return jsonify(data_dict)

# run the app
if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0')
