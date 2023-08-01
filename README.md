# Simopticon

1. [Overview](#overview)
2. [Setup](#setup)
3. [Usage](#usage)
4. [Extension](#extension)

## Overview

*Simopticon* is a framework which automates the search for optimal parameters for simulated processes.
The key strategy is to define parameters that shall be optimized, automatically run simulations with certain parameters,
evaluate their performance by calculating a number rating (the lower, the better) and trying to find parameter
combinations that minimize the rating.

The described process is distributed over four major components:

1. Optimizer: An optimization strategy capable of finding the minimum of a blackbox function only accessible through
   argument-value pairs.
2. SimulationRunner: A component used to run simulations with certain parameters automatically.
3. Evaluation: A component capable of calculating a rating value based on result files of simulations.
4. Controller: A component managing the optimization process and communication between Optimizer, SimulationRunner and
   Evaluation. Used to abstract components 1-3 from each other.

Extensions of the framework may introduce new Optimizer, SimulationRunner and Evaluation implementations (
see [Extension](#extension)).
Currently, there is only one implementation of each component, tailored for the optimization of platoon controllers
using the [Plexe](https://plexe.car2x.org/) framework.

## Setup

## Usage

## Extension