# Peripherals Lab Report

## mqtt

we have used paho-mqtt for connecting Raspberry Pi to mqtt broker. To install it in Python run the following command.
```
$ pip install paho-mqtt
```

we have installed mqtt broker in Raspberry Pi itself. In order to communicate with Raspberry Pi we need another mqtt device. So, we have installed `MQTT Dashboard` app from Android store.

create username and password for mqtt broker. we have use `username` and `password` as username and password for mqtt broker. mqtt broker uses `1883` as the port for communication.

since mqtt broker is also in raspberry pi, the python code can communicate with mqtt broker using `127.0.0.1` as hostname, `1883` as port, username and password.

we used wifi adapter to connect Raspberry pi to `hotspot` and connected android phone to the same hotspot. The ip address given by the hotspot to raspberry pi is `192.168.43.108`. MQTT Dashboard can communicate with mqtt broker using `192.168.43.108` as hostname, `1883` as port, username and password.

for Python to communicate with mqtt broker we need to import a package
```python
import paho.mqtt.Client as mqtt
```

then we need to start a client
```python
mqtt_client = mqtt.Client()
```

then we need assign some functions to the client as follows
```python
mqtt_client.on_publish = on_publish()
mqtt_client.on_message = on_message()
mqtt_client.on_connect = on_connect()
```

we need to provide username, password and then connect using ip and port.
```python
some = code
```

then we loop forever.
```python
mqtt_client.loop_forever()
```
what this does is start listening to mqtt messages asynchronously.

when to connect to mqtt broker, on_connect() function is executed.

whenever we publish something, on_publish() function is executed.

whenever we receive a message, on_message() function is executed.

to publish a message the following must be done.
```python
mqtt_client.publish("topic", "message")
```
where `topic` and `message` must be replaced with your own topic and message.

The 
