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
 * WebServerens client loop, der bliver kaldt 
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
            
            const char index_html1[] PROGMEM = R"rawliteral(
                <!DOCTYPE html>
                <html>
                <style>
                input[type=text], select {
                  width: 100%;
                  padding: 12px 20px;
                  margin: 8px 0;
                  display: inline-block;
                  border: 1px solid #ccc;
                  border-radius: 4px;
                  box-sizing: border-box;
                }
                input[type=submit] {
                  width: 100%;
                  background-color: #4CAF50;
                  color: white;
                  padding: 14px 20px;
                  margin: 8px 0;
                  border: none;
                  border-radius: 4px;
                  cursor: pointer;
                }
                input[type=submit]:hover {
                  background-color: #45a049;
                }
                div {
                  border-radius: 5px;
                  background-color: #f2f2f2;
                  padding: 20px;
                }
                </style>
                <body>
                    <h3>Varmestyringsanl&aelig;g</h3>
                    <br>
                    <h4>Temperatur: )rawliteral";
                

            const char index_html2[] PROGMEM = R"rawliteral(
                </h4>
                <div>
                  <form action="/action_page.php">
                    <label for="fname">Indtast temperatur</label>
                    <input type="text" id="tempValue" name="tempValue">
                                   
                    <input type="submit" value="Bekr&aelig;ft temperatur">
                    <label for="lname">Indtast komfort temperatur</label>
                    <input type="text" id="comfortTempValue" name="comfortTempValue">
                     <label for="lname">Indtast spare temperatur</label>
                    <input type="text" id="savingTempValue" name="savingTempValue">
                  
                    <input type="submit" value="Bekr&aelig;ft komfort- og sparetemperatur">
                  </form>
                </div>
                <br>
              </form><br>
            </body></html>)rawliteral";

            client.print(index_html1);
            client.println(tempString);
            client.println(index_html2);
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