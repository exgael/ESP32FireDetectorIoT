/*
 * Fichier esp-spiffs_ls.ino
 * a) Faire un ls
 * b) Ouvrir un fichier (si il y est ?)
 * Si il n'y est pas ... le mettre avec l'IDE Arduino !
 */
#include "SPIFFS.h"
 
void setup() {
  Serial.begin(9600);
 
  if (!SPIFFS.begin(true)) {// "true" enable format if mount fails (only for ESP32)
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

 // Getting information from SPIFFs
 Serial.println("SPIFFS Information:"); 
 Serial.print("Total bytes:    "); Serial.println(SPIFFS.totalBytes());
 Serial.print("Used bytes:     "); Serial.println(SPIFFS.usedBytes());

 // Make an "ls" from SPIFFs
 File root = SPIFFS.open("/");
 File file = root.openNextFile();
  while(file){
      Serial.print("FILE: ");
      Serial.println(file.name());
      file = root.openNextFile();
  }
  
  // Find a file and read it  
  if (SPIFFS.exists("/readme.txt")) {
    Serial.println("Found sample file.");
    File f = SPIFFS.open("/readme.txt", "r");
    if (!f) {
      Serial.println("Failed to open file for reading");
      return;
    }
    Serial.println("Opened file for reading. File contents:\n");
    while (f.available()) {
      Serial.write(f.read());
    }
    f.close();
    Serial.println("\n\nEnd of file contents.");
  }
  else Serial.println("Sample file not found.");
}
 
void loop() {}
