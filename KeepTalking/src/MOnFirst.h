#pragma once

#define ONFIRST_PIXEL 15

class MOnFirst : public Module
{
private:

public:
    MOnFirst() {
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