#pragma once

#define ONFIRST_PIXEL 15

class MOnFirst : public Module
{
private:

public:
    MOnFirst() {
        slotID = 4;
    }

    void menu()
    {
        
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