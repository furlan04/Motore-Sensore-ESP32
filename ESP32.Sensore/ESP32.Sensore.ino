#include <DHT.h>
#include <EspMQTTClient.h>
#include <ArduinoJson.h>

DHT dht(14, DHT22);

EspMQTTClient client(
  "Greppi-2G",
  //"Vodafone-A41734402",
  "withProxy",
  //"bqyKhMXsqtTsyK2g",
  "172.17.3.46",
  "",
  "",
  "ESP32_Sensore_Temperatura",
  1883
);

void invia()
{
  if (client.isConnected()) {
    float t = dht.readTemperature();
    Serial.print("Temperatura: ");
    Serial.println(t);
    StaticJsonDocument<96> doc_temp;
    doc_temp["valore-sensore"] = t;
    String output_temp;
    serializeJson(doc_temp, output_temp);
    float h = dht.readHumidity();
    Serial.print("Umidità: ");
    Serial.println(h);
    StaticJsonDocument<96> doc_hum;
    doc_hum["valore-sensore"] = h;
    String output_hum;
    serializeJson(doc_hum, output_hum);
    Serial.println(output_temp);
    Serial.println(output_hum);
    client.publish("tps/sensore/temperatura", output_temp);
    client.publish("tps/sensore/umidità", output_hum);
    client.executeDelayed(10000, invia);
  }
}

void onConnectionEstablished()
{
 client.executeDelayed(5000, invia);
}


void setup() {
  Serial.begin(9600);
  dht.begin();
  client.enableDebuggingMessages();
}

void loop() {
  client.loop();
}
