# IoT Car Controller

## Overview
The IoT Car Controller is a project that allows users to control a car remotely using MQTT and visualize data in real-time using a web interface. The project integrates various technologies, including Flask for the web server, MQTT for communication, and Chart.js for data visualization.

## Features
- Control the car's movement (forward, backward, left, right, stop, rotate) via buttons or voice commands.
- Real-time data visualization of distance measurements using Chart.js.
- Alerts for obstacle detection based on distance data.
- Speech recognition for controlling the car using voice commands.

## Technologies Used
- **Flask**: A lightweight WSGI web application framework for Python.
- **MQTT**: A lightweight messaging protocol for small sensors and mobile devices optimized for high-latency or unreliable networks.
- **Chart.js**: A JavaScript library for creating beautiful charts and graphs.
- **Paho MQTT**: A Python client library for MQTT.
- **SpeechRecognition**: A library for performing speech recognition, with support for several engines and APIs.

## Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/iot-car-controller.git
   cd iot-car-controller
