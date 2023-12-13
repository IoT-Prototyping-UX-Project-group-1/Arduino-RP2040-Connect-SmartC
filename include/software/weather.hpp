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
    char* city                          = NULL;
    WeatherForecast* firstForecast       = NULL;
    WeatherForecast* secondForecast     = NULL;
    WeatherForecast* thirdForecast      = NULL;
};

bool convertOpenWeatherToWeatherInformationStruct(const char* response, WeatherInformation& weatherInformation);
