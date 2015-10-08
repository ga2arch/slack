# slack++
c++ slack ncurses client

TODO:

- [x] Create a class for every pane
- [x] Send messagges to user
- [ ] properly format sent and received messages: https://api.slack.com/docs/formatting.
- [ ] Better text input handling with vim like mode to escape insertion mode into win selection mode
- [ ] Make everything terminal window size agnostic

MUST FIX (graphical part):

- [x] fix backspace in wgetch: it should remove latest input_str char and move back cursor
- [ ] key_left/right support in Input?
- [x] fix scrolling in Input win too
- [ ] fix in resize: how many lines must now be printed? If a message was broken on multiple lines before the resize, are we sure it is not broken on more lines after?
Or may be (if terminal window was enlarged) it now fits in one line only.
- [ ] scroll even Roster win if needed