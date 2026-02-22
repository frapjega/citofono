#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "time.h"
#include "arduino_secrets.h"
#include <map>
#include "esp_task_wdt.h"

const int rele = 18;
const int led = 2;


// Server NTP per l'orario
const char* ntpServer = "pool.ntp.org";

// Fuso orario con gestione ora legale per l’Italia (CET/CEST)
const char* timeZone = "CET-1CEST,M3.5.0/2,M10.5.0/3";

//mappa per salvare nomi utente, chatID, ruolo  
std::map<String, std::pair<String, String>> UserData;


WiFiClientSecure client;
UniversalTelegramBot bot(SECRET_BOT_TOKEN, client);


String keyboardJson = R"([
  ["Apri porta", "log aperture"],
  ["scopri il tuo chat ID"]
])";

String keyboardJsonAdmin = R"([
  ["Apri porta", "log aperture"],
  ["scopri il tuo chat ID", "manage authorization"]
])";



void ConnectWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(SECRET_SSID, SECRET_PASS);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) { // reduced attempts
    delay(500);
    esp_task_wdt_reset(); // feed the watchdog
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection failed!");
  } else {
    Serial.println("\nConnected!");
  }
}


void setup() {

  Serial.begin(115200);
  pinMode(rele, OUTPUT);
  pinMode(led, OUTPUT);

  Serial.println("Inizio Sketch");



  ConnectWiFi();

  client.setInsecure();

  if (WiFi.status() == WL_CONNECTED) {
    configTzTime(timeZone, ntpServer);
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Errore nel recupero dell'orario");
    }
  }

  UserData["6676478511"] = {"Pietro", "admin"};
  UserData["812251602"] = {"Guido", "user"};
  UserData["931534163"] = {"Anna", "user"};
  UserData["-1002606950320"] = {"Group", "user"};

  digitalWrite(rele, LOW);
  digitalWrite(led, LOW);

}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
      ConnectWiFi();
    }

  struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      // If we can't get time, we just log it once in a while, 
      // but we let the rest of the code run!
      static unsigned long lastError = 0;
      if (millis() - lastError > 10000) { 
        Serial.println("Time not available yet...");
        lastError = millis();
      }
    }

  int NewMessage = bot.getUpdates(bot.last_message_received+1);


  for (int i = 0; i < NewMessage; i++) {

    String chatID = bot.messages[i].chat_id;
    String textMessage = bot.messages[i].text;
    String typeMessage = bot.messages[i].type;

    bool admin = false;
    bool user = false;

    String reply = "";


    for (auto const& [chat_id, pair] : UserData) {

      if (chat_id == chatID) {
        if (pair.second == "user") {
          user = true;
        } else if (pair.second == "admin") {
          admin = true;
        }
      }
    
    }

    if (textMessage == "/start") {
      reply = "Ciao!";

    } else if (textMessage == "Apri porta") {
      if (admin || user) {
        digitalWrite(rele, HIGH);
        digitalWrite(led, HIGH);
        delay(250);
        digitalWrite(rele, LOW);
        digitalWrite(led, LOW);

        reply = "Porta aperta con successo";


      } else {
        reply = "Non possiedi le autorizzazioni necessarie per eseguire questo comando";       
      }

    } else if (textMessage == "log aperture") {
      reply = "funzione al momento non disponibile";


    } else if (textMessage == "scopri il tuo chat ID") {
      reply = "Il tuo chatID è: " + chatID;

    } else if (textMessage == "manage authorization") {
      if (admin) {
        reply = "funzione al momento non disponibile";

      } else {
      reply = "Non possiedi le autorizzazioni necessarie per eseguire questo comando";

      }

    }

    if (!admin) {
      bot.sendMessageWithReplyKeyboard(chatID, reply, "", keyboardJson, true);
    } else {
      bot.sendMessageWithReplyKeyboard(chatID, reply, "", keyboardJsonAdmin, true);
    }
  }

  delay(500);
}
