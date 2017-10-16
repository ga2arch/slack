# slack++
c++ slack ncurses client


![](https://github.com/ga2arch/slack/raw/master/slack++.png)


To build you need:

* jsoncpp
* cmake
* openssl
* libcurl
* ncursesw

**Optional**:

* libnotify

and a fairly recent c++ compiler.  
Note that cmake is required only at build time.  
It was tested on Archlinux and Mac OS X 10.10.  
Archlinux user will find a proper PKGBUILD in Arch folder. I'm gonna push it to AUR by the way.

## How to build & run?

    $ mkdir build
    $ cd build
    $ cmake -G "Unix Makefiles" ../
    $ make
    $ ./slack++

You've got to export at least SLACK_TOKEN, eg: by adding at the end of .bashrc in your home:  

export SLACK_TOKEN=xxxxxxxxxxxxx

If you're interested in multiple companies, you can export $companyname_SLACK_TOKEN too.  
Then you can start slack by passing it your desired company name:  

    $ ./slack++ $companyname
    
This is specially useful as slack++ still hasn't gained multicompany support (and i'm not even sure it will ever gain that),  
so you can simply start 2 or more instances of slack++.  
You can create an alias for each in .bashrc, something like:  

export mycompany_SLACK_TOKEN=xxxxxxx  
alias company='./slack++ mycompany'  
  
Then you can even change shipped .desktop file (on linux) to start your desired company. Or, obviously, creating as many desktop file as you need for your companies.  

## How to create slack token:

* go to https://api.slack.com/docs/oauth-test-tokens
* select your desired company and create token for it
* you'll be asked your password, enter it then confirm
* you now have your token!

## Instructions:

* ESC to quit
* tab to change focus from Input mode to roster selection mode
* while in roster mode, enter to choose an user to chat with
* "m" (case insensitive) to mute current focused chat, "n" to mute all chats (muted chats will appear without bold)
* muted chats won't show any libnotify notification
* while in input mode, enter to send a message
* while in input mode, key_up to go back in history, key_down to come back
* enjoy

## Features:

* libnotify support
* unread counter
* mute single chat/mute all support (no libnotify notification)
* history support
* very low resource usage
* mouse support: on Roster: wheels to scroll up/down, left click to select an user, right click to mute it.
On Input mode: wheels to scroll up/down chat history, right click to change user to chat with.
* on linux, SIGTERM and SIGINT will be gracefully handled
* multi platform (osx and linux)
* small log file located in user home (.slack.logs), that will be resetted every time slack++ is started

**IT DOES NOT SUPPORT TERMINAL RESIZE**  
Given our data structure, and the effort that we should put to have proper terminal resize support, we won't implement it.  
We consider it useless for a ncurses client. If you're willing to, you can send us a pull request.

