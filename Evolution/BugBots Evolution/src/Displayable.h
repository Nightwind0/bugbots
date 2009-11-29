#ifndef DISPLAYABLE_H_INCLUDED
#define DISPLAYABLE_H_INCLUDED

namespace BugBots
{

    class Displayable
    {
        public:
        Displayable();
        virtual ~Displayable();

        virtual void Draw(float x, float y, float scale=1.0f, float rotation = 0.0f)=0;


    };

}


#endif // DISPLAYABLE_H_INCLUDED
