#ifndef VIEW_H_INCLUDED
#define VIEW_H_INCLUDED

#include "World.h"

namespace BugBots {


    class View {
        public:
        View(const World& );
        ~View();




        private:
        const World& m_world;
    };

};


#endif // VIEW_H_INCLUDED
