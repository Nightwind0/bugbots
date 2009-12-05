#ifndef __BB_BUGBOT_H_
#define __BB_BUGBOT_H_

#include <map>
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

        enum eSprite{
            BUGBOT_BLUE,
            BUGBOT_RED,
            BUGBOT_DEAD
        };

        static void LoadResources();

        void SetAlignment ( Alignment alignment );
        Alignment GetAlignment() const;

        virtual Position GetPosition() const;
        virtual void HandleCollision ( WorldObject &other );

        virtual void Draw(float x, float y, float scale=1.0f, float rotation = 0.0f);

    private:
        static std::map<eSprite,GLuint> m_textures;
        Position m_pos;
        Alignment m_alignment;

    };

    inline void BugBot::SetAlignment ( Alignment alignment ){
        m_alignment = alignment;
    }
    inline Alignment BugBot::GetAlignment ( ) const {
        return m_alignment;
    }
    inline Position BugBot::GetPosition() const{
        return m_pos;
    }

}

#endif
