
##########
#  Presage, an extensible predictive text entry system
#  ------------------------------------------------------
#
#  Copyright (C) 2008  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>
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

## Process this file with automake to produce Makefile.in

if BUILD_ARPA_NGRAM

BUILT_SOURCES =	\
	arpa_en.vocab \
	arpa_en.arpa \
	arpa_it.vocab \
	arpa_it.arpa 

arpa_en.vocab:	../the_picture_of_dorian_gray.txt
	$(TEXT2WFREQ) < $< | $(WFREQ2VOCAB) -top 20000 > $@

arpa_en.idngram:	 ../the_picture_of_dorian_gray.txt arpa_en.vocab
	$(TEXT2IDNGRAM) -n 3 -vocab arpa_en.vocab -temp . < $< > $@

arpa_en.arpa:	arpa_en.idngram arpa_en.vocab
	$(IDNGRAM2LM) -idngram arpa_en.idngram -vocab arpa_en.vocab -arpa arpa_en.arpa 

arpa_it.vocab:	../the_picture_of_dorian_gray.txt
	$(TEXT2WFREQ) < $< | $(WFREQ2VOCAB) -top 20000 > $@

arpa_it.idngram:	 ../the_picture_of_dorian_gray.txt arpa_it.vocab
	$(TEXT2IDNGRAM) -n 3 -vocab arpa_it.vocab -temp . < $< > $@

arpa_it.arpa:	arpa_it.idngram arpa_it.vocab
	$(IDNGRAM2LM) -idngram arpa_it.idngram -vocab arpa_it.vocab -arpa arpa_it.arpa 

# ${prefix}/share/${package-name} directory
pkgdata_DATA =	arpa_en.arpa \
		arpa_en.vocab \
		arpa_it.arpa \
		arpa_it.vocab 

# Clean out files created during tests.
# Required to make distcheck happy.
DISTCLEANFILES =	*.arpa \
			*.vocab \
			*.idngram 

endif
