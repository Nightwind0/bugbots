#ifndef VIEW_H_INCLUDED
#define VIEW_H_INCLUDED

#include "World.h"

namespace BugBots {


    class View {
        public:
        View(const World& );
        ~View();


        void DrawWorld ( void );

        private:

        void draw_objects();

        const World& m_world;
    };

};


#endif // VIEW_H_INCLUDED
