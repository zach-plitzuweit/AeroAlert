#include <iostream>
#include <string>
#include "httplib.h"
#include "json.hpp"

using json = nlohmann::json;

int main() {
    std::string city;
    std::cout << "Enter city name: ";
    std::getline(std::cin, city); // Get user input

    // Create a client for OpenWeatherMap API
    httplib::Client cli("api.openweathermap.org", 80);
    std::string url = "/data/2.5/weather?q=" + city + "&appid=f193257d5cc4c0740688c601c9229a0b";

    // Make a GET request to fetch weather data for London
    auto res = cli.Get(url.c_str());

    // Check if the response is valid
    if (res && res->status == 200) {
        // Parse the JSON response
        json weather_data = json::parse(res->body);

        // Extract relevant information
        std::string country = weather_data["sys"]["country"];
        double temp_kelvin = weather_data["main"]["temp"];
        double temp_celsius = temp_kelvin - 273.15; // Convert to Celsius
        double humidity = weather_data["main"]["humidity"];
        std::string weather_description = weather_data["weather"][0]["description"];

        // Output friendly weather information
        std::cout << "Weather in " << city << ", " << country << ":\n";
        std::cout << "Temperature: " << temp_celsius << "°C\n";
        std::cout << "Humidity: " << humidity << "%\n";
        std::cout << "Conditions: " << weather_description << "\n";
    }
    else {
        std::cout << "Error: " << (res ? std::to_string(res->status) : "No response") << std::endl;
    }

    return 0;
}