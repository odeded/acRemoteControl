#pragma once

#include <vector>

#define MAX_TEMP (30)

class electraAcRecordedSender
{
  public:
    electraAcRecordedSender(int _IRledPin);

    void sendAcCoolCommand(int temp);
    void sendAcHeatCommand(int temp);

  private:
    void pulseIR(long microsecs);
    void SendBuffer(std::vector<int> buf);

  private:
    int IRledPin;

	static std::vector<int> coolIrBuffer[30];
	static std::vector<int> heatIrBuffer[30];

	static std::vector<int> hot24;
	static std::vector<int> hot25;
	static std::vector<int> hot26;
	static std::vector<int> hot27;
	static std::vector<int> hot28;

	static std::vector<int> cold20;
	static std::vector<int> cold21;
	static std::vector<int> cold22;
	static std::vector<int> cold23;
	static std::vector<int> cold24;
	static std::vector<int> cold25;

	static std::vector<int> emptyVector;
};