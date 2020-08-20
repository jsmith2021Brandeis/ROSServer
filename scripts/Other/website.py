#Jacob Smith 8/2/20 Brandeis Automation Lab-
#The website interface for the ROSSERVER Project

#Import statements required for the website
from flask import Flask, render_template, Response
import pyscreenshot as ImageGrab
import flask
from io import BytesIO
import socket
from flask import Flask, request, render_template, url_for, redirect
import os, shutil
import webbrowser
import cv2
import subprocess
import pyscreenshot
import flask
from StringIO import StringIO
from flask_basicauth import BasicAuth




#Class used for webcam
class VideoCamera(object):
    def __init__(self):
        # Using OpenCV to capture from device 0. If you have trouble capturing
        # from a webcam, comment the line below out and use a video file
        # instead.
        self.video = cv2.VideoCapture("/dev/video2")
        # If you decide to use video.mp4, you must have this file in the folder
        # as the main.py.
        # self.video = cv2.VideoCapture('video.mp4')
    
    def __del__(self):
        self.video.release()
    
    def get_frame(self):
        success, image = self.video.read()
        # We are using Motion JPEG, but OpenCV defaults to capture raw images,
        # so we must encode it into JPEG in order to correctly display the
        # video stream.
        ret, jpeg = cv2.imencode('.jpg', image)
        return jpeg.tobytes()

#create app object representing the website
app = Flask(__name__)

#require password logic
app.config['BASIC_AUTH_FORCE'] = True
app.config['BASIC_AUTH_USERNAME'] = 'Louis'
app.config['BASIC_AUTH_PASSWORD'] = 'Brandeis'
basic_auth = BasicAuth(app)

#Set home directory of website
@app.route('/')
def index():
    return render_template('index.html')

#for webcam
def gen(camera):
    while True:
        frame = camera.get_frame()
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n\r\n')

#for webcam
@app.route('/video_feed')
def video_feed():
    return Response(gen(VideoCamera()),
                    mimetype='multipart/x-mixed-replace; boundary=frame')

#logic to handle file uploads
@app.route("/handleUpload", methods=['POST'])
def handleFileUpload():
    if 'photo' in request.files:
        photo = request.files['photo']
        if photo.filename != '':    
            path='/home/robotics/catkin_ws/src/ROSServer/scripts/Other/userScripts'
            #saving the file with the same names means we dno't have to delete previous files
            photo.save(os.path.join(path,'userScript.py'))
           
    return redirect(url_for('index'))

#needed for screens
@app.route('/screen.png')
def serve_pil_image():
    img_buffer = StringIO()
    pyscreenshot.grab().save(img_buffer, 'PNG', quality=50)
    img_buffer.seek(0)
    return flask.send_file(img_buffer, mimetype='image/png')


#needed for screenhsaring ROSSERVER Host
def get_ip():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        # doesn't even have to be reachable
        s.connect(('10.255.255.255', 1))
        IP = s.getsockname()[0]
    except:
        IP = '127.0.0.1'
    finally:
        s.close()
    return IP

#logic to handle file uploads
@app.route("/selectProfile", methods=['POST'])
def selectProfile():
    
    folderName=""
    bashName= ""
    
    if request.form['submit_button'] == 'Teleop Live Robot':
        folderName= "teleopDemo"
        bashName="runLiveRobot"
    elif request.form['submit_button'] == 'Roomba Live Robot':
        folderName= "ESPRoombaDemo"
        bashName="runLiveRobot"
    elif request.form['submit_button'] == 'Roomba Simulation':
        folderName= "RoombaDemo"
        bashName="runRoomba"
    elif request.form['submit_button'] == 'My Saved Simulation':
        folderName= "Other/userScripts"
        bashName="runUserScript.sh"
    elif request.form['submit_button'] == 'End Program':
        folderName= "Other/userScripts"
        bashName="endProgram.sh"
    
    #Launch the appropriate ROS command based on what button was pressed
    command = "gnome-terminal -- /bin/sh -c 'cd .. ; cd "+folderName+" ; bash "+ bashName +" ; bash'"
    os.system(command)
    
    return redirect(url_for('index'))

#Main method
if __name__ == '__main__':
    webbrowser.open('http://'+get_ip()+':5000/')
    app.run(host=get_ip(), debug=False)
