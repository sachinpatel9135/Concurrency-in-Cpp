/*
Entities are Player, Board, Dice, Game
Player - name, position
Dice - will have method *roll()* which should randomly return values from 1 to 6
Board - will have size*size board, (start, end) stored for each snake and ladder
Game - list of players who play game, Board, Dice, Game status
*/

#include<iostream>
#include<queue>
#include<map>
#include<unordered_map>

using namespace std;

class Player{
private:
    string name;
    int position;
public:
    Player(string name, int position = 0) {
        this->name = name;
        this->position = position;
    }

    string getName() {
        return name;
    }

    int getPosition() {
        return position;
    }

    void setPosition(int newPosition) {
        position = newPosition;
    }
};

class Dice{
public:
    int roll() const {
        return ((rand()%6)+1);
    }
};

class Board{
private:
    int size;
    map<int,int> snakes;
    unordered_map<int,int> ladders;
public:
    Board(int size) {
        this->size = size;
    }

    int getSize() {
        return size;
    }

    void addSnake(int start, int end) {
        if(!ladders[start]) snakes[start] = end;
    }

    void addLadder(int start, int end) {
        if(!snakes[start]) ladders[start] = end;
    }

    int getUpdatedPosition(int position) {
        if(snakes.find(position) != snakes.end()) return snakes[position];
        if(ladders.find(position) != ladders.end()) return ladders[position];
        return position;
    }
};

class Game{
private:
    Board board;
    Dice dice;
    queue<Player> players;
    bool isGameOver;
public:
    Game(int boardSize): board(boardSize), isGameOver(false){}
    void addPlayer(string name) {
        Player player(name);
        players.push(player);
    }

    void addSnake(int start, int end) {
        board.addSnake(start,end);
    }

    void addLadder(int start, int end) {
        board.addLadder(start, end);
    }

    void play() {
        while(!isGameOver) {
            Player currentPlayer = players.front();
            cout << currentPlayer.getName() << "'s turn\n";
            players.pop();
            int val = dice.roll();
            int newPosition = currentPlayer.getPosition() + val;
            if(newPosition > board.getSize()) {
                cout << "Exceeded board limit, keep skipping.\n";
            }
            else {
                if(newPosition == board.getSize()) {
                    cout << currentPlayer.getName() << " has won the game.\n";
                    isGameOver = true;
                    return;
                }
                int updatedPosition = board.getUpdatedPosition(newPosition);
                currentPlayer.setPosition(updatedPosition);
                cout << currentPlayer.getName() << " has moved to position " << updatedPosition << "\n";
            }
            if(!isGameOver) players.push(currentPlayer);
        }
    }
};

int main() {
    Game game(100);

    //add snakes
    game.addSnake(99, 54);
    game.addSnake(70, 55);
    game.addSnake(52, 42);

    //add ladders
    game.addLadder(2, 38);
    game.addLadder(15, 26);
    game.addLadder(8, 31);

    //add players
    game.addPlayer("sachin");
    game.addPlayer("opponent");

    game.play();
    return 0;
}