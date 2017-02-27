//
// Created by xobtah on 27/02/17.
//

#include <iostream>

#include "../src/Window.hpp"

int main()
{
    Pixium::Window  win;
    bool            running = true;

    win.On("Quit", [&]() { running = false; }).On("Escape", [&]() { win.Emit("Quit"); });
    while (running)
        win.HandleEvents();
    return (0);
}