#ifndef __COORDINATOR_AGENT__H
#define __COORDINATOR_AGENT__H 

#include "enviro.h"

using namespace enviro;


    class PhaseOne : public State, public AgentInterface {
        public:
        void entry(const Event& e) {
            std::cout << "IN PHASE ONE ENTRY \n";
        }
        void during() { 
        }
        void exit(const Event& e) {}
        void set_tick_name(std::string s) { tick_name = s; }
        std::string tick_name;
    };

    class PhaseTwo : public State, public AgentInterface {
        public:
        void entry(const Event& e) { }
        void during() { 
        }
        void exit(const Event& e) {
        }
        double rate;
        void set_tick_name(std::string s) { tick_name = s; }
        std::string tick_name;        
    };

    class GameOver : public State, public AgentInterface {
        public:
        void entry(const Event& e) { }
        void during() {}
        void exit(const Event& e) {}
        double rate;
        void set_tick_name(std::string s) { tick_name = s; }
        std::string tick_name;        
    };

class CoordinatorController : public StateMachine, public AgentInterface {
    PhaseOne phase_one;
    PhaseTwo phase_two;
    GameOver game_over;

    std::string tick_name;

    int total_hits;

    public:
    CoordinatorController() : StateMachine(), total_hits(0) {

        set_initial(phase_one);
        tick_name = "tick_" + std::to_string(rand()%1000); // use an agent specific generated 
                                                            // event name in case there are 
                                                            // multiple instances of this class
        add_transition("switch_two", phase_one, phase_two);
        add_transition("switch_two", phase_two, game_over);
        phase_one.set_tick_name("switch_two");
        phase_two.set_tick_name("switch_two");
        game_over.set_tick_name("switch_two");
    }

    void init() {
            watch("agent_click", [&](Event& e) {
                total_hits = total_hits + 1;

                std::cout << "total_hits: " << total_hits << " \n";
            });

            StateMachine::init();
    }
    void start() {}
    void update() {
        if (total_hits > 2) {
            emit(Event("switch"));
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