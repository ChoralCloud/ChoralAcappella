# Soprano
Rasberry Pi + BME280 Digital Humidity, Pressure, and Temperature sensor

![soprano](images/sopgrano.jpg)

This sensor will monitor a room's humidity, pressure, and temperature every second. It will send data to ChoralAllegro for processing. The json format is as follows:
```
{
    "device_id":"e8d5413873148e5fce79687c42429391bc14d113", //sha-1 hash of "temperature sensor"
    "user_secret":"secret",
    "data": {
        "humidity":"h1",
        "pressure":"p1",
        "temperature":"t1",
    },
    "timestamp":123456789
}
```

### Setup
1. Flash Raspberry Pi with Debian-lite
2. Follow this [raspberry pi zero guide]
3. `ssh pi@raspberrypi.local` with password `raspberry` and perform the below:
```
sudo apt-get update -y
sudo apt-get upgrade -y
sudo vim /boot/config.txt
    uncomment dtparam=i2c_arm=on
sudo raspi-config
    enable i2c
```
4. We will use Python and [smbus-cffi] to communicate between Rasberry Pi and sensor, so install dependencies
5. Download python file and place into `/home/pi/soprano/`
6. Run python file

License
----
Copyright (c) 2017 Choral

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

[smbus-cffi]: https://pypi.python.org/pypi/smbus-cffi/0.5.1
[raspberry pi zero guide]: https://davidmaitland.me/2015/12/raspberry-pi-zero-headless-setup/