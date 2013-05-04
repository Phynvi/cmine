cmine
=====
Cmine is a barebones coin miner for the virtual currency system @bloocoin. It doesn't have any functionality to connect
to a server. Its only job is to check for coins.

aims
====
program
-------
This program is intended for speed, that's why it's so lightweight and written in C. At the time of writing, this
program can achieve about 1.2MHash/s with the default settings (string length 25, 2 threads), and I'm still trying to
optimize it further.

development
-----------
I never intended this to become a public thing, therefore the code is quite messy, it probably isn't very portable and
it could be a lot better. At the moment I'm completely inept at Github, so please excuse me if I trip up or make a
stupid mistake. At this stage, all suggestions and contributions are welcomed.
