#ifndef __WANDERER_AGENT__H
#define __WANDERER_AGENT__H 

#include <stdio.h>
#include <string>
#include <math.h>
#include "enviro.h"

namespace {

    using namespace enviro;

    class Waiting : public State, public AgentInterface {
        public:
        void entry(const Event& e) {
        }
        void during() { 
        }
        void exit(const Event& e) {}
        void set_tick_name(std::string s) { tick_name = s; }
        std::string tick_name;
    };

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
            // emit(Event(tick_name));
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
        InPlay in_play;
        Remove remove;
        Destroyed destroyed;
        Waiting waiting;
        std::string tick_name;

        // GAME LOGIC VARIABLES
        bool hit = false;
        int total_hits = 0;
        int HITS_TILL_FASTER = 5;
        int velocity = 10;

        public:
        WandererController() : StateMachine() {

            set_initial(in_play);
            // tick_name = "tick_" + std::to_string(rand()%1000); // use an agent specific generated 
                                                               // event name in case there are 
            add_transition("switch", waiting, in_play);                                                   // multiple instances of this class
            add_transition("other", in_play, remove);
            add_transition("other", remove, destroyed);
            in_play.set_tick_name("switch");
            remove.set_tick_name("other");
            destroyed.set_tick_name("other");
            waiting.set_tick_name("switch");
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
            // StateMachine::init();
        }

        void update() {
            damp_movement();
            track_velocity(velocity,0.15);
            if (hit) {
                total_hits++;

                int random_x = rand() % 701 - 350; 
                int random_y = rand() % 701 - 350; 
                double random_theta = rand() % (4*3) - (2*3); // Use int rounded value for Pi
                teleport(random_x, random_y, random_theta);

                hit = false;

                if (total_hits % 4 == 0) {
                    velocity = velocity + 2;
                }
            }
        }



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