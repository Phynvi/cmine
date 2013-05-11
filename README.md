**Important note**: Currently there is *no support for Windows*. This miner is only supported on linux, and has unsupported functionality on Mac OSX. I don't have access to a windows install to port it onto.


cmine
=====
Cmine is a barebones coin miner for the virtual currency system [bloocoin](http://bloocoin.org/). It doesn't have any functionality to connect to a server; its only job is to check for coins. It does, however, provide functionality to syscall() a path upon mining a coin, which could be used to call a script which checks the coins with the server.
The bloocoin repository can be found [here](https://github.com/bloocoin).

aims
====
I have some goals for this program. It's a mostly personal project and I intend for it to stay only as a miner.

program
-------
This program is intended for speed, that's why it's so lightweight and written in C. At the time of writing, this program can achieve about 2.1MHash/s with the default settings (string length 25, 2 threads) on an Intel Core 2 Quad @2.33GHz per process, and I'm still trying to optimize it further. I hope for this miner to be recognized for raw speed, not features, so please refrain from bloating the main loop of the program with anything that will reduce its speed.

development
-----------
I never intended this to become a public thing, therefore the code probably isn't very portable and it could be a lot better in that respect. At the moment I'm a little inept at Github, so please excuse me if I trip up or make a stupid mistake. At this stage, all suggestions and contributions are welcomed.

versioning
---------
The versioning system for CMine is a.bbc, where a is the major version, b is the minor version and c is the revision. The major version is only incremented when a huge feature update has happened, or the program has been majorly rewritten. The minor version is for individual stable features or feature sets. The revision is omitted unless a mistake with the previous release was made, in which case the letters a-z are used to signify rereleases. This is all subject to change.

settings
========
The way you set up and run this program can have a significant effect on the speed of the mining. I have found, for me, that the ideal configuration is to have 2 threads per process, and run (cores/2) processes. This maximizes speed while minimizing the number of processes running. I am currently working on an alternate method for this, as POSIX threads don't seem to work quite right - they start to *decrease* the speed after (cores/2) is surpassed. Therefore, multiple processes seems to be the best option. Again, I am still working on this, and I hope to have it more efficient sooner rather than later.

You must set the difficulty to level to the correct one provided by the server, or you may end up with less hashes (if you set it too high) or loads of invalid hashes (if you set it too low). The current difficulty level is **7** at the time of writing, but don't rely on that for a time-accurate number.

The **-ca** or **--claim-action** flag can be used to perform an action upon mining a coin. The arguments passed to the call are the plaintext and hash in that order. For example:

    ./cmine -d 7 -a c0b6bb11d1462b67b7dc6d556f9c4d180bd8ba10 -ca "./checkcoin.py" coins.blc

This command would call the command:

    ./checkcoin.py c0b6bb11d1462b67b7dc6d556f9c4d180bd8ba10 PLAINTEXT HASH

Every time a coin is mined. Replace *c0b6bb11d1462b67b7dc6d556f9c4d180bd8ba10* with your own address, or the coins will be sent to me instead (not that I'm complaining!) Note that check.py must have the execute bit enabled for it to work - a simple *chmod 777 checkcoin.py* would do the trick. This flag is quite powerful and adds a lot of functionality to cmine.
