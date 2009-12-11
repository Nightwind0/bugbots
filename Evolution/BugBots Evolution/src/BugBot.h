#ifndef __BB_BUGBOT_H_
#define __BB_BUGBOT_H_

#include <map>
#include "Constants.h"
#include "Position.h"
#include "WorldObject.h"

namespace BugBots
{

    class BugBot : public WorldObject
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
        void SetPosition ( const Position& pos);

        virtual Position GetOrigin() const;
        virtual double GetRadius() const;
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
    inline Position BugBot::GetOrigin() const{
        return m_pos;
    }
    inline double BugBot::GetRadius() const{
        return BUGBOT_RADIUS;
    }

    inline void BugBot::SetPosition ( const Position& pos ){
        m_pos = pos;
    }

}

#endif
