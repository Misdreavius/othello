//#include<simplecpp>
#include "SDL.h"
#include<iostream>
#include<math.h>
#include<windows.h>

using namespace std;
int blackcounter = 0; int whitecounter = 0;




void printCircle(int x, int  y, int colour, SDL_Renderer* renderer)//prints circle
{
    colour = 255-colour * 255;
    
    int r = 25;
    for (int i = r; i >= -r; i--)
    {
        int ylevel = y + i;
        int xlevel = sqrt(pow(r, 2) - pow(i, 2));
        for (int j = x - xlevel; j <= x + xlevel; j++)
        {
            if (j == x - xlevel || j == x + xlevel || j == x + xlevel-1 || j == x - xlevel+1)
            {
                SDL_SetRenderDrawColor(renderer, 0,0,0, 255);
            }
            else {
                SDL_SetRenderDrawColor(renderer, colour, colour, colour, 255);
            }
                SDL_RenderDrawPoint(renderer, j, ylevel);
            
        }
    }
    SDL_RenderPresent(renderer);


    /*
    setfillstyle(SOLID_FILL, WHITE);
    setcolor(BLUE);
    circle(x, y, 25);
    floodfill(x, y, BLUE);//Blanks out the circle
    setcolor(BLACK);
    circle(x, y, 25);//Makes the boundary black again
    if (colour == 1)
        setfillstyle(SOLID_FILL, BLACK);
    floodfill(x, y, BLACK);
    */


    //Circle clean(x,y,25); clean.setColor(COLOR("white"));clean.setFill();clean.imprint();//blanks out the previous circle if applicable
    //Circle pt(x,y,25);
    //if(colour==1)
    //pt.setFill();
    //pt.imprint();

}



bool Centre(int& cenx, int& ceny, int x, int y)//finds centre of clicked region
{
    int i;//counter 1
    for (i = 0; i < 8; i++)
    {
        cenx = 75 + (i * 50);//goes through all the x-centres

        if (fabs(cenx - x) <= 25)//finds the correct x centre
            break;

    }
    if (i == 8)
        return false;//Invalid Click


    int j;//counter 2
    for (j = 0; j < 8; j++)
    {
        ceny = 75 + (j * 50);//goes through all the y-centres

        if (fabs(ceny - y) <= 25)//finds the correct y centre
            break;

    }
    if (j == 8)
        return false;//Invalid Click

    return true;

}







bool checkLegalMove(int xarr, int yarr, int arr[8][8], int colour)
{
    bool flag = false;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            int xdirectional = xarr + i; int ydirectional = yarr + j;//checks given direction

            if (arr[xdirectional][ydirectional] == abs(colour - 1))//checks if there exist at least one circle of the opposite colour
            {

                while ((xdirectional >= 0) && (xdirectional <= 7) && (ydirectional >= 0) && (ydirectional <= 7))//this loop checks that if there are any same colour pieces in this direction, till the end of the board
                {
                    if (arr[xdirectional][ydirectional] == colour)

                    {
                        flag = true;
                        return flag;
                    }
                    xdirectional = xdirectional + i; ydirectional = ydirectional + j;
                    if (arr[xdirectional][ydirectional] >= 2)//if there is an unnocuppied spot in the given direction, the direction is wrong
                        break;
                }
            }
        }

    }


    return flag;
}

void playMove(int xarr, int yarr, int arr[8][8], int colour, bool printer, SDL_Renderer* renderer)
{
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            int xdirectional = xarr + i; int ydirectional = yarr + j;

            if (arr[xdirectional][ydirectional] == abs(colour - 1))
            {

                while ((xdirectional >= 0) && (xdirectional <= 7) && (ydirectional >= 0) && (ydirectional <= 7))//same as earlier but for prinitng instead of just checking
                {
                    if (arr[xdirectional][ydirectional] == colour)
                    {
                        int l = xarr; int k = yarr;

                        while (true)//this prints the circle in the given direction
                        {
                            if (printer)//prints the circles if printer is true, otherwise it just updates the array, useful for intelligent computer
                            {
                                printCircle((k * 50) + 75, (l * 50) + 75, colour, renderer); SDL_Delay(300);
                            }
                            arr[l][k] = colour;
                            l += i; k += j;
                            if ((l == xdirectional) && (k == ydirectional))//break on finishing colouring
                                break;
                        }
                        break;
                    }
                    xdirectional += i; ydirectional += j;//used for finding the point to enter the colour printer loop in the given direction
                    if (arr[xdirectional][ydirectional] >= 2)
                        break;
                }

            }
        }
    }
}
bool Existence(int arr[8][8], int colour)//checks for existence of legal move
{
    bool flag = false;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (arr[i][j] >= 2)
                flag = checkLegalMove(i, j, arr, colour);
            if (flag)
                return flag;
        }
    }

    return flag;
}

int ComputerTurn(int arr[8][8], int colour, int& x, int& y, int turncounter, SDL_Renderer* renderer)
{
    int xdemo = 0; int ydemo = 0;//demo unused variables for the sake of function call, since the referencing is only needed in the last call to pinpoint the best move after comparing paths
    static int originalcall = turncounter;
    int arrcopy[8][8];
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
            arrcopy[i][j] = arr[i][j];
    }//copy of array

    if (turncounter == 0) //the last move which the machine is seeing is reached
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (arr[i][j] == 0) whitecounter++;
                if (arr[i][j] == 1) blackcounter++;
            }
        }
        int diff = (blackcounter - whitecounter);
        blackcounter = 0; whitecounter = 0;
        return diff;

    }
    if (Existence(arr, colour) == false && turncounter != originalcall) //if any path leads either player to skip, ignore the path(for now)
    {
        return ComputerTurn(arrcopy, fabs(colour - 1), xdemo, ydemo, (turncounter - 1), renderer);
    }


    int jtot = 0; int itot = 0;//coordinates to return to original caller after cycling
    int maxdiff;
    if (colour == 1)
        maxdiff = -64;
    else
        maxdiff = 64;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {

            if (arrcopy[i][j] < 2)
                continue;
            if (!checkLegalMove(i, j, arrcopy, colour))
                continue;
            playMove(i, j, arrcopy, colour, false, renderer);
            //legal maove found

            int diff = ComputerTurn(arrcopy, fabs(colour - 1), xdemo, ydemo, turncounter - 1, renderer);
            if (colour == 1) {
                if (diff > maxdiff) { maxdiff = diff; jtot = j; itot = i; }//max
            }
            else
            {
                if (diff < maxdiff) { maxdiff = diff; jtot = j; itot = i; }//min
            }



            for (int k = 0; k < 8; k++)//resetting array copy
            {
                for (int l = 0; l < 8; l++)
                    arrcopy[k][l] = arr[k][l];
            }
        }

    }
    if (turncounter == originalcall)
    {
        x = (jtot * 50) + 75; y = (itot * 50) + 75;
    }//returning the coordinates through an uodate
    return maxdiff;//useless return
}



int main(int argc, char* argv[]) {
    int init=SDL_Init(SDL_INIT_EVERYTHING);
    if (init < 0)
    {
        cout << "Failed initializtion";
        return 0;
    }
    int arr[8][8];
    int filler = 2;//filling the array
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            arr[i][j] = filler;
            filler++;
        }
    }
    int choice;
    cout << "Enter 1 for Multiplayer, and 2 for Computer";
    cin >> choice;
    int choice2;
    if (choice == 2)
    {
        cout << "Enter 1 if you want to play black(first) or 2 for white(second)";
        cin >> choice2;
    }
    SDL_Event event;
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    window = SDL_CreateWindow("Othello", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 500, 500, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    //setfillstyle(SOLID_FILL, WHITE);
    //floodfill(250, 250, WHITE);
    for (int i = 0; i < 9; i++)//printing board
    {
        int x = 50 * (i + 1);
        //setcolor(BLACK);
        //line(x, 50, x, 450);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawLine(renderer, x, 50, x, 450);
    }

    for (int i = 0; i < 9; i++)
    {
        int y = 50 * (i + 1);
        //line(50, y, 450, y);
        SDL_RenderDrawLine(renderer, 50, y, 450, y);
    }
    


    //printing starting circles
    arr[3][3] = 0; printCircle(225, 225, 0, renderer);

    arr[3][4] = 1; printCircle(275, 225, 1, renderer);

    arr[4][3] = 1; printCircle(225, 275, 1, renderer);
    arr[4][4] = 0; printCircle(275, 275, 0, renderer);
    SDL_RenderPresent(renderer);
    
    


    int checker = 0;//if checker =2, then that means there have been 2 back to back passes. the game ends
    int movecounter = 0;//move counter
    int checkerflag = 0;//when a player has no legal move, then the checker is updated to 1, if the opponent has a legal move, the checker stays a 1, and hence skips the opponents move. To prevent this a flag will be made to lock entry into checker==1 after first entry. If a legal move is found, then this will reset to orignial value

    while (true)
    {
        if ((choice != 1) && (choice != 2))
            break;
        if ((choice == 2) && (choice2 != 1) && (choice2 != 2))
            break;
        int colour = abs((movecounter % 2) - 1);
        if (!Existence(arr, colour))//checks if legal move doesn't exist
            checker++;

        if ((checker == 1 && checkerflag == 0))//if there is no legal move it skips the turn, but since checker is stll 1, to allow opponent to attempt to play a move after skipper, skipper updates a flag to skip the 'skip turn' command
        {
            movecounter++;
            checkerflag = 1;//changes checkerflag to prevent opponent from forcibly skipping turn

            continue;//skip turn
        }
        if (checker == 2)//double skip- end of game
        {
            break;
        }
        checker = 0; checkerflag = 0;//resetting both if a legal move is found
        int x = 0; int y = 0;
        if ((choice == 1) || ((choice == 2) && (choice2 == (movecounter % 2) + 1)))
        {
            while (true)
            {
                SDL_PollEvent(&event);
                if (SDL_MOUSEBUTTONDOWN == event.type && SDL_BUTTON_LEFT == event.button.button)
                {
                    SDL_GetMouseState(&x, &y);
                    break;
                }
            }
            
            
            //getmouseclick(WM_LBUTTONDOWN, x, y);
            
            
            
            //int Pos=getClick();
            //x=Pos/65536;
            //y=Pos%65536;
        }
        else
        {
            ComputerTurn(arr, colour, x, y, 6, renderer);

        }
        int cenx = 0;
        int ceny = 0;


        bool validclick = Centre(cenx, ceny, x, y);
        int arr_x = (ceny - 75) / 50;
        int arr_y = (cenx - 75) / 50;
        if (validclick == false)//if click is outside the board
            continue;

        if (arr[arr_x][arr_y] < 2)//checks if we are clicking an empty spot
            continue;


        if (checkLegalMove(arr_x, arr_y, arr, colour))
        {
            printCircle(cenx, ceny, colour, renderer);//print and save move if legal
            arr[arr_x][arr_y] = colour;
            SDL_Delay(500);
        }
        else
        {
            continue;//skip loop if wrong click
        }
        playMove(arr_x, arr_y, arr, colour, true, renderer);//plays move and prints circles
        
        SDL_Delay(500);
        movecounter++;


    }
    int blackcounter = 0; int whitecounter = 0;
    for (int i = 0; i < 8; i++)//win checker
    {
        for (int j = 0; j < 8; j++)
        {
            if (arr[i][j] == 0)
                whitecounter++;
            if (arr[i][j] == 1)
                blackcounter++;

        }
    }
    if (movecounter > 0) {
        SDL_Delay(4000);

        cout << "Black Score:" << blackcounter << " White Score:" << whitecounter << endl;
        if (whitecounter > blackcounter)
        {
            if (choice == 1)
                cout << "White Wins";
            else
            {
                if (choice2 == 2)
                    cout << "You Win";
                else if (choice2 == 1)
                    cout << "Computer Win";
            }
        }
        else if (whitecounter < blackcounter)
        {
            if (choice == 1)
                cout << "Black Wins";
            else
            {
                if (choice2 == 1)
                    cout << "You Win";
                else if (choice2 == 2)
                    cout << "Computer Win";
            }
        }
        else
            cout << "Draw";
    }
    else
        cout << "Invalid Input";
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

