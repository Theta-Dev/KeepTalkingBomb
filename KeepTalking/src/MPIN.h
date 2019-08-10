#pragma once

#define PIN_PIXEL 0

class MPIN : public Module
{
private:

public:
    MPIN() {
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