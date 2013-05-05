// Quick 'n' dirty bloocoin miner
// Copyright (C) 2013 T. Galvin

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

// ONLY CHANGE THESE!
#define STRING_LENGTH 25
// This defines the string length to try and hash, 25-35 seems to be a sweet spot

#define DIFFICULTY_LEVEL 7
// This is the difficulty level

#define MULTITHREADING
// Comment the above line out to use single-threaded mode

#define THREAD_COUNT 2
// The number of threads to use. The hash/s doesn't increase linearly with
// this, and it stops going up at all really on my box, so if you want more
// than 2 threads you'll have to experiment yourself or run more than one
// process, which is what I do

// STOP CHANGING NOW

#define CLEN 62
#define CHR(x) characterList[x % CLEN]
#define MASK(x) ((x & 1) == 0 ? 0xf0 : 0x0f)
// #define MASK(x) (0xf << ((x & 1) * 4))

// Initializess a string to all random values.
void initializeString(unsigned char *ptr);

// Changes x characters in the string to a random value
// Only calls rand() once for maximum speed.
void randomizeString(unsigned char *ptr, int times);

// Prints the status report at the end of execution.
void finalize(void);

// I don't like using signal() but this code isn't designed
// for cleanliness or safety so whatever.
void sigintHandler(int sig);

// Main thread function.
void *thread(void *tid);

// (unused) print a char array of length n as hex.
void printHexString(char *cp, int n);

// Huuuuueee I wonder what this one does?
int main(void);