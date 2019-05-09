# Smart-Agriculture
Project liscenced under: CC BY-NC-SA

This project's more information: http://bit.do/saWeb.
This repository currently contains code for presentation at MakerFaire Bay Area 2019: http://bit.do/saMF.

The code for Raspberry Pi whixh acts as a local server is in Raspberry Pi folder. You will need to flash an SD card with Raspbian Streatch Lite from Website and connect to local network.
Though you need to follow steps of some installations in 'Things required for RaspberryPi.txt'
After its installation you need to run the commands as mentioned in txt.
The server to handle the requests is the Mosquitto Broker here.
Also you need to open NodeRed panel from <pi_ip>:1880 and install NodeRed Dashboard for offline information. Then go to import > Clipboard > upload JSON. Upload the JSON 'NodeRed flow'. If you have set passwords for MQTT broker add it to the fields.

For the ESP8266 which has Soil Moisture Sensor attached along with the pump according to 'schematics_pw' upload the code to the board vailable in Arduino folder. Don't forget to change SSID and PSK of your WiFi and also IP adress/port/user/pass of your broker i.e., RPi.

For the plant disease detection the code for training the model and inference is in form of Google Colab Jupyter NB which can be easily run on cloud for free. The DataSet of images and some checkpoints are available here: http://bit.do/eRYrt. 
You can download the weights and get simple tensorflow Raspberry Pi script to detect plant disease. Some detected images are in Test_Images folder.

The Website has frontend in HTML which is embedding the information from Thingspeak and has backed in NodeJS. It also gets the live camera view from Raspberry Pi on running 'streamCamera.py' and changing the IP address and Broker (MQTT) as well. You can also control the Robot via the website.

For thingspeak the place where are data will be stored. Create account on https://thingspeak.com and create new channel. Add 7 fields with different weather conditions and pump status. You also need to add widgets and change its src in the HTML code of website.

For ideas/suggesions: Twitter.com/Vimarsh244
