#ifndef __WANDERER_AGENT__H
#define __WANDERER_AGENT__H 

#include <stdio.h>
#include <string>
#include <math.h>
#include "enviro.h"


using namespace enviro;

/*
Moving square target that gets faster over time 
counter - used for calculating the new radius size to set on each update
total_hits- count the total number of times the block target has been hit 

Will continue to increase in velocity as the number of total target hits increases.

If the moving target disappears the game will end.
*/
class WandererController : public Process, public AgentInterface {

    // GAME LOGIC VARIABLES
    bool hit = false;
    int total_hits = 0;
    int HITS_TILL_FASTER = 5;
    int velocity = 10;

    public:
    WandererController() : Process(), AgentInterface(), total_hits(0) {}

    void init() {
        watch("agent_click", [&](Event& e) {
            e.stop_propagation();

            int clicked_agent_id = e.value()["id"];
            if ( id() ==  clicked_agent_id  ) {
                hit = true;                
            }
        });
    }

    void start() {}

    void update() {
        track_velocity(velocity,0.15);

        // End game if target moves off screen
        if( x() > 400 || y() > 400 ) {
            std::cout << "GAME OVER \n";
            remove_agent(id());

            // Reset Button Logic
            add_agent(
                "Coordinator",
                100,
                0,
                0,
                {{"fill", "grey"}}
            );
        } else if (hit) {
            total_hits++;

            int random_x = rand() % 701 - 350; 
            int random_y = rand() % 701 - 350; 
            double random_theta = rand() % (4*3) - (2*3); // Use int rounded value for Pi
            teleport(random_x, random_y, random_theta);

            hit = false;

            if (total_hits % 3 == 0) {
                velocity = velocity + 2;
            }
        }
    }

    void stop() {}


};

class Wanderer : public Agent {

    public:
    Wanderer(json spec, World& world) : Agent(spec, world) {
        add_process(wc);
    }

    WandererController wc;

};

DECLARE_INTERFACE(Wanderer);
#endif