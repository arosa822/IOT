import time
import requests
import math
import random

TOKEN = ""  # Put your TOKEN here

VARIABLE_LABEL_1 = "temperature"  # Put your first variable label here
VARIABLE_LABEL_2 = "humidity"  # Put your second variable label here
VARIABLE_LABEL_3 = "position"  # Put your second variable label here

#specify number of devices for each location
numDevice = 5

#generate multiple devices for 2 different locations
LOC1 =[]
LOC2 =[]
for i in range(numDevice):
    LOC1.append('Loc1_'+ str(i))
    LOC2.append('Loc2_' + str(i))

def build_payload(variable_1, variable_2, variable_3):
    # Creates two random values for sending data
    value_1 = random.randint(-10, 50)
    value_2 = random.randint(0, 85)

    # Creates a random gps coordinates
    lat = random.randrange(34, 36, 1) + \
        random.randrange(1, 1000, 1) / 1000.0
    lng = random.randrange(-83, -87, -1) + \
        random.randrange(1, 1000, 1) / 1000.0
    payload = {variable_1: value_1,
               variable_2: value_2,
               variable_3: {"value": 1, "context": {"lat": lat, "lng": lng}}}

    return payload


def post_request(payload, DEVICE_LABEL):
    # Creates the headers for the HTTP requests
    url = "http://things.ubidots.com"
    url = "{}/api/v1.6/devices/{}".format(url, DEVICE_LABEL)
    headers = {"X-Auth-Token": TOKEN, "Content-Type": "application/json"}

    # Makes the HTTP requests
    status = 400
    attempts = 0
    while status >= 400 and attempts <= 5:
        req = requests.post(url=url, headers=headers, json=payload)
        status = req.status_code
        attempts += 1
        time.sleep(1)

    # Processes results
    if status >= 400:
        print("[ERROR] Could not send data after 5 attempts, please check \
            your token credentials and internet connection")
        return False

    print("[INFO] request made properly, your device is updated")
    return True

def main():
    for i in range(numDevice):
        #generate data for location 1
        payload = build_payload(
            VARIABLE_LABEL_1, VARIABLE_LABEL_2, VARIABLE_LABEL_3)

        print("[INFO] Attemping to send data")
        post_request(payload,LOC1[i])
        print("[INFO] finished")
        
        #generate data for location 2
        payload = build_payload(
            VARIABLE_LABEL_1, VARIABLE_LABEL_2, VARIABLE_LABEL_3)
        
        print("[INFO] Attemping to send data")
        post_request(payload,LOC2[i])
        print("[INFO] finished")

if __name__ == '__main__':
    while (True):
        main()
        time.sleep(1)