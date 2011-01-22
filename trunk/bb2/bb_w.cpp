
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/application.h>
#include <ClanLib/signals.h>
#include "clump.h"
#include "food.h"
#include "BugBot.h"
#include "mainbrain.h"
#include "utility.h"
#include <list>
#include <vector>
#include <algorithm>
//#include <fstream>
#include <time.h>
//#include <conio.h>
#include <iostream>
#include "sutil.h"
#include "MapHandler.h"
#include "soundmanager.h"

using namespace std;


bool double_pixel_mode = true;

//ofstream errorlog;



const extern int SCREENWIDTH ;
const extern int SCREENHEIGHT;

MapHandler MH;

const int size=2;

extern std::vector<std::list<BugBot>::iterator> BotIts;
//according to Dan Smith, this was the offending line

//Spot GlobalMap[320][200];



struct color{
    float r;
    float g;
    float b;
};


color color_map[15] = {
    {0.0,0.0,0.0}, //0 Black
    {0.5,0.0,0.0}, //1 Red
    {0.0,0.5,0.0}, //2 Green
    {0.5,0.5,0.0}, //3 Yellow
    {0.0,0.0,0.5}, //4 Blue
    {0.5,0.0,0.5}, //5 Magenta
    {0.0,0.5,0.5}, //6 Cyan
    {0.5,0.5,0.5}, //7 White (Gray)
    {1.0,0.0,0.0}, //8 Bright Red
    {0.0,1.0,0.0}, //9 Bright Green
    {1.0,1.0,0.0}, //10 Bright Yellow
    {0.0,0.0,1.0}, //11 Bright Blue
    {1.0,0.0,1.0}, //12 Bright Magenta
    {0.0,1.0,1.0}, //13 Bright Cyan
    {1.0,1.0,1.0}};//14 Bright White



bool CheckPosition(const Position &pos)
{
    if(pos.x <0 || pos.x >= SCREENWIDTH)
	std::cout << "Bad x " << pos.x << endl;
    if(pos.y <0 || pos.y >= SCREENHEIGHT)
	std::cout << "Bad y " << pos.y << endl;
											  
    return true;
}



class BBApp: CL_ClanApplication
{
public:
    char *get_title(){ 

		
			return "BugBots 2 v1.0";
	}


	Position translate_pos(int x, int y)
	{
		int rx,ry;

		rx = x / size;
		ry = y / size;

		return Position(rx,ry);
	}


	void on_button_press(CL_InputDevice *device, const CL_Key &key)
	{
		std::list<BugBot>::iterator bb = NULL;


		Position pos = translate_pos(key.x,key.y);
		switch(key.id)
		{

#ifdef _DEBUG
		case CL_MOUSE_RIGHTBUTTON:
			

			bb = MH.GetBugBotAt(pos);
		//	std::cout << "Checking " << pos.x << ',' << pos.y << endl;
			if(bb != NULL)
			{

			if(bb->IsCorpse())
				std::cout << "Corpse";
			else std::cout << "BugBot";	

			std::cout << " at " << pos.x << pos.y << endl;

			if(bb->IsRenegade())
			{
				std::cout << "[RENEGADE]";
			}
			if(bb->GetFlags().IsSet(CANNIBAL))
			{
				std::cout << "[CANNIBAL]";
			}
			if(bb->IsHungry())
			{
				std::cout << "[HUNGRY]";
			}
			if(bb->IsStarving())
			{
				std::cout << "[STARVING]";
			}
			if(bb->IsCarried())
			{
				std::cout << " (CARRIED) " ;
			}
		
			std::cout << endl;

			if(bb->HasItem())
			{
				std::cout << "         CARRYING";
				if(bb->GetItemBot())
				{
					std::cout << " CORPSE ";
				}
				else std::cout << " FOOD ";

				std::cout << bb->GetItemVNUM() << endl;
			}

			std::cout << "         Heading towards " << bb->GetDest().x << ',' << bb->GetDest().y << endl;
			
			}

			bb = MH.GetCorpseAt(pos);

			if(bb != NULL)
			{
				std::cout << "Corpse at " << pos.x << ',' << pos.y<< endl;
				if(bb->IsCarried())
				{
					std::cout << "And it's carried." << endl;
				}
			}


			break;

#endif
		case CL_KEY_P:
			m_paused = m_paused?false:true;
			break;
		case CL_KEY_SPACE:
			m_sound = m_sound?false:true;
			break;
		}

	}



    int main(int argc, char **argv)
	{
	    CL_SetupCore::init();
	    CL_SetupDisplay::init();
		CL_SetupSound::init();
		
	//int time_of_pause = 0;


		m_sound = true;
	    if(argc > 1 )
	    {
		if(!strcmp(argv[1],"--fullscreen"))
		{	
		    double_pixel_mode = false;
		}
	    }

	//	CL_Slot slot1 = CL_Input::sig_button_press().connect(this,
		//	&BBApp::on_button_press);

		CL_Slot anyinput = CL_Input::sig_button_press().connect(
				this, &BBApp::on_button_press);
					     
#ifdef _DEBUG
	    CL_ConsoleWindow console("Console",80,1000);
	    console.redirect_stdio();
	//	console.redirect_stdio("log.txt");

		

#endif
		
		
		CL_ResourceManager *manager = new CL_ResourceManager("data.dat",true);
	    if( double_pixel_mode )		
		CL_Display::set_videomode(SCREENWIDTH * size,SCREENHEIGHT *size,24,false);
	    else
		CL_Display::set_videomode(SCREENWIDTH,SCREENHEIGHT,32,true);

		
		
		SoundManager *sm = SoundManager::GetInstance();
		sm->LoadSounds(manager);

	    m_target = CL_Display::get_target();

	    srand(time(NULL));
 
	    Position pmb1;
	    pmb1.Random();
    
	    MH.NewMainBrain(pmb1,1,8,8,8,3);
   
	    Position pmb2;
	    pmb2.Random();
	    MH.NewMainBrain(pmb2,4,11,11,11,10);

	    Position pc1;
	    pc1.Random();
	    MH.NewClump(pc1,10);

	    Position c2pos;
	    c2pos.Random();
	    MH.NewClump(c2pos,20);

    
	    Position c3pos;
	    c3pos.Random();
	    MH.NewClump(c3pos,30);


    
	    bool done = false;

	    while(!CL_Keyboard::get_keycode(CL_KEY_ESCAPE) && !done)
	    {

		std::vector<Position> corpse_pos;
		m_target = CL_Display::get_target();



		for(std::list<MainBrain>::iterator i=MH.GetMainBrainBegin();
		    i!=MH.GetMainBrainEnd();i++)
		{
				if(!m_paused)
					(*i).Update();
/*
  m_target->draw_pixel((*i).GetPos().x,(*i).GetPos().y,
  color_map[14].r, color_map[14].g,
  color_map[14].b); */
		    if(double_pixel_mode)
		    {
			if(!MH.WithinSpace(Position(i->GetPos())))
			{
			    std::cout << "MB found outside space" << endl;
			}
			for(int r=0; r < size; r++)
			    for(int c=0; c < size; c++)
			    {
				m_target->draw_pixel((*i).GetPos().x*size + r,
						     (*i).GetPos().y*size +c,
						     color_map[14].r, color_map[14].g,
						     color_map[14].b);
			    }
		    }else{
			 
			CheckPosition(i->GetPos());
			m_target->draw_pixel((*i).GetPos().x,(*i).GetPos().y,
					     color_map[14].r, color_map[14].g,
					     color_map[14].b);
		    }

		}

		for(std::list<BugBot>::iterator b=MH.GetBugBotBegin();
		    b!=MH.GetBugBotEnd();b++)
		{
		
			if(!m_paused)
				(*b).Update();

		    Position pos= (*b).GetPos();
											     
		    if(!MH.WithinSpace(pos))
		    {
			std::cout << "BB found outside space" << endl;
		    }

		    if(!(*b).IsCorpse())
		    {
			int team = (*b).GetTeam();
			int color = (*MH.GetMainBrainIter(team)).
			    GetColor((*b).GetFlags());
	      
							     
			//      m_target->
			//	draw_pixel(pos.x,pos.y,color_map[color].r, color_map[color].g,
			//		   color_map[color].b);
				if (double_pixel_mode)
				{
					for(int r=0; r < size; r++)
					for(int c=0; c < size; c++)
					{
						m_target->draw_pixel((pos.x *size) + r,
								 (pos.y *size) +c,
								 color_map[color].r, color_map[color].g,
								 color_map[color].b);
					}

				}else
				{
 
					CheckPosition(pos);
					m_target->draw_pixel(pos.x,pos.y,color_map[color].r, color_map[color].g,
							 color_map[color].b);

				}
		    }
		    else
		    {
			corpse_pos.push_back(pos);
		    }
		}
		for(std::vector<Position>::iterator pp=corpse_pos.begin();
		    pp!=corpse_pos.end();pp++)
		{
		    // m_target->draw_pixel((*pp).x,(*pp).y,color_map[7].r,
		    //		 color_map[7].g,
		    //		 color_map[7].b);
		    if( double_pixel_mode)
		    {
			for(int r=0; r < size; r++)
			    for(int c=0; c < size; c++)
			    {
				m_target->draw_pixel((*pp).x *size + r,
						     (*pp).y *size +c,
						     color_map[7].r, color_map[7].g,
						     color_map[7].b);
			    }
		    }
		    else{
			CheckPosition(*pp);
			m_target->draw_pixel((*pp).x,(*pp).y,color_map[7].r,
					     color_map[7].g,
					     color_map[7].b);

		    }
		} 

		for(std::list<Clump>::iterator c=MH.GetClumpBegin();
		    c!=MH.GetClumpEnd();c++)
		{
	
			if(!m_paused)
				(*c).Update();
	
		}

      	
		for(std::list<Food>::iterator f=MH.GetFoodBegin();f!=MH.GetFoodEnd();f++)
		{
											     
		    if(!MH.WithinSpace(f->GetPos()))
		    {
			std::cout << "Food found outside space" << endl;
		    }
	   
//	  m_target->draw_pixel((*f).GetPos().x,(*f).GetPos().y,
		    //		       color_map[2].r, color_map[2].g, color_map[2].b);
		    if( double_pixel_mode)
		    {
			for(int r=0; r < size; r++)
			    for(int c=0; c < size; c++)
			    {
				m_target->draw_pixel((*f).GetPos().x *size + r,
						     (*f).GetPos().y *size +c,
						     color_map[2].r, color_map[2].g,
						     color_map[2].b);
			    }
		 
		    }
			else
		    {
			CheckPosition(f->GetPos());
			m_target->draw_pixel((*f).GetPos().x,(*f).GetPos().y,
					     color_map[2].r, color_map[2].g, color_map[2].b);


		    }
		}

		if(!m_sound)
		{


			for(int xx=0; xx < size; xx++)
				for(int yy=0; yy < size; yy++)
					m_target->draw_pixel((SCREENWIDTH *size - size + xx) , (SCREENHEIGHT * size -size + yy), 0,1,1,0.75);
			
		}
      
		// MH.CorrectMap();
		corpse_pos.clear();


		//   CL_Display::sync_buffers();

		CL_Display::flip_display();
		CL_Display::clear_display(0,0,0);


		CL_System::keep_alive();

	    }

	    return 0;
	}
private:

    CL_Target *m_target;
	bool m_sound;  
	bool m_paused;

};

BBApp app;




