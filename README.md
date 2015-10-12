# slack++
c++ slack ncurses client

TODO:

- [x] Create a class for every pane
- [x] Send messagges to user
- [x] delta and chat_line (chat.hpp) per-chat
- [x] start program with focus on roster win, to choose user to chat with
- [x] Add groups support
- [ ] add channel support
- [x] split every message in (COLS - 24) substr. Print chat message in chat_context_switch() from the bottom.
- [ ] properly format received messages: https://api.slack.com/docs/formatting.
- [ ] get accented chars too in input...
- [x] properly remove notification (if present) after the switch to a new chat
- [x] drop websocketpp(and boost) in favour of a lighter solution?
- [x] input_str per-chat
- [x] Better text input handling with vim like mode to escape insertion mode into win selection mode -> not vim mode still.
- [x] someway notify user about new messages in other chats
- [ ] ring buffer for messages with a limit of 1000
- [ ] multi companies support
- [ ] proper checks for connection errors in websocket
- [x] leave program if no socket connection could be established (time out 30seconds)

MUST FIX (UI):

- [x] fix backspace in wgetch: it should remove latest input_str char and move back cursor
- [ ] key_left/right support in Input?
- [x] assure no messages is printed during startup (while connecting)
- [x] fix scrolling in Input win too
- [ ] Make everything terminal window size agnostic
- [ ] scroll even Roster win if needed
- [x] do not reprint user name in chat if last message was from the same user