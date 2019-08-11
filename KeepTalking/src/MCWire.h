#pragma once

class MCWire : public Module
{
private:

public:
    MCWire() {
        slotID = 3;
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