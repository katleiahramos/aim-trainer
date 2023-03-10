# Aim Trainer Game in C++

## Overview
This project is an aim-trainer-like app. The idea is that the user would use a mouse to click the targets that spawn in the browser. The targets get progressively more difficult to hit (spawning at smaller sizes or moving at faster velocities). Once a target is missed the game will end.

## Starting the App

1. Clone this repo
2. Navigate into the repository directory
3. Download and run `v1.61` of klavin's enviro docker image:
```
docker run -p80:80 -p8765:8765 -v $PWD:/source -it klavins/enviro:v1.61 bash
```
4. Build the project:
```
$ make
$ esm start
$ enviro
```
5. Open a browser use the URL bar to navigate to `localhost`
## Game Instructions
To start the game simply click the first block that appears. 

The continue clicking the targets to progress through the game. 

If a target disappears before you click it, you will lose and the game will end.

Once the game ends, you will need to restart the `enviro` server to start over.

## Resources
This project is inspired by aim trainer like games. Specifically AimLabs:
https://aimlab.gg/


The main resource used for this project was the public `enviro` github repository:
https://github.com/klavinslab/enviro

Specifically, the `virus` example:
https://github.com/klavinslab/enviro/tree/master/examples/virus

## Obstacles
My original goal was to create an aim-trainer-like game that had multiple levels of difficulty. Each level would include multiple types of agents that would spawn

### Enviro Library
#### Adding / Removing Agents
The biggest issue I ran into was with adding and removing agents. Initially I tried to remove the current agent and add a new agent of the same type when the current agent was clicked. The issue I ran into was trying to add the agent in the `watch` block in the `init()` method:

```
class BlockController : public Process, public AgentInterface {
    void init() {
        watch("agent_click", (Event& e) => {
                if (e.value()["id"] == id()) {
                    add_agent("Block", ... );
                    remove_agent(id());
                }
            });
    }
}
```

This code block was throwing the following error:

```
terminate called after throwing an instance of 'elma::Exception'
  what():  Elma Error: Cannot access events in a process before the process is scheduled.
```

My initial thought was that I needed a `StateMachine` to separate the process of adding and removing the agents. I then created a state machine with the states `InPlay`, `Remove`, `Destroyed`. However, even when utilizing a state machine threw the same error. I was able to fix this issue by creating a class variable that tracked the `hit` state of the agent. Once I did that I was able to move the `add` and `remove` to the `update()`. 

Unfortunately, I had to remove the `add_agent` logic completely because it was causing issues with some of my agent classes' ability to keep a `total_hits` counter and would cause it to reset. This was an issue as the `total_hits` counter is required logic to handle the game logic of changing "phases" of the game.

The work around I implemented was to use `teleport()` to just move the current target instead of removing and and adding agents. Unfortunately the side affect was that I wasn't able to implement targets (agent type classes) of different types at the same time.

#### Accessing Agent Properties
Another issue I faced was accessing certain properties of the agent. For example, I wanted to be able to programatically change the radius of the `ShrinkingAgent`s but grabbing the current radius and incrementing it down. However, there is no accessor in the `agent` or `agent_interface` for the `radius` property. The work around I created to handle this was to hard code the initial radius and and create a counter that would allow me to incrementally set the radius smaller and smaller on each `update()` execution.  

#### State Machine Functionality
I also wanted to implement a state machine to handle changing the game "phases". The `Coordinator` class was originally made for this purpose. My hope was that I would be able to use it as a state machine and emit events to the other controllers that handled the target type like `block`, `wanderer`, and `shrinking_target`. The state machine would allow me to implement the spawning of different target types at the same time. Unfortunately, I was not able to get the state machine event emitted to trigger the other state machines, so I decided to not move forward with that idea due to the time constraint. However, this area would be of interest to me in how to expand and build on this project in the future.



