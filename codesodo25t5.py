from ast import Str
from unittest import result
import cv2
from cv2 import LINE_8
import numpy as np
from pyparsing import Char
import serial
ser = serial.Serial('COM6',115200, timeout=0)
import time
from firebase import firebase
import array as arr
import datetime

firebase = firebase.FirebaseApplication("https://so-do-4d921-default-rtdb.firebaseio.com/", None)
tung =thang =son =lanh =linh =nam =ngan =lap =manh =thu =tan =vi =yen =dat =loi =huy =tien =thanh =duong =0

datafb = {
          }
# dat=249237206153
# son=73179198153
# lanh=169205201153
f_width = 1280
f_height = 720
so_do= cv2.imread("nen2.jpg")
so_do = cv2.resize(so_do,(560,640))
cap = cv2.VideoCapture(0)
cv2.imshow("sodo", so_do)
nresult2 = firebase.get('/HS-da-den','')
def up_data(name):
    datafb[name]=1
    nresult = firebase.put('/','HS-da-den',datafb)

#ten=[tung ,thang ,son ,lanh ,linh ,nam ,ngan ,lap ,manh ,thu ,tan ,vi ,yen ,dat ,loi ,huy ,tien ,thanh ,duong ]
ten=[2, 3, 4, 5, 5, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22]
while True:
    s = ser.readline()#ham doc du lieu va ket thuc voi timeout la 1s
    data = s.decode()#giai ma du lieu va gan vao data
    #inputs=(bytes(s,'utf-8'))
    input = s.rstrip()

    if data:
        #print(data)
        #print(type(data))
        data=int(data)
        up_data(data)

    ten2=[ "tung","thang",'73179198153','169205201153' , "linh " ,  "nam " ,  "ngan " ,  "lap " ,  "manh " ,  "thu " ,  "tan " ,  "vi " ,  "yen " ,  "dat " ,  "loi " ,  "huy " ,  "tien " ,  "thanh " ,  "duong "]
    numbers_array = arr.array('i', ten) 
    nresult = firebase.get('/HS-da-den','')
    #print(nresult)
    if nresult:
        if nresult2 != nresult:
            for ten2 in nresult:
                #print(ten2)
                if ten2=='73179153':
                    ten[0]=1

                elif ten2=='169201153':
                    ten[1]=1
                #son
                elif ten2=='73179198153':
                    ten[2]=1

            #lanh
                elif ten2=='169205201153':
                    ten[3]=1
                
                elif ten2=='linh':
                    ten[4]=1
                elif ten2=='nam':
                    ten[5]=1
                elif ten2=='ngan':
                    ten[6]=1
                elif ten2=='lap':
                    ten[7]=1
                elif ten2=='manh':
                    ten[8]=1
                elif ten2=='thu':
                    ten[9]=1
                elif ten2=='tan':
                    ten[10]=1
                elif ten2=='vi':
                    ten[11]=1
                elif ten2=='yen':
                    ten[12]=1
                #dat
                elif ten2=='249237206153':
                    ten[13]=1
                elif ten2=='loi':
                    ten[14]=1
                elif ten2=='huy':
                    ten[15]=1
                elif ten2=='tien':
                    ten[16]=1
                elif ten2=='thanh':
                    ten[17]=1
                elif ten2=='duong':
                    ten[18]=1
                elif ten2=='tung':
                    ten[19]=1
            nresult2 = nresult


    ret,frame = cap.read()
    #frame = cv2.resize(frame, ( f_width, f_height))
    frame = cv2.resize(frame, (640,360))
    cv2.imshow("so do", frame)
    cv2.imshow("so_do", so_do)   
    if tung == 1: 
        cv2.rectangle(so_do,(50, 70), (140,160), (0, 255, 255),1,LINE_8,0)
        cv2.putText(so_do,"tung",(80,120), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    else:
        cv2.rectangle(so_do,(50, 70), (140,160), (0, 255, 255),1,LINE_8,0)
        cv2.putText(so_do,"tung",(80,120), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    if thang == 1:
        cv2.rectangle(so_do,(50, 170), (140,260), (0, 255, 255),1,LINE_8,0)
        cv2.putText(so_do,"thang",(80,220), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    else:
        cv2.rectangle(so_do,(50, 170), (140,260), (0, 255, 255),1,LINE_8,0)
        cv2.putText(so_do,"thang",(80,220), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    #print(ten[2])
    if ten[2]==1:
        cv2.rectangle(so_do,(50, 270), (140,360), (0, 255, 0),1,LINE_8,0)
        cv2.putText(so_do,"son",(80,320), cv2.FONT_HERSHEY_PLAIN,1,(0,255,0),1)
    else:
        cv2.rectangle(so_do,(50, 270), (140,360), (0, 255, 255),1,LINE_8,0)
        cv2.putText(so_do,"son",(80,320), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    if ten[3] == 1:
        cv2.rectangle(so_do,(50, 370), (140,460), (0, 255, 0),1,LINE_8,0)
        cv2.putText(so_do,"lanh",(80,420), cv2.FONT_HERSHEY_PLAIN,1,(0,255,0),1)
    else:
        cv2.rectangle(so_do,(50, 370), (140,460), (0, 255, 255),1,LINE_8,0)
        cv2.putText(so_do,"lanh",(80,420), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    if linh == 1:
        cv2.rectangle(so_do,(50, 470), (140,560), (0, 255, 255),1,LINE_8,0)
        cv2.putText(so_do,"linh",(80,520), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    else:
        cv2.rectangle(so_do,(50, 470), (140,560), (0, 255, 255),1,LINE_8,0)
        cv2.putText(so_do,"linh",(80,520), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    if nam == 1:

        cv2.rectangle(so_do,(150, 70), (240,160), (0, 255, 0),1,LINE_8,0)
        cv2.putText(so_do,"nam",(180,120), cv2.FONT_HERSHEY_PLAIN,1,(0,255,0),1)
    else:
        cv2.rectangle(so_do,(150, 70), (240,160), (0, 255, 255),1,LINE_8,0)
        cv2.putText(so_do,"nam",(180,120), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)

    if ngan == 1:
        cv2.rectangle(so_do,(150, 170), (240,260), (0, 255, 255),1,LINE_8,0)
        cv2.putText(so_do,"ngan",(180,220), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    else:
        cv2.rectangle(so_do,(150, 170), (240,260), (0, 255, 255),1,LINE_8,0)
        cv2.putText(so_do,"ngan",(180,220), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    if lap == 1:
        cv2.rectangle(so_do,(150, 270), (240,360), (0, 255, 255),1,LINE_8,0)
        cv2.putText(so_do,"lap",(180,320), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    else:
        cv2.rectangle(so_do,(150, 270), (240,360), (0, 255, 255),1,LINE_8,0)
        cv2.putText(so_do,"lap",(180,320), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    if manh == 1:
        cv2.rectangle(so_do,(150, 370), (240,460), (0, 255, 255),1,LINE_8,0)
        cv2.putText(so_do,"manh",(180,420), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    else:
        cv2.rectangle(so_do,(150, 370), (240,460), (0, 255, 255),1,LINE_8,0)
        cv2.putText(so_do,"manh",(180,420), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    if thu == 1:
        cv2.rectangle(so_do,(150, 470), (240,560), (0, 255, 255),1,LINE_8,0)
        cv2.putText(so_do,"thu",(180,520), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    else:
        cv2.rectangle(so_do,(150, 470), (240,560), (0, 255, 255),1,LINE_8,0)
        cv2.putText(so_do,"thu",(180,520), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    if tan == 1:    
        cv2.rectangle(so_do,(300, 70), (390,160), (0, 255, 0),1,LINE_8,0)
        cv2.putText(so_do,"tan",(330,120), cv2.FONT_HERSHEY_PLAIN,1,(0,255,0),1)        
    else:
        cv2.rectangle(so_do,(300, 70), (390,160), (0, 255, 255),1,LINE_8,0)
        cv2.putText(so_do,"tan",(330,120), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)

    if yen == 1:
        cv2.rectangle(so_do,(300, 170), (390,260), (0, 255, 255),1,LINE_8,0)
        cv2.putText(so_do,"yen",(330,220), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    else:
        cv2.rectangle(so_do,(300, 170), (390,260), (0, 255, 255),1,LINE_8,0)
        cv2.putText(so_do,"yen",(330,220), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    if ten[13]==1:
        cv2.rectangle(so_do,(300, 270), (390,360), (0, 255, 0),1,LINE_8,0)
        cv2.putText(so_do,"dat",(330,320), cv2.FONT_HERSHEY_PLAIN,1,(0,255,0),1)
    else:
        cv2.rectangle(so_do,(300, 270), (390,360), (0, 255, 255),1,LINE_8,0)
        cv2.putText(so_do,"dat",(330,320), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1) 
    if huy == 1:
        cv2.rectangle(so_do,(300, 370), (390,460), (0, 255, 255),1,LINE_8,0)
        cv2.putText(so_do,"huy",(330,420), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    else:
        cv2.rectangle(so_do,(300, 370), (390,460), (0, 255, 255),1,LINE_8,0)
        cv2.putText(so_do,"huy",(330,420), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    if thanh == 1:
        cv2.rectangle(so_do,(300, 470), (390,560), (0, 255, 255),1,LINE_8,0)
        cv2.putText(so_do,"thanh",(330,520), cv2.FONT_HERSHEY_PLAIN,1,(255,255,0),1)
    else:
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
    now = datetime.datetime.now()
    #current_time = now.strftime("%H:%M:%S")
    #current_time=Char(current_time)
    if now.hour==7:
        if now.minute==49:
            nresult = firebase.delete('/','HS-da-den','-LJW0ehbbZxO8qL7y-FS')
    nresult = firebase.delete('https://so-do-4d921-default-rtdb.firebaseio.com/HS-da-den','-LJW0ehbbZxO8qL7y-FS')
    #if current_time
    
    


###cap.release()
#cv2.destroyAllWindows()