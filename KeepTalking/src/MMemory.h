#pragma once

#define MEMORY_PIXEL 15

class MMemory : public Module
{
private:

public:
    MMemory() {
        statusPixel = PIN_PIXEL;
    }

    bool menu()
    {
        return true;
    }

    void reset()
    {
    }

    void setup()
    {
        state = 2;
    }

    void run()
    {
    }
};