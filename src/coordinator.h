#ifndef __COORDINATOR_AGENT__H
#define __COORDINATOR_AGENT__H 

#include "enviro.h"
#include <string>
using namespace enviro;

/*
Controller to handle high level game logic
i.e. "game over" svg at the end of the game, ideally resetting the game state.
*/
class CoordinatorController : public Process, public AgentInterface {
    bool hit = false;

    public:
    CoordinatorController() : Process(), AgentInterface()  {
    }

    void init() {
        label("Game Over.", -100, 0 );
            // Logic to handle re-starting the game
            // Commented for now because logic is not working.
            watch("agent_click", [&](Event& e) {
                int clicked_agent_id = e.value()["id"];
                if ( id() ==  clicked_agent_id  ) {
                    // hit = true;                
                }
            });
    }
    void start() {}
    void update() {

        damp_movement();

        // Logic to handle re-starting the game
        // Commented for now because logic is not working.
        // if (hit) {
        //     add_agent("Block",100,0,0,{{"fill", "blue"}});
        //     remove_agent(id());
        // }
    }
    void stop() {}

};

class Coordinator : public Agent {
    public:
    Coordinator(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    CoordinatorController c;
};

DECLARE_INTERFACE(Coordinator)

#endif