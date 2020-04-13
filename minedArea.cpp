#include <iostream>
#include <time.h>//rand
#include <stdlib.h>//srand
#include "windows.h"//coord 
using namespace std;

void gotoxy(int column, int line);

class mine
{
    public: 
        int height=6;
        int width=5;
        int score =0 ;
        int mineIndex;
        char minedArea[25][25];
        COORD touchPoint;

        char safeAreaChar=char(176);
        char minedAreaChar= char(254);
        char explodedAreaChar = char(177);
        char footprintChar = char(178);
        char road = char(186);

        char roadBRight = char(188);
        char roadBLeft = char(200);
        char roadBottom = char(205);
        char roadTRight = char(187);
        char roadTLeft = char(201);

        bool showMine = false; // for testing, you can see the mine(s)
        bool isAlive;


     mine(int height, int width)
     {
         this->height = height;
         this->width = width;
     }

     void drawArea(int line) 
     {
         int row = 0;
         cout << "\t\t\t" << roadTLeft;//top
         for (int i = 0; i < width; i++)
             cout << roadBottom;
         cout << roadTRight << endl;

         cout << "\t\t\t"<<road;
         for (int i = 0; i < width; i++)//number line
         {
             cout << i + 1;
         }
         cout <<road<< endl;
         for (row; row < line; row++)
         {
             cout << "\t\t\t" << road;

             for (int j = 0; j < width; j++)
             {
                 if (showMine)
                     cout << minedArea[row][j];
                 else
                 {
                     if (safeAreaChar != minedArea[row][j] && footprintChar != minedArea[row][j])
                     {
                         cout << safeAreaChar;
                     }
                     else
                         cout << minedArea[row][j];
                 }
             }    
             cout <<road<< "\n";   
         }
         cout << "\t\t\t" << roadBLeft;//bottom
         for (int i = 0; i < width; i++)
             cout << roadBottom;
         cout<< roadBRight << endl;
         writeScore();
         gotoxy(0, row+5);
     }

     void toMine(bool expertMode)
     {
         srand(time(NULL));
         for (int i = 0; i < height; i++)
         {
             mineIndex = rand() % (width+1);
             int j = 0;
             for ( j ; j < mineIndex - 1; j++) // safe area before mine
                 minedArea[i][j] = safeAreaChar;
             minedArea[i][j+1] = minedAreaChar;// mined area
             for (int k = 0; k < width - mineIndex; k++)// safe area after mine
                 minedArea[i][j+k+1] = safeAreaChar;
         }
         if (expertMode)
         {
             for (int i = 0; i < height; i++)
             {
                 mineIndex = rand() % (width + 1);
                 minedArea[i][mineIndex] = minedAreaChar;
             }
         }
     }

     void explosion()
     {
         touchPoint.X -= 1;
         minedArea[touchPoint.Y-1][touchPoint.X] = explodedAreaChar;
         minedArea[touchPoint.Y][touchPoint.X -1 ] = explodedAreaChar;
         minedArea[touchPoint.Y][touchPoint.X] = explodedAreaChar;
         minedArea[touchPoint.Y][touchPoint.X +1] = explodedAreaChar;
         minedArea[touchPoint.Y+1][touchPoint.X] = explodedAreaChar;
         showMine = true;
         drawArea(height);
         cout << "\nGAME OVER\a\nYou DIED \n\n";
         isAlive = false;
     }

     void walk()
     {
         touchPoint.X = 2;
         int input=2;
         for (int i = 0; i < height; i++)
         {
             drawArea(height);
             do {
                 cout << "Numara ile bir kare secin.../Select a square with the number...\n"; cin >> input;
                 if (!(input == touchPoint.X || input == touchPoint.X + 1 || input == touchPoint.X - 1))
                     cout << "\nSadece komsu karelere hareket edebilirsin.../You can only move to neighboring squares...\n";
             }while(i && !(input == touchPoint.X || input == touchPoint.X + 1 || input == touchPoint.X - 1));
             
             touchPoint.X=input; touchPoint.Y = i;
             if (safeAreaChar == minedArea[i][touchPoint.X - 1])
             {
                 score++;
                 system("cls");
             }
                 
             else
             {
                 system("cls");
                 explosion();
                 return;
             } 
             minedArea[touchPoint.Y][touchPoint.X - 1] = footprintChar;
         }
         cout << "\n\t\t    You KEEP ALIVE \a\n";
         system("cls");
         isAlive = true;
     }

     void writeScore()
     {
         gotoxy(27+width, 2); cout << "SCORE";
         gotoxy(28 + width, 3); cout << roadTLeft;
         if (score < 10)cout << roadBottom;
         else cout << roadBottom << roadBottom;
         cout << roadTRight;
         gotoxy(28 + width, 4); cout << road << score << road;
         gotoxy(28 + width, 5); cout << roadBLeft;
         if (score < 10)cout << roadBottom;
         else cout << roadBottom << roadBottom;
         cout << roadBRight << endl;
     }
    
     ~mine()
     {}
};

int main()
{
    cout << "\t\t  ------EASY------\n";
    mine m1(2, 8);
    m1.toMine(false);
    m1.walk();
    if (m1.isAlive)
    {
         cout << "\t\t ------MEDIUM------\n";
         mine m2(4, 7); m2.score=m1.score;
         m2.toMine(false);
         m2.walk();
         if (m2.isAlive)
         {
             cout << "\t\t  ------HARD------\n";
             mine m3(8, 6); m3.score = m2.score;
             m3.toMine(false);
             m3.walk();

             if (m3.isAlive)
             {
                 cout << "\t\t ------EXPERT------\n\t      Some line have two mines\n";
                 mine m4(8, 5); m4.score = m3.score;
                 m4.toMine(true);
                 m4.walk();
                 if (m4.isAlive)
                 {
                         cout << "\t\t ------EXCELLENCY------\n\t\tSee the mines\n";
                         mine m5(25, 4); m5.score = m4.score;
                         m5.showMine = true;
                         m5.toMine(true);
                         m5.walk();
                 }
             }
         }
    }
    system("pause");
}
void gotoxy(int column, int line)
{
    COORD coord;
    coord.X = column;
    coord.Y = line;
    SetConsoleCursorPosition(
        GetStdHandle(STD_OUTPUT_HANDLE),
        coord
    );
}
