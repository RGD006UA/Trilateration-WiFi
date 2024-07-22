#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "matrix.h"
#include <cmath>

#define ANCHORS 3
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64 
#define OLED_RESET 0

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

matrix_t* coordinates_anchors = init_matrix(ANCHORS, ANCHORS);
matrix_t* length_between_beacon_anchor = init_matrix(ANCHORS, 1);

// SSID of anchors 
const String SSID_anchors[ANCHORS] = {"anchor 1", "anchor 2", "anchor 3"};

// constants for calculating distance beetween anchors and beacon
const int16_t measured_power = -44;
const uint8_t N = 2.5;

double get_distance(String SSID_anchor, 
                    uint8_t number_anchors, 
                    int16_t measured_power, 
                    double N);

void display_coordinates();

void setup()
{
    Serial.begin(9600);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) // Address 0x3D for 128x64
    { 
        Serial.println("SSD1306 allocation failed");
        for (;;)
            ; 
    }

    // writting the anchors the coordinates in 2D:

    coordinates_anchors->matrix[0][0] = 1; // x
    coordinates_anchors->matrix[0][1] = 1; // y
    coordinates_anchors->matrix[0][2] = 0; // z

    coordinates_anchors->matrix[1][0] = 1; // x
    coordinates_anchors->matrix[1][1] = 6; // y
    coordinates_anchors->matrix[1][2] = 0; // z

    coordinates_anchors->matrix[2][0] = 7; // x
    coordinates_anchors->matrix[2][1] = 1; // y
    coordinates_anchors->matrix[2][2] = 0; // z
}

void loop()
{
    uint8_t number_anchors = WiFi.scanNetworks();

    for (size_t i = 0; i < ANCHORS; i++)
        length_between_beacon_anchor->matrix[i][0] = get_distance(SSID_anchors[i], 
                                                                  number_anchors,
                                                                  measured_power,
                                                                  N);

    display_coordinates();

    delay(30);
}

double get_distance(String SSID_anchor, 
                    uint8_t number_anchors, 
                    int16_t measured_power, 
                    double N)
{
    for (size_t i = 0; i < number_anchors; i++)
    {
        if (WiFi.SSID(i) == SSID_anchor)
        {
            /* 
            // formula for converting RSSI to distance is:
            // (10 ^ (Measured Power - Instant RSSI)) / (10 * N)
            // Measaured Power is the RSSI value at one meter
            // N is the constant for the environmental factor
            */ 
            return ( pow(10, (measured_power - WiFi.RSSI(i)) ) ) / (10 * N);
        }
    }

    Serial.println("There are no this SSID near beacon");
    return -1;
}

void display_coordinates()
{
    if (length_between_beacon_anchor == NULL)
        return;

    // Clear the buffer.
    display.clearDisplay();
    display.display();
    display.clearDisplay();

    display.setTextSize(1);
    display.setCursor(0, 5);
    display.println("Coordinates:"); 

    matrix_t* coordinates_beacon = solve_equation(coordinates_anchors, length_between_beacon_anchor);

    const char* dimensions = "xyz";

    for (size_t i = 0; i < coordinates_beacon->size_row; i++)
        display.printf("%s: %f\n", dimensions[i], coordinates_beacon->matrix[i][0]);

    display.println("distance between anchors: ");

    for (size_t i = 0; i < ANCHORS; i++)
        display.printf("%s: %f\n", SSID_anchors[i].c_str(), get_distance(SSID_anchors[i], ANCHORS, measured_power, N));

    free_matrix(coordinates_beacon);
}

