var client;

var message;
var outMessage;
var inpData=0;
var outData="";

startConnection();



function startConnection() {

    client = new Paho.MQTT.Client("217.71.129.139", Number(4060), "root");
    //client = new Paho.MQTT.Client("172.17.2.246", Number(1884), "root");

    client.onConnectionLost = onConnectionLost;

    client.onMessageArrived = onMessageArrived;

    client.connect({onSuccess:onConnect});    
   

}



function onConnect() {

  // Once a connection has been made, make a subscription.

  console.log("onConnect");

  client.subscribe("iot/morozov/fromNode");
  client.subscribe("iot/morozov/toNode");


};

function onConnectionLost(responseObject) {

  if (responseObject.errorCode !== 0)

  console.log("onConnectionLost:"+responseObject.errorMessage);

};

function onMessageArrived(message) {
  inpData = JSON.parse( message.payloadString);
  try
  {
    console.log("onMessageArrived:"+message.payloadString);
    //вывод на экран
    var theElement = d.getElementById("componentNum");
    // ЗДЕСЬ ЗАДАЕТСЯ НОМЕР УСТРОЙСТВА
    if (inpData.find != "3" ) 
    {
    //theElement.innerHTML = "Число компонент = " + inpData.count + ", номер метки = " + inpData.label;
    d.getElementById('count').value = inpData.count;
    d.getElementById('label').value = inpData.label;

    
  
    }
  }  
  catch (err) {}
  //client.disconnect();

};

function messageTransmite(outData) {
  //MqttMessage mes = new MqttMessage();
  try
  {
  outMessage = new Paho.MQTT.Message("{\"find\": 1, \"label\":\""  + outData +"\"}");
  outMessage.destinationName = "iot/morozov/toNode";
  client.send(outMessage);
  console.log("Serch: "+ outMessage);
  }
  catch (err) {}

  //client.disconnect();

};

function findLabel()   
{
    outData= d.getElementById('outLabel').value;
    messageTransmite(outData);
}

function findResistor()   
{
    outData = "45eb95e5"; //resistor label
    messageTransmite(outData);
}
function findСapacitor()   
{
    outData = "45eb95e5"; //capacitor label
    messageTransmite(outData);
}

function findStop()   
{
  outMessage = new Paho.MQTT.Message("{\"find\": 0, \"label\":\"\"}");
  outMessage.destinationName = "iot/morozov/toNode";
  client.send(outMessage);
  console.log("Stop serch");

}