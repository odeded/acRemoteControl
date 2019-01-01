#include "freertos/task.h"
#include <memory>
#include <functional>
#include <bits/shared_ptr.h>
#include <pthread.h>

using namespace std;

template <class T>
class Esp32Thread
{
public:
    struct RunnningParams
    {
        RunnningParams(T* _threadClass, void* _funcParams) : threadClass(_threadClass), funcParams(_funcParams)
            {}
        T* threadClass;
        void* funcParams;
    };

private:
    static void threadRunner(void* param)
    {
        RunnningParams* runningParams = (RunnningParams*)param;

        runningParams->threadClass->mainLoop(runningParams->funcParams);
        
        vTaskDelete( NULL );
    }

public:

    void start(RunnningParams params, const char* threadName,int taskCore = -1)
    {
        if (taskCore >= 0)
        {
            xTaskCreatePinnedToCore(
                threadRunner,       /* Function to implement the task */
                threadName, /* Name of the task */
                10000,      /* Stack size in words */
                &params,      /* Task input parameter */
                1,          /* Priority of the task */
                NULL,       /* Task handle. */
                taskCore);  /* Core where the task should run */
        }
        else
        {
            xTaskCreate(
                threadRunner,   /* Task function. */
                threadName, /* String with name of task. */
                10000,     /* Stack size in bytes. */
                &params,      /* Parameter passed as input of the task */
                1,         /* Priority of the task. */
                NULL);     /* Task handle. */
        }
    }
};