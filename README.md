# PowSimulator

Description
 - It's for testing POW.


Environment
 - gcc/g++ 
 - linux

Pre-install
 - $ sudo apt-get update
 - $ sudo apt-get upgrade
 - $ sudo apt-get install gcc
 - $ sudo apt-get install g++

Settings
 - Edit "config.h"
 - POW_SIM_DIFFICULT_BITS 11 // difficultyBits.
 - POW_SIM_MIN_TRX_CNT 6 // min tx
 - POW_SIM_MAX_TRX_CNT 10 // max tx
 - POW_SIM_TRX_SIZE 16 // size of tx
 - *Difficulty bits's set easy for test

build
 - $ chmod 777 build.sh
 - $ ./build.sh
  
Run
  - $ ./pow_sim
  - press 'm' -> start mining
  - press 'v' -> verify bloks so far
  - press 's' -> show blocks detail.
  
Rule
  - difficultyBits range from 1 to 255(the most difficulty)
  - Genesis block generate not following to the difficulty rule.
  
