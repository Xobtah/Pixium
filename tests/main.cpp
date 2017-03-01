//
// Created by xobtah on 27/02/17.
//

#include <iostream>

#include "../src/Window.hpp"

int main()
{
    Pixium::Window  win;

    win.On("d_Escape", [&]() { win.Stop(); });
    win.On("d_ButtonLeft", [&](int x, int y) {
        win.On("MouseMotion", [&](int x, int y) { win.Draw(x, y, Pixium::Window::Color(255, 0, 0)); });
    });
    win.On("u_ButtonLeft", [&](int x, int y) { win.RemoveListener("MouseMotion"); });
    while (win.IsRunning())
        win.Display();
    return (0);
}