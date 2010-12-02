____________________________________________
Linking to libpresage DLL with Visual Studio
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


In order to link to libpresage DLL (built with MinGW/MSYS) with Visual Studio, one first needs to create an import library compatible with Visual Studio.


This is done using the LIB tool that comes with Visual Studio:

     LIB /MACHINE:x86 /DEF:libpresage-1.def /OUT:libpresage-1.lib

The LIB tool takes libpresage-1.def and libpresage-1.dll files as input, and generates the libpresage-1.lib import library.


One can now link their application against the libpresage DLL with Visual Studio with the following command:

    CL /EHsc /Ic:\presage\include presage_c_demo.c c:\presage\bin\libpresage-1.lib

