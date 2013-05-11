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
#include <stdio.h>
#include <stdlib.h>

#define MAX_ACTION_IDS 256
#define MAX_STR_LEN 384

typedef unsigned int errno_ct;
typedef struct
{
	char *actions[MAX_ACTION_IDS];
	int count;
} actiongroup_ct;

// Claim a hash by calling a specified command
errno_ct claimHash(char *cmd, char *arg);

// Performs a claim action on all registered commands in a group
// Note: plaintext and hash are unsigned char to be the same as the
// variables in cmine.c:thread(), str and md_str
void performClaim(actiongroup_ct *group, char *address, unsigned char *plaintext, unsigned char *hash);

// Register a command to a group to call upon claiming of a hash
void registerAction(actiongroup_ct *group, char *cmd);
