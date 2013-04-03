ABGtoC
======

Where this comes from
---------------------

This repo is my playing with the code and examples from the _Absolute Beginner's Guide to C (Second Edition)_, by Greg Perry.  I found this book in a swap pile in college, and read it to learn a few fundamentals of C.  I evetually picked up a copy of K&R C, and started learning from that, but the _ABGtoC_ is where I started.

blackjack.c
-----------

blackjack.c is the intended result of working though the exercises and examples in _ABGtoC_.  When compiled, this source produces a simple blackjack program - the user plays against a computer dealer through an interactive text interface.

TODO
----

I plan to make some tweaks to the program.  There are a few improvements I'd like to make.

* The computer recognizes a blackjack when the player hits one.
* Optional '5 Card Charlie' rule - player wins if they draw a 5 card hand without busting.
* A properly shuffled multi-deck shoe from which cards are dealt.
* Persistent won-loss-push score for the duration of this play session.
* Persistent shoe state, leading to a reshuffle when the shoe gets low.
* Program allows several players.
