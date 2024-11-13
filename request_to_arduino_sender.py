import paho.mqtt.client as mqtt
import time

mqtt_broker = "broker.hivemq.com"  # Use the HiveMQ broker address
mqtt_port = 1883
mqtt_topic = "iot_crvt4722"  # Specify the same MQTT topic used in Arduino code

client = mqtt.Client()

def on_connect(client, userdata, flags, rc):
    print(f"Connected with result code {rc}")
    client.subscribe(mqtt_topic)

def on_message(client, userdata, msg):
    command = msg.payload.decode()
    print(f"Received command: {command}")
    control_car(command)

def control_car(command):
    # Add logic to control the car based on the received command
    if command == "forward":
        print("Moving forward")
        # Add your forward motion code here
    elif command == "backward":
        print("Moving backward")
        # Add your backward motion code here
    elif command == "left":
        print("Turning left")
        # Add your left turn code here
    elif command == "right":
        print("Turning right")
        # Add your right turn code here
    elif command == "stop":
        print("Stopping")
        # Add your stop code here
    elif command == "rotate":
        print("rotate")
    else:
        print("Unknown command")

client.on_connect = on_connect
client.on_message = on_message

client.connect(mqtt_broker, mqtt_port, 60)
client.loop_start()
def send_request_to_arduino(command):
    client.publish(mqtt_topic, command)