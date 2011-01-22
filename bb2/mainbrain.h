#include <algorithm>
#include "utility.h"
#ifndef NP_MAINBRAIN_H
#define NP_MAINBRAIN_H

#include "utility.h"

#include <list>
#include <vector>


class MainBrain
{
private:
	std::vector<int> m_bugbots;
	std::vector<int> m_clumps;
	std::vector<int> m_corpses; //discovered corpses
	int m_resources;
	Position m_pos;

	int m_me;
	void Assimilate(Item item);
	void CreateBugBot();
	int m_colors[5];
		// Normal, Renagade, Cannibal, Ren + Can, all + food, all + corpse
	

public:
    void MainBrainReport();
	MainBrain();
	void SetColors(int,int,int,int,int);
	void SetPos(Position pos);
	bool CanFeed(int bothealth);

	Position GetTargetPos(const Position& current_pos);				//tells the bugbots where to go
	bool InTargetZone(Position pos);			//whether or not a position is in the target zone
	Position GetPos(); 					//the position of the mainbrain
	void SweepTargetZone(); 				//check target zone for food or corpses and assimilate. Checks for bugbots standing on it and tells them if they are
	void Update();						//sweep and create new bugbots if possable. Also returns false if no bugbots are left

	void ReportFood(int food);				//a bugbot uses this function to say "Hey, I found food in this clump"
	void ReportCorpse(int corpse);				//a bugbot uses this to say that it found a corpse but it can't pick it up
	bool Feed(int bot);					//called by a bot in the target zone instructs brin to feed it, returns false if it can't for some reason.
	int GetColor(Flag param);				//tell a bot what color it should be
	bool OwnerOf(int bot);					//does this brain own the bot with index bot

	int NumberOfBotsControlled();				//used in calculating whether or not a bot will go renegade
	bool RemoveBugBot(int bot);
	void RemoveRefOfCorpse(int bot);
	void SetMe(int i);
	int GetMe();
};


#endif
