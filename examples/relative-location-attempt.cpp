#include "Buffer.h"
#include "Record.h"
#include "Label.h"
#include "MDNS.h"
#include "OneWire.h"
#include "vector.h"
#include "quaternion.h"
#include "matrix.h"
#include "imumaths.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_BNO055.h"
#include "Particle.h"

SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);

float vx = 0, vy = 0, vz = 0;
float dx = 0, dy = 0, dz = 0;

Adafruit_BNO055 bno = Adafruit_BNO055(55);

void setup(void)
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
}

void loop(void)
{
        /* Get a new sensor event */
        sensors_event_t event;
        bno.getEvent(&event);

        /* Display the floating point data */

        /*Serial.println("");
           Serial.print("X: ");
           Serial.print(event.orientation.x, 4);
           Serial.print("\tY: ");
           Serial.print(event.orientation.y, 4);
           Serial.print("\tZ: ");
           Serial.print(event.orientation.z, 4);
           Serial.println("");*/



        imu::Vector<3> acc = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
        imu::Vector<3> gravity = bno.getVector(Adafruit_BNO055::VECTOR_GRAVITY);

        float ax = acc.x() - gravity.x();
        float ay = acc.y() - gravity.y();
        float az = acc.z() - gravity.z();

        if (abs(ax) < 0.5) {
                ax = 0;
        }
        if (abs(ay) < 0.5) {
                ay = 0;
        }
        if (abs(az) < 0.5) {
                az = 0;
        }

        /* Display the floating point data */

        /*Serial.print("AX: ");
           Serial.print(ax);
           Serial.print("\tAY: ");
           Serial.print(ay);
           Serial.print("\tAZ: ");
           Serial.print(az);
           Serial.println("");*/


        vx = vx + ax;
        vy = vy + ay;
        vz = vz + az;

        dx = dx + vx;
        dy = dy + vy;
        dz = dz + vz;

        /*Serial.println("vx"+String(vx));
           Serial.println("vy"+String(vy));
           Serial.println("vz"+String(vz));
           Serial.println();*/


        Serial.println();
        Serial.print("X: ");
        Serial.print(event.orientation.x, 4);
        Serial.print("\tY: ");
        Serial.print(event.orientation.y, 4);
        Serial.print("\tZ: ");
        Serial.print(event.orientation.z, 4);
        Serial.println();

        Serial.println();
        Serial.print("ax: ");
        Serial.print(ax, 4);
        Serial.print("\tax: ");
        Serial.print(ax, 4);
        Serial.print("\taz: ");
        Serial.print(az, 4);
        Serial.println();

        Serial.println();
        Serial.print("vx: ");
        Serial.print(vx, 4);
        Serial.print("\tvy: ");
        Serial.print(vy, 4);
        Serial.print("\tvz: ");
        Serial.print(vz, 4);
        Serial.println();

        Serial.println();
        Serial.print("dx: ");
        Serial.print(dx, 4);
        Serial.print("\tdx: ");
        Serial.print(dx, 4);
        Serial.print("\tdz: ");
        Serial.print(dz, 4);
        Serial.println();

        /*Serial.println("dx"+String(dx));
           Serial.println("dy"+String(dy));
           Serial.println("dz"+String(dz));
           Serial.println();*/




        delay(100);
}
