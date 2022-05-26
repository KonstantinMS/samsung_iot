# -*-coding: utf-8 -*-
import paho.mqtt.client as mqtt
import json
# Колбэк на подключение к брокеру
def on_connect(client, userdata, flags, rc):
	print("Connected with result code "+str(rc))
	# Подписка на топики
	client.subscribe("iot/morozov/#")
	#client.subscribe([("iot/morozov/def1",0), ("iot/morozov/def2",0)])
# Колбэк на получение сообщения
def on_message(client, userdata, msg):
	# Печать топика и содержания сообщения
	print(json.loads(msg.payload))
	try: 
		temperature = json.loads(msg.payload)['data'][ 'temperature']
		if ((temperature < 18) or (temperature > 17)) and msg.topic=="iot/morozov/def1":
			print("Выход за диапазон температуры деф1, темп="+str(temperature))
		if ((temperature < 15) or (temperature > 20)) and msg.topic=="iot/morozov/def1":
			print("Выход за диапазон температуры деф2, темп="+str(temperature))
		print("-----------------------------------------------------------------------")
	except:
		pass
	#print(msg.topic+" "+str(msg.payload))
	#print(temperature)
# Создание клиента
client = mqtt.Client()
# Привязка колбэков
client.on_connect = on_connect
client.on_message = on_message
# Подключение к брокеру по адресу и номеру порта, с указанием таймаута
client.connect("217.71.129.139", 4019, 60)
#client.connect("192.168.1.3", 1883, 60)
# Бесконечный цикл, в котором принимаются сообщения

client.loop_forever()

#