 var client = new Messaging.Client("192.168.43.122",9004,"myclientid_" + parseInt(Math.random() * 10, 10));
 
 //Gets  called if the websocket/mqtt connection gets disconnected for any reason
 client.onConnectionLost = function (responseObject) {
     //Depending on your scenario you could implement a reconnect logic here
     alert("connection lost: " + responseObject.errorMessage);
 };

 //Gets called whenever you receive a message for your subscriptions
 client.onMessageArrived = function (message) {
     //Do something with the push message you received
	 var strT='<span>Topic: ' + message.destinationName + '  | ' + message.payloadString + '</span><br/>'
     //$('#messages').append('<span>Topic: ' + message.destinationName + '  | ' + message.payloadString + '</span><br/>');
	document.getElementById("show").innerHTML = strT;
	console.log(strT);
 };

 //Connect Options
 var options = {
     //useSSL:true,
         //Gets Called if the connection has sucessfully been established
     
     //Gets Called if the connection could not be established
     onFailure: function (message) {
         alert("Connection failed: " + message.errorMessage);
     }
 }

 //Creates a new Messaging.Message Object and sends it to the HiveMQ MQTT Broker
 var publish = function (payload, topic, qos) {
     //Send your message (also possible to serialize it as JSON or protobuf or just use a string, no limitations)
     var message = new Messaging.Message(payload);
     message.destinationName = topic;
     message.qos = qos;
     client.send(message);
 }