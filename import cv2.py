import cv2
from cv2 import LINE_8
import numpy as np
import serial
ser = serial.Serial('COM12',115200, timeout=0)
import time


#ser = serial.Serial(
#    port = 'COM3',
#    baudrate = 9600,
#    parity = serial.PARITY_NONE,
#    stopbits = serial.STOPBITS_ONE,
#    bytesize = serial.EIGHTBITS,
#    timeout = 1
#)
#print(' pc receiving data:')


import firebase_admin
from firebase_admin import credentials

cred = credentials.Certificate("path/to/serviceAccountKey.json")
firebase_admin.initialize_app(cred)

// Import the functions you need from the SDKs you need
import { initializeApp } from "firebase/app";
import { getAnalytics } from "firebase/analytics";
// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

// Your web app's Firebase configuration
// For Firebase JS SDK v7.20.0 and later, measurementId is optional
const firebaseConfig = {
  apiKey: "AIzaSyAJ3b5ZWwObVBW0mZQqAlbatlBNPHl56IU",
  authDomain: "diem-danh-8d35a.firebaseapp.com",
  databaseURL: "https://diem-danh-8d35a-default-rtdb.firebaseio.com",
  projectId: "diem-danh-8d35a",
  storageBucket: "diem-danh-8d35a.appspot.com",
  messagingSenderId: "1083266568312",
  appId: "1:1083266568312:web:7b7499dc69f833cf6b5d8d",
  measurementId: "G-MBW3423JWX"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const analytics = getAnalytics(app);

nam=0
tan=0

f_width = 1280
f_height = 720
so_do= cv2.imread("nen2.jpg")
so_do = cv2.resize(so_do,(560,640))
#cap = cv2.VideoCapture(0)
#cv2.imshow("sodo", so_do)

while True:
    s = ser.readline()#ham doc du lieu va ket thuc voi timeout la 1s
    data = s.decode()#giai ma du lieu va gan vao data
    #inputs=(bytes(s,'utf-8'))
    input = s.rstrip()
    
    if data:
        
        print(data)
        #print(type(data))
        data=int(data)
    if data ==5790232151:
        nam=1
        print(nam)
    if data ==73179198153:
        tan=1
    ##ret,frame = cap.read()
    #frame = cv2.resize(frame, ( f_width, f_height))
    ##frame = cv2.resize(frame, (640,360))
    #cv2.imshow("class_room", frame)
    cv2.imshow("so_do", so_do)
    '''
    cv2.line(so_do, (0, 70), (560,70), (0, 0, 255), 1)
    cv2.line(so_do, (0, 170), (560,170), (0, 0, 255), 1)
    cv2.line(so_do, (0, 270), (560,270), (0, 0, 255), 1)
    cv2.line(so_do, (0, 370), (560,370), (0, 0, 255), 1)
    cv2.line(so_do, (0, 470), (560,470), (0, 0, 255), 1)
    cv2.line(so_do, (0, 570), (560,570), (0, 0, 255), 1)
    
    cv2.line(so_do, (50, 0), (50,640), (0, 0, 255), 1)
    cv2.line(so_do, (150, 0), (150,640), (0, 0, 255), 1)
    cv2.line(so_do, (250, 0), (250,640), (0, 0, 255), 1)
    cv2.line(so_do, (350, 0), (350,640), (0, 0, 255), 1)
    cv2.line(so_do, (450, 0), (450,640), (0, 0, 255), 1)'''
    
    cv2.rectangle(so_do,(50, 70), (140,160), (0, 255, 255),1,LINE_8,0)
    cv2.putText(so_do,"tung",(80,120), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    cv2.rectangle(so_do,(50, 170), (140,260), (0, 255, 255),1,LINE_8,0)
    cv2.putText(so_do,"thang",(80,220), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    cv2.rectangle(so_do,(50, 270), (140,360), (0, 255, 255),1,LINE_8,0)
    cv2.putText(so_do,"son",(80,320), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    cv2.rectangle(so_do,(50, 370), (140,460), (0, 255, 255),1,LINE_8,0)
    cv2.putText(so_do,"lanh",(80,420), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    cv2.rectangle(so_do,(50, 470), (140,560), (0, 255, 255),1,LINE_8,0)
    cv2.putText(so_do,"linh",(80,520), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    
    if nam == 1:

        cv2.rectangle(so_do,(150, 70), (240,160), (0, 255, 0),1,LINE_8,0)
        cv2.putText(so_do,"nam",(180,120), cv2.FONT_HERSHEY_PLAIN,1,(0,255,0),1)
    else:
        cv2.rectangle(so_do,(150, 70), (240,160), (0, 255, 255),1,LINE_8,0)
        cv2.putText(so_do,"nam",(180,120), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)

    
    cv2.rectangle(so_do,(150, 170), (240,260), (0, 255, 255),1,LINE_8,0)
    cv2.putText(so_do,"ngan",(180,220), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    cv2.rectangle(so_do,(150, 270), (240,360), (0, 255, 255),1,LINE_8,0)
    cv2.putText(so_do,"linh",(180,320), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    cv2.rectangle(so_do,(150, 370), (240,460), (0, 255, 255),1,LINE_8,0)
    cv2.putText(so_do,"manh",(180,420), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    cv2.rectangle(so_do,(150, 470), (240,560), (0, 255, 255),1,LINE_8,0)
    cv2.putText(so_do,"thu",(180,520), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)

    if tan == 1:    
        cv2.rectangle(so_do,(300, 70), (390,160), (0, 255, 0),1,LINE_8,0)
        cv2.putText(so_do,"tan",(330,120), cv2.FONT_HERSHEY_PLAIN,1,(0,255,0),1)        
    else:
        cv2.rectangle(so_do,(300, 70), (390,160), (0, 255, 255),1,LINE_8,0)
        cv2.putText(so_do,"tan",(330,120), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)

    cv2.rectangle(so_do,(300, 170), (390,260), (0, 255, 255),1,LINE_8,0)
    cv2.putText(so_do,"yen",(330,220), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    cv2.rectangle(so_do,(300, 270), (390,360), (0, 255, 255),1,LINE_8,0)
    cv2.putText(so_do,"dat",(330,320), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    cv2.rectangle(so_do,(300, 370), (390,460), (0, 255, 255),1,LINE_8,0)
    cv2.putText(so_do,"huy",(330,420), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    cv2.rectangle(so_do,(300, 470), (390,560), (0, 255, 255),1,LINE_8,0)
    cv2.putText(so_do,"thanh",(330,520), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    
    cv2.rectangle(so_do,(400, 70), (490,160), (0, 255, 255),1,LINE_8,0)
    cv2.putText(so_do,"vi",(430,120), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    cv2.rectangle(so_do,(400, 170), (490,260), (0, 255, 255),1,LINE_8,0)
    cv2.putText(so_do,"van",(430,220), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    cv2.rectangle(so_do,(400, 270), (490,360), (0, 255, 255),1,LINE_8,0)
    cv2.putText(so_do,"loi",(430,320), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    cv2.rectangle(so_do,(400, 370), (490,460), (0, 255, 255),1,LINE_8,0)
    cv2.putText(so_do,"tien",(430,420), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    cv2.rectangle(so_do,(400, 470), (490,560), (0, 255, 255),1,LINE_8,0)
    cv2.putText(so_do,"duong",(430,520), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    

    key = cv2.waitKey(1)
    if key==27:
        break

cap.release()
#cv2.destroyAllWindows()
