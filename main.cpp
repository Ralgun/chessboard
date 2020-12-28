#include "Game.hpp"
#include <iostream>
#include <string>

//Takes care of the entire loop and communication

Game gGame;

void run()
{
    std::cout << gGame.getStringPosition();
    while(true)
    {

        std::string move;
        std::cin >> move;

        if (move != "show")
        {
            while (gGame.rawMove(move))
            {
                //Error occured
                std::cout << "\n" << gGame.getStringPosition();
                std::cout << "Please, write your input again...\n";
                std::cin >> move;

            }
        }
        else
        {
            
        }
        std::cout << "\n" << gGame.getStringPosition();
    }
}

int main()
{

    std::cout << "Chessboard starting...\n";
    run();

    return 0;
}