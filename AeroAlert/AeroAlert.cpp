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
        double humidity = weather_data["main"]["humidity"];
        std::string weather_description = weather_data["weather"][0]["description"];

        // Ask the user for temperature unit preference
        char unit_choice;
        std::cout << "Choose temperature unit (C for Celsius, F for Fahrenheit): ";
        std::cin >> unit_choice;

        double temp_converted;
        std::string unit_symbol;

        // Convert temperature based on user choice
        if (unit_choice == 'C' || unit_choice == 'c') {
            temp_converted = temp_kelvin - 273.15; // Convert to Celsius
            unit_symbol = "°C";
        }
        else if (unit_choice == 'F' || unit_choice == 'f') {
            temp_converted = (temp_kelvin - 273.15) * 9 / 5 + 32; // Convert to Fahrenheit
            unit_symbol = "°F";
        }
        else {
            std::cout << "Invalid unit choice. Defaulting to Celsius." << std::endl;
            temp_converted = temp_kelvin - 273.15; // Default to Celsius
            unit_symbol = "°C";
        }
        
        // Output friendly weather information
        std::cout << "Weather in " << city << ", " << country << ":\n";
        std::cout << "Temperature: " << temp_converted << unit_symbol << "\n";
        std::cout << "Humidity: " << humidity << "%\n";
        std::cout << "Conditions: " << weather_description << "\n";
    }
    else {
        if (res) {
            std::cout << "Error: " << std::to_string(res->status) << " - ";
            if (res->status == 404) {
                std::cout << "City not found." << std::endl;
            }
            else {
                std::cout << "An error occurred while fetching data." << std::endl;
            }
        }
        else {
            std::cout << "Error: No response from the server." << std::endl;
        }
    }
    // Keep the console window open until user presses a key
    std::cout << "Press Enter to exit...";
    std::cin.get();

    return 0;
}