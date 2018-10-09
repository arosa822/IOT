
import serial # import Serial Library
import numpy  # Import numpy
import matplotlib.pyplot as plt #import matplotlib library
from drawnow import *
 
Sensor1= []
#Sensor2=[]
#Sensor3=[]
#Sensor4=[]
arduinoData = serial.Serial('/dev/ttyUSB0') #Creating our serial object named arduinoData
plt.ion() #Tell matplotlib you want interactive mode to plot live data
cnt=0

def makeFig(): #Create a function that makes our desired plot
    plt.ylim(0,1000)                                 #Set y min and max values
    plt.title('My Live Streaming Sensor Data')      #Plot the title
    plt.grid(True)                                  #Turn the grid on
    plt.ylabel('AnalogPin')                            #Set ylabels
    plt.plot(Sensor1, 'ro-', label='Sensor 1')       #plot the temperature
    #plt.plot(Sensor2, 'b^-', label='Sensor 2')
    plt.legend(loc='upper left')                    #plot the legend
    #plt2=plt.twinx()                                #Create a second y axis
    plt.ylim(0,1000)                           #Set limits of second y axis- adjust to readings you are getting

    #plt2.set_ylabel('T & H')                    #label second y axis
    #plt2.plot(Sensor3, 'ro-', label='Sensor 3')       #plot the temperature
    #plt2.plot(Sensor4, 'b^-', label='Sensor 4')
    #plt2.ticklabel_format(useOffset=False)           #Force matplotlib to NOT autoscale y axis
    #plt2.legend(loc='upper right')                  #plot the legend
    
 
while True: # While loop that loops forever
    while (arduinoData.inWaiting()==0): #Wait here until there is data
        pass #do nothing
    arduinoString = arduinoData.readline() #read the line of text from the serial port
    
    dataPiece = arduinoString.split()
    s1 = dataPiece[0]
    #s2 = dataPiece[2]
    #s3 = dataPiece[4]
    #s4 = dataPiece[6]
    
    sens1 = float(s1)            #Convert first element to floating number and put in temp
    #sens2 =    float(s2)            #Convert second element to floating number and put in P
    Sensor1.append(sens1)                     #Build our Sensor1 array by appending temp readings
    #Sensor2.append(sens2)                     #Building our Sensor2 array by appending P readings
    
    drawnow(makeFig)                       #Call drawnow to update our live graph
    plt.pause(.000001)                     #Pause Briefly. Important to keep drawnow from crashing
    cnt=cnt+1

    
    if(cnt>50):                            #If you have 50 or more points, delete the first one from the array
        Sensor1.pop(0)                       #This allows us to just see the last 50 data points
        #Sensor2.pop(0)
    
    

   