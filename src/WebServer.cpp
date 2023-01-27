#include <SPI.h>
#include <Ethernet.h>

// Sætter MAC addresse, IP addresse og porten for HTTP kommunikation på Arduino WebServeren
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 177);
EthernetServer server(80);

/**
 * Initialiserer WebServer konfigurationen.
 * Sætter IP addresse, MAC addresse og ether port til HTTP forbindelse.
 * Tjekker om Ethernet modulet er tilsluttet og starter WebServeren
*/
void clientSetup() {
  // Venter på at Serial porten åbner
  while (!Serial) {
    ;
  }

  Serial.println("Ethernet WebServer Example");

  // Sætter ethernet konfiguration på WebServeren
  Ethernet.begin(mac, ip);

  // Tjekker at ethernet shield er sat på Arduino Mega 2560.
  // Gør intet, hvis der ikke er forbindelse.
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1);
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // Starter serveren
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

/**
 * 
 * @param tempString En formatteret string med en temperatur.
*/
void clientLoop(String tempString) {
  // Lytter efter indkommende klienter
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // HTTP anmodning ender med en blank linje
    bool currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // Hvis request er kommet til en blank linje, så kan serveren sende
        // information tilbage til klienten
        if (c == '\n' && currentLineIsBlank) {
            // Sender en standard HTTP response
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close"); 
            client.println("Refresh: 5");
            client.println();
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<html>");
            client.println("<html>");
            client.print("Temperature output: ");
            client.print(tempString);
            client.println("<br />");
            client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}