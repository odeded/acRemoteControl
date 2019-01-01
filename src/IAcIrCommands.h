

class IAcIrCommands
{
public:
    virtual void sendOn(int temp) = 0;
    virtual void sendOff(int temp) = 0;

    virtual void setTemp(int temp) = 0;
};
