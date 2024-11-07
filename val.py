import jsonschema
import json

schema = {
    "type": "object",
    "properties": {
        "status": {
            "type": "object",
            "properties": {
                "temperature": {"type": "number"},
                "light": {"type": "integer"},
                "regul": {
                    "type": "string",
                    "enum": ["HALT", "ACTIVE", "ERROR"]
                },
                "fire": {"type": "boolean"},
                "heat": {
                    "type": "string",
                    "enum": ["ON", "OFF"]
                },
                "cold": {
                    "type": "string",
                    "enum": ["ON", "OFF"]
                },
                "fanspeed": {"type": "integer", "minimum": 0}
            },
            "required": ["temperature", "light", "regul", "fire", "heat", "cold", "fanspeed"]
        },
        "location": {
            "type": "object",
            "properties": {
                "room": {"type": "string"},
                "gps": {
                    "type": "object",
                    "properties": {
                        "lat": {"type": "number"},
                        "lon": {"type": "number"}
                    },
                    "required": ["lat", "lon"]
                },
                "address": {"type": "string"}
            },
            "required": ["room", "gps", "address"]
        },
        "regul": {
            "type": "object",
            "properties": {
                "lt": {"type": "integer"},
                "ht": {"type": "integer"}
            },
            "required": ["lt", "ht"]
        },
        "info": {
            "type": "object",
            "properties": {
                "ident": {"type": "string"},
                "user": {"type": "string"},
                "loc": {"type": "string"}
            },
            "required": ["ident", "user", "loc"]
        },
        "net": {
            "type": "object",
            "properties": {
                "uptime": {"type": "integer", "minimum": 0},
                "ssid": {"type": "string"},
                "mac": {
                    "type": "string",
                    "pattern": "^([0-9A-Fa-f]{2}:){5}([0-9A-Fa-f]{2})$"
                },
                "ip": {
                    "type": "string",
                    "pattern": "^([0-9]{1,3}\\.){3}[0-9]{1,3}$"
                }
            },
            "required": ["uptime", "ssid", "mac", "ip"]
        },
        "reporthost": {
            "type": "object",
            "properties": {
                "target_ip": {
                    "type": "string",
                    "pattern": "^([0-9]{1,3}\\.){3}[0-9]{1,3}$"
                },
                "target_port": {"type": "integer", "minimum": 1, "maximum": 65535},
                "sp": {"type": "integer", "minimum": 1}
            },
            "required": ["target_ip", "target_port", "sp"]
        }
    },
    "required": ["status", "location", "regul", "info", "net", "reporthost"]
}

# Example usage
file_path = 'esp_output.json'

try:
    with open(file_path, 'r') as file:
        data = json.load(file)
    jsonschema.validate(instance=data, schema=schema)
    print("JSON is valid!")
except FileNotFoundError:
    print(f"File '{file_path}' not found. Please check the file path and try again.")
except json.JSONDecodeError as err:
    print(f"Error decoding JSON: {err}")
except jsonschema.exceptions.ValidationError as err:
    print("JSON validation error:", err)
