# Barebones optimized bloocoin miner
# Copyright (C) 2013 T. Galvin

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.


# You might want to play around with the following variables to suit your
# build environment.

CXX = gcc
CXXFLAGS = -Wall -O3
OBJECTS = cmine.o claim.o
SHELL = /bin/bash
PNAME = cmine
INCL_DIR = -L/usr/include
INCLUDES = -lcrypto -lpthread

cmine: clean ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${PNAME} ${INCL_DIR} ${INCLUDES}

clean:
	rm *.o || true
	rm ${PNAME} || true
	
%.o: %.c
	${CXX} ${CXXFLAGS} -c $^ -o $@
