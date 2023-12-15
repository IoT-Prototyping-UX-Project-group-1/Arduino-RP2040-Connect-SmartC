#include <Arduino.h>

enum WeatherState
{
    CLEAR_SKY,
    FEW_CLOUDS,
    CLOUDY,
    LIGHT_RAIN,
    RAINY,
    HEAVY_RAIN,
    MISTY,
    SNOWY
};

enum TemperatureState
{
    FROST,
    COLD,
    MODERATE,
    WARM,
    HOT
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
