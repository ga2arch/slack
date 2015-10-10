# slack++
c++ slack ncurses client

TODO:

- [x] Create a class for every pane
- [x] Send messagges to user
- [x] delta and chat_line (chat.hpp) per-chat
- [x] start program with focus on roster win, to choose user to chat with
- [x] Add groups support
- [ ] add channel support
- [ ] split every message in (COLS - 24) substr. Print chat message in chat_context_switch() from the bottom.
- [ ] properly format sent and received messages: https://api.slack.com/docs/formatting.
- [ ] drop websocketpp(and boost) in favour of a lighter solution?
- [x] Better text input handling with vim like mode to escape insertion mode into win selection mode -> not vim mode still.
- [ ] play sound when income message / someway notify user about new messages in other chats

MUST FIX (UI):

- [x] fix backspace in wgetch: it should remove latest input_str char and move back cursor
- [ ] key_left/right support in Input?
- [x] fix scrolling in Input win too
- [ ] Make everything terminal window size agnostic
- [ ] scroll even Roster win if needed