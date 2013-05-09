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

#define __STDC_FORMAT_MACROS
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <pthread.h>
#include <openssl/sha.h>

#include "cmine.h"

int time_st, running = 1;
uint64_t hashes = 0, successfulHashes = 0;
char *filename = DEFAULT_FILE_NAME;
char *logFormat = DEFAULT_LOG_FORMAT;
int threadCount = DEFAULT_THREAD_COUNT;
int difficulty = -1;
int difficultyChanged = 0, threadCountChanged = 0, pathChanged = 0;

void initializeString(unsigned char *ptr)
{
	int i;
	char chr;
	for(i = 0; i < STRING_LENGTH; i++)
	{
		chr = characterList[rand() % CHARARRAY_LENGTH];
		ptr[i] = chr;
	}
}

void randomizeString(unsigned char *ptr, int times)
{
	int replaceIndex, randNum, i;
	char chr;
	for(i = 0; i < times; i++)
	{
		randNum = rand();
		replaceIndex = randNum % STRING_LENGTH;
		chr = characterList[randNum % CHARARRAY_LENGTH];
		ptr[replaceIndex] = chr;
	}
}

void finalize(void)
{
	struct timeval currentTime;
	int timeElapsed;
	gettimeofday(&currentTime, NULL);
	timeElapsed = currentTime.tv_sec - time_st;
	printf("Totalled %"PRIu64" hashes in %d seconds (%"PRIu64" h/s, %"PRIu64" s/coin)\n", hashes,
		timeElapsed, hashes / timeElapsed, timeElapsed / (successfulHashes == 0 ? 1 : successfulHashes));
}

void sigintHandler(int sig)
{
	running = 0;
}

void *thread(void *tid)
{
	unsigned char str[STRING_LENGTH + 1], digest[SHA512_DIGEST_LENGTH], md_str[SHA512_DIGEST_LENGTH*2+1];
	FILE *fi;
	int i, accumulator;
	initializeString(str);
	str[STRING_LENGTH] = '\0';
	if(threadCount > 1)
		printf("Starting miner thread with %s...\n",  str);
	else
		printf("Starting miner with %s...\n", str);
	while(running)
	{
		accumulator = 0;
		randomizeString(str, 1);
		SHA512((unsigned char*)&str, STRING_LENGTH, (unsigned char*)&digest);

        for(i = 0; i < difficulty; i++)
		{
        	accumulator |= (digest[i / 2] & MASK(i));
        	// this is a hack to check for zeroes by looking
        	// at the bits directly, this is much faster than
        	// strncmp and sprintf :D
        }

        if(accumulator == 0)
        {
        	successfulHashes++;
        	for(i = 0; i < SHA512_DIGEST_LENGTH; i++)
			{
	        	sprintf((char*)&md_str[i*2], "%02x", (unsigned int)digest[i]);
	        }
			if(threadCount > 1)
        		printf("MT%d: #%"PRIu64" Valid: %s\n", i, successfulHashes, str);
        	else
        		printf("M: #%"PRIu64" Valid: %s\n", successfulHashes, str);
        	fi = fopen(filename, "a");
        	fprintf(fi, logFormat, str, md_str);
        	fclose(fi);
        }
        hashes++;
	}
	printf("Stopping thread %d...\n", i);
	return NULL;
}

void printHexString(char *cp, int n)
{
	int i;
	for(i = 0; i < n; i++)
	{
		printf("%02x", cp[i] & 0xff);
	}
	printf("\n");
}

void printHelpString(void)
{
	puts("cmine [FLAG1 [FLAG2 ... [FLAGn] ] ] [PATH]");
	puts("PATH: The path to the file to which mined coins will be saved");
	puts("Available flags:");
	puts("-d X | --difficulty X: The current difficulty of the server");
	puts("-t X | --thread-count X: The current number of threads to use");
	puts("-f X | --format X: The fprintf format to log coins with");
	puts("-h --help: Shows this help message");
}

void processCLArguments(int argc, char **argv)
{
	int i;
	for(i = 1; i < argc; i++)
	{
		if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
		{
			printHelpString();
			exit(0);
		}
		else if(strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--difficulty") == 0)
		{
			i++;
			if(i == argc)
			{
				printf("Error: No argument following %s, exiting.\n", argv[i - 1]);
				exit(-1);
			}
			difficulty = (int)(strtol(argv[i], NULL, 10) & 0x7fffffff);
			printf("Difficulty set to %d\n", difficulty);
			difficultyChanged = -1;
		}
		else if(strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--thread-count") == 0)
		{
			i++;
			if(i == argc)
			{
				printf("Error: No argument following %s, exiting.\n", argv[i - 1]);
				exit(-1);
			}
			threadCount = (int)(strtol(argv[i], NULL, 10) & 0x7fffffff);
			printf("Thread count set to %d\n", threadCount);
			threadCountChanged = -1;
		}
		else if(strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--format") == 0)
		{
			i++;
			if(i == argc)
			{
				printf("Error: No argument following %s, exiting.\n", argv[i - 1]);
				exit(-1);
			}
			logFormat = argv[i];
			printf("Log format set to %s\n", logFormat);
		}
		else
		{
			filename = argv[i];
			pathChanged = 1;
			printf("Filename set to %s\n", filename);
		}
	}
	if(!difficultyChanged)
	{
		printf("Error: No difficulty specified, exiting.\n");
		printf("Note: If you need help, specify the flag -h\n");
		exit(-1);
	}
	if(!pathChanged)
	{
		printf("No filename specified, defaulting to %s.\n", DEFAULT_FILE_NAME);
	}
	if(!threadCountChanged)
	{
		printf("No thread count specified, defaulting to %d.\n", DEFAULT_THREAD_COUNT);
	}
	if(threadCount < 1)
	{
		printf("Error: Invalid number of threads: %d.\n", threadCount);
		exit(-1);
	}
	if(threadCount > 6)
	{
		printf("Discouraged number of threads: %d.\n", threadCount);
	}
}

int stringStartsWith(char *str, char *prefix)
{
	if(strlen(str) > strlen(prefix)) return 0;
	return strncmp(str, prefix, strlen(prefix)) == 0;
}

void mainMultithread()
{
	pthread_t threads[threadCount];
	int i;
	for(i = 0; i < threadCount; i++)
	{
		pthread_create(&threads[i], NULL, thread, NULL);
	}

	for(i = 0; i < threadCount; i++)
	{
		pthread_join(threads[i], NULL);
	}
}

int main(int argc, char **argv)
{
	struct timeval currentTime;
	puts(WELCOME_MESSAGE);
	processCLArguments(argc, argv);

	gettimeofday(&currentTime, NULL);
	time_st = currentTime.tv_sec;

	atexit(finalize);
	signal(SIGINT, sigintHandler);

	srand(currentTime.tv_sec);
	if(threadCount > 1)
	{
		// If we're in multi thread mode, run mainMulithread();
		mainMultithread();
	}
	else
	{
		// if we're in single thread mode, just run the thread function directly
		thread(NULL);
	}
	return 0;
}
