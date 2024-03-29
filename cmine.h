// Barebones optimized bloocoin miner
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
#include "action.h"

// ONLY CHANGE THESE!
// This defines the string length to try and hash, 25-35 seems to be a sweet spot
#define STRING_LENGTH 25

// The default filename if one isn't specified via the command line
#define DEFAULT_FILE_NAME "hashes.blc"

// The default printf-style log format to write to a file
#define DEFAULT_LOG_FORMAT "check %s %s\n"

// The default number of threads to use
#define DEFAULT_THREAD_COUNT 2

// The welcome message to print upon execution of the program
#define WELCOME_MESSAGE \
"  _________ ___   _ ___  ___ \n" \
" / ___/ __ `__ \\/ / __ \\/ _ \\\n" \
"/ /__/ / / / / / / / / /  __/\n" \
"\\___/_/ /_/ /_/_/_/ /_/\\___/ \n" \
"CMine 0.02 by Potassium\n"

// Reuse a SHA512_CTX rather than using the SHA512() helper function every time (experimental)
// #define REUSE_CONTEXT

// STOP CHANGING NOW

#define CHARARRAY_LENGTH 62
#define MASK(x) ((x & 1) == 0 ? 0xf0 : 0x0f)

// Represents the data that must be passed to the action thread
typedef struct
{
	actiongroup_ct *actions;
	char *addr;
	unsigned char *str;
	unsigned char *md;
	int silent;
} claim_ct;

char characterList[64] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

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

// Creates a new thread for handling tasks so it doesn't block the mining thread.
void *taskThread(void *vp);

// Create a valid mask from the current difficulty level.
uint64_t maskFromDifficulty(int diff);

// Main thread function.
void *thread(void *tid);

// (unused) print a char array of length n as hex.
void printHexString(char *cp, int n);

// Prints the command line flag help.
void printHelpString(void);

// Process command-line arguments passed to the executable.
void processCLArguments(int argc, char **argv);

// Determines whether a given string starts with a prefix.
int stringStartsWith(char *str, char *prefix);

// Handles the main program logic with multithreading enabled
void mainMultithread();

// Huuuuueee I wonder what this one does?
int main(int argc, char **argv);
