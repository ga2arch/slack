TODO:

- [ ] add channel support
- [x] ring buffer for messages with a limit of 1000
- [x] when changing chat, save old chat history position (if scrolled back) and restore new chat position
- [ ] properly check for connection errors in websocket
- [x] drop curlpp in favour of libcurl
- [ ] notification silenced per-chat or globally
- [x] libnotify optional
- [x] add timestamp to messages
- [x] owner's messages should not trigger libnotify notifications
- [ ] cleanup code...private things etc etc

MUST FIX (UI):

- [ ] Make everything terminal window size agnostic
- [ ] scroll even Roster win if needed
- [x] scroll chat back (with up arrow).
- [x] proper unicode support
- [x] re-focus input after chat redraw
- [x] highlight with bold senders
- [x] key left/right support in input (wmove/std::string.insert + reprint)
- [x] remove Roster highlight when chat is scrolled down and the last message is read

LONGTERM AIMS:

- [ ] multi companies support
- [ ] handle more things...(eg: inline code, tags...)
- [ ] chat history call
