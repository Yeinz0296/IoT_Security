import requests
import time
from datetime import datetime

url = "http://apiv2.favoriot.com/v2/streams"
api_key = "XY0ocqWVqjhYrENup9x8bpsiT71015In"
content_type = "application/json"
devicedeveloperid = "deviceDefault@Yein0296"
msg = "Haii"

header={
    'apikey':api_key, 
    'Content-type':content_type
    }

json = {
    'device_developer_id':devicedeveloperid, 
    'data':{
        'message': msg
        }
    }

def connect_http():
    res = requests.post(url,headers=header, json=json)
    if res:
        print("Connected to Victim data!")
    else:
        print("Connection Victim data failed! Error: ", res.status_code)
    
    return res

def attack(res):
    while True:
        time.sleep(1)
        now = datetime.now()
        t = now.strftime("%H:%M:%S")

        if res:
            print(t, res.text, "Message sent is {}".format(msg))
        else:
            print(res.text)

def main():
    res = connect_http()
    attack(res)

main()

