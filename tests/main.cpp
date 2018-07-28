//
// Created by xobtah on 27/02/17.
//

#include <iostream>

#include "../src/Window.hpp"

class TestWindow : public ium::Window
{
public:
    TestWindow() : _isMouseDown(false) { std::cout << "Hello World! :)" << std::endl; }
    ~TestWindow() { std::cout << "Bye bye! :)" << std::endl; }

private:
    void OnKeydown(ium::KeyCode kc)
    {
        if (kc == ium::KeyCode::ESCAPE)
            this->Stop();
    }

    void OnMouseDown(ium::KeyCode kc, int x, int y)
    {
        if (kc == ium::KeyCode::MOUSELEFT)
            _isMouseDown = true;
    }

    void OnMouseUp(ium::KeyCode kc, int x, int y)
    {
        if (kc == ium::KeyCode::MOUSELEFT)
            _isMouseDown = false;
    }

    void OnMouseMotion(int x, int y)
    {
        if (_isMouseDown)
            this->Draw(x, y, ium::Window::Color(255, 0, 0));
    }

private:
    bool _isMouseDown;
};

int main()
{
    TestWindow win;

    while (win.IsRunning())
        win.Display();
    return (0);
}