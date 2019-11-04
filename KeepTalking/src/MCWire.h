#pragma once

#define CWIRE_NUM 6
#define CWIRE_NUMCOLOR 4

class MCWire : public Module
{
private:
    uint8_t nWires;
    uint8_t divergentWires;
    uint8_t cutWires;
    uint8_t solution;
    
    // Syntax:
    // Bit 3:Red, 2:Blue, 1:LED, 0:Star
    uint8_t wires[CWIRE_NUM];

    uint64_t updateTime = 0;
    uint8_t wireState;


    void checkWires()
    {       
        divergentWires = 0;
        
        for(uint8_t i=0; i<CWIRE_NUM; i++) {
            bitWrite(divergentWires, i, (wires[i] > 0) != inputPressed(BTN_WIRE+i));
        }
    }

    void setWireRGB(uint8_t n, uint8_t c)
    {
        pixel.setPixelColor(RGB_WIRE + n, colors[3*c], colors[3*c+1], colors[3*c+2]);
        pixel.setPixelColor(RGB_WIRE + WIRE_NUM + n, colors[3*c], colors[3*c+1], colors[3*c+2]);
    }

    void showWireColors()
    {
        for(uint8_t i=0; i<CWIRE_NUM; i++) {
            if(i >= nWires || bitRead(cutWires, i)) setWireRGB(i, 0);
            else {
                uint8_t c;

                switch(wires[i] & B1100) {
                    case B1000: c = COLOR_RED; break;
                    case B0100: c = COLOR_BLUE; break;
                    case B1100: c = COLOR_PURPLE; break;
                    default: c=COLOR_WHITE; break;
                }
                setWireRGB(i, c);
            }
            
            max.setLed(MAX_LEDS, LED_WIRE_R, LED_WIRE_C+i, bitRead(wires[i], 1));
            max.setLed(MAX_LEDS, LED_WIRESTAR_R, LED_WIRESTAR_C+i, bitRead(wires[i], 0));
        }
    }

public:
    MCWire() {
        slotID = 5;
    }

    void menu()
    {
        if(bombType == BOMBCUSTOM) {
            uint8_t c = 0;
            for(uint8_t i=0; i<CWIRE_NUM; i++)
                if(inputPressed(BTN_WIRE+i)) c++;
            setModule(CWIRE_ID, c>1);
        }
    }

    void reset()
    {
        // 3-6 wires
        nWires = random(3, CWIRE_NUM+1);

        for(uint8_t i=0; i<CWIRE_NUM; i++) {
            wires[i] = i<nWires ? random(1, 256) : 0;
        }

        wireState = 0;
        cutWires = 0;
        solution = 0;
        
        // Determining wire to be cut
        for(uint8_t i=0; i<nWires; i++)
        {           
            uint8_t w = wires[i] & B1111;
            
            // Cut
            if(w==B0000 || w==B0001 || w==B1001) bitSet(solution, i);
            // Cut if even
            else if((w==B0100 || w==B1000 || w==B1100 || w==B1110) && serialEven) bitSet(solution, i);
            // Cut if 2 or more batteries
            else if((w==B1010 || w==B1011 || w==B0011) && batteryLevel >= 2) bitSet(solution, i);
            // Cut if parallel port
            else if((w==B0110 || w==B0111 || w==B1101) && bitRead(indicators, INDICATOR_PAR)) bitSet(solution, i);
        }
    }

    void setup()
    {
        if(millis() - updateTime >= 500)
        {
            checkWires();

            for(uint8_t i=0; i<CWIRE_NUM; i++) {
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
        showWireColors();
        
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
                        showWireColors();
                        
                        if(!bitRead(solution, i)) add_strike();
                        else if((cutWires & solution) == solution) defused();
                        else click();
                    }
                    break;
                }
            }

            if(resetState) wireState = 0;
            updateTime = millis();
        }
    }
};