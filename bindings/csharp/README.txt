Copyright (C) 2015  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>
___________________
The Presage project
~~~~~~~~~~~~~~~~~~~

__________________________________________________
Building the presage C# binding with Visual Studio
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Building the presage C# binding requires Visual Studio 2013.

Visual Studio Express edition should work just fine.


To build, simply open the presage.net.sln solution in Visual Studio
IDE and build it.

Alternatively, to rebuild on the command line in a Visual Studio
shell:

  devenv /Rebuild "Release|x86" presage.net.sln

or:

  devenv /Rebuild "Release|x64" presage.net.sln


To rebuild in Debug mode:

  devenv /Rebuild "Debug|x86" presage.net.sln

or:

  devenv /Rebuild "Debug|x64" presage.net.sln




