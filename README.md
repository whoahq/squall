# Squall

Squall is an unofficial open source implementation of the Storm standard
library.

The Storm standard library is a collection of data structures and functions
created by the talented folks at Blizzard Entertainment for use in their
games.

This project attempts to provide a version of Storm compatible with the
version used in the final release build of World of Warcraft: Wrath of the
Lich King in 2010: 3.3.5a (12340).

Squall also attempts to provide versions of Storm that work with other
projects centered around classic Blizzard games, like
[Starsurgical](https://github.com/Starsurgical). Note that Squall does not
natively support being built as a dynamic library, although it may support
such scenarios in the future.

In the spirit of documenting what is presumed to have existed at the time,
this project makes every attempt to maintain the canonical names, layouts, and
side effects of the original implementation of Storm. At the same time, it
attempts to ensure portability and compatibility with modern 64-bit systems.

## Storm Flavors

Squall supports different flavors depending on the type of project consuming
it. These flavors can be configured by setting a CMake variable called
`WHOA_STORM_FLAVOR`:

- `WOW`: Ensures the resulting build is compatible with World of Warcraft
  3.3.5a (12340)
- `SC1`: Ensures the resulting build is compatible with StarCraft 1.16/1.17

Flavors control groups of behavior-related defines. These defines can be used
on their own if the groups defined by the flavors are not desired.

## FAQ

**Why?**

It's fascinating to explore the development practices used to build a modern
major video game.

**Why does Squall focus on specific game versions?**

Storm changed a lot over the years it was used. By focusing on specific
versions of the games that used Storm, it's possible to imagine this
implementation will eventually be complete.

**Will this let me cheat in a game?**

No. Storm is a standard library, and none of its routines are particularly
relevant to the anti-cheat measures found in the games that used it.

**Can I use this in my own development projects?**

Maybe. Keep in mind the original library remains closed source and this
project is in no way sanctioned or official. Also keep in mind that C++, its
standard library, and OS-provided capabilities like memory management,
networking stacks, and rendering frameworks have substantially improved since
the 1990s-2010.

## Legal

This project is released into the public domain. No claim is made to any
intellectual property contained in this project.

World of Warcraft: Wrath of the Lich King ©2008 Blizzard Entertainment, Inc.
All rights reserved. Wrath of the Lich King is a trademark, and World of
Warcraft, Warcraft and Blizzard Entertainment are trademarks or registered
trademarks of Blizzard Entertainment, Inc. in the U.S. and/or other countries.
