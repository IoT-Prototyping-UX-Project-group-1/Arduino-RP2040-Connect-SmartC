#include <Arduino.h>

#define SSID "iOtE"
#define PASS "sgri*+x/esdfi17ao77#=3o"


void printNetworkInformation()
{
  Serial.print("IPv4 address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Subnet Mask: ");
  Serial.println(WiFi.subnetMask());

  Serial.print("Gateway IP: ");
  Serial.println(WiFi.gatewayIP());

  Serial.print("Signal Strength: ");
  Serial.println(WiFi.RSSI());

  byte routerMac[6];
  WiFi.BSSID(routerMac);
  Serial.print("Router MAC: ");
  for (int i = 5; i >= 0; i--)
  {
    if (routerMac[i] < 0x10)
    {
      Serial.print("0");
    }
    Serial.print(routerMac[i], HEX);
    if (i > 0)
    {
      Serial.print(":");
    }
  }
  Serial.println();
}
