# Light Dip Detector BeagleBone
Detect the number of light dips with your BeagleBone

## Pre-requisite
1. Install photoresistor light sensor

## Installation
1. Clone the repository
```bash
(host)$ git clone https://github.com/Tooo/Light-Dip-Detector-BeagleBone.git
```

2. Make executable file on the host
```bash
(host)$ make
```

3. Run executable on the BeagleBone
```bash
(bbg)$ ./light_sampler
```

## Connect to UDP Server
```bash
(host)$ netcat -u 192.168.7.2:12345
```

## Features
- Detect the number of light dips from the light sensor.
- Change the light sampling buffer size with the potentiometer.
- Display the number of dips on LED 2-character display.
- Display stats every second to the terminal.
- Listen to UDP server to respond to commands.

## Commands
| Command | Description | 
| :------ | :------ |
| help | Display accepted commands |
| count | Display total number of samples taken. |
| length | Display number of samples in history (both max and current). |
| history | Display the full sample history being saved. |
| get (number) | Display the (number) most recent history values. |
| dips | Display number of dips. |
| stop | Cause the server program to end. |
| `<enter>` | Repeat last command. |


## Threads
| Threads | Purpose | 
| :------ | :------ |
| Main | Start other threads and waits for shutdown to cleanup |
| Light Sampler | Samples the light from the light sensor every 1ms |
| Potentiometer Inputer | Gets the input from the potentiometer every 1s |
| Dip Detector | Calculate and set the number of dips to the LED Digit display every 100ms |
| Digit Display | Change the LED Digit display to the correct number with changing memory of I2C |
| Terminal Output | Display terminal output of stats and timing jittering every 1s |
| UDP Listener | Listen on the UDP server and respond to commands |
