#include "utility.h"

#ifndef NP_CLUMP_H
#define NP_CLUMP_H

#include <vector>
//using namespace std;

class Clump
{
private:
	std::vector<int> m_food;
	Position m_pos;
	int m_SpawnRate;
	int m_progress;
	int m_me;


	void SpawnAFood();

public:
    void  ClumpReport();
    Clump();
    void SetSpawnRate(int sr);
    void Update();
    int GetMe();

    bool OwnerOf(int food);
    int GetAFood();
    bool HasFood();
    void SetPos(const Position& apos);
    void SetMe(int i);

    Position GetPosition();
    int GetIndex();
    bool RemoveFood(int food);
    int GetFoodCount();
};


#endif

