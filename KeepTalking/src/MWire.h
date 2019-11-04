#pragma once

#define WIRE_NUM 6
#define WIRE_NUMCOLOR 6


class MWire : public Module
{
private:
    uint8_t wires[WIRE_NUM];
    uint8_t wireColorCount[WIRE_NUMCOLOR];
    uint8_t nWires;
    uint8_t solution;
    uint8_t divergentWires;
    uint8_t cutWires;

    uint64_t updateTime = 0;
    uint8_t wireState;

    uint8_t lastColoredWire(uint8_t c)
    {
        for(uint8_t i=WIRE_NUM-1; i>=0; i--) {
            if(wires[i] == c) {
                return i;
            }
        }
        return 0;
    }

    void checkWires()
    {       
        divergentWires = 0;
        
        for(uint8_t i=0; i<WIRE_NUM; i++) {
            bitWrite(divergentWires, i, (wires[i] > 0) != inputPressed(BTN_WIRE+i));
        }
    }

    void setWireRGB(uint8_t n, uint8_t c)
    {
        pixel.setPixelColor(RGB_WIRE + n, colors[3*c], colors[3*c+1], colors[3*c+2]);
        pixel.setPixelColor(RGB_WIRE + WIRE_NUM + n, colors[3*c], colors[3*c+1], colors[3*c+2]);
    }

public:
    MWire() {
        slotID = 5;
    }

    void menu()
    {
        uint8_t c = 0;
        for(uint8_t i=0; i<WIRE_NUM; i++) {
            if(inputPressed(BTN_WIRE+i)) {
                c++;
                setWireRGB(i, COLOR_WHITE);
            }
            else setWireRGB(i, 0);
        }
        if(bombType == BOMBCUSTOM) setModule(WIRE_ID, c==1);
    }
    
    void reset()
    {
        // 3-6 wires
        nWires = random(3, WIRE_NUM+1);

        // Initialize variables
        for(uint8_t i=0; i<WIRE_NUMCOLOR; i++) wireColorCount[i] = 0;
        wireState = 0;
        cutWires = 0;

        // Random wire colors
        for(uint8_t i=0; i<WIRE_NUM; i++) {
            wires[i] = i < nWires ? random(1, WIRE_NUMCOLOR) : 0;

            wireColorCount[wires[i]]++;
        }

        // Determining wire to be cut
        if(nWires == 3)
        {
            if(wireColorCount[COLOR_RED] == 0) solution = 1;
            else if(wires[2] == COLOR_WHITE) solution = 2;
            else if(wireColorCount[COLOR_BLUE] > 1) solution = lastColoredWire(COLOR_BLUE);
            else solution = 2;
        }
        else if(nWires == 4)
        {
            if(wireColorCount[COLOR_RED] > 1 && !serialEven) solution = lastColoredWire(COLOR_RED);
            else if(wires[3] == COLOR_YELLOW && wireColorCount[COLOR_RED] == 0) solution = 0;
            else if(wireColorCount[COLOR_BLUE] == 1) solution = 0;
            else if(wireColorCount[COLOR_YELLOW] > 1) solution = 3;
            else solution = 1;
        }
        else if(nWires == 5)
        {
            if(wires[4] == COLOR_GREEN && !serialEven) solution = 3;
            else if(wireColorCount[COLOR_RED] == 1 && wireColorCount[COLOR_YELLOW] > 1) solution = 0;
            else if(wireColorCount[COLOR_GREEN] == 0) solution = 1;
            else solution = 0;
        }
        else
        {
            if(wireColorCount[COLOR_YELLOW] == 0 && !serialEven) solution = 2;
            else if(wireColorCount[COLOR_YELLOW] == 1 && wireColorCount[COLOR_WHITE] > 1) solution = 3;
            else if(wireColorCount[COLOR_RED] == 0) solution = 5;
            else solution = 3;
        }
    }

    void setup()
    {
        if(millis() - updateTime >= 500)
        {
            checkWires();

            for(uint8_t i=0; i<WIRE_NUM; i++) {
                if(bitRead(divergentWires, i)) setWireRGB(i, i<nWires ? COLOR_BLUE : COLOR_RED);
                else setWireRGB(i, i<nWires ? COLOR_GREEN : 0);
            }

            if(divergentWires > 0) {
                pixel.setPixelColor(statusPixel[slotID], 180*pixelB, 75*pixelB, 0);
                wireState = 0;
            }
            else {
                pixel.setPixelColor(statusPixel[slotID], 0, 0, 255*pixelB);
                
                if(wireState < 1) wireState++;
                else {
                    // quit setup
                    wireState = 0;
                    state = 2;
                }
            }
            updateTime = millis();
        }
    }

    void run()
    {
        for(uint8_t i=0; i<WIRE_NUM; i++) setWireRGB(i, wires[i]);
        
        checkWires();

        if(millis() - updateTime >= 500)
        {
            bool resetState = true;
            
            for(uint8_t i=0; i<nWires; i++) {
                if(bitRead(cutWires, i)) continue;

                if(bitRead(divergentWires, i)) {
                    resetState = false;

                    if(wireState < 1) wireState++;
                    else {
                        bitSet(cutWires, i);
                        if(i == solution) defused();
                        else add_strike();
                    }
                    break;
                }
            }

            if(resetState) wireState = 0;
            updateTime = millis();
        }
    }
};