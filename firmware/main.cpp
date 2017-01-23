#include "Buffer.h"
#include "Record.h"
#include "Label.h"
#include "MDNS.h"
#include "OneWire.h"
#include "particle-BNO055.h"
#include "Particle.h"

SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);

#define HTTP_PORT 80

MDNS mdns;

TCPServer server = TCPServer(HTTP_PORT);

Adafruit_BNO055 bno = Adafruit_BNO055(55);

void setup()
{
        Serial.begin(9600);
        Serial.println("Orientation Sensor Test");
        Serial.println("");

        /* Initialise the sensor */
        if(!bno.begin())
        {
                /* There was a problem detecting the BNO055 ... check your connections */
                Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
                while(1) ;
        }

        delay(1000);

        bno.setExtCrystalUse(true);



        WiFi.on();
        WiFi.connect();

        server.begin();

        bool success = mdns.setHostname("photon");

        if (success) {
                success = mdns.addService("tcp", "http", HTTP_PORT, "photon");
        }

        mdns.addTXTEntry("normal");

        if (success) {
                success = mdns.begin();
        }
}

void loop()
{
        /* Get a new sensor event */
        sensors_event_t event;
        bno.getEvent(&event);

        String x = String(event.orientation.x, 4);
        String y = String(event.orientation.y, 4);
        String z = String(event.orientation.z, 4);

        mdns.processQueries();

        TCPClient client = server.available();

        if (client) {
                while (client.read() != -1)
                {
                        String json="{\"x\": \""+x+"\",\"y\": \""+y+"\",\"z\": \""+z+"\"}";

                        client.write("HTTP/1.1 200 Ok\n");
                        client.write("access-control-allow-origin: *\n");
                        client.write("\n" + json);
                        client.flush();
                        delay(5);
                        client.stop();
                }
           }

           delay(75);

}
