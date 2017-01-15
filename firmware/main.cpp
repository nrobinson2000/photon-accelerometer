#include "Particle.h"
#include "MDNS.h"

SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);

#define HTTP_PORT 80

MDNS mdns;

TCPServer server = TCPServer(HTTP_PORT);

bool goodData = false;

String full;

double g = 9.8;

double ax;
double ay;
double az;

double lastax;
double lastay;
double lastaz;

double roll;
double pitch;
double yaw;

double lastroll;
double lastpitch;
double lastyaw;

unsigned int hexToDec(String hexString) {

        unsigned int decValue = 0;
        int nextInt;

        for (int i = 0; i < hexString.length(); i++) {

                nextInt = int(hexString.charAt(i));
                if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
                if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
                if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
                nextInt = constrain(nextInt, 0, 15);

                decValue = (decValue * 16) + nextInt;
        }

        return decValue;
}

void setup() // Put setup code here to run once
{
        Serial.begin(9600);
        Serial1.begin(115200);

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

void loop() // Put code here to loop forever
{
        full = "";

        while (Serial1.available()) // Get the full string from serial
        {
                String s = String(Serial1.read(), HEX);
                s = s.toUpperCase();
                if (s.length() == 1 )
                {
                        s = "0" + s;
                }
                full += s;
                full += " ";
        }

        if (full.length() == 165)
        {
                goodData = true;
        }
        else
        {
                goodData = false;
        }

// Lets get the accelrations x,y,x!

        String axL = full.substring(39,41);
        String axH = full.substring(42,44);
        String ayL = full.substring(45,47);
        String ayH = full.substring(48,50);
        String azL = full.substring(51,53);
        String azH = full.substring(54,56);

        unsigned int axLDec = hexToDec(axL);
        unsigned int axHDec = hexToDec(axH);
        unsigned int ayLDec = hexToDec(ayL);
        unsigned int ayHDec = hexToDec(ayH);
        unsigned int azLDec = hexToDec(azL);
        unsigned int azHDec = hexToDec(azH);

        ax = (axHDec * 256.0 + axLDec) / 32768.0 * 16.0 * g;
        ay = (ayHDec * 256.0 + ayLDec) / 32768.0 * 16.0 * g;
        az = (azHDec * 256.0 + azLDec) / 32768.0 * 16.0 * g;


// Lets get the roll, pitch, and yaw!

        String rollL = full.substring(105,107); //105 - 111 00
        String rollH = full.substring(108,110);
        String pitchL = full.substring(111,113); // 111 - 118
        String pitchH = full.substring(114,116); // 111 - 118
        String yawL = full.substring(117,119);
        String yawH = full.substring(120,122);

        unsigned int rollLDec = hexToDec(rollL);
        unsigned int rollHDec = hexToDec(rollH);
        unsigned int pitchLDec = hexToDec(pitchL);
        unsigned int pitchHDec = hexToDec(pitchH);
        unsigned int yawLDec = hexToDec(yawL);
        unsigned int yawHDec = hexToDec(yawH);

        roll = ((rollHDec * 256.0 + rollLDec) - 32768.0) / 32768.0 * 180.0;
        pitch = ((pitchHDec * 256.0 + pitchLDec) - 32768.0) / 32768.0 * 180.0;
        yaw = ((yawHDec * 256.0 + yawLDec) - 32768.0 ) / 32768.0 * 180.0;


        if (goodData)
        {
                lastax = ax;
                lastay = ay;
                lastaz = az;
                lastroll = roll;
                lastpitch = pitch;
                lastyaw = yaw;
        }
        else
        {
                ax = lastax;
                ay = lastay;
                az = lastaz;
                roll = lastroll;
                pitch = lastpitch;
                yaw = lastyaw;
        }


        mdns.processQueries();

        TCPClient client = server.available();

        if (client) {
                while (client.read() != -1)
                {
                        String json="{\"ax\": \""+String(ax)+"\",\"ay\": \""+String(ay)+"\",\"az\": \""+String(az)+"\",\"roll\": \""+String(roll)+"\",\"pitch\": \""+String(pitch)+"\",\"yaw\": \""+String(yaw)+"\"}";

                        client.write("HTTP/1.1 200 Ok\n");
                        client.write("access-control-allow-origin: *\n");
                        client.write("\n" + json);
                        client.flush();
                        delay(5);
                        client.stop();

                        Serial.println(full.length());
                }
        }


        delay(75);
}
