from flask import Flask, Response
from picamera import PiCamera
import time
import io

app = Flask(__name__)

camera = PiCamera()
camera.resolution = (640, 480)
camera.framerate = 30
time.sleep(2)

@app.route('/image.jpeg')
def get_image():
    stream = io.BytesIO()
    camera.capture(stream, format='jpeg', use_video_port=True)
    stream.seek(0)
    return Response(stream.read(), mimetype='image/jpeg')

@app.route('/video_feed')
def video_feed():
    return Response(generate_frames(), mimetype='multipart/x-mixed-replace; boundary=frame')

def generate_frames():
    while True:
        stream = io.BytesIO()
        camera.capture(stream, format='jpeg', use_video_port=True)
        stream.seek(0)
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + stream.read() + b'\r\n')
        time.sleep(0.1)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8000)