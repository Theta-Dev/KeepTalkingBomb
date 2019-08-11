#pragma once

#define MAZE_MAX 1
#define MAZE_BUTTON 22
#define MAZE_NBUTTON 4

#define MAZE_NUM 9
#define MAZE_SIZE 6

/*
Maze syntax:
BIT 4 3 2 1 0
POS M L D R U
*/

const uint8_t PROGMEM pgm_maze[] = {
    // Maze 0
    B00000, B00100, B00010, B01000, B00100, B00100,
    B10010, B01001, B00110, B01100, B00101, B00001,
    B00010, B01100, B00011, B01001, B00101, B10000,
    B00010, B01101, B00100, B00110, B01101, B00000,
    B00000, B00101, B00011, B01001, B00111, B01000,
    B00000, B00011, B01000, B00010, B01001, B00000,
    // Maze 1
    B00100, B00000, B00110, B01000, B00000, B00100,
    B00001, B00110, B01001, B00110, B11100, B00001,
    B00010, B01001, B00110, B01001, B00101, B00000,
    B00000, B10110, B01001, B00110, B01011, B01000,
    B00010, B01011, B01010, B01001, B00110, B01000,
    B00010, B01000, B00010, B01000, B00001, B00000,
    // Maze 2
    B00000, B00100, B00010, B01010, B01000, B00000,
    B00110, B01011, B01010, B01100, B00110, B01000,
    B00001, B00010, B01010, B01001, B00011, B01000,
    B00010, B01010, B01010, B11010, B01010, B11000,
    B00010, B01100, B00110, B01010, B01010, B01000,
    B00000, B00001, B00001, B00000, B01000, B00000,
    // Maze 3
    B10000, B00010, B01100, B00100, B00100, B00000,
    B00010, B01010, B01001, B00101, B00101, B00000,
    B00010, B01100, B00110, B01001, B00111, B01000,
    B10010, B01101, B00101, B00100, B00101, B00000,
    B00000, B00101, B00101, B00101, B00011, B01000,
    B00000, B00001, B00011, B01001, B00010, B01000,
    // Maze 4
    B00100, B00100, B00100, B00100, B00000, B00000,
    B00001, B00101, B00101, B00001, B00110, B01100,
    B00000, B00011, B01101, B00110, B11001, B00001,
    B00010, B01100, B00101, B00011, B01110, B01000,
    B00010, B01001, B00101, B00100, B00111, B01000,
    B00010, B01000, B00001, B10001, B00001, B00000,
    // Maze 5
    B00010, B01000, B00010, B01100, B10000, B00000,
    B00010, B01010, B01010, B01001, B00110, B01000,
    B00000, B00110, B01110, B01010, B01001, B00100,
    B00100, B00011, B01001, B00010, B01010, B01001,
    B00001, B00110, B11110, B01010, B01100, B00000,
    B00000, B00001, B00001, B00010, B01001, B00000,
    // Maze 6
    B00000, B10100, B00100, B00010, B01000, B00000,
    B00010, B01001, B00111, B01100, B00110, B01000,
    B00100, B00110, B01001, B00111, B01001, B00100,
    B00001, B00011, B01000, B00101, B00110, B01001,
    B00010, B01110, B01100, B00101, B00011, B01000,
    B00000, B10001, B00001, B00001, B00000, B00000,
    // Maze 7
    B00010, B01000, B00100, B10010, B01000, B00000,
    B00000, B00100, B00111, B01100, B00110, B01000,
    B00010, B01001, B00101, B00101, B00011, B01000,
    B00010, B01100, B10011, B01101, B00100, B00100,
    B00010, B01011, B01100, B00101, B00101, B00101,
    B00000, B00000, B00001, B00001, B00001, B00001,
    // Maze 8
    B00010, B01000, B00100, B00100, B00000, B00000,
    B00010, B01010, B11001, B00111, B01010, B01000,
    B00000, B00100, B00110, B01001, B00110, B01000,
    B00010, B01011, B01001, B00110, B01101, B00000,
    B10010, B01010, B01010, B01001, B00011, B01100,
    B00000, B00010, B01000, B00010, B01000, B00001
};


class MMaze : public Module
{
private:
    uint8_t mazeId;
    uint8_t blinkState = 0;
    uint64_t blinkTime = 0;

    uint8_t mazePos[2];
    uint8_t mazeTarget[2];

    uint8_t getField(uint8_t x, uint8_t y)
    {
        return pgm_read_byte_near(pgm_maze + MAZE_SIZE*MAZE_SIZE*mazeId + MAZE_SIZE*y + x);
    }

public:
    MMaze() {
        slotID = 5;
    }

    void menu()
    {
        if(inputClicked(MAZE_BUTTON, MAZE_NBUTTON) > -1) toggleModule(MAZE_ID);
    }

    void reset()
    {
        // Random maze
        mazeId = random(MAZE_NUM);

        // Random positions
        mazePos[0] = random(MAZE_SIZE);
        mazePos[1] = random(MAZE_SIZE);
        mazeTarget[0] = random(MAZE_SIZE);
        mazeTarget[1] = random(MAZE_SIZE);

        while(bitRead(getField(mazePos[0], mazePos[1]), 4)) {
            mazePos[0]++;
            if(mazePos[0] >= MAZE_SIZE) mazePos[0] = 0;
        }

        for(uint8_t i=0; i<2; i++) {
            while(abs(mazePos[i]-mazeTarget[i]) < 2 || bitRead(getField(mazeTarget[0], mazeTarget[1]), 4)) {
                mazeTarget[i]++;
                if(mazeTarget[i] >= MAZE_SIZE) mazeTarget[i] = 0;
            }
        }
        
        // Display border
        max.setRow(MAZE_MAX, 0, 255);
        max.setRow(MAZE_MAX, 7, 255);
        max.setColumn(MAZE_MAX, 0, 255);
        max.setColumn(MAZE_MAX, 7, 255);
    }

    void setup()
    {
        state = 2;
    }

    void run()
    {
        // Read buttons
        char in = inputClicked(MAZE_BUTTON, MAZE_NBUTTON);

        if(in >= 0) {
            if(bitRead(getField(mazePos[0], mazePos[1]), in)) add_strike();
            else {
                mazePos[0] += (in==1 && mazePos[0] < MAZE_SIZE-1);
                mazePos[0] -= (in==3 && mazePos[0] > 0);
                mazePos[1] += (in==2 && mazePos[1] < MAZE_SIZE-1);
                mazePos[1] -= (in==0 && mazePos[1] > 0);

                if(mazePos[0] == mazeTarget[0] && mazePos[1] == mazeTarget[1]) defused();
                else click();
            }
        }
        
        // Update blink state
        uint64_t now = millis();

        if(now-blinkTime >= 100) {
            blinkState++;
            if(blinkState > 19) blinkState = 0;
            blinkTime = now;
        }
        
        // Display maze
        for(uint8_t y=0; y<MAZE_SIZE; y++) {
            uint8_t row=B10000001;
            for(uint8_t x=0; x<MAZE_SIZE; x++) {
                if(mazePos[0]==x && mazePos[1]==y) bitWrite(row, MAZE_SIZE-x, blinkState%2);
                else if(mazeTarget[0]==x && mazeTarget[1]==y) bitWrite(row, MAZE_SIZE-x, blinkState<10);
                else bitWrite(row, MAZE_SIZE-x, bitRead(getField(x, y), 4));
            }
            max.setRow(MAZE_MAX, y+1, row);
        }
    }
};