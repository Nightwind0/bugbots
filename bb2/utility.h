#ifndef NP_UTILITY_H
#define NP_UTILITY_H


#include <fstream>
#include <list>


// It's spelled: "Cannibal"

// Utility Class Prototypes
class Flag;
class Avoid;
class Position;
class Item;
class Pixel;
class BugBot;
class MainBrain;
class Clump;
class Food;

struct Spot;

//extern ofstream errorlog;


// Utility Function Prototypes


double DistPTP(Position pt1, Position pt2); //distance between points in pixels
void KillIanMoore();

const int NumWords = 2;
const int MaxFlag = 32 * NumWords;


// Utility Class Definitions
class Position
{
public:
	Position(int param1=0 , int param2 =0);
	Position(const Position& pos);
	bool operator<(Position operand){return true;}
	bool operator==(Position operand);
	bool operator!=(Position operand) {return !operator==(operand);}

	Position operator=(const Position operand);
	void Random(); //sets x and y random on the screen in-descriminantly


	int x;
	int y;
};

class Flag
{
private:
	long int flag[NumWords];
	long int FlagNumToBit(int FlagNum);
public:
	Flag();
	int SetFlag(int FlagNum);
	int RemoveFlag(int FlagNum);
	bool IsSet(int FlagNum);
	bool operator[](int FlagNum);

	int operator+=(int FlagNum);
	int operator-=(int FlagNum);
};


class Avoid
{
public:
	Avoid();
	Avoid(Position pos, int time);
	Avoid(int x, int y, int time);
	void Update();
	bool IsOver();
	bool operator==(Avoid op);
	

	Position Location;
	int Timer;
};

class Item
{
public:
	Item();
	Item(int avnum, bool bot);

	
	int vnum;	
	bool bot;
};

class Pixel
{
 private:

 public:
    Pixel();
    Pixel(int x, int y, double dist);
    ~Pixel(){}
    Pixel(Position InitLoc, double dist);
    bool operator<(Pixel operand) const;
    bool operator>(Pixel operand) const ;
    bool operator==(Pixel operand) const;
    
    double DistToDest;
    Position Location;
};

// Global Constants
const Position NoPosition = Position(-1,-1);
const int AvoidLength = 5;
const int RadarDist = 5;
const int TargetZoneSize = 10;
const int FoodValue=300;
const int CorpseValue=1000;
enum {CANNIBAL = 1, HUNGRY, STARVING, CORPSE, RENEGADE, SEARCHING, CARRIED};
#endif

