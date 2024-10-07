# Minesweeper AI Project

## Overview
This project is a Minesweeper game with different AI agents that play the game autonomously. There are three main AI agents implemented:

* MyAI: Your custom AI that implements a logic-based approach to uncover tiles and flag mines.
* RandomAI: A simple AI that randomly selects an action.
* ManualAI: Allows manual play, giving the player control over the agent.
* The game can be run with or without debug information, and it can read game board setups from files or generate random ones.

## Files and Structure
* Main.cpp: The entry point for the program. It handles command-line options and interacts with the world and AI agents.
* World.hpp/World.cpp: These files define the World class, which manages the game environment, including the game board, agent actions, and game rules.
* Agent.hpp: Defines the Agent class, which is the abstract base class for all AI agents.
* MyAI.hpp/MyAI.cpp: Implements your custom AI logic.
* RandomAI.hpp: Defines the RandomAI, which selects random actions in the game.
* ManualAI.hpp: Provides an interface for manually controlling the agent.

## How It Works
1. The **World** class generates a Minsweeper board with tiles that may contain mines. The agent can uncover a tile, flag it as a mine, unflag it, or leave the game
2. Each turn, the agent performs an action based on the board's current state. The game ends when all non-mine tiles are uncovered or the agent triggers a mine.
3. The **MyAI** class implements the logic for the custom agent, which uses strategies to uncover tiles and flag mines based on the game state

# How to Run

## Command-line Usage
```bash
./Minesweeper [Options] [InputFile] [OutputFile]
```
Options
* -m: use the `ManualAI` instead of `MyAI`
* -r: use the `RandomAI` instead of `MyAI`
* -d: enable debug mode, which displays the board after every move.
* -v: verbose mode, displaying the world file names before loading them.
* -f: input file mode. If a folder is provided, it runs the AI on every file in the folder.

## Example Commands
1. Run with the custom AI on a random World
```bash
./Minesweeper
```
2. Run with the `RandomAI` on a random world:
```bash
./Minesweeper -r
```
3. Run with the `ManualAI` to control the game manually
```bash
./Minesweeper -m
```
4. Run with a specific world file:
```bash
./Minesweeper -d world.txt
```
5. Run in folder mode:
```bash
./Minesweeper -f worlds_folder
```

# AI Agents
## MyAI
* **Description**: this is your custom AI. It attempts to logically deduce the position of mines and safely uncover all non-mine tiles. It uses techniques like backtracking and model checking to ensure safe moves
* File: `MyAI.hpp`, `MyAI.cpp`

## Random AI
* **Description**: This agent chooses a random action at each turn, with no logic to avoid mines or flag them correctly
* File: `RandomAI.hpp`

## ManualAI:
* **Description**: Allows manual control of the agent. The player inputs actions directly into the terminal to control the game.
* File: `ManualAI.hpp`

#Compilation
Use the following command to compile the program:
```bash
g++ -o Minesweeper Main.cpp World.cpp MyAI.cpp
```
