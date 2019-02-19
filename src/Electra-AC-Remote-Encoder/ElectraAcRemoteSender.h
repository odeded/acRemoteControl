#pragma once

class ElectraAcRemoteSender
{
  public:
    enum State
    {
        off = 0,
        on = 1
    };
    enum Fan
    {
        low = 1,
        medium = 2,
        high = 3,
        autoFan = 4
    };
    enum Mode
    {
        cool = 1,
        heat = 2
    };

  public:
    ElectraAcRemoteSender(int _IRledPin);
    void sendAcCommand(State state, Fan fan, Mode mode, int temp);

  private:
    void irRemoteSendRaw(int *codes, int len);
    void mark(int time);
    void space(int time);

    int IRledPin;
};