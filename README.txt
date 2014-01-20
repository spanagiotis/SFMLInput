SFML Input Extension
Code for the input extention to SFML.

The SFML Input extension replaces the need for the standard Keyboard/Mouse includes and the need to
have the window event loop.

The input class is a static class that just needs to be initialized with Input::Initialize(window) after a SFML window has been created. Here are it's features.

- Saves a keyinfo with the decimal, symbol, time pressed, pressed state

- Query the keyboard and find what key is being pressed and what is current state is (just pressed, just released, or being held).

- Can check for specific key up/down states.

- Query the mouse buttons for pressed, released, and held states.

- Keeps a history of all key presses up to a maximum count that can be specified in the define.

- Polls the window->pollEvent for window resize/close/etc.

