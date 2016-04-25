# slack++
c++ slack ncurses client

To build you need:

* rapidjson
* cmake
* openssl
* libcurl

**Optional**:

* libnotify

and a fairly recent c++ compiler.
It was tested on Archlinux and Mac OS X 10.10.
Archlinux user will find a proper PKGBUILD in Arch folder.
I'm gonna push it to AUR by the way.

## How to build & run?

    $ mkdir build
    $ cd build
    $ cmake -G "Unix Makefiles" ../
    $ make
    $ ./slack

You've got to export SLACK_TOKEN, eg: by adding at the end of .bashrc in your home:  

export SLACK_TOKEN=xxxxxxxxxxxxx

or you can start slack by passing it a company token:

    $ ./slack COMPANY_TOKEN
    
This is especially useful as slack++ still hasn't gained multicompany support (and i'm not even sure it will ever gain that),  
so you can simply start 2 or more instances of slack++, with every company token.  
You can create an alias for each in .bashrc, something like:

alias company="./slack company_token"


## Instructions:

* ESC to quit
* tab to change focus from Input mode to roster selection mode
* while in roster mode, enter to choose an user to chat with
* "m" (case insensitive) to mute current focused chat, "n" to mute all chats (muted chats will appear without bold)
* muted chats won't show any libnotify notification
* while in input mode, enter to send a message
* while in input mode, key_up to go back in history, key_down to come back
* enjoy
