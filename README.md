# Stats Module
A simple stats module for videogames to track stats of a player, such as health, stamina, etc

## Table of Contents
- [Installation](#installation)
- [Usage](#usage)

## Installation
1. Clone the repo in your game dir:
   ```bash
   cd /my_game_dir
   git clone git@github.com:pexgear/CPP_stats_module.git

### Usage
1. Be sure the repo is included in your codedir
2. Instantiate a new object of the preferred numerical type:
   ```c++
   stats_set my_set = stats_set<int>();
3. That will create a new set, of type **int**
4. Start adding a new stat:
   ```c++
   my_set.add_stat("health", 100);
   my_set.add_stat("stamina", 20, 80);
5. Like so Health will be a value from 0 to 100, while Stamina will be from 20 to 80
