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

#include <stdio.h>
#include "claim.h"

errno_ct claimHash(char *cmd, char *arg)
{
	char syscall[MAX_STR_LEN];
	sprintf((char*)syscall, "%s %s", cmd, arg);
	return system(syscall);
}

void performClaim(actiongroup_ct *group, unsigned char *plaintext, unsigned char *hash)
{
	int i;
	char arg[MAX_STR_LEN];
	sprintf((char*)arg, "%s %s", plaintext, hash);
	for(i = 0; i < group->count; i++)
	{
		claimHash(group->actions[i], arg);
	}
}

void registerAction(actiongroup_ct *group, char *cmd)
{
	if(group->count >= MAX_ACTION_IDS)
	{
		printf("Error: Can only register up to %d actions.\n", MAX_ACTION_IDS);
		exit(-1);
	}
	group->actions[group->count++] = cmd;
}
