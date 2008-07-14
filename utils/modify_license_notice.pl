#!/usr/bin/perl

##########
#  Presage, an extensible predictive text entry system
#  ---------------------------------------------------
#
#  Copyright (C) 2008  Matteo Vescovi <matteo.vescovi\@yahoo.co.uk>
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License along
#  with this program; if not, write to the Free Software Foundation, Inc.,
#  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

# Simple perl script to change the license notice in source code files.
# 
# Usage: find . -name "*.cpp" -type f | xargs perl modify_license_notice.pl
#

use strict;
use warnings;

# use heredoc string to supply new license
my $new_license = <<HEREDOC;
/******************************************************
 *  Presage, an extensible predictive text entry system
 *  ---------------------------------------------------
 *
 *  Copyright (C) 2008  Matteo Vescovi <matteo.vescovi\@yahoo.co.uk>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
                                                                             *
                                                                **********(*)*/
HEREDOC


foreach my $file (@ARGV) {
    open FILE, "<", $file or die "Cannot open file $file for reading.\n";
    # slurp file contents in
    my $contents = do { local $/; <FILE> };
    close FILE;

    if ($contents =~ s%/\*{50,}.*\*{10}\(\*\)\*/%$new_license%ms) {
	my $temp_file = "${file}.out.bak";
	open OUTFILE, ">", "$temp_file" or die "Cannot open file $temp_file for writing.\n";
	print OUTFILE "$contents";
	close OUTFILE;
	rename "$temp_file", "$file";
	print "Substituted license in file $file\n";
    } else {
	print "No substitutions to be made in $file\n";
    }
}
