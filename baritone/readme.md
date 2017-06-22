# Baritone
Particle Photon + TSL45315 Light Sensor

<img src="../images/baritone.jpg" width="400" />

This sensor will monitor a room's luminance every second. It will send data to ChoralAllegro for processing. The json format is as follows:
```
{
    "device_id":"8ba5f37f7745f8e450b9a9f42534eb983099854a", //sha-1 hash of "light sensor"
    "user_secret":"secret",
    "device_data": {
        "luminance":"l1"
    },
    "device_timestamp":123456789
}
```

### Setup
1. Connect Particle Photon to wifi using [this setup guide]
2. Create a Particle account and go to the [Web IDE]
3. Load baritone.ino

[this setup guide]: <https://docs.particle.io/guide/getting-started/start/photon/#step-1-power-on-your-device>
[Web IDE]: <https://build.particle.io>