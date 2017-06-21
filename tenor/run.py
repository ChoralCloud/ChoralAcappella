# Distributed with a free-will license.
# Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
# TSL45315
# This code is designed to work with the TSL45315_IS2C I2C Mini Module available from ControlEverything.com.
# https:#www.controleverything.com/content/Light?sku=TSL45315_I2CS#tabs-0-product_tabset-2

import smbus
import time
import json
import requests

# device data
device_id = "8ba5f37f7745f8e450b9a9f42534eb983099854a"
user_secret = "secret"
url = "http://192.168.0.14:3000/"

# Get I2C bus
bus = smbus.SMBus(1)

def sense():
    # TSL45315 address, 0x29(41)
    # Select Control register, 0x00(0), with Command register, 0x80(128)
    #		0x03(03)	Normal operation
    bus.write_byte_data(0x29, 0x00 | 0x80, 0x03)
    # TSL45315 address, 0x29(41)
    # Select Configuration register, 0x01(1), with Command register, 0x80(128)
    #		0x00(00)	Multiplier 1x, Tint : 400ms
    bus.write_byte_data(0x29, 0x01 | 0x80, 0x00)

    time.sleep(0.5)

    # TSL45315 address, 0x29(41)
    # Read data back from 0x04(4), with Command register, 0x80(128)
    # 2 bytes, LSB first
    data = bus.read_i2c_block_data(0x29, 0x04 | 0x80, 2)

    # Convert the data to lux
    luminance = data[1] * 256 + data[0]
    return luminance
    
def emit(luminance):
    payload = {
        "device_id":device_id, \
        "user_secret":user_secret, \
        "device_data": { \
            "luminance":luminance, \
        }, \
        "device_timestamp":int(round(time.time() * 1000)) \
    }
    print payload    
    r = requests.post(url, json=payload)
    print r.status_code

# Output data to screen
while (True):
    luminance = sense()
    emit(luminance)
    print "Ambient Light Luminance : %d lux" %luminance
    time.sleep(1.0)
