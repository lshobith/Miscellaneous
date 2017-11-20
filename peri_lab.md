# Peripherals Lab Report

## mqtt

we have used paho-mqtt for connecting Raspberry Pi to mqtt broker. To install it in Python run the following command.
```
$ pip install paho-mqtt
```

we have installed mqtt broker in Raspberry Pi itself. In order to communicate with Raspberry Pi we need another mqtt device. So, we have installed `MQTT Dashboard` app from Android store.

create username and password for mqtt broker. we have used `starboy` and `godfather` as username and password for mqtt broker. mqtt broker uses `1883` as the port for communication.

since mqtt broker is also in raspberry pi, the python code can communicate with mqtt broker using `127.0.0.1` as hostname, `1883` as port, username and password.

we used wifi adapter to connect Raspberry pi to `hotspot` and connected android phone to the same hotspot. The ip address given by the hotspot to raspberry pi is `192.168.43.108`. MQTT Dashboard can communicate with mqtt broker using `192.168.43.108` as hostname, `1883` as port, username and password.

for Python to communicate with mqtt broker we need to import a package
```python
import paho.mqtt.client as mqtt
```

then we need to start a client
```python
test_client = mqtt.Client()
```

then we need assign some functions to the client as follows
```python
test_client.on_publish = on_publish
test_client.on_message = on_message
test_client.on_connect = on_connect
```
`on_publish()` does nothing
```python
def (mosq, userdata, mid):
  pass
```

`on_connect()` prints a message to console and connect to the topic `swa_news`.
```python
def on_connect(client, userdata, flags, rc):
  print("Connected with result code "+str(rc))
  client.subscribe("swa_news")
```

we need to provide username, password and then connect using ip and port.
```python
test_client.username_pw_set("starboy", password="godfather")
test_client.connect("127.0.0.1", 1883, 60)
```

then we loop forever.
```python
test_client.loop_forever()
```
what this does is start listening to mqtt messages asynchronously.

when to connect to mqtt broker, on_connect() function is executed.

whenever we publish something, on_publish() function is executed.

whenever we receive a message, on_message() function is executed.

to publish a message the following must be done.
```python
test_client.publish("topic", "message")
```
where `topic` and `message` must be replaced with your own topic and message.

The weighing machine has 3 modes and there is a function for each one of them.
```python
mode_one() # for mode one.
mode_two() # for mode two.
mode_three() # for mode three.
```

mode one is for adding a single object or viewing the details of a single object. It publishes weight and RFID tag to the topic-`reverse`.

mode two is for getting the number of items, if weight of single item of same type exists in the database. It published number of items to topic-`reverse.`

mode three is for monitoring the weight of something. A Threshold value to used by this mode. If its weight is more than the value, buzzer doesn't make any noise and the message `safe` is published to the topic `warning`. If its weight is less than the value, buzzer stats making noise and the message `critical` is published to the topic `warning`.

the messages that can be sent to the topic-`swa_news` are `1`, `2` and `3`.

When `swa_news` receives message, it triggers `on_message()` function in the python and runs mode 1 or 2 or 3 accordingly.
```python
def on_message(client, userdata, msg):
  print("["+msg.topic+"]: "+str(msg.payload))
  a_string = str(msg.payload)
  if (a_string == "1"):
    mode_one()
  elif (a_string == "2"):
    mode_two()
  elif (a_string == "3"):
    mode_three()
```
