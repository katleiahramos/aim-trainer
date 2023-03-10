# Aim Trainer Game in C++

## Overview

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

## Resources
This project is inspired by aim trainer like games. Specifically AimLabs:
https://aimlab.gg/


The main resource used for this project was the public `enviro` github repository:
https://github.com/klavinslab/enviro

Specifically, the `virus` example:
https://github.com/klavinslab/enviro/tree/master/examples/virus

## Obstacles



