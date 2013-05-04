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
#define CHR(x) cl[x % CLEN]
// #define MASK(x) ((x & 1) == 0 ? 0xf0 : 0x0f)
#define MASK(x) (0xf << ((x & 1) * 8))

// ONLY CHANGE THESE!
#define SLEN 25
// This defines the string length to try and hash, 25-35 seems to be a sweet spot

#define DIFF 7
// This is the difficulty level

#define MULTI
// Comment the above line out to use single-threaded mode

#define THREADC 2
// The number of threads to use. The hash/s doesn't increase linearly with
// this, and it stops going up at all really on my box, so if you want more
// than 2 threads you'll have to experiment yourself or run more than one
// process, which is what I do

// STOP CHANGING NOW

char cl[64] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
int time_st, running = 1;
uint64_t hashes = 0, shashes = 0;

// Initializess a string to all random values
void init_string(unsigned char *ptr)
{
	int i;
	char rw;
	for(i = 0; i < SLEN; i++)
	{
		rw = cl[rand() % CLEN];
		ptr[i] = rw;
	}
}

// Changes one character in the string to a random value
// Only calls rand() once for maximum speed
void randomize_string(unsigned char *ptr, int rpi)
{
	int ri, r;
	char rw;
	struct timeval ct;
	uint64_t rl = (uint64_t)ct.tv_usec + (uint64_t)ct.tv_sec;
	r = (int)(rl % 2147483648ul);
	ri = r % SLEN;
	rw = cl[r % CLEN];
	ptr[ri] = rw;
}

// Prints the status report at the end
void finalize(void)
{
	struct timeval ct;
	int tts;
	gettimeofday(&ct, NULL);
	tts = ct.tv_sec - time_st;
	printf("Totalled %"PRIu64" hashes in %d seconds (%"PRIu64" h/s, %"PRIu64" s/coin)\n", hashes, tts, hashes / tts, tts / (shashes == 0 ? 1 : shashes));
}

// I don't like using signal() but this code isn't designed
// for cleanliness or safety so whatever
void sig(int sig)
{
	running = 0;
}

void *thread(void *m)
{
	unsigned char str[SLEN + 1], digest[SHA512_DIGEST_LENGTH], md_str[SHA512_DIGEST_LENGTH*2+1];
	FILE *fi;
	int i, ac, tn = *((int*)m);
	init_string(str);
	str[SLEN] = '\0';
	if(tn != -1)
		printf("Starting miner thread %d with %s...\n", tn, str);
	else
		printf("Starting miner with %s...\n", str);
	while(running)
	{
		ac = 0;
		randomize_string(str, 1);

		SHA512((unsigned char*)&str, SLEN, (unsigned char*)&digest);

        for(i = 0; i < DIFF; i++)
		{
        	ac |= (digest[i / 2] & MASK(i));
        	// this is a hack to check for zeroes by looking
        	// at the bits directly, this is much faster than
        	// strncmp and sprintf :D
        }

        if(ac == 0)
        {
        	for(i = 0; i < SHA512_DIGEST_LENGTH; i++)
			{
	        	sprintf((char*)&md_str[i*2], "%02x", (unsigned int)digest[i]);
	        }
			if(tn != -1)
        		printf("MT%d: check %s %s\n", tn, str, md_str);
        	else
        		printf("M: check %s %s\n", str, md_str);
        	fi = fopen("hashes.blc", "a");
        	fprintf(fi, "check %s %s\n", str, md_str);
        	fclose(fi);
        	shashes++;
        }
        hashes++;
	}
	printf("Stopping thread %d...\n", tn);
	return NULL;
}

int main(void)
{
	struct timeval ct;
#ifdef MULTI
	pthread_t threads[THREADC];
	int thread_rets[THREADC];
#endif
	int thr;

	gettimeofday(&ct, NULL);
	time_st = ct.tv_sec;

	atexit(finalize);
	signal(SIGINT, sig);

	srand(ct.tv_sec);

#ifdef MULTI
	for(thr = 0; thr < THREADC; thr++)
	{
		thread_rets[thr] = pthread_create(&threads[thr], NULL, thread, (void*)&thr);
	}

	for(thr = 0; thr < THREADC; thr++)
	{
		pthread_join(threads[thr], NULL);
	}
#endif
#ifndef MULTI
	// if we're in single thread mode, just run the thread function directly
	int tn = -1;
	thread((void*)&tn);
#endif
	return 0;
}