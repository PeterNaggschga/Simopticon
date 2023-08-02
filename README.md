# Simopticon

1. [Overview](#overview)
2. [Setup](#setup)
   1. [Requirements](#requirements)
   2. [Installation](#installation)
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

### Requirements

The following sections describe the requirements your machine has to fulfill to run *Simopticon*.
They may differ depending on the Optimizer, SimulationRunner and Evaluation implementations you plan to use, therefore,
the implementations have their own dependency sections.

#### Simopticon

The framework itself is developed for Debian-based Unix/Linux machines. Other operating systems might work but are not
actively supported.
To be able to install the framework, you need the following software:

- Git (see [Git](#git))
- CMake Version 3.25 or higher (see [CMake](#cmake))
- Python3 development tools (see [Python3 Development Tools](#python3-development-tools))

#### PlexeSimulationRunner

To enable simulations with Plexe, Version 3.1 of the framework must be installed.
Refer to the [Plexe install guide](https://plexe.car2x.org/building/) for more information.
Please mind that you might want to install OMNeT++ Version 6 or higher in order to use
the [ConstantHeadway](#constantheadway) Evaluation, even though the installation guide might suggest an older version.

#### ConstantHeadway

To use the ConstantHeadway Evaluation, OMNeT++ Version 6 or higher is needed.
Please refer to the [OMNeT++ Install Guide](https://doc.omnetpp.org/omnetpp/InstallGuide.pdf) for more information on
the requirements.

### Installation

#### Prerequisites

##### Git

Check whether Git is installed on your machine and install it if necessary using:

``` 
sudo apt install git
 ```

##### CMake

CMake Version 3.25 or higher is needed for building *Simopticon*.
If you don't have CMake installed, follow the guide below.
If you have an older version installed, you must first remove it.

First, make sure to install g++ and OpenSSL Development tools.

```
sudo apt install g++ libssl-dev
```

Then you need to download the latest version of CMake from their [download page](https://cmake.org/download/) - search
for the source distribution tar package.
Unpack the downloaded package using:

```
tar xf cmake-[version number].tar.gz
```

Open the newly created directory and run the configuration script with:

```
cd cmake-[version number] && ./configure
```

When the configuration has completed successfully, you are ready to build and install using:

```
make -j $(nproc)
sudo make install
```

You may remove the downloaded tar file and extracted directory if needed.

##### Python3 Development Tools

Check whether Python3 development tools are installed on your machine and install them if necessary using:

```
sudo apt install python3-dev
```

#### Simopticon

Go to the directory you want to install *Simopticon* in, e.g. `~/src`.
To get the source code, clone the git repository using:

```
git clone https://github.com/PeterNaggschga/simopticon.git
```

Create a build directory in the downloaded files with:

```
mkdir simopticon/build
cd simopticon/build
```

Build *Simopticon* by calling:

```
cmake ..
make -j  $(nproc)
```

The resulting executable `simopticon` may be copied to other locations or referenced via symlinks for more convenient
access.
The same applies to the `config` directory in `~/src/simopticon` which is used to configure the optimization process (
see [Usage](#usage)).

## Usage

## Extension