#include "hardware.h"

void setup() {
  // put your setup code here, to run once:

  pinMode(LED_BUILTIN, OUTPUT);

  //Initialize serial and wait for port to open:
  Serial.begin(9600, SERIAL_8N1);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

}

char commandLine[128];
int commandLineLength=0;
char tmpChar;

void loop() {

  if (Serial.available() > 0) {
    
    tmpChar = char(Serial.read());

    if (tmpChar<32) {
      commandLine[commandLineLength] = (char)NULL;
      commandRouter(commandLine);
      commandLineLength=0;
      Serial.println("Ready");
    }
    else
    {
      commandLine[commandLineLength++] = tmpChar;
      if (commandLineLength > (sizeof(commandLine)-1)) {
        Serial.println("Error: Buffer Overflow");
        commandLineLength=0;
        Serial.println("Ready");
      }
    }
  }
 
}

void commandRouter(char line[])
{
  if (strlen(line)>0) {

    char *ptr = strtok(line, " ");

    if (strcmp(ptr, "ping") == 0) {
      ptr = strtok(NULL, " ");
      if (ptr != NULL) {
        long l = strtol(ptr, NULL, 16);
        digitalWrite(LED_BUILTIN, l);
        Serial.print("ping ");
        Serial.println(l, HEX);
        return;
      }
      Serial.println("ping");
      return;  
    }
    
  }
}
