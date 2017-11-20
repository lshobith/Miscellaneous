# Peripherals Lab Report
## mqtt
we have used paho-mqtt for connecting Raspberry Pi to mqtt broker. To install it in Python run the following command.
```
$ pip install paho-mqtt
```
we have installed mqtt broker in Raspberry Pi itself. In order to communicate with Raspberry Pi we need another mqtt device. So, we have installed `MQTT Dashboard` app from Android store.
create username and password for mqtt broker. we have use `username` and `password` as username and password for mqtt broker. mqtt broker uses `1883` as the port for communication.
since mqtt broker is also in raspberry pi, the python code can communicate with mqtt broker using `127.0.0.1` as hostname, `1883` as port, username and password.
