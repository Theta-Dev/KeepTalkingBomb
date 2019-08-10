#pragma once

#define CWIRE_PIXEL 2

class MCWire : public Module
{
private:

public:
    MCWire() {
        statusPixel = CWIRE_PIXEL;
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