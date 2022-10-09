![SelicTrackerCapa](https://user-images.githubusercontent.com/64144293/194761774-0ff0c09b-7ab9-4f65-8882-813ff48bba8e.png)

> Status: Developing ⚠️

## An embedded application for monitoring the Brazilian base rate

# Requiriments
* Notify you when the Selic rate is lower than a certain amount
* Visual alarm
* Alarm sound

# Design
* Connect to the internet and search a database for the current rate
* Shows in an LED Display the current value of this rate
* Use an LED to visually notify that the rate is less than the specified amount
* Use an buzzer to audibly notify you that the rate is less than the specified value

## Architecture
Is used an ESP32 development board as a server at first as a captive portal to get network informations like SSID and password. After that, the product is able to connect to WiFi and assume the role of client to make http request to a database. This database makes the data available through a JSON file. The information of current Selic data will be show on a LED display together with her last atualization date. With the information of current Selic rate will be make a comparsion between this data and the value confugured as a target. If the rate is less than the configured target, it will trigger the alarm system that will keep playing until someone press the button SW1.

<div align = "center">
  <img src ="https://user-images.githubusercontent.com/64144293/194775605-2da374da-28e7-4003-8fae-ebe4bd0a946a.png" width = "600px" />
</div>

# Implementation
## Software Tasks
- [ ] Captive portal
- [x] WiFi connection logic
- [x] http request to a data base
- [x] JSON parse
- [x] Comparsion between the data and the target
- [x] Display logic
- [ ] Alarm logic

## Hardware Tasks
- [x] Schematic project
- [x] Board routing
- [ ] Prototyping

<div align = "center">
  <img src ="https://user-images.githubusercontent.com/64144293/194765383-223f35c0-eb69-4d84-bb74-3a62f55f5691.jpeg" width = "600px" />
</div>

# Verification
- [x] Protoboard test
- [ ] Prototype test

# Maintenance

Not yet applicable.

