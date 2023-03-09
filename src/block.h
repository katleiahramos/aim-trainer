#ifndef __BLOCK_AGENT__H
#define __BLOCK_AGENT__H 

#include "enviro.h"

using namespace enviro;

class BlockController : public Process, public AgentInterface{
    bool hit = false;

    // GAME LOGIC VARIABLES
    int total_hits;
    int HITS_TO_SPAWN_SHRINKING_TARGET = 5;

    public:
    BlockController() : Process(), AgentInterface(), total_hits(0){}

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
            total_hits++;

            // Move target
            int random_x_block = rand() % 801 - 400; 
            int random_y_block = rand() % 801 - 400; 
            teleport(random_x_block, random_y_block, 0);

            // Reset hit state
            hit = false;

            if ( total_hits == HITS_TO_SPAWN_SHRINKING_TARGET ) {
                int random_x_wanderer = rand() % 801 - 400; 
                int random_y_wanderer = rand() % 801 - 400; 
                add_agent("ShrinkingTarget",random_x_wanderer,random_y_wanderer,0,{{"fill", "blue"}});
                remove_agent(id());
            }
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