
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/application.h>
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

using namespace std;


#define DOUBLE_PIXEL_MODE

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
  char *get_title(){ return "BugBots";}
  int main(int argc, char **argv)
  {
    CL_SetupCore::init();
    CL_SetupDisplay::init();


#ifdef _DEBUG
	CL_ConsoleWindow console("Console",80,1000);
	console.redirect_stdio();
#endif


#ifdef DOUBLE_PIXEL_MODE
    CL_Display::set_videomode(SCREENWIDTH * size,SCREENHEIGHT *size,24,false);
#else
	CL_Display::set_videomode(SCREENWIDTH,SCREENHEIGHT,24,false);
#endif

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
	  (*i).Update();
/*
	  m_target->draw_pixel((*i).GetPos().x,(*i).GetPos().y,
	  color_map[14].r, color_map[14].g,
	  color_map[14].b); */
#ifdef DOUBLE_PIXEL_MODE
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
#else
		  CheckPosition(i->GetPos());
	m_target->draw_pixel((*i).GetPos().x,(*i).GetPos().y,
	  color_map[14].r, color_map[14].g,
	  color_map[14].b);
#endif
	}

      for(std::list<BugBot>::iterator b=MH.GetBugBotBegin();
	  b!=MH.GetBugBotEnd();b++)
	{
		
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
#ifdef DOUBLE_PIXEL_MODE

	  for(int r=0; r < size; r++)
	      for(int c=0; c < size; c++)
	      {
		  m_target->draw_pixel((pos.x *size) + r,
				       (pos.y *size) +c,
				       color_map[color].r, color_map[color].g,
				       color_map[color].b);
	      }


#else

		  	  CheckPosition(pos);
	m_target->draw_pixel(pos.x,pos.y,color_map[color].r, color_map[color].g,
	      		   color_map[color].b);

#endif
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
#ifdef DOUBLE_PIXEL_MODE

	  for(int r=0; r < size; r++)
	      for(int c=0; c < size; c++)
	      {
		  m_target->draw_pixel((*pp).x *size + r,
				      (*pp).y *size +c,
				       color_map[7].r, color_map[7].g,
				       color_map[7].b);
	      }

#else
			  CheckPosition(*pp);
m_target->draw_pixel((*pp).x,(*pp).y,color_map[7].r,
	    		 color_map[7].g,
	    		 color_map[7].b);

#endif
	} 

      for(std::list<Clump>::iterator c=MH.GetClumpBegin();
	  c!=MH.GetClumpEnd();c++)
	{
	
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
#ifdef DOUBLE_PIXEL_MODE
	  for(int r=0; r < size; r++)
	      for(int c=0; c < size; c++)
	      {
		  m_target->draw_pixel((*f).GetPos().x *size + r,
				      (*f).GetPos().y *size +c,
				       color_map[2].r, color_map[2].g,
				       color_map[2].b);
	      }

#else
		CheckPosition(f->GetPos());
m_target->draw_pixel((*f).GetPos().x,(*f).GetPos().y,
	    	 color_map[2].r, color_map[2].g, color_map[2].b);


#endif
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
  

};

BBApp app;




