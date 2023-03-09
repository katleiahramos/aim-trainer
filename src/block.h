#ifndef __BLOCK_AGENT__H
#define __BLOCK_AGENT__H 

#include "enviro.h"

using namespace enviro;

class BlockController : public Process, public AgentInterface{
    bool hit = false;

    // GAME LOGIC VARIABLES
    int total_hits;
    int HITS_TILL_PHASE_1 = 3;
    // int HITS_TILL_PHASE_2 = 5;

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

            // std::cout << "Total Hits: " << total_hits << "\n";

        if( hit ) {
            total_hits++;
            

            int random_x_block = rand() % 801 - 400; 
            int random_y_block = rand() % 801 - 400; 
            teleport(random_x_block, random_y_block, 0);

            hit = false;
            // remove_agent(id());

            if ( total_hits == HITS_TILL_PHASE_1 ) {
                std::cout << "in if statement \n";
                int random_x_wanderer = rand() % 801 - 400; 
                int random_y_wanderer = rand() % 801 - 400; 
                add_agent("Wanderer",random_x_wanderer,random_y_wanderer,0,{{"fill", "blue"}});
                remove_agent(id());
            }

            //  add_agent("Wanderer",random_x,random_y,0,{{"fill", "blue"}});

            // if( total_hits == HITS_TILL_PHASE_1) {
            //     add_agent("Wanderer",random_x,random_y,0,{{"fill", "blue"}});
            // } else if (total_hits == HITS_TILL_PHASE_2) {
            //     add_agent("Wanderer",random_x,random_y,0,{{"fill", "blue"}});
            //     add_agent("ShrinkingAgent",random_x,random_y,0,{{"fill", "blue"}});
            // } else {
            //     add_agent("Block",random_x,random_y,0,{{"fill", "blue"}});
            // }


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