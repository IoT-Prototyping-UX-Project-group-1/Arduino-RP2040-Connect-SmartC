#include "software/network.hpp"

HttpClient::HttpClient(const char *host, const char *path, const uint16_t port)
{
    this->host = (char *)calloc(strlen(host) + 1, sizeof(char));
    this->path = (char *)calloc(strlen(path) + 1, sizeof(char));
    strcpy(this->host, host); // DevSkim: ignore DS185832
    strcpy(this->path, path); // DevSkim: ignore DS185832
    this->port = port;
}

char* HttpClient::fetch(const uint32_t fetchSize)
{
    WiFiSSLClient client;
    if (!client.connect(host, port))
    {
        Serial.println("Connection to the provided hostname failed...");
        return NULL;
    }

    client.print(String("GET ") + path + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");

    while (client.connected())
    {
        String line = client.readStringUntil('\n');
        if (line == "\r")
        {
            Serial.println("HTTP response received...");
            break;
        }
    }

    char *returnValue = (char *)calloc(fetchSize + 1, sizeof(char));
    while (client.available())
    {
        String line = client.readStringUntil('\n');
        strcat(returnValue, line.c_str()); // DevSkim: ignore DS141863
    }

    client.flush();
    client.stop();
    return returnValue;
}

HttpClient::~HttpClient()
{
    free(host);
    free(path);
}
