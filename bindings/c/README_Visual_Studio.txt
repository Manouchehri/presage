____________________________________________
Linking to libpresage DLL with Visual Studio
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


In order to link to libpresage DLL (built with MinGW/MSYS) with Visual Studio, you first need to create an import library compatible with Visual Studio.


This is done using the LIB tool that comes with Visual Studio:

    LIB /MACHINE:x86 /DEF:libpresage-1.def /OUT:libpresage-1.lib

or, for x64 builds:

    LIB /MACHINE:x64 /DEF:libpresage-1.def /OUT:libpresage-1.lib


The LIB tool takes libpresage-1.def and libpresage-1.dll files as input, and generates the libpresage-1.lib import library.


Copy the generated import library to your local presage installation:

    COPY libpresage-1.lib c:\presage\lib\


You can now link their application against the libpresage DLL with Visual Studio with the following command:

    CL /EHsc /Ic:\presage\include presage_c_demo.c c:\presage\lib\libpresage-1.lib

