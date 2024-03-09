import serial
from picamera import PiCamera
import os
import datetime

ser = serial.Serial("/dev/ttyAMA0", 115200)
camera = PiCamera()

path = "/var/www/html/"
filename_gb = ""

def savePhoto(filename):
    camera.resolution = (3280, 2460)
    print("Photo OK")
    camera.capture(f'{filename}.jpg')
    return 3

def startVideo(filename):
    print("Video Start")
    camera.resolution = (1920, 1080)
    camera.framerate = 30
    camera.video_stabilization = True
    global filename_gb
    filename_gb = filename
    camera.start_recording(f'{filename}.h264')
    return 8

def stopVideo():
    print("Video Stop")
    global filename_gb
    camera.stop_recording()
    cmd = f'ffmpeg -i {filename_gb}.h264 -vcodec copy -acodec copy {filename_gb}.mp4'
    os.system(cmd)
    cmd = f'rm {filename_gb}.h264'
    os.system(cmd)
    return 4

def readData():
    # ??????
    recv_buffer = ser.read(1)
    # ??????????
    return recv_buffer

def main():
    data = readData()
    filename = path + datetime.datetime.now().strftime("%Y-%m-%d_%H:%M:%S")
    if data == b'1':
        savePhoto(filename)
    elif data == b'2':
        startVideo(filename)
    elif data == b'0':
        stopVideo()
    else:
        pass
    ser.flushInput()
    ser.flushOutput()
    



if __name__ == "__main__":
    if os.path.isdir(path):
        while True:
            main()
            pass
    else:
        print("Disk Error")