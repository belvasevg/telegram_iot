#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define WIFI_SSID "MoreSeksa"
#define PASSWORD "89219255365"
#define TG_TOKEN "6141473405:AAEqGD06VA-3Fp2aGI8En4bQLkXpWQ_nedY"

Adafruit_BME280 air_bme;
WiFiClientSecure CLIENT;
UniversalTelegramBot tgbot(TG_TOKEN, CLIENT);
const unsigned short TGBOT_MTBS = 1000;
long TGBOT_LASTTIME;

void setup() 
{
  Serial.begin(115200);
  
  bool bme_status = air_bme.begin();
  if (!bme_status) 
  Serial.println("Проверьте подключение датчика bme280.");
  
  Serial.println("***********");
  Serial.println("Подключение к WiFi " + String(WIFI_SSID));
  WiFi.begin(WIFI_SSID, PASSWORD);
  CLIENT.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  while (WiFi.status() != WL_CONNECTED)
  {
      Serial.print('.');
      delay(300);
  }

  Serial.println("WiFi connected to IP: " + String(WiFi.localIP()));
}

void loop() 
{
  if (millis() > TGBOT_LASTTIME + TGBOT_MTBS)
  {
    int num_new_mess = tgbot.getUpdates(tgbot.last_message_received + 1);
    while (num_new_mess)
    {
      Serial.println("Поступило указание");
      handle_mess(num_new_mess);
      num_new_mess = tgbot.getUpdates(tgbot.last_message_received + 1); 
    }
    TGBOT_LASTTIME = millis();
  }
}

void handle_mess (int num_new_mess)
{
  Serial.print("Обработка сообщения...");
  Serial.println(String(num_new_mess));
  float t = air_bme.readTemperature();
  float p = air_bme.readPressure()/100.0F;
  float h = air_bme.readHumidity();
  for (int i = 0; i <num_new_mess; i++)
  {
    String chat_id = tgbot.messages[i].chat_id;
    String text = tgbot.messages[i].text;
    text.toLowerCase();
    String who_knocked = tgbot.messages[i].from_name;
    if (who_knocked = "") who_knocked = "Guest";
    if (text == "/air" || text == "air")
    {
      //float t = air_bme.readTemperature();
      Serial.println("Температура воздуха равна " + String(t,1) + " градусов Цельсия");
      tgbot.sendMessage(chat_id, "Температура воздуха равна " + String(t,1) + " градусов Цельсия");
    }
    if (text == "/pressure" || text == "pressure")
    {
      //float p = air_bme.readPressure()/100.0F;
      Serial.println("Давление воздуха равно " + String(p,1) + " Гектопаскалей");
      tgbot.sendMessage(chat_id, "Давление воздуха равно " + String(p,1) + " Гектопаскалей");
    }
    if (text == "/humidity" || text == "humidity")
    {
      //float h = air_bme.readHumidity();
      Serial.println("Влажность воздуха равна " + String(h,1) + " %");
      tgbot.sendMessage(chat_id, "Влажность воздуха равна " + String(h,1) + " %");
    }
    if (text == "/start" || text == "start")
    {
      tgbot.sendMessage(chat_id, "Приветствую" + String(who_knocked) + "!");
      tgbot.sendMessage(chat_id, "Я бот Васи Белая./nМоя задача следить за домом./nЧтобы узнать как я могу тебе помочь, напиши \"help\" или \"/help\".");
    }
    if (text == "/help" || text == "help")
    {
      tgbot.sendMessage(chat_id, "/air - выведет показания температуры воздуха в квартире.");
      tgbot.sendMessage(chat_id, "/pressure - выведет показания давления воздуха в квартире.");
      tgbot.sendMessage(chat_id, "/humidity - выведет показания влажности воздуха в квартире.");
      tgbot.sendMessage(chat_id, "/reflections - выведет планы автора по развитию своего бота.");
      tgbot.sendMessage(chat_id, "Команды можно писать без слэша слева и буквами любого регистра", "");
    }
    if (text == "/reflections" || text == "reflections")
    {
      tgbot.sendMessage(chat_id, "Планирую добавить датчик пламени, летучих соединений и датчик освещённости.");
    }
    if (t < 20)
    {
      tgbot.sendMessage(chat_id, "Дома холодно. Включите полы или закройте окна.");
    }
    if (h >= 50)
    {
      tgbot.sendMessage(chat_id, "Дома высокая влажность. Проветрите квартиру.");
    }
    if (t >= 20)
    {
      tgbot.sendMessage(chat_id, "Дома достигнута комнатная температура.");
    }
    if (h < 50)
    {
      tgbot.sendMessage(chat_id, "Дома нормальная влажность. Можете заканчивать проветривание.");
    }
  }
}














