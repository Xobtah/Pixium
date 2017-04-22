//
// Created by xobtah on 27/02/17.
//

#include <iostream>

#include "../src/Window.hpp"

int main()
{
    ium::Window win;

    win.On("Escape", [&](bool isDown) { if (isDown) win.Stop(); });
    win.On("ButtonLeft", [&](bool isDown, int x, int y) {
        if (isDown)
            win.On("MouseMotion", [&](int x, int y) { win.Draw(x, y, ium::Window::Color(255, 0, 0)); });
    });
    win.On("ButtonLeft", [&](bool isDown, int x, int y) { if (!isDown) win.RemoveListener("MouseMotion"); });
    while (win.IsRunning())
        win.Display();
    return (0);
}