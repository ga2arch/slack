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

or you can start slack by passing it a company token:

    $ ./slack COMPANY_TOKEN
    
This is especially useful as slack++ still hasn't gained multicompany support (and i'm not even sure it will ever gain that),  
so you can simply start 2 or more instances of slack++, with every company token.  
You can create an alias for each in .bashrc, something like:

alias company="./slack company_token"
