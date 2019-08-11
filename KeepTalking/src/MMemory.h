#pragma once

#define MEMORY_PIXEL 15

class MMemory : public Module
{
private:

public:
    MMemory() {
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