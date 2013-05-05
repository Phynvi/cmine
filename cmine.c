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
#include <openssl/sha.h>
#include <sys/time.h>
#include <inttypes.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

#define CLEN 62
#define CHR(x) characterList[x % CLEN]
#define MASK(x) ((x & 1) == 0 ? 0xf0 : 0x0f)
// #define MASK(x) (0xf << ((x & 1) * 4))

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

char characterList[64] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
int time_st, running = 1;
uint64_t hashes = 0, successfulHashes = 0;

// Initializess a string to all random values
void initializeString(unsigned char *ptr)
{
	int i;
	char chr;
	for(i = 0; i < STRING_LENGTH; i++)
	{
		chr = characterList[rand() % CLEN];
		ptr[i] = chr;
	}
}

// Changes x characters in the string to a random value
// Only calls rand() once for maximum speed
void randomizeString(unsigned char *ptr, int times)
{
	int replaceIndex, randNum, i;
	char chr;
	for(i = 0; i < times; i++)
	{
		randNum = rand();
		replaceIndex = randNum % STRING_LENGTH;
		chr = characterList[randNum % CLEN];
		ptr[replaceIndex] = chr;
	}
}

// Prints the status report at the end
void finalize(void)
{
	struct timeval currentTime;
	int timeElapsed;
	gettimeofday(&currentTime, NULL);
	timeElapsed = currentTime.tv_sec - time_st;
	printf("Totalled %"PRIu64" hashes in %d seconds (%"PRIu64" h/s, %"PRIu64" s/coin)\n", hashes,
		timeElapsed, hashes / timeElapsed, timeElapsed / (successfulHashes == 0 ? 1 : successfulHashes));
}

// I don't like using signal() but this code isn't designed
// for cleanliness or safety so whatever
void sig(int sig)
{
	running = 0;
}

void *thread(void *tid)
{
	unsigned char str[STRING_LENGTH + 1], digest[SHA512_DIGEST_LENGTH], md_str[SHA512_DIGEST_LENGTH*2+1];
	FILE *fi;
	int i, accumulator, threadId = *((int*)tid);
	initializeString(str);
	str[STRING_LENGTH] = '\0';
	if(threadId != -1)
		printf("Starting miner thread %d with %s...\n", threadId, str);
	else
		printf("Starting miner with %s...\n", str);
	while(running)
	{
		accumulator = 0;
		randomizeString(str, 1);
		SHA512((unsigned char*)&str, STRING_LENGTH, (unsigned char*)&digest);

        for(i = 0; i < DIFFICULTY_LEVEL; i++)
		{
        	accumulator |= (digest[i / 2] & MASK(i));
        	// this is a hack to check for zeroes by looking
        	// at the bits directly, this is much faster than
        	// strncmp and sprintf :D
        }

        if(accumulator == 0)
        {
        	for(i = 0; i < SHA512_DIGEST_LENGTH; i++)
			{
	        	sprintf((char*)&md_str[i*2], "%02x", (unsigned int)digest[i]);
	        }
			if(threadId != -1)
        		printf("MT%d: check %s %s\n", threadId, str, md_str);
        	else
        		printf("M: check %s %s\n", str, md_str);
        	fi = fopen("hashes.blc", "a");
        	fprintf(fi, "check %s %s\n", str, md_str);
        	fclose(fi);
        	successfulHashes++;
        }
        hashes++;
	}
	printf("Stopping thread %d...\n", threadId);
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

int main(void)
{
	struct timeval currentTime;
	int threadId;
#ifdef MULTI
	pthread_t threads[THREADC];
	int threadReturnValues[THREADC];
#endif

	gettimeofday(&currentTime, NULL);
	time_st = currentTime.tv_sec;

	atexit(finalize);
	signal(SIGINT, sig);

	srand(currentTime.tv_sec);

#ifdef MULTI
	for(threadId = 0; threadId < THREADC; threadId++)
	{
		threadReturnValues[threadId] = pthread_create(&threads[threadId], NULL, threadId, (void*)&threadId);
	}

	for(threadId = 0; threadId < THREADC; threadId++)
	{
		pthread_join(threads[threadId], NULL);
	}
#endif
#ifndef MULTI
	// if we're in single thread mode, just run the thread function directly
	threadId = -1;
	thread((void*)&threadId);
#endif
	return 0;
}