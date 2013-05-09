cmine
=====
Cmine is a barebones coin miner for the virtual currency system [bloocoin](http://bloocoin.org/). It doesn't have any functionality to connect to a server; its only job is to check for coins. The bloocoin repository can be found [here](https://github.com/bloocoin).

aims
====
I have some goals for this program. It's a mostly personal project and I intend for it to stay only as a miner.

program
-------
This program is intended for speed, that's why it's so lightweight and written in C. At the time of writing, this program can achieve about 2.1MHash/s with the default settings (string length 25, 2 threads) on an Intel Core 2 Quad @2.33GHz per process, and I'm still trying to optimize it further. I hope for this miner to be recognized for raw speed, not features, so please refrain from bloating the main loop of the program with anything that will reduce its speed.

development
-----------
I never intended this to become a public thing, therefore the code is quite messy, it probably isn't very portable and it could be a lot better. At the moment I'm completely inept at Github, so please excuse me if I trip up or make a stupid mistake. At this stage, all suggestions and contributions are welcomed.

versioning
---------
The versioning system for CMine is a.bbc, where a is the major version, b is the minor version and c is the revision. The major version is only incremented when a huge feature update has happened, or the program has been majorly rewritten. The minor version is for individual stable features or feature sets. The revision is omitted unless a mistake with the previous release was made, in which case the letters a-z are used to signify rereleases. This is all subject to change.

settings
========
The way you set up and run this program can have a significant effect on the speed of the mining. I have found, for me, that the ideal configuration is to have 2 threads per process, and run (cores/2) processes. This maximizes speed while minimizing the number of processes running. I am currently working on an alternate method for this, as POSIX threads don't seem to work quite right - they start to ''decrease'' the speed after (cores/2) is surpassed. Therefore, multiple processes seems to be the best option. Again, I am still working on this, and I hope to have it more efficient sooner rather than later.

You must set the difficulty to level to the correct one provided by the server, or you may end up with less hashes (if you set it too high) or loads of invalid hashes (if you set it too low). The current difficulty level is '''7''' at the time of writing, but don't rely on that for a time-accurate number.
