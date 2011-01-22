

#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <queue>


class SoundManager
{

public:

	enum Sound{SND_EAT,SND_DIE,SND_ATTACK,SND_FOOD_FROM_GOD, SND_EAT_BOT,
			SND_RENEGADE, SND_CANNIBAL, SND_BORN};



	~SoundManager();

	static SoundManager *GetInstance();


	
	void SetSound(bool on);


	bool PlaySound(Sound snd);

private:
	
	SoundManager();
	static SoundManager *m_instance;
	bool m_sound;

	int NUM_SOUNDS;
};


#endif
