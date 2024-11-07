#!/bin/bash

# Base URL of the server
BASE_URL="http://192.168.0.34"

echo "Testing GET / (Root)"
curl "$BASE_URL/"
echo -e "\n"

# Testing /value endpoint with all parameters at once
echo "Testing GET /value with all parameters at once"
curl "$BASE_URL/value?temperature&light&running&fire&uptime&cooler&FTT&FLT&lt&ht"
echo -e "\n"

# Testing specific commands on /set endpoint

# Turn cooler on
echo "Turning cooler ON"
curl "$BASE_URL/set?cool=on"
echo -e "\n"
sleep 1  # 1-second delay after on/off request

# Turn cooler off
echo "Turning cooler OFF"
curl "$BASE_URL/set?cool=off"
echo -e "\n"
sleep 1  # 1-second delay after on/off request

# Turn heater on
echo "Turning heater ON"
curl "$BASE_URL/set?heat=on"
echo -e "\n"
sleep 1  # 1-second delay after on/off request

# Turn heater off
echo "Turning heater OFF"
curl "$BASE_URL/set?heat=off"
echo -e "\n"
sleep 1  # 1-second delay after on/off request

# Set low temperature threshold
echo "Setting low temperature threshold to 15"
curl "$BASE_URL/set?lt=15"
echo -e "\n"

# Set high temperature threshold
echo "Setting high temperature threshold to 25"
curl "$BASE_URL/set?ht=25"
echo -e "\n"

# Set fire temperature threshold
echo "Setting fire temperature threshold (FTT) to 50"
curl "$BASE_URL/set?FTT=50"
echo -e "\n"

# Set fire light threshold
echo "Setting fire light threshold (FLT) to 100"
curl "$BASE_URL/set?FLT=100"
echo -e "\n"

# Testing POST /target endpoint with required parameters
echo "Setting reporting target with POST /target"
curl -X POST "$BASE_URL/target" -d "ip=192.168.0.10&port=8080&sp=5"
echo -e "\n"
