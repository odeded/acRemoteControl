#include "RgbLed.h"

#include "Arduino.h"

RgbLed::map_type RgbLed::colorValues = {
    {RgbLed::color::WHITE, {255, 255, 255}},
    {RgbLed::color::BLACK, {0, 0, 0}},
    {RgbLed::color::RED, {255, 0, 0}},
    {RgbLed::color::GREEN, {0, 255, 0}},
    {RgbLed::color::BLUE, {0, 0, 255}},
    {RgbLed::color::GRAY, {128, 128, 128}},
    {RgbLed::color::YELLOW, {255, 255, 0}},
    {RgbLed::color::CYAN, {0, 255, 255}},
    {RgbLed::color::MAGENTA, {255, 0, 255}}
};

RgbLed::RgbLed(int rPin, int gPin, int bPin)
{
    redPin = rPin;
    greenPin = gPin;
    bluePin = bPin;

    ledcAttachPin(redPin, 1);
    ledcAttachPin(greenPin, 2);
    ledcAttachPin(bluePin, 3);

    ledcSetup(1, 12000, 8);
    ledcSetup(2, 12000, 8);
    ledcSetup(3, 12000, 8);

    setColor(0, 250, 0);
}

void RgbLed::setColor(int red, int green, int blue)
{
    ledcWrite(1, red);
    ledcWrite(2, green);
    ledcWrite(3, blue);
}

void RgbLed::setColor(RgbLed::color _color)
{
    auto value = colorValues.find(_color);
    if (value == colorValues.end())
    {
        return;
    }
    setColor(std::get<0>(value->second), std::get<1>(value->second), std::get<2>(value->second));
}