Audio Visualizer
======
Produces an audio spectrum analyzer on an LCD attached to the STM32F4 Board.
Also interfaces with a separate custom board designed to visualize audio by voltage measurement.

## Setup
Environment
To use the STM32F4 in the Linux environment, follow this [guide](http://www.wolinlabs.com/blog/linux.stm32.discovery.gcc.html).

Makefile and Scripts are written relative to their dependencies and references within the project.

## Commands
Make: compile program
Make burn: flash the board
Make clean: removes targets

ST-UTIL: starts utility server to talk to board
DEBUG: starts debug console and hooks to server

## Contact
Maintainer: [Sharan ***REMOVED***](https://github.com/AtomicAntimatter)  ***REMOVED***

## LICENSE
Audio Visualizer is available under the MIT License. See the LICENSE file for more info.
