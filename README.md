cmine
=====
Cmine is a barebones coin miner for the virtual currency system [bloocoin](http://bloocoin.org/). It doesn't have any functionality to connect to a server; its only job is to check for coins. The bloocoin repository can be found [here](https://github.com/bloocoin).

aims
====
I have some goals for this program. It's a mostly personal project and I intend for it to stay only as a miner.

program
-------
This program is intended for speed, that's why it's so lightweight and written in C. At the time of writing, this program can achieve about 1.2MHash/s with the default settings (string length 25, 2 threads), and I'm still trying to optimize it further. I hope for this miner to be recognized for raw speed, not features, so please refrain from bloating the main loop of the program with anything that will reduce its speed.

development
-----------
I never intended this to become a public thing, therefore the code is quite messy, it probably isn't very portable and it could be a lot better. At the moment I'm completely inept at Github, so please excuse me if I trip up or make a stupid mistake. At this stage, all suggestions and contributions are welcomed.
