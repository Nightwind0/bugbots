#ifndef __BB_BUGBOT_H_
#define __BB_BUGBOT_H_

#include "Displayable.h"
#include "Constants.h"
#include "Position.h"
#include "WorldObject.h"

namespace BugBots
{

    class BugBot : public Displayable, public WorldObject
    {
    public:
        BugBot();
        virtual ~BugBot();


        void SetAlignment ( Alignment alignment );
        Alignment GetAlignment() const;

        Position GetPosition() const;

        virtual void Draw(float x, float y, float scale=1.0f, float rotation = 0.0f);

    private:
        Position m_pos;

    };

}

#endif
