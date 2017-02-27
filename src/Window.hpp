//
// Created by xobtah on 27/02/17.
//

#ifndef PIXIUM_WINDOW_HPP
#define PIXIUM_WINDOW_HPP

#include <string>

#include <exception>
#include <SDL2/SDL.h>

#include "../Emitium/src/EventEmitter.hpp"

namespace Pixium
{
    class WindowException : public std::exception
    {
    public:
        WindowException(std::string what) throw() : _what(what) {}
        ~WindowException() {}

        virtual const char  *what() const throw() { return _what.c_str(); }

    private:
        std::string _what;
    };

    class   Window : public Emitium::EventEmitter
    {
    public:
        Window(int sizeX = 640, int sizeY = 480, std::string title = "Untitled", int positionFlag = SDL_WINDOWPOS_CENTERED);
        ~Window();

        Window  &HandleEvents();

    private:
        SDL_Window      *_win;
        SDL_Renderer    *_ren;
        SDL_Event       _eve;
    };
}

#endif //PIXIUM_WINDOW_HPP
