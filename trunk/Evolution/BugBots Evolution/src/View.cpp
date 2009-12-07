#include "View.h"

using BugBots::View;

namespace BugBots {
    class DrawObjectVisitor : public Quadtree::Visitor<WorldObject*> {
        public:
        DrawObjectVisitor(){
        }
        virtual ~DrawObjectVisitor(){
        }

        virtual bool Visit(WorldObject* object){
            object->Draw ( object->GetOrigin().m_x, object->GetOrigin().m_y );
            return true;
        }

        private:
    };
}

View::View(const World& world):m_world(world){
}

View::~View(){
}


void View::DrawWorld ( void ){
}

void View::draw_objects ( void ) {
    DrawObjectVisitor visitor;

    // TODO: Construct a view circle and only traverse those within that circle
    m_world.TraverseAllObjects(visitor);
}
