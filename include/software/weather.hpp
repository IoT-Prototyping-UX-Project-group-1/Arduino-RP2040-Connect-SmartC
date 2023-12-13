#include <Arduino.h>

enum WeatherState {
    CLOUDY,
    RAINY,
    SUNNY,
    SNOWY
};

typedef struct {
    time_t epochTime;
    float temperature;
    float humidity;
    float windSpeed;
    WeatherState weatherState;
} WeatherForecast;

typedef struct {
    char* city;
    WeatherForecast firstForecast;
    WeatherForecast secondForecast;
    WeatherForecast thirdForecast;
} WeatherInformation;

void convertOpenWeatherToWeatherInformationStruct(char* response, WeatherInformation &weatherInformation);
