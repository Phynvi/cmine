#!/usr/bin/env python
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
# along with this program.  If not, see <http:#www.gnu.org/licenses/>.

import socket
import json
import string
import sys
import io

addr = sys.argv[1]
string = sys.argv[2]
str_hash = sys.argv[3]
log_format = "check %s %s\n"
write_on_connect_fail = "store.blc"
server = ("server.bloocoin.org", 3122)

try:
    s = socket.socket()
    s.connect(server)
    s.send(json.dumps({"cmd": "check", "winning_string": string, "winning_hash": str_hash, "addr": addr}))
    data = s.recv(1024)
    s.close()
    data = json.loads(data)
    if data['success']:
        print "Claimed coin: " + string
    else:
        print "Invalid coin: " + string
except Exception, e:
	stor = open(write_on_connect_fail, "a")
	stor.write(log_format % (string, str_hash))
	stor.close()
	print "Can't connect to server, logged coin to " + write_on_connect_fail
