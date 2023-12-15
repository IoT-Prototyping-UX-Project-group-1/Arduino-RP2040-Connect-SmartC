#include "software/weather.hpp"
#include <ArduinoJson.h>

WeatherState ConvertWeatherStringToWeatherStateEnum(float cloudsAll, char *weatherMain, char *weatherDescription)
{
    if (strcmp(weatherMain, "Clouds") == 0)
    {
        if (strcmp(weatherDescription, "few clouds") == 0 || strcmp(weatherDescription, "scattered clouds") == 0)
        {
            return FEW_CLOUDS;
        }
        else
        {
            return CLOUDY;
        }
    }
    else if (strcmp(weatherMain, "Rain") == 0)
    {
        if (strcmp(weatherDescription, "light rain") == 0)
        {
            return LIGHT_RAIN;
        }
        else if (strcmp(weatherDescription, "moderate rain") == 0)
        {
            return RAINY;
        }
        else
        {
            return HEAVY_RAIN;
        }
    }
    else if (strcmp(weatherMain, "Mist") == 0)
    {
        return MISTY;
    }
    else if (strcmp(weatherMain, "Clear") == 0 || cloudsAll < 10 || strcmp(weatherDescription, "clear sky") == 0)
    {
        return CLEAR_SKY;
    }
    return CLEAR_SKY;
}

void convertOpenWeatherToWeatherInformationStruct(char *response, WeatherInformation &weatherInformation)
{
    // Create filter to extract just what we require from the JSON response:
    StaticJsonDocument<256 /* Bytes */> filter;

    JsonObject filter_list_0 = filter["list"].createNestedObject();
    filter_list_0["dt"] = true;

    JsonObject filter_list_0_main = filter_list_0.createNestedObject("main");
    filter_list_0_main["temp"] = true;
    filter_list_0["weather"][0]["main"] = true;
    filter_list_0["weather"][0]["description"] = true;
    filter_list_0["clouds"]["all"] = true;
    filter["city"]["name"] = true;

    // Step 1: Convert the response to a JSON object
    StaticJsonDocument<1024 * 16 /* 10KB */> jsonDocument;
    DeserializationError error = deserializeJson(jsonDocument, response, strlen(response), DeserializationOption::Filter(filter));
    response = "";
    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
    }
    else
    {
        Serial.println("JSON successfully parsed.");
        WeatherForecast &firstForecast = weatherInformation.firstForecast;
        WeatherForecast &secondForecast = weatherInformation.secondForecast;
        WeatherForecast &thirdForecast = weatherInformation.thirdForecast;

        // Step 2: Extract the data from the JSON object
        weatherInformation.city = (char *)calloc(strlen(jsonDocument["city"]["name"]) + 1, sizeof(char));
        strcpy(weatherInformation.city, jsonDocument["city"]["name"]); // DevSkim: ignore DS185832

        firstForecast.epochTime = jsonDocument["list"][0]["dt"];
        float feelsLike = jsonDocument["list"][0]["main"]["temp"];
        float cloudsAll = jsonDocument["list"][0]["clouds"]["all"];
        char *weatherMain = jsonDocument["list"][0]["weather"][0]["main"];
        char *weatherDescription = jsonDocument["list"][0]["weather"][0]["description"];

        // firstForecast.temperatureState = feelsLike < 1 ? FROST : feelsLike < 10 ? COLD
        //                                                      : feelsLike < 17   ? MODERATE
        //                                                      : feelsLike < 27   ? WARM
        //                                                                         : HOT;

        // firstForecast.weatherState = ConvertWeatherStringToWeatherStateEnum(cloudsAll, weatherMain, weatherDescription);

        // Serial.println("#1 -> Temperature State: " + String(firstForecast.temperatureState));
        Serial.println("#1 -> Temperature State: " + String(feelsLike));
        Serial.println("#1 -> Temperature State: " + String(cloudsAll));
        Serial.println("#1 -> Temperature State: " + String(weatherMain));
        Serial.println("#1 -> Temperature State: " + String(weatherDescription));
        // Serial.println("#1 -> Weather State: " + String(firstForecast.weatherState));

        // secondForecast.epochTime = jsonDocument["list"][1]["dt"];
        // secondForecast.temperature = jsonDocument["list"][1]["main"]["temp"];
        // secondForecast.humidity = jsonDocument["list"][1]["main"]["humidity"];
        // Serial.println("#2 -> Temperature: " + String(secondForecast.temperature));
        // Serial.println("#2 -> Humidity: " + String(secondForecast.humidity));

        // thirdForecast.epochTime = jsonDocument["list"][2]["dt"];
        // thirdForecast.temperature = jsonDocument["list"][2]["main"]["temp"];
        // thirdForecast.humidity = jsonDocument["list"][2]["main"]["humidity"];
        // Serial.println("#3 -> Temperature: " + String(thirdForecast.temperature));
        // Serial.println("#3 -> Humidity: " + String(thirdForecast.humidity));

        Serial.println("Weather information successfully converted.");
    }
}
