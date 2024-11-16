# ESP32FireDetectorIoT

https://github.com/exgael/ESP32FireDetectorIoT.git

## **Specification Update** (TODO)

1. **Device Update**  
   The system now monitors pool temperatures using a **smart thermometer**

2. **Purpose**  
   The device is designed for **pool temperature surveillance**.  

3. **"Hotspot" Indicator**  
   - A "hotspot" is determined **on the edge** by the ESP microcontroller.  
   - If the pool is classified as a hotspot, the ESP activates its internal LED (connected to Pin 2); otherwise, the LED remains off.  
   - The decision is based on the latest temperature values published on the broker's temperature topic.  

4. **"Occupied" Indicator**  
   - An **occupancy status** is also calculated **on the edge** using a local light sensor.  
   - If the light sensor detects an intensity below a certain threshold, it indicates the area is occupied (`"occupied" = true`).  
   - Otherwise, the pool is considered unoccupied.

## Run CURL script 
```bash
chmod +x ./run_endpoint
./run_endpoint.sh
```
## JSON Validator Setup

### Step 1: Create a Virtual Environment
```bash
python3 -m venv json-validator-env

# Activate the virtual environment
# On Windows
json-validator-env\Scripts\activate

# On macOS/Linux
source json-validator-env/bin/activate
```

### Step 2: Install Required Packages
```bash
pip install -r requirements.txt
```

### Step 3: Running the JSON Validator

- Replace the content of `eps_output.json`.
To run the JSON validator, use the following command:
```bash
python val.py
```

### Step 4: Deactivating the Virtual Environment
```bash
deactivate
```

## Node-RED Setup

### Install Node-RED + Dashboard UI Library
```bash
cd ~/.node-red
npm install node-red-dashboard
npm install node-red-contrib-ui-led
```
ui can be found on `http://localhost:1880/ui`
