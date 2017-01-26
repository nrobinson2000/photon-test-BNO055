#include "Buffer.h"
#include "Record.h"
#include "Label.h"
#include "MDNS.h"
#include "OneWire.h"
#include "particle-BNO055.h"
#include "Particle.h"

SYSTEM_MODE(MANUAL);
/*SYSTEM_THREAD(ENABLED);*/

float vx = 0, vy = 0, vz = 0;
float dx = 0, dy = 0, dz = 0;

unsigned int lastTime = 0;

float tx = 0, ty = 0, tz = 0;

float tol = 5.0;

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


        for ( int i = 0; i < 1000; i++)
        {
                imu::Vector<3> acc = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
                imu::Vector<3> gravity = bno.getVector(Adafruit_BNO055::VECTOR_GRAVITY);

                tx = tx + acc.x()  - gravity.x();
                ty = ty + acc.y() - gravity.y();
                tz = tz + acc.z() - gravity.z();

                Serial.println(i);

        }

        tx = tx / 1000.0;

        ty = ty / 1000.0;

        tz = tz / 1000.0;



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



        if (ax > (tx - abs(tx) * tol) && ax < (tx + abs(tx) * tol))
        {
                ax = 0.0;
                vx = 0.0;
        }
        if (ay > (ty - abs(ty) * tol) && ay < (ty + abs(ty) * tol))
        {
                ay = 0.0;
                vy = 0.0;
        }
        if (az > (tz - abs(tz) * tol) && az < (tz + abs(tz) * tol))
        {
                az = 0.0;
                vz = 0.0;
        }

        /* Display the floating point data */

        /*Serial.print("AX: ");
           Serial.print(ax);
           Serial.print("\tAY: ");
           Serial.print(ay);
           Serial.print("\tAZ: ");
           Serial.print(az);
           Serial.println("");*/


        /*if (millis() - lastTime > 1000)
           {*/

        float dt = ((millis() - lastTime) / 1000.0);

        vx = vx + ax * dt;
        vy = vy + ay * dt;
        vz = vz + az * dt;

        dx = dx + vx * dt;
        dy = dy + vy * dt;
        dz = dz + vz * dt;


        lastTime = millis();


        /*}*/




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
        Serial.print(dx * 100.0, 4);
        Serial.print("\tdy: ");
        Serial.print(dy * 100.0, 4);
        Serial.print("\tdz: ");
        Serial.print(dz * 100.0, 4);
        Serial.println();


        Serial.println();
        Serial.print("tx: ");
        Serial.print(tx, 4);
        Serial.print("\tty: ");
        Serial.print(ty, 4);
        Serial.print("\ttz: ");
        Serial.print(tz, 4);
        Serial.println();

        /*Serial.println("dx"+String(dx));
           Serial.println("dy"+String(dy));
           Serial.println("dz"+String(dz));
           Serial.println();*/




        /*delay();*/
}
