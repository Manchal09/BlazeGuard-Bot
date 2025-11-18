# BlazeGuard-Bot
BlazeGuard Bot is an autonomous fire-fighting robotic system designed to detect, locate, and extinguish fire hazards using smart sensor technology. It integrates flame sensors, an MQ2 smoke/gas sensor, a servo-based directional scanning system, and a water pump module for extinguishing fire. The bot additionally sends real-time SMS alerts using the SIM800L GSM module, ensuring fast emergency response even when no one is present.

This project focuses on creating a low-cost, portable, and intelligent fire safety system ideal for homes, labs, and industrial spaces.

📌 **Features**
🔥 Fire Direction Detection using multiple flame sensors
🧪 Smoke/Gas Detection using the MQ2 sensor
🔄 Autonomous Rotation & Navigation using a servo motor & motor driver
🚿 Automatic Water Pump Activation through a relay module
📩 SMS Alert System using SIM800L GSM module
🔋 Battery-Powered Operation using 18650 Li-ion cells
🔌 Stable Power Output using LM2596 buck converter
🤖 Fully Autonomous Fire-Fighting Behavior

🧰 **Technology & Tools Used**
🛠️ Hardware
Arduino Uno
Flame Sensors (3-way fire direction detection)
MQ2 Smoke/Gas Sensor
Servo Motor
GSM Module – SIM800L
Relay Module
Water Pump + Pipe Nozzle
18650 Battery Pack
LM2596 Voltage Regulator
Chassis + Motors + Wheels
💻 Software
C++
Arduino IDE

🚀 **How It Works**

BlazeGuard Bot continuously monitors its environment and operates through the following steps:

1️⃣ Initialization
Sensors (flame + MQ2) calibrated
GSM module connected to the network
Servo set to initial position
Motors and pump prepared

2️⃣ Fire Monitoring
Flame sensors continuously scan for heat signatures
MQ2 sensor monitors for smoke/gas levels
When threshold is reached → Fire Detected

3️⃣ Direction Identification
Sensor with highest flame intensity indicates fire direction
Servo rotates the bot toward that direction

4️⃣ Navigation
Bot moves toward the fire source
Stops at an optimum safe distance

5️⃣ Fire Extinguishing
Relay activates the water pump
Water sprayed directly at flame
Pump stops once flame sensors report no fire

6️⃣ SMS Alert
Using SIM800L, the bot sends:
ALERT: Fire detected and extinguishing activated.
Location: [Custom/Static Location]
Status: ACTIVE

7️⃣ Safety & Fail-Safe
Low battery alert
Water tank low alert
Fire not extinguished → Escalation SMS
Sensor errors → Diagnostic SMS

📈 **Results**
Fire detected in under 3–5 seconds
Servo aligns the bot to fire direction within 1 second
Pump begins extinguishing within 8–10 seconds of detection
SMS alert sent within 15–20 seconds
Successfully extinguished small controlled flames during testing
System stable under battery-powered operation

📚 **Applications**
Home & Kitchen Safety
Laboratories
Industrial Warehouses
Server Rooms
Schools & Offices
Robotics Fire-Safety Training
