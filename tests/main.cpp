//
// Created by xobtah on 27/02/17.
//

#include <iostream>

#include "../src/Window.hpp"

int main()
{
    Pixium::Window  win;

    win.On("Escape", [&]() { win.Stop(); });
    for (int x = 0; x < 640; x++)
        for (int y = 0; y < 480; y++)
            win.Draw(x, y, Pixium::Window::Color(255, 0, 0));
    while (win.IsRunning())
        win.Display();
    return (0);
}