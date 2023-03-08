#ifndef __WANDERER_AGENT__H
#define __WANDERER_AGENT__H 

#include <stdio.h>
#include <string>
#include <math.h>
#include "enviro.h"

namespace {

    using namespace enviro;

    class InPlay : public State, public AgentInterface {
        public:
        void entry(const Event& e) {
        }
        void during() { 
            track_velocity(4,0);
        }
        void exit(const Event& e) {}
        void set_tick_name(std::string s) { tick_name = s; }
        std::string tick_name;
    };

    class Remove : public State, public AgentInterface {
        public:
        void entry(const Event& e) { }
        void during() {  
            emit(Event(tick_name));
        }
        void exit(const Event& e) {
        }
        double rate;
        void set_tick_name(std::string s) { tick_name = s; }
        std::string tick_name;        
    };

    class Destroyed : public State, public AgentInterface {
        public:
        void entry(const Event& e) { }
        void during() {}
        void exit(const Event& e) {}
        double rate;
        void set_tick_name(std::string s) { tick_name = s; }
        std::string tick_name;        
    };

    class WandererController : public StateMachine, public AgentInterface {
        bool hit = false;

        public:
        WandererController() : StateMachine() {

            set_initial(in_play);
            tick_name = "tick_" + std::to_string(rand()%1000); // use an agent specific generated 
                                                               // event name in case there are 
                                                               // multiple instances of this class
            add_transition(tick_name, in_play, remove);
            add_transition(tick_name, remove, destroyed);
            in_play.set_tick_name(tick_name);
            remove.set_tick_name(tick_name);
            destroyed.set_tick_name(tick_name);

        }

        void init() {
            watch("agent_click", [&](Event& e) {
                e.stop_propagation();

                int clicked_agent_id = e.value()["id"];
                if ( id() ==  clicked_agent_id  ) {
                    hit = true;
                
                    // emit(Event(tick_name));
                }
            });
            StateMachine::init();
        }

        void update() {
            track_velocity(10,0.15);
            if (hit) {
                int random_x = rand() % 801 - 400; 
                int random_y = rand() % 801 - 400; 
                Agent& new_agent = add_agent("Block",random_x,random_y,0,{{"fill", "blue"}});
                remove_agent(id());
            }
        }

        InPlay in_play;
        Remove remove;
        Destroyed destroyed;
        std::string tick_name;

    };

    class Wanderer : public Agent {

        public:
        Wanderer(json spec, World& world) : Agent(spec, world) {
            add_process(wc);
        }

        WandererController wc;

    };

    DECLARE_INTERFACE(Wanderer);

}

#endif