Sur macOS, voici les étapes pour installer et configurer Mosquitto en tant que broker MQTT local :

# find lib/EasyLife/src -name "*.cpp" -o -name "*.h" | xargs clang-format -i

### 1. Installation de Mosquitto
Sur macOS, vous pouvez installer Mosquitto via **Homebrew**. Si Homebrew n’est pas installé, vous pouvez le faire en suivant les instructions [ici](https://brew.sh/).

```bash
brew install mosquitto
```

### 2. Démarrage du service Mosquitto
Une fois Mosquitto installé, vous pouvez démarrer le service avec la commande suivante :

```bash
brew services start mosquitto
```

Pour vérifier son statut :
```bash
brew services list
```

### 3. Configuration de Mosquitto
Contrairement à Linux où le fichier de configuration se trouve dans `/etc/mosquitto/mosquitto.conf`, sous macOS, il peut être nécessaire de créer ou modifier le fichier de configuration dans un emplacement Homebrew :

1. Créez le dossier de configuration si nécessaire :
   ```bash
   mkdir -p /usr/local/etc/mosquitto
   ```

2. Créez un fichier de configuration (`mosquitto.conf`) dans ce dossier :
   ```bash
   nano /usr/local/etc/mosquitto/mosquitto.conf
   ```

3. Ajoutez la configuration minimale, par exemple :
   ```conf
   pid_file /usr/local/var/run/mosquitto.pid
   persistence true
   persistence_location /usr/local/var/lib/mosquitto/
   log_dest file /usr/local/var/log/mosquitto/mosquitto.log

   # Listener pour accepter les connexions de tous les réseaux
   listener 1883
   protocol mqtt
   allow_anonymous true
   ```

4. Sauvegardez le fichier et redémarrez Mosquitto :
   ```bash
   brew services restart mosquitto
   ```

### 4. Test de Mosquitto
Pour tester que le broker fonctionne correctement :

1. **Ouvrez un terminal** pour souscrire à un topic :
   ```bash
   mosquitto_sub -h localhost -t test/topic -v
   ```

2. **Ouvrez un autre terminal** pour publier un message sur ce topic :
   ```bash
   mosquitto_pub -h localhost -t test/topic -m "Hello MQTT"
   ```

Si tout fonctionne, vous verrez le message "Hello MQTT" apparaître dans la première fenêtre de terminal.

### 5. Vérification des statistiques
Pour obtenir un dump des statistiques du broker, souscrivez au topic système `$SYS/#` :

```bash
mosquitto_sub -h localhost -v -t \$SYS/#
```

Cela vous permettra d’afficher des statistiques telles que le nombre de messages reçus, envoyés, clients connectés, etc.

N'hésitez pas si vous avez des questions supplémentaires !



### Check Mosquitto Process Logs
If the log file is still missing, check the system logs for any error messages Mosquitto might be producing when it starts up. On macOS, you can use:
```bash
log show --predicate 'process == "mosquitto"' --info --last 10m
```
This command will show any log messages from Mosquitto from the past 10 minutes, which may give insight into why the log file isn’t being created.


### Storage 
```bash
tail -f /usr/local/var/log/mosquitto/mosquitto.log
```