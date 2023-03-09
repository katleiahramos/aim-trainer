#ifndef __SHRINKING_TARGET_AGENT__H
#define __SHRINKING_TARGET_AGENT__H 

#include "enviro.h"

using namespace enviro;

class ShrinkingTargetController : public Process, public AgentInterface {

    // Target Size
    int counter;
    int MAX_RADIUS = 50;
    int SMALLER_TARGET_RADIUS = 30;

    // GAME LOGIC VARIABLES
    bool hit = false;
    int total_hits;
    int HITS_TO_SMALLER_TARGET = 5;
    int HITS_TO_SPAWN_WANDERER = 10;

    public:
    ShrinkingTargetController() : Process(), AgentInterface(), counter(0), total_hits(0) {}

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

        // Shrink in size and remove
        if (counter > MAX_RADIUS ) {
          remove_agent(id());
        } else {
          set_style({{"r", MAX_RADIUS - counter}});
          counter++;
        }

        if( hit ) {
            total_hits++;
            int random_x = rand() % 801 - 400; 
            int random_y = rand() % 801 - 400; 

            teleport(random_x, random_y, 0);
            set_style({{"r", MAX_RADIUS}});

            // Increase difficulty 
            if (total_hits > HITS_TO_SMALLER_TARGET ) {
              MAX_RADIUS = SMALLER_TARGET_RADIUS;
            }

            // Reset radius and counter
            counter = 0;
            hit = false;

            if ( total_hits == HITS_TO_SPAWN_WANDERER ) {
              int random_x_wanderer = rand() % 801 - 400; 
              int random_y_wanderer = rand() % 801 - 400; 
              double random_theta = rand() % (4*3) - (2*3); // Use int rounded value for pi
              add_agent("Wanderer",random_x_wanderer,random_y_wanderer,random_theta,{{"fill", "blue"}});
              remove_agent(id());
            }
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