import sys
import binascii
import struct
import time
from bluepy.btle import UUID, Peripheral
import datetime
import logging
import paho.mqtt.client as paho

#define callback
def on_message(client, userdata, message):
    time.sleep(1)
    print("received message =",str(message.payload.decode("utf-8")))
	
client2=paho.Client("Smart-Agriculture")
ts_broker="127.0.0.1"
#create client object client1.on_publish = on_publish 
#assign function to callback client1.connect(broker,port) 
#establish connection client1.publish("house/bulb1","on")
######Bind function to callback
client2.on_message=on_message  
#print("Connecting to broker ",mq_broker)

deviceID="NXP_onboard"

service_uuid = UUID("1305b3ca-096e-4366-9f68-1ae8df01f278")
char_light_uuid = UUID("1305b3ca-096e-4366-9f68-1ae8df01f27b")
char_temp_uuid = UUID("1305b3ca-096e-4366-9f68-1ae8df01f279")
char_humid_uuid = UUID("1305b3ca-096e-4366-9f68-1ae8df01f27a")
char_pressure_uuid = UUID("1305b3ca-096e-4366-9f68-1ae8df01f27c")
char_co2_uuid = UUID("bec2f36b-e4c4-455d-90a4-25b0e8c255b5")
char_voc_uuid = UUID("1305b3ca-096e-4366-9f68-1ae8df01f27d")


deviceFrequency=30

#takes input of device adress and if no adress present then logs error
if len(sys.argv) != 2:
    print("Fatal, must pass device address:", sys.argv[0], "<device address="">")
    quit()

p = Peripheral(sys.argv[1])
print("Connected")

#for service in p.getServices():
#    print(service.uuid)

ButtonService=p.getServiceByUUID(service_uuid)
print("Got service")
try:
	ch_light = ButtonService.getCharacteristics(char_light_uuid)[0]
	ch_temp = ButtonService.getCharacteristics(char_temp_uuid)[0]
	ch_humid = ButtonService.getCharacteristics(char_humid_uuid)[0]
	ch_pressure = ButtonService.getCharacteristics(char_pressure_uuid)[0]
	ch_voc = ButtonService.getCharacteristics(char_voc_uuid)[0]
	ch_co2 = ButtonService.getCharacteristics(char_co2_uuid)[0]

	print("Got characteristics")
	while 1:
		print("read")
		
		lightVal = str(struct.unpack('<f', ch_light.read())[0])
		tempVal = str(struct.unpack('<f', ch_temp.read())[0])
		humidVal = str(struct.unpack('<f', ch_humid.read())[0])
		PressureVal = str(struct.unpack('<f', ch_pressure.read())[0])
		co2Val = str(struct.unpack('<f', ch_co2.read())[0])
		vocVal = str(struct.unpack('<f', ch_voc.read())[0])

		print ("Lux: ",lightVal)
		print ("Temp: ",tempVal)
		print ("Humidity: ",humidVal)
		print ("Pressure: ",PressureVal)
		print ("CO2_val: ",co2Val)
		print ("tVOC: ",vocVal)

		#####
		print("connecting to broker ",ts_broker)
		#client2.username_pw_set(username="admin",password="admin")
		client2.connect(ts_broker,1887)#connect
		client2.loop_start() #start send and receive of messages
		
		print("publishing ")
		toPublish=deviceID+";"+"LightVal="+lightVal+";"+"Temp="+tempVal+";"+"Humidity="+humidVal+";"+"AP="+PressureVal+";"+"CO2="+co2Val+";"+"VOC="+vocVal
		DataToPublish=str(toPublish)

		#publish
		
		client2.publish("MF/temp",tempVal)
		client2.publish("MF/humid",humidVal)
		client2.publish("MF/ap",PressureVal)
		client2.publish("MF/voc",vocVal)
		client2.publish("MF/co2",co2Val)
		client2.publish("MF/light",lightVal)

		

		#time.sleep(2)
		
		client2.disconnect() #disconnect
		client2.loop_stop() #stop loop
		time.sleep(deviceFrequency)
finally:
	print("will try again")