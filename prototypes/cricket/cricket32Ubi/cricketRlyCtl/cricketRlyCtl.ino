Ubidots client(TOKEN);

void setup() {
  Serial.begin(9600);
  client.wifiConnection(SSID,PASS);
}

void loop() {
  float value;
  value = client.getValue(ID);
} 