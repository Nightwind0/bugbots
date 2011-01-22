//MoveToDest calls move.
//Move looks and if there is an bot in the path, it calls BotInPath()
//if there is a bot in path, it returns false.
//MoveToDest() calls ItemInPath() if it moved into a square with an Item

//#include "utility.h"

using namespace std;

#ifndef NP_BUGBOT_H
#define NP_BUGBOT_H

#include <vector>
#include <list>
#include "mainbrain.h"
#include "utility.h"


class BugBot
{
private:
	int m_me;
	int m_color;
	Position m_pos;
	int m_team; //which mainbrain you belong to
	int m_health;

	Flag m_flags;
	Position m_dest; //(-1,-1) for none
	Item m_item; //-1 for none
	int m_targetbot; //The bot that this bot is hunting. -1 for none.
	std::vector<Avoid> m_avoids;


	void CheckRenegade();
	void CheckHunger(); //for use in update
	void CheckFollow(); //for use in update
	void CheckDest(); //for use in update
	void AreaScan(); //for use in update	

	void AvoidUpdate(Avoid Param); //for use in update
	bool Move(const Position& pos); //try to move to a square
	bool Attack(int enemy); //attack another bugbot, return true if winner
	void Eat(Item item); //eat a food or a corpse
	void Die();

	void PickUp(Item item);
	bool MoveToDest(); //go towards destination
	bool IsCannibal();


	void DropItem();
	void ItemInPath(const Position& pos);
	bool BotInPath(const Position& pos); //true if a bot still exists at pos, false if not
	std::vector<Item> Radar();

	bool starved;
	int ticks_dead;

public:

	bool IsHungry();
	bool IsStarving();
	bool HasItem();


	Position GetDest(){ return m_dest; }
	int GetTargetBot(){ return m_targetbot; }
	int GetItemVNUM(){ return m_item.vnum; }
	bool GetItemBot(){ return m_item.bot; }
	void BugBotReport();
	BugBot();
	void SetMe(int i);
	int GetMe();
	int GetColor();

	Flag GetFlags();
	int GetTeam();
	int GetHealth();
	Position GetPos();
	bool IsCorpse();

	bool IsRenegade();
	bool IsCarried();
	void SetCarried(bool b);
	void PrepareForAttack(); //called before a battle
	void Update();

	void FoodFromGod(int FoodValue); //called by a mainbrain when the brain feeds it
	void ReachedTarget(); //called by mainbrain during its sweep if it finds a mainbrain
	void SetPos(const Position& pos, bool replace=false); //called when another bot drags a corpse
	void SetTeam(int ateam);
	void RemoveRefOfBot(int i);
	void RemoveRefOfFood(int i);
};
    
    
#endif

