#include <queue>

#include "soundmanager.h"

//	enum Sound{SND_EAT,SND_DIE,SND_ATTACK,SND_FOOD_FROM_GOD, SND_EAT_BOT,
//			SND_RENEGADE, SND_CANNIBAL, SND_BORN};

SoundManager * SoundManager::m_instance = 0;

SoundManager::SoundManager()
{
	m_instance = 0;
	NUM_SOUNDS = 8;
	m_sound = true;
}

SoundManager::~SoundManager()
{

}


void SoundManager::SetSound(bool on)
{
	m_sound = on;
}
	
bool SoundManager::PlaySound(Sound snd)
{
	return false;
}

SoundManager * SoundManager::GetInstance()
{
	if(m_instance == 0)
	{
		m_instance = new SoundManager();
	}

	return m_instance;
}





