# GPIO C Sample

This sample interacts with GPIOs, in C, through `libgpiod2` .

In this sample, if no GPIO pin is defined, it lists the GPIO pins on all GPIO
chips, and if the pin is defined it toggles the pin on and off.

In the docker-compose file, we expose all devices in the `/dev` folder and grants access permissions only for those in the gpio cgroup (254). 

```yaml
    device_cgroup_rules:
      - "c 254:* rmw"
    volumes:
      - type: bind
        source: /dev
        target: /dev
```

While this ensures that the container has access to the GPIO pin you are going to use, for production, for production it's a best practice to give access only to the devices required by your application.

In another words, replace the above block for the device tag:

```yaml
    devices:
      - "/dev/gpiochip0" # for gpiochip0 gpio bank
```