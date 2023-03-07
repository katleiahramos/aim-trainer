#ifndef __MY_ROBOT_AGENT__H
#define __MY_ROBOT_AGENT__H 

#include "enviro.h"
using namespace enviro;

class MyRobotController : public Process, public AgentInterface {

    public:
    MyRobotController() : Process(), AgentInterface() {}

    void init() {
         watch("agent_click", [&](Event& e) {
            // create state machine for when being removed and when adding a new block?
            Agent& bullet = add_agent("MyRobot", 
            0, 
            0, 
            0, 
            {});   
            int clicked_agent_id = e.value()["id"];
             if ( id() ==  clicked_agent_id  ) {
                remove_agent(id());
                // teleport(e.value()["x"], e.value()["y"], 0);
             }

            // Agent& bullet = add_agent("MyRobot", 
            // 0, 
            // 0, 
            // 0, 
            // {});   

            });
    }
    void start() {}
    void update() {
        track_velocity(10,0.15);
    }
    void stop() {}

};

class MyRobot : public Agent {
    public:
    MyRobot(json spec, World& world) : Agent(spec, world) {
        add_process(c); // Make AgentInterface methods available 
    }
    private:
    MyRobotController c;
};

DECLARE_INTERFACE(MyRobot)

#endif