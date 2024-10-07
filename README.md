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
