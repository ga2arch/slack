TODO:

- [ ] add channel support
- [x] ring buffer for messages with a limit of 1000
- [x] when changing chat, save old chat history position (if scrolled back) and restore new chat position
- [ ] properly check for connection errors in websocket
- [x] drop curlpp in favour of libcurl
- [ ] notification silenced per-chat and globally
- [x] libnotify optional
- [x] add timestamp to messages
- [x] owner's messages should not trigger libnotify notifications
- [x] add company name to UI
- [x] add new messages counter beside user name in ROster
- [ ] send "message read" events to slack (to avoid N unread messages when entering in slack site)
- [ ] support https://api.slack.com/methods/channels.mark and https://api.slack.com/methods/groups.mark
- [ ] cleanup code...private things etc etc

MUST FIX (UI):

- [ ] Make everything terminal window size agnostic
- [ ] scroll even Roster win if needed
- [x] support del button in Input
- [x] scroll chat back (with up arrow).
- [x] proper unicode support
- [x] re-focus input after chat redraw
- [x] highlight with bold senders
- [x] key left/right support in input (wmove/std::string.insert + reprint)
- [x] remove Roster highlight when chat is scrolled down and the last message is read

LONGTERM AIMS:

- [ ] multi companies support (is it needed?)
- [ ] handle more things...(eg: inline code, tags...)
- [ ] chat history call : https://api.slack.com/methods/im.history
