#ifndef __COORDINATOR_AGENT__H
#define __COORDINATOR_AGENT__H 

#include "enviro.h"
#include <string>
using namespace enviro;
class CoordinatorController : public Process, public AgentInterface {
    std::string tick_name;
    bool hit = false;

    public:
    CoordinatorController() : Process(), AgentInterface()  {
    }

    void init() {
        // decorate("<text x='-120' y='0' r='0' style='fill: red'>Game Over. Click Here to start over</text>");
        // label("Game Over. Click Here to start over", -120, 0 );

            watch("agent_click", [&](Event& e) {
                std::cout << "IN AGENT CLICKED \n";
                int clicked_agent_id = e.value()["id"];
                if ( id() ==  clicked_agent_id  ) {
                    std::cout << "IN AGENT CLICKED \n";
                    hit = true;                
                }
            });
    }
    void start() {}
    void update() {

        damp_movement();

        if (hit) {
            add_agent("Block",100,0,0,{{"fill", "blue"}});
            remove_agent(id());
        }
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