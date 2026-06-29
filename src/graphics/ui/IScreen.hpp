#pragma once

/*
Abstract class for game screen implementation e.g. loading screen, gameplay screen
*/

class IScreen {
public: 
    virtual ~IScreen() {}

    // called when switched to the screen
    virtual void on_enter () = 0;

    // called befor swithcing to another screen
    virtual void on_exit () = 0;

    // called with a fixed frequency 
    virtual void update () = 0;

    // called every frame 
    virtual void draw () = 0;
};