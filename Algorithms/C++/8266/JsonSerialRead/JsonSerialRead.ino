
//unsigned long previousMillis = 0;
//const long interval = 5000;

char inData[500];
char inChar;
int stringPos = 0;
bool readingJson;
int status = 0;

void setup() {

  Serial.begin(9600); 

}

void loop() {
  // use this to keep track of time 
  // loop condition -> (currentMillis - previousMillis >= interval)
  //unsigned long currentMillis = millis();
  //pass in data to mega every 5 seconds

  getData();
}

void getData()
{
  if (Serial.available() > 0){ 
    char inChar = Serial.read();

    if(inChar == '{') {
      Serial.println("incomming data...");
      stringPos = 0;
      memset( &inData, 0, 500 );
      readingJson = true;
      }

    if(readingJson) {
      if(stringPos < 500) {
        inData[stringPos] = inChar; // Store it
        stringPos++;
      } 
      if(inChar == '}') {
        readingJson = false;

        Serial.println("Got JSON: ");
        Serial.println(inData);  
      } // Finished reading JSON
    } // Reading JSON
  }// Serial Avalilable
}


