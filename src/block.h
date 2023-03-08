#ifndef __BLOCK_AGENT__H
#define __BLOCK_AGENT__H 

#include "enviro.h"

using namespace enviro;

class BlockController : public Process, public AgentInterface {
    bool hit = false;

    public:
    BlockController() : Process(), AgentInterface() {}

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

        if( hit ) {
            int random_x = rand() % 801 - 400; 
            int random_y = rand() % 801 - 400; 
            Agent& new_agent = add_agent("Wanderer",random_x,random_y,0,{{"fill", "blue"}});
            remove_agent(id());
        }
    }
    void stop() {}


};

class Block : public Agent {
    public:
    Block(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    BlockController c;
};

DECLARE_INTERFACE(Block)

#endif