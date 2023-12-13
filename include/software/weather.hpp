#include <Arduino.h>

enum WeatherState {
    CLOUDY,
    RAINY,
    SUNNY,
    SNOWY
};

typedef struct WeatherForecast {
    time_t epochTime;
    float temperature;
    float humidity;
    float windSpeed;
    WeatherState weatherState;
};

typedef struct WeatherInformation {
    char* city;
    WeatherForecast firstForecast;
    WeatherForecast secondForecast;
    WeatherForecast thirdForecast;
};

void convertOpenWeatherToWeatherInformationStruct(char* response, WeatherInformation &weatherInformation);
