import time
from datetime import datetime
from random import randint
import paho.mqtt.client as mqtt

broker = 'mqtt3.thingspeak.com'
port = 1883
topic = "channels/1778922/publish"
client_id = 'LAcWPTAQPSsINScALBArHQg'
username = 'LAcWPTAQPSsINScALBArHQg'
password = 'KZ3WGWUvNZSUeK6djIyKO0zk'

def connect_mqtt():
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code {}\n".format(rc))

    client = mqtt.Client(client_id)
    client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client

def publish(client):
    while True:
        time.sleep(1)
        now = datetime.now()
        t = now.strftime("%H:%M:%S")

        ran1, ran2 = randint(1, 1000), randint(1, 1000)

        msg = "field1={}&field2={}&status=MQTTPUBLISH".format(ran1, ran2)

        result = client.publish(topic, msg)
        status = result[0]
        if status == 0:
            print("{} Send `{}` to topic `{}`".format(t, msg, topic))
        else:
            print("Failed to send message to topic {}".format(topic))
        

def run():
    client = connect_mqtt()
    client.loop_start()
    publish(client)

run()
