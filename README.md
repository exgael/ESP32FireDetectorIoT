# ESP32FireDetectorIoT

https://github.com/exgael/ESP32FireDetectorIoT.git

Points à améliorer :
1. Ajouter des validations middleware pour les entrées.
2. Utiliser le dossier `include` (j'ai utilisé des `index.h` comme en serveur côté JS/TS).
3. Une erreur survient lorsqu'une demande de reporting est lancée à un moment spécifique de la boucle.
4. J'ai un mélange de `snake_case` et de `CamelCase`; j'ai fait la transition vers `CamelCase`, mais il en reste encore un peu.

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
