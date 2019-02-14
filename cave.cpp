/*
:: Cave game

The player's aim is to discover the largest number of fields
and get out of the cave

Possible moves are
: L - turn left  (costs 10 pts energy)
: R - turn right (costs 10 pts energy)
: N - move player one field forward
    in the direction of sight
    directions are unknown except the first move when you look west
    (costs 15 pts energy)
: T - teleports player in one of the undiscovered fields
    but you lose your orientation
    (costs 30 pts energy)

:: Directions

    ^ 1
2   |
<---|--->
    |   4
  3 v

  1 - up
  2 - left
  3 - down
  4 - right
*/

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <iomanip>
using namespace std;

struct place {
    int x;
    int y;
};

const int width = 10;
const int height  = 8;

void displayBoard(int Board[height][width]) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (Board[i][j] == 7 || Board[i][j] == 9) // *: undiscovered fields
                cout << setw(2) << "*";
            else if (Board[i][j] == 8) // discovered wall
                cout << setw(2) << "#";
            else if (Board[i][j] == 0) // discovered tunnel
                cout << setw(2) << "_";
            else if (Board[i][j] == 5) // player's position
                cout << setw(2) << "O";
        }
        cout << endl;
	}
    cout << endl << endl;
}

bool movePossible(int Board[height][width], int direction, int gx, int gy) {
    if      (direction == 1 && (Board[gx-1][gy] != 7 && Board[gx-1][gy] != 8)) return true;
    else if (direction == 2 && (Board[gx][gy-1] != 7 && Board[gx][gy-1] != 8)) return true;
    else if (direction == 3 && (Board[gx+1][gy] != 7 && Board[gx+1][gy] != 8)) return true;
    else if (direction == 4 && (Board[gx][gy+1] != 7 && Board[gx][gy+1] != 8)) return true;
    else return false;
}

void revealWall(int Board[height][width], int direction, int gx, int gy) { // there should be &Board[height][width] imo
    if      (direction == 1) Board[gx-1][gy] = 8;
    else if (direction == 2) Board[gx][gy-1] = 8;
    else if (direction == 3) Board[gx+1][gy] = 8;
    else if (direction == 4) Board[gx][gy+1] = 8;
}

int makeMove(int Board[height][width], char command, int &direction, int &gx, int &gy, int &energy, int &points) {
    if (command == 'L' || command == 'l') { // turning left
       if (direction == 4) direction = 1;
       else direction++;
       energy -= 10;
    }
    else if (command == 'P' || command == 'p') { // turn right
        if (direction == 1) direction = 4;
        else direction--;
        energy -= 10;
    }
    else if (command == 'N' || command == 'n') {
        if (movePossible(Board, direction, gx, gy)) {
            energy -= 15;
            points++;

            Board[gx][gy] = 0;
            if      (direction == 1) gx--;
            else if (direction == 2) gy--;
            else if (direction == 3) gx++;
            else if (direction == 4) gy++;

            Board[gx][gy] = 5; // new Player's position
        }
        else {
            cout << "Impossible move!" << endl;
            revealWall(Board, direction, gx, gy);
            return gx, gy;
        }
    }
    else if (command == 'T' || command == 't') {
        srand(time(NULL));
        int randomX;
        int randomY;
        bool fit = true;
        do {
            randomX = rand() % height;
            randomY = rand() % width;
            if (Board[randomX][randomY] != 7 && Board[randomX][randomY] != 8) {
                Board[gx][gy] = 0; // marking the tunnel
                gx = randomX;
                gy = randomY;
                Board[gx][gy] = 5; // marking new position of Player
                energy -= 30;
                points++;
                fit = false;
            }
            else fit = true;
        } while (fit);
    }
    else cout << "You didn't give an order!" << endl;
    return gx, gy;
}

void informator(int Board[height][width], int gx, int gy, int direction, int energy, int points) {
    cout << "In front of you there is a  ";
    if (direction == 1) { // walkin' north
        if (Board[gx-1][gy] == 7 || Board[gx-1][gy] == 8)       cout << "wall";
        else if (Board[gx-1][gy] == 9 || Board[gx-1][gy] == 0)  cout << "tunnel";
    }
    else if (direction == 2) { // walkin west
        if (Board[gx][gy-1] == 7 || Board[gx][gy-1] == 8)       cout << "wall";
        else if (Board[gx][gy-1] == 9 || Board[gx][gy-1] == 0)  cout << "tunnel";
    }
    else if (direction == 3) { // walkin' south
        if (Board[gx+1][gy] == 7 || Board[gx+1][gy] == 8)       cout << "wall";
        else if (Board[gx+1][gy] == 9 || Board[gx+1][gy] == 0)  cout << "tunnel";
    }
    else if (direction == 4) { // walkin' east
        if (Board[gx][gy+1] == 7 || Board[gx][gy+1] == 8)       cout << "wall";
        else if (Board[gx][gy+1] == 9 || Board[gx][gy+1] == 0)  cout << "tunnel";
    }

    cout << endl
         << "points: " << points << endl
         << "You have " << energy << " points of energy" << endl
         //<< "direction: " << direction << endl // testin'
         << "What you want to do? (L/P/N/T): ";
}

int main() {
    // 7 - '?' : undiscovered wall
    // 8 - '#' : discovered wall
    // 9 - '?' : undiscovered tunnel
    // 0 - '_' : discovered tunnel
    // 6 : entrance
    // 5 : Player
    int Board[height][width] = {
    {7,7,7,7,7,7,7,7,7,7},
    {7,7,7,7,7,7,7,7,7,7},
    {7,7,9,9,9,7,7,9,7,7},
    {7,7,9,7,9,7,7,9,7,7},
    {7,7,9,9,9,9,9,9,9,5},
    {7,7,7,7,7,7,7,7,7,7},
    {7,7,7,7,7,7,7,7,7,7},
    {7,7,7,7,7,7,7,7,7,7},
    };

    place Player;
    Player.x = 4;
    Player.y = 9;

    place Exit;
    Exit.x = 4;
    Exit.y = 9;

    int energy = 500;
    int points = 0;
    char command; // moving
    int direction = 2; // on default Player is oriented on west
    cout << "points: " << points << endl
         << "You have " << energy << " points of energy." << endl
         << "There is a entrance to the cave in front of you" << endl
         << "What you want to do? (L/P/N/T): ";

    while(true) {
        if ((Player.x == Exit.x) && (Player.y == Exit.y) && points > 0) { // win condition
            cout << endl << "YOU WON!" << endl << "Your points: " << points << endl;
            return false;
        }
        if (energy <= 0) { // loose condition
            cout << "You lose!" << endl << "Your points: " << points << endl;
            return false;
        }
        cin >> command;
        system("cls");
        makeMove(Board, command, direction, Player.x, Player.y, energy, points);
        displayBoard(Board);
        informator(Board, Player.x, Player.y, direction, energy, points);

    }

	return 0;
}
