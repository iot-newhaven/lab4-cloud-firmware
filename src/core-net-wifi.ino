#include "include/net-wifi.h"

void NetWifiStart(const char *wifiAPname,
                  const char *password)
{
    // Ensure that WiFi module is on
    WiFi.on();
   
    // Set up WPA2 access point "My AP" with password "mypassword" and AES cipher
    WiFiCredentials credentials(wifiAPname, WPA2);
    
    credentials.setPassword(password)
               .setCipher(WLAN_CIPHER_AES);

    WiFi.setCredentials(credentials);

    // Connect if settings were successfully saved   
    WiFi.connect();

    delay(1000);
}

void NetWifiStop(void)
{
    WiFi.clearCredentials();
 
    WiFi.off();

    delay(1000);
}

bool NetWifiReady(void)
{
    bool rc;
    static IPAddress ip;

    rc = WiFi.ready();

    if(rc)
    {
        SerialMonitorPrint("Wifi Connected:");
        
        ip = WiFi.localIP();
        
        SerialMonitorPrint("Local IP: %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
        
        ip = WiFi.gatewayIP();

        SerialMonitorPrint("Gateway IP: %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    }

    return rc;
}