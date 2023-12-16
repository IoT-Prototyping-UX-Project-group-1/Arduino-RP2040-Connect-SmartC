#include <Arduino.h>

enum WeatherState
{
    CLEAR_SKY,  // 0
    FEW_CLOUDS, // 1
    CLOUDY,     // 2
    LIGHT_RAIN, // 3
    RAINY,      // 4
    HEAVY_RAIN, // 5
    MISTY,      // 6
    SNOWY       // 7
};

enum TemperatureState
{
    FROST,    // 0
    COLD,     // 1
    MODERATE, // 2
    WARM,     // 3
    HOT       // 4
};

typedef struct
{
    time_t epochTime;
    TemperatureState temperatureState;
    WeatherState weatherState;
} WeatherForecast;

typedef struct
{
    char *city;
    WeatherForecast firstForecast;
    WeatherForecast secondForecast;
    WeatherForecast thirdForecast;
} WeatherInformation;

void convertOpenWeatherToWeatherInformationStruct(char *response, WeatherInformation &weatherInformation);