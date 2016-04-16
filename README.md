# slack++
c++ slack ncurses client

To build you need:

* rapidjson
* cmake
* openssl
* libcurl

** Optional:**

* libnotify

and a fairly recent c++ compiler.
It was tested on Archlinux and Mac OS X 10.10.

## How to build & run?

    $ mkdir build
    $ cd build
    $ cmake -G "Unix Makefiles" ../
    $ make
    $ ./slack

You've got to export SLACK_TOKEN from eg .bashrc, eg: by adding at the end of .bashrc in your home:
export SLACK_TOKEN=xxxxxxxxxxxxx
