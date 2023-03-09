#ifndef __SHRINKING_TARGET_AGENT__H
#define __SHRINKING_TARGET_AGENT__H 

#include "enviro.h"

using namespace enviro;

class ShrinkingTargetController : public Process, public AgentInterface {
    bool hit = false;
    int counter;
    int MAX_RADIUS = 50;

    public:
    ShrinkingTargetController() : Process(), AgentInterface(), counter(0) {}

    void init() {
        watch("agent_click", [&](Event& e) {
            int clicked_agent_id = e.value()["id"];
                if ( id() ==  clicked_agent_id  ) {
                    hit = true;
                }
        });
    }
    void start() {}
    void update() {
        damp_movement();
        // Agent& target = find_agent(id());
        // int new_radius =  (int) target.definition()["radius"] - 2;
        // std::cout << "existing " << target.definition()["radius"] << "new_radius " << new_radius << "\n";


        // Shrink in size and remove
        if (counter > MAX_RADIUS ) {
          remove_agent(id());
        } else {
          set_style({{"r", MAX_RADIUS - counter}});
          counter++;
        }



        if( hit ) {
            int random_x = rand() % 801 - 400; 
            int random_y = rand() % 801 - 400; 
            Agent& new_agent = add_agent(
                "ShrinkingTarget",
                random_x,
                random_y,
                0,
                {
                    {"fill", "green"},
                    {"shape", "omni"},
                    {"r", MAX_RADIUS},
                }
            );
            remove_agent(id());
        }
    }
    void stop() {}


};

class ShrinkingTarget : public Agent {
    public:
    ShrinkingTarget(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    ShrinkingTargetController c;
};

DECLARE_INTERFACE(ShrinkingTarget)

#endif