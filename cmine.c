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
#include "claim.h"

int time_st, running = 1;
uint64_t hashes = 0, successfulHashes = 0;
char *filename = DEFAULT_FILE_NAME;
char *logFormat = DEFAULT_LOG_FORMAT;
char *minerAddress = NULL;
int threadCount = DEFAULT_THREAD_COUNT;
int difficulty = -1;
int difficultyChanged = 0, threadCountChanged = 0, logFormatChanged = 0,
	pathChanged = 0, minerAddressChanged = 0;
actiongroup_ct *claimActions;

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
	free(claimActions);
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
#ifdef REUSE_CONTEXT
	SHA512_CTX *context = malloc(sizeof(SHA512_CTX));
#endif
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
#ifdef REUSE_CONTEXT
		SHA512_Init(context);
		SHA512_Update(context, (unsigned char*)&str, STRING_LENGTH);
		SHA512_Final((unsigned char*)&digest, context);
#else
		SHA512((unsigned char*)&str, STRING_LENGTH, (unsigned char*)&digest);
#endif

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
        		printf("MT: #%"PRIu64" Valid: %s\n", successfulHashes, str);
        	else
        		printf("M: #%"PRIu64" Valid: %s\n", successfulHashes, str);
        	fi = fopen(filename, "a");
        	fprintf(fi, logFormat, str, md_str);
        	fclose(fi);
        	if(minerAddress != NULL)
        	{
        		performClaim(claimActions, minerAddress, str, md_str);
        	}
        }
        hashes++;
	}
	printf("Stopping thread %d...\n", i);
#ifdef REUSE_CONTEXT
    free(context);
#endif
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
	puts("-a X | --address X: Specify an address to pass to any claim actions");
	puts("-ca X | --claim-action X: Performs X with args PLAINTEXT, HASH upon mining a coin");
	puts("The above flag can be called multiple times to perform multiple actions.");
	puts("(Note that no action will be performed if no address is specified to send to)");
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
		else if(strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--address") == 0)
		{
			i++;
			if(i == argc)
			{
				printf("Error: No argument following %s, exiting.\n", argv[i - 1]);
				exit(-1);
			}
			minerAddress = argv[i];
			printf("Miner address set to %s\n", minerAddress);
			minerAddressChanged = 1;
		}
		else if(strcmp(argv[i], "-ca") == 0 || strcmp(argv[i], "--claim-action") == 0)
		{
			i++;
			if(i == argc)
			{
				printf("Error: No argument following %s, exiting.\n", argv[i - 1]);
				exit(-1);
			}
			registerAction(claimActions, argv[i]);
			printf("Registered claim action %d as %s\n", claimActions->count, argv[i]);
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
	if(!minerAddressChanged && claimActions->count > 0)
	{
		printf("Warning: No miner address specified, claim actions given won't be called.\n");
	}
	if(!logFormatChanged)
	{
		printf("No log format specified, defaulting to %s.\n", DEFAULT_LOG_FORMAT);
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
		printf("Warning: Discouraged number of threads: %d.\n", threadCount);
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
	claimActions = malloc(sizeof(actiongroup_ct));
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
