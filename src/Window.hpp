//
// Created by xobtah on 27/02/17.
//

#ifndef PIXIUM_WINDOW_HPP
#define PIXIUM_WINDOW_HPP

#include <string>
#include <map>
#include <exception>
#include <SDL2/SDL.h>

#include "../Emitium/src/EventEmitter.hpp"
#include "../Poolium/src/Thread.hpp"

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
        Window(unsigned int sizeX = 640, unsigned int sizeY = 480, std::string title = "Untitled", int positionFlag = SDL_WINDOWPOS_CENTERED);
        ~Window();

        Window  &Display();
        Window  &Draw(unsigned int, unsigned int, uint32_t);
        Window  &Stop();
        Window  &SetMaxFps(unsigned int maxFps = 60);
        unsigned int    GetFps() const;

        bool    IsRunning() const;

        static uint32_t Color(uint8_t, uint8_t, uint8_t, uint8_t opacity = 255);

    private:
        SDL_Window      *_win;
        SDL_Renderer    *_ren;
        SDL_Texture     *_tex;
        SDL_Event       _eve;

        unsigned int    _winSizeX;
        unsigned int    _winSizeY;
        uint32_t        *_pixels;

        bool            _isRunning;
        Poolium::Thread _thread;
        std::map<int, std::string>  _keyMap;
        std::map<int, std::string>  _mouseMap;

        unsigned int    _maxFps;
        unsigned int    _lastTick;
        unsigned int    _fpsCounter;
        unsigned int    _fpsTimeCounter;
        unsigned int    _lastSecondFps;

        void    InitKeyMap();
        void    HandleEvents();
    };
}

#endif //PIXIUM_WINDOW_HPP
