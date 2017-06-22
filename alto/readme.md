# Alto
Particle Photon + ADC121C021_PIZO Vibration Sensor

<img src="../images/alto.jpg" width="400" />

This sensor will monitor a number of knocks given a knock threshold. It will send data to ChoralAllegro for processing. The json format is as follows:
```
{
    "device_id":"3df97c177d96280618348dbc7723d33870034e4e", //sha-1 hash of "vibration sensor"
    "user_secret":"secret",
    "device_data": {
        "knocks":"k1"
    },
    "device_timestamp":123456789
}
```

### Setup
1. Connect Particle Photon to wifi using [this setup guide]
2. Create a Particle account and go to the [Web IDE]
3. Load alto.ino

[this setup guide]: <https://docs.particle.io/guide/getting-started/start/photon/#step-1-power-on-your-device>
[Web IDE]: <https://build.particle.io>