#include <tuple>
#include <map>

using namespace std;

class RgbLed
{
public:
    RgbLed(int rPin, int gPin, int bPin);
    
    enum color 
    {
        RED,
        GREEN,
        BLUE,
        BLACK,
        WHITE,
        GRAY,
        YELLOW,
        CYAN,
        MAGENTA
    };
    
    void setColor(int red, int green, int blue);
    void setColor(RgbLed::color _color);

    using tuple_type = std::tuple<int, int, int>;
    using map_type = std::map<RgbLed::color, RgbLed::tuple_type>;
    static const map_type colorValues;

private:
    //LED Pins
	int redPin;
	int greenPin;
	int bluePin;
};