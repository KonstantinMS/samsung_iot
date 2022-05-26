import paho.mqtt.client as mqtt
import json
import datetime
import random
import secrets

from time import sleep
# Создание клиента MQTT
client = mqtt.Client()
#json.loads(msg.payload)
# Подключение к брокеру
client.connect("217.71.129.139", 4019, 60)
sleep(5)
while(1):
	# Складываем данные в JSON
	mytemp = random.randint(1, 100)
	#myLabel = random.randi(1, 255)
	#myLabel = secrets.token_hex(8)
	now = datetime.datetime.now()
	#data_to_send1 = json.dumps({ "data": {"temperature":mytemp, "humidity":0, "luminosity":0}}) #	data_to_send = json.dumps({"data": 1000}) 
	
	#data_to_send1 = json.dumps({"id": 1, "count":"80", "label":"a6cfcf1f"})
	data_to_send1 = json.dumps({"find": 0, "label":"FFFFFF02"}) #	data_to_send = json.dumps({"data": 1000}) 
	data_to_send2 = json.dumps({ "data": {"temperature":mytemp, "humidity":0, "luminosity":0}})
	print(data_to_send1)
	#temperature = json.loads(msg.payload)['data'][ 'temperature']
	# Публикуем данные
	client.publish(("iot/morozov/toNode"), data_to_send1)
	#client.publish(("iot/morozov/def2"), data_to_send2)
	#client.publish(topic, payload=data)
	# Пауза 1 секунда
	sleep(5)