#include "utility.h"
#include "food.h"
#include <list>
#include <map>
#include "sutil.h"
#include "MapHandler.h"

const extern int SCREENWIDTH;
const extern int SCREENHEIGHT;

extern MapHandler MH;


Food::Food()
{
     m_me = 0;
     m_carried = false;
}

void Food::SetMe(int i)
{
     m_me = i;
}

int Food::GetMe()
{
     return m_me;
}

void Food::NoCarryClump()
{
     m_carried = false;
}

bool Food::IsCarried()
{
     return m_carried;
}

void Food::CarryClump()
{
     m_clump = -1;
     m_carried=true;
}

void Food::SetClump(int aclump)
{
     m_clump = aclump;
}

int Food::GetClump()
{
     return m_clump;
}

Position Food::GetPos()
{
     return m_pos;
}

void Food::NoClump()
{
     m_clump = -1;
}

void Food::SetPos(const Position& npos,bool replace)
{
     Position tpos = m_pos;
     if(npos == m_pos)
	  replace = false;
	else replace=true; // WOOP

     m_pos=npos;
     
     MH.SetFoodAt(m_pos,MH.GetFoodIter(m_me));//GlobalMap[m_pos.x][m_pos.y].food=GetFoodIt(m_me);
     if(replace)
     {
	  //GlobalMap[tpos.x][tpos.y].food=0;
       MH.SetFoodAt(tpos,MH.GetFoodEnd());
     }
}

