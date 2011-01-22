#include<iostream>
#include"utility.h"
#include"BugBot.h"
#include"clump.h"
#include"food.h"
#include"mainbrain.h"
#include "sutil.h"
#include <algorithm>
#include <iterator>

void BugBot::BugBotReport()
{
	cout << "BugBot # " << m_me << ":" << endl
		<< "\t";
	if(m_flags[CANNIBAL]) cout << "[CAN]";
	if(m_flags[HUNGRY]) cout << "[HUNGRY]";
	if(m_flags[STARVING]) cout << "[STARVE]";
	if(m_flags[CORPSE]) cout << "[DEAD]";
	if(m_flags[RENEGADE]) cout << "[REN]";
	if(m_flags[SEARCHING]) cout << "[SEARCH]";
	if(m_flags[CARRIED]) cout << "[CARRIED]";
	cout << endl;
	cout << "\t" << "Team: " << m_team << endl
		<< "\t" << "Position: " << m_pos.x << ", " << m_pos.y << endl
		<< "\t" << "Destination: " << m_dest.x << ", " << m_dest.y << endl
		<< "\t" << "Health: " << m_health << endl;
	if(m_item.vnum != -1)
	{
		cout << "\t" << "Carrying ";
		m_item.bot ? cout << "Bot # " : cout << "Food # ";
		cout << m_item.vnum << endl;
	}
	if(m_targetbot != -1)
		cout << "\t" << "Following Bot # " << (int)m_targetbot << endl;


        /*if(!IsCorpse())
                cout << "BotatLoc(m_pos): " << (*BotAtLoc(m_pos)).GetMe() << endl;
        else
                cout << "CorpseAtLoc(m_pos):" << (*CorpseAtLoc(m_pos)).GetMe() << endl;
			*/
	cout << endl;

}

void MainBrain::MainBrainReport()
{

	cout << "MainBrain # " << (int)m_me << endl
		<< "\t" << "Position: " << m_pos.x << ", " << m_pos.y << endl
		<< "\t" << "Resources: " << m_resources << endl
		<< "\t" << "Controlling " << m_bugbots.size() << " BugBots" << endl
		<< "\t" << "Knows of:" << endl
		<< "\t\t" << m_corpses.size() << " Corpses" << endl
		<< "\t\t" << m_clumps.size() << " Clumps" << endl
		<< "\t" << "BugBots controlled indicies:" << endl
		<< "\t\t";
//	copy(m_bugbots.begin(), m_bugbots.end(), ostream_iterator<int>(cout, "\n\t\t"));
	cout << endl;

}

void Clump::ClumpReport()
{

	cout << "Clump # " << (int)m_me << endl
		<< "\t" << "Position: " << m_pos.x << ", " << m_pos.y << endl
		<< "\t" << "Food in progress at : " << m_progress << "/" << m_SpawnRate << endl
		<< "\t" << "Controlling: " << m_food.size() << " foods" << endl
		<< "\t" << "Food controlled indicies:" << endl
		<< "\t\t";
//	copy(m_food.begin(), m_food.end(), ostream_iterator<int>(cout, "\n\t\t"));
	cout << endl;

}

void Food::FoodReport()
{
	cout << "Food # " << (int)m_me << endl
		<< "\t" << "Position: " << m_pos.x << ", " << m_pos.y << endl
		<< "\t" << "Owned by: " << m_clump << endl
		<< endl;
       // cout << "FoodAtLoc(m_pos)" << (*FoodAtLoc(m_pos)).GetMe() << endl;
}

