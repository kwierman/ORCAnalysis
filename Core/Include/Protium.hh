/*!
	\mainpage The Protium Libraries

	\tableofcontents

	\section intro Introduction

	The Tritium APIs are designed to provide a highly modular approach to cross-machine analysis and simulation tasks.
	The API's are split into 3 levels to isolate functionality between systems to avoid a "Do-All" API.

	The three API's are:

	- Protium
	    - Base framework and primitive functionality.
	    - Designed to run on low-level systems.
	- Deuterium
	    - Networking and Filesystem functionality. Future scripting functionality.
	    - Designed to run on host-side machines.
	- Tritium
	    - Graphics and advanced mathematical functionality.
	    - Designed to run on client machines.
	
	\section downloading Downloading Software

	This software is hosted on <a href="https://github.com/">GitHub</a>. 
	The URL for the repository is: https://github.com/kwierman/Protium .

	Pulling from the repository can be accomplished by downloading <a href="https://github.com/">Git</a>
	
	~~~~~~~~~~~~~~~~~~~~~
	git clone https://github.com/kwierman/Protium.git
	~~~~~~~~~~~~~~~~~~~~~

	It is advisable that if the software is being worked on to create a separate branch. 
	This can be accomplished with the following line:

	~~~~~~~~~~~~~~~~~~~~~
	git checkout -b <your_branch_name>
	~~~~~~~~~~~~~~~~~~~~~

	Pushing to the repository can be accomplished with the following command:

	~~~~~~~~~~~~~~~~~~~~~
	git push -u origin <your_branch_name>
	~~~~~~~~~~~~~~~~~~~~~

	---

	\section dep Dependencies

	The following libraries must be built and in the paths in order to build properly:

	-  CMake
		- <a href="http://www.cmake.org/">CMake Website</a>
		- Required for building the program
	- pthread.h
	    - Required for threading models
	    - Available on most Unix platforms


	---

	\section building Building

	Protium prohibits building within the directory tree. Therefore the following definitions will be made:

	- <source_directory>
	    - Where the source is located
	- <build_directory>
	    - Where the build will be located
	    - It's advised that for naming conventions, that this be similar to the source directory

	In order to generate build files for the program, issue the following command(s):

	~~~~~~~~~~~~~~~~~~~~~
	cd <build_directory>
	cmake <source_directory>
	~~~~~~~~~~~~~~~~~~~~~

	Or, to use the graphical editor:

	~~~~~~~~~~~~~~~~~~~~~
	cd <build_directory>
	ccmake <source_directory>
	~~~~~~~~~~~~~~~~~~~~~
	
	If configuring cmake and generating build files 

	\subsection nix_build Unix-Like Systems

	This includes MacOSX (Darwin).
	The commands to issue at this point are:

	~~~~~~~~~~~~~~~~~~~~~
	make
	make install
	~~~~~~~~~~~~~~~~~~~~~

	Make sure to set `LD_LIBRARY_PATH` or, on Darwin, `DYLD_LIBRARY_PATH` to the location of the libraries.
	This will be automated in the future.

	\subsection windows_build Windows

	<!--Open the build solution in windows by either clicking on `Protium.vcproj`, or opening 'Visual Studio' and open the solution 'Protium'.

	Once Visual Studio has completed parsing the documents, navigate `Solution->Build->Release` or hit the green arrow.
	-->

	\warning There are no plans to make Protium Windows-compatable.

	---
	\section running Running

	Protium will include unit tests

	~~~~~~~~~~~~~~~~~~~~~
	<name of exec> <optional path to macro>
	~~~~~~~~~~~~~~~~~~~~~

	The source code for the main executables is found in the Applications/ folder
	
	---

	\section code_hierarchy Code Hierarchy

	Protium is designed to be a <b>VERY</b> modular library. Every module will have a header for the full library located in the directory.

	\note Example:
	~~~~~~~~~~~~~~~~~~~~~
	#include <Protium/LinearAlgebra/LinearAlgebra.h>
	~~~~~~~~~~~~~~~~~~~~~	

	All headers are included in the top-down file, Protium.h .

	The following modules are part of the Protium libraries

	 - Design
	 	- Provides design technologies. Specifically, assertions, dimensionful quantities and types
	 	- Dependencies:
	 		- None
	 - Threads
	 	- Provides threading technologies
	 	- Explicit Dependencies:
	 		- Design
	 - Singleton
	 	- Defines singleton utilities such as creation and deletion policies
	 	- Explicit Dependencies:
	 		- Threads
	 		- Design
	 - Utilites
	 	- Miscellanious utility functions
	 	- Explicit Dependencies:
	 		- Singleton
	 - Allocation
	 	- Provides base classes for small object allocation
	 	- Explicit Dependencies:
	 		- Threads
	 		- Design
	 		- Singleton
	 - Linear Algebra
	 	- Provides matrix and vector classes to perform linear algebra
	 	- Explicit Dependencies:
	 		- Allocation
	 - Containers
	 	- Provides histogram and function classes for data storage
	 	- Explicit Dependencies:
	 		- Allocation


	---

	\section contact Contact

	Please contact Kevin Wierman at kwierman@gmail.com with issues, flaws or questions
*/

/*! \file Protium.h
	\author Kevin Wierman
	\email kwierman@gmail.com
	\breif Umbrella include file for all of the protium libraries
*/
#ifndef Protium_h_
#define Protium_h_

#include "Protium/Design/Design.h"
#include "Protium/Threads/Threads.h"
#include "Protium/Singleton/Singleton.h"
#include "Protium/Utilities/SignalCatching.h"
#include "Protium/Allocation/Allocation.h"
#include "Protium/LinearAlgebra/LinearAlgebra.h"


//! Encapsulates all Protium Modules
namespace Protium{}

#endif //File Guardian

