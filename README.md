# ESP32FireDetectorIoT

https://github.com/exgael/ESP32FireDetectorIoT.git

Il y a une problem de memoire qui arrive de temps en temps en debut d execution. S'il n'arrive pas tot, alors pas de probleme. Je n'ai pas encore fait toutes les `pre-conditions/post-conditions check` pour trouver la cause. 

I found the error. global renaming affected external libraries.

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
