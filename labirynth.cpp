/*
:: Labirynth game

:: You start at the bottom left corner
:: The meta is at the top right corner

:: There are 4-6 random generated traps
    if you step on one of them, you lose
    you will be noticed while trap are next to you

:: Get the meta as fast as you can
*/

#include <stdlib.h>  // rand things
#include <time.h>    // random, huh?
#include <iostream>  // probably enough for "cls"?
#include <iomanip>
using namespace std;

const unsigned short MW = 20; // max width of the board
const unsigned short MH = 20; // max height

struct object {
    int x;
    int y;
}; // SURPRISE: 'x' of the object is an 'y' in practice


void displayWholeBoard(unsigned short Board[MW][MH]) {
    for (int i = 0; i < MW; i++) {
        for (int j = 0; j < MH; j++) {
            if      (Board[i][j] == 1) cout << setw(2) << "=";
            else if (Board[i][j] == 2) cout << setw(2) << "|";
            else if (Board[i][j] == 3) cout << setw(2) << "+";
            else if (Board[i][j] == 5) cout << setw(2) << ".";
            else if (Board[i][j] == 6) cout << setw(2) << "#";
            else if (Board[i][j] == 7) cout << setw(2) << "M";
            // else if (Board[i][j] == 9) cout << setw(2) << "X"; // for dev purposes
            else if (Board[i][j] == 0 || Board[i][j] == 9) cout << setw(2) << " ";
        }
        cout << endl;
    }
}


void displayQuarter(unsigned short Board[MW][MH], int Qx, int Qy) { // included in displayQuarter()
    cout << endl;
    for (int i = Qx; i < Qx + 10; i++) {
        for (int j = Qy; j < Qy + 10; j++) {
            if      (Board[i][j] == 1) cout << setw(2) << "=";
            else if (Board[i][j] == 2) cout << setw(2) << "|";
            else if (Board[i][j] == 3) cout << setw(2) << "+";
            else if (Board[i][j] == 4) cout << setw(2) << "O";
            else if (Board[i][j] == 5) cout << setw(2) << ".";
            else if (Board[i][j] == 6) cout << setw(2) << "#";
            else if (Board[i][j] == 7) cout << setw(2) << "M";
            else if (Board[i][j] == 9) cout << setw(2) << " "; // make trap not visible
            else if (Board[i][j] == 0) cout << setw(2) << " ";
        }
        cout << endl;
    }
}


void whichQuarter(unsigned short Board[MW][MH], int Px, int Py) { // includes displayQuarter();
    object Quarter;
    if      ((Px <= 9)  && (Py >= 10)) {
        Quarter.x = 0;
        Quarter.y = 10;
    } // first  quarter: x=< 0, 9> and y=<10,19>
    else if ((Px <= 9)  && (Py <=  9)) {
        Quarter.x = 0;
        Quarter.y = 0;
    } // second quarter: x=< 0, 9> and y=< 0, 9>
    else if ((Px >= 10) && (Py <=  9)) {
        Quarter.x = 10;
        Quarter.y = 0;
    } // third  quarter: x=<10,19> and y=< 0, 9>
    else if ((Px >= 10) && (Py >= 10)) {
        Quarter.x = 10;
        Quarter.y = 10;
    } // fourth quarter: x=<10,19> and y=<10,19>
    displayQuarter(Board, Quarter.x, Quarter.y);
}


void trapWarning(unsigned short Board[MW][MH], int Px, int Py) {
    if ((Board[Px][Py-1] == 9 ) || // trap is on the left side
        (Board[Px][Py+1] == 9 ) || // on the right side
        (Board[Px-1][Py] == 9 ) || // above the player
        (Board[Px+1][Py] == 9 ))   // beneath the player
        cout << "Be careful! Trap is next to you!";
}


void createTraps(unsigned short Board[MW][MH], int &t) {
    object buf;
    buf.x = rand() % 18 + 1;
    buf.y = rand() % 18 + 1;
    if (Board[buf.x][buf.y] == 0) // is there a space to place the trap?
        Board[buf.x][buf.y] = 9;  // place trap if ok
    else t--; // generating one more loop for new trap, old was bad
}


bool checkCollision(unsigned short Board[MW][MH], int Px, int Py) {
    if ((Board[Px][Py] == 0) || // 0 means free space
        (Board[Px][Py] == 9) || // 9 means trap
        (Board[Px][Py] == 5) || // 5 means dotted track
        (Board[Px][Py] == 7))   // 7 means finish
         return true;
    else return false;
}


bool changePosition(unsigned short Board[MW][MH], const char choice, int &Px, int &Py, unsigned short &movesCounting) {
    movesCounting++;
    object buf; // track purposes
    buf.x = Px;
    buf.y = Py;
    cout << "Last move: ";
    switch(choice) {
        case 'w': {Px -= 1; cout << "TOP"   << endl; break;}
        case 's': {Px += 1; cout << "DOWN"  << endl; break;}
        case 'a': {Py -= 1; cout << "LEFT"  << endl; break;}
        case 'd': {Py += 1; cout << "RIGHT" << endl; break;}
        default: {cout << "Wrong key, you could only use WSAD set."  << endl; break;}
    }

    if (checkCollision(Board, Px, Py)) { // returns false if everything is ok
        Board[buf.x][buf.y] = 5;  // make a track
        if (Board[Px][Py] == 9) { // if player step on a trap
            Board[Px][Py] = 4;    // make it more visible
            cout << "You lose in " << movesCounting << " moves";
            return false;
        }
        Board[Px][Py] = 6;       // updating the position of player
    }
    else { // due to false player cant move, something in collision
        Px = buf.x;
        Py = buf.y;
        movesCounting--; // do not save bad move
    }
    trapWarning(Board, Px, Py);
    return true;
}

bool winCondition(int Px, int Py, int Fx, int Fy, unsigned short &movesCounting) {
    if (Px == Fx && Py == Fy) {
        cout << "You won in " << movesCounting << " moves" << endl;
        return true;
    }
    else return false;
}

int main() {

    /*
    ----- Legenda -----
    1 stands for '=' barrier
    2 stands for '|' barrier
    3 stands for '+' wall
    4 stands for 'O' which is a symbol of loss
    5 stands for dotted trail/track
    6 will stand for player position
    7 will stand for meta position necessary in checking collision
    9 stands for traps
    0 stands for free space (' ')
    */

    unsigned short Board[MW][MH] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {2,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,3,3,3,3,3,3,0,0,3,0,3,3,3,0,3,0,0,2},
    {2,0,0,0,0,0,3,0,0,3,3,0,0,0,3,0,3,3,0,2},
    {2,3,0,0,3,0,3,0,3,3,3,3,3,0,0,0,3,0,0,2},
    {2,3,3,0,3,3,3,0,3,0,3,0,0,0,0,3,0,0,0,2},
    {2,0,0,0,0,0,0,0,3,0,0,0,3,3,0,0,0,3,0,2},
    {2,0,0,0,3,0,0,0,3,0,3,3,3,3,3,0,3,0,0,2},
    {2,0,0,3,3,0,0,0,0,0,3,0,0,0,0,0,3,0,0,2},
    {2,0,3,3,0,0,0,0,3,0,0,0,3,3,3,0,3,3,0,2},
    {2,0,3,3,3,0,0,3,3,3,3,0,3,3,0,0,0,3,0,2},
    {2,0,0,0,3,3,3,3,0,0,3,0,3,0,0,0,3,3,0,2},
    {2,0,0,0,0,3,0,0,0,3,0,0,3,0,3,0,3,0,0,2},
    {2,3,3,0,0,3,0,3,0,3,0,0,0,0,3,0,3,0,0,2},
    {2,3,3,0,0,3,3,3,0,3,0,0,0,3,3,0,3,0,0,2},
    {2,0,0,0,0,3,0,0,0,3,0,0,3,3,0,0,3,3,0,2},
    {2,0,0,0,0,0,0,0,0,3,0,0,3,0,0,3,3,0,0,2},
    {2,0,0,0,3,0,3,3,0,0,0,0,3,0,0,3,0,0,0,2},
    {2,0,0,0,3,0,0,0,0,0,0,3,3,3,0,0,0,0,0,2},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}, };


    object Player;
    Player.x = 18; // starting player's position
    Player.y = 1;

    object Finish;
    Finish.x = 1;  // you could set finish position
    Finish.y = 18; // if u want

    Board[Finish.x][Finish.y] = 7; // setting the meta
    Board[Player.x][Player.y] = 6; // starting position

    srand(time(NULL));  // seed for some randomness
    int TrAmount = rand() % 3 + 4; // 4-6 generated traps
    for (int t = 0; t < TrAmount; t++)
        createTraps(Board, t);

    displayWholeBoard(Board);

    unsigned short movesCounting = 0;
    char control;
    for(;;) { // here the main loop starts
        cout << "Your move: ";
        cin >> control;
        if (control == 'q') // player want to quit
            return 0;
        else {
            system("cls");
            if (changePosition(Board, control, Player.x, Player.y, movesCounting)) {
                whichQuarter(Board, Player.x, Player.y);
            }
            else {
                whichQuarter(Board, Player.x, Player.y);
                return 0;
            }
        }
        // to inform
        cout << "Position: (" << Player.y << ", " << Player.x << ")"
             << setw(10) << " Moves: " << movesCounting << endl
             << "Press 'q' to quit." << endl;
        if (winCondition(Player.x, Player.y, Finish.x, Finish.y, movesCounting))
            return 0;
    }
}
