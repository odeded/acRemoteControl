#include "RgbLed.h"

#include "Arduino.h"

const RgbLed::map_type RgbLed::colorValues = {
    {RgbLed::color::WHITE, tuple_type(255, 255, 255)},
    {RgbLed::color::BLACK, tuple_type(0, 0, 0)},
    {RgbLed::color::RED, tuple_type(255, 0, 0)},
    {RgbLed::color::GREEN, tuple_type(0, 255, 0)},
    {RgbLed::color::BLUE, tuple_type(0, 0, 255)},
    {RgbLed::color::GRAY, tuple_type(128, 128, 128)},
    {RgbLed::color::YELLOW, tuple_type(255, 255, 0)},
    {RgbLed::color::CYAN, tuple_type(0, 255, 255)},
    {RgbLed::color::MAGENTA, tuple_type(255, 0, 255)}
};

RgbLed::RgbLed(int rPin, int gPin, int bPin)
{
    redPin = rPin;
    greenPin = gPin;
    bluePin = bPin;

    ledcAttachPin(redPin, 1);
    ledcAttachPin(greenPin, 2);
    ledcAttachPin(bluePin, 3);

    int frequency = 500;
    ledcSetup(1, frequency, 8);
    ledcSetup(2, frequency, 8);
    ledcSetup(3, frequency, 8);

    setColor(color::BLACK);
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