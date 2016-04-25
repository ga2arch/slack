TODO:

- [x] add channel support
- [x] when changing chat, save old chat history position (if scrolled back) and restore new chat position
- [x] drop curlpp in favour of libcurl
- [x] notification silenced per-chat and globally
- [x] libnotify optional
- [x] add timestamp to messages
- [x] owner's messages should not trigger libnotify notifications
- [x] add company name to UI
- [x] add new messages counter beside user name in ROster
- [x] send "message read" events to slack (to avoid N unread messages when entering in slack site)
- [x] support https://api.slack.com/methods/channels.mark and https://api.slack.com/methods/groups.mark
- [x] call mark before changing chat (need to pass SlackUI to Roster)
- [x] fixed circular array implementation
- [ ] add a "--help" cmdline switch
- [ ] cleanup code...private things/fix includes

MUST FIX (UI):

- [x] Make everything terminal window size agnostic
- [x] Mouse wheels to scroll up/down chat history
- [x] fix roster win...keep various info even when scrolling
- [x] scroll even Roster win if needed
- [x] fix roster win...
- [x] support del button in Input
- [x] scroll chat back (with up arrow).
- [x] proper unicode support
- [x] re-focus input after chat redraw
- [x] highlight with bold senders
- [x] key left/right support in input (wmove/std::string.insert + reprint)
- [x] remove Roster highlight when chat is scrolled down and the last message is read

CHAT_HISTORY:

- [x] fix include message sender in history too
- [x] move session.messages to a std::vector and insert history in it
- [x] update first_mesg_ts with every first mesg from history call
- [x] only ask for mesg since first session mesg timestamp
- [x] fix wclear at first mesg even if is a history one
- [x] fix scroll_forward...
- [x] initial polling of old messages (since "now")
- [x] fix chat draw...

INTERNAL REWORK:

- [x] ptr to active_win in ui->main_cycle
- [x] 2 fd: stdscr and a pipe.
- [x] stdscr events will be passed to active_win
- [x] on event, websocket will write a byte in the pipe, that will wake up
main thread. Main thread will call > client->process_data that will retrieve data from "wc.get_data()"
- [x] move websocket on main poll too?
- [x] using ppoll (linux only) to catch external sigterm and sigint

LONGTERM AIMS:

- [ ] multi companies support (is it needed? Probably won't implement)
- [ ] handle more things...
- [x] chat history call : https://api.slack.com/methods/im.history
- [x] properly check for connection errors in websocket
