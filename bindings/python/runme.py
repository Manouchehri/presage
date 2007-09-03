
 ##############################################################################
 #                                                                            #
 # Soothsayer, an extensible predictive text entry system                     #
 # ------------------------------------------------------                     #
 #                                                                            #
 # Copyright (C) Matteo Vescovi <matteo.vescovi@yahoo.co.uk>                  #
 #                                                                            #
 # This program is free software; you can redistribute it and/or              #
 # modify it under the terms of the GNU General Public License                #
 # as published by the Free Software Foundation; either version 2             #
 # of the License, or (at your option) any later version.                     #
 #                                                                            #
 # This program is distributed in the hope that it will be useful,            #
 # but WITHOUT ANY WARRANTY; without even the implied warranty of             #
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              #
 # GNU General Public License for more details.                               #
 #                                                                            #
 # You should have received a copy of the GNU General Public License          #
 # along with this program; if not, write to the Free Software                #
 # Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. #
 #                                                                            #
 ##############################################################################

import soothsayer
import sys

if len(sys.argv) != 2:
	print "Usage: python runme.py soothsayer_config_file"
	sys.exit(1)
else:
	config = sys.argv[1]


# Create Soothsayer object
soothie = soothsayer.Soothsayer(config)

print "Enter text at the prompt (press enter on empty line to exit):"

while str != "":
	str = raw_input(">  ")
	print soothie.predict(str)

# Destroy Soothsayer object
del soothie

print "Goodbye"
