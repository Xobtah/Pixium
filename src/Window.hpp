//
// Created by xobtah on 27/02/17.
//

#ifndef PIXIUM_WINDOW_HPP
#define PIXIUM_WINDOW_HPP

#include <string>
#include <map>
#include <exception>
#include <SDL2/SDL.h>

#include "../Poolium/src/Thread.hpp"

namespace ium
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

    enum KeyCode
    {
        ESCAPE, SPACE,
        a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z,
        LEFT, RIGHT, UP, DOWN,
        MOUSELEFT, MOUSERIGHT
    };

    class   Window
    {
    public:
        Window(unsigned int sizeX = 640, unsigned int sizeY = 480, std::string title = "Untitled", int positionFlag = SDL_WINDOWPOS_CENTERED);
        ~Window();

        Window &Display();
        Window &Draw(unsigned int, unsigned int, uint32_t);
        Window &Stop();

        Window &SetMaxFps(unsigned int maxFps = 60);
        unsigned int GetFps() const;

        bool IsRunning() const;

        static uint32_t Color(uint8_t, uint8_t, uint8_t, uint8_t opacity = 255);

    protected:
        virtual void OnKeydown(KeyCode kc) {}
        virtual void OnKeyup(KeyCode kc) {}
    
        virtual void OnMouseMotion(int x, int y) {}
        virtual void OnMouseDown(KeyCode kc, int x, int y) {}
        virtual void OnMouseUp(KeyCode kc, int x, int y) {}

        SDL_Window      *_win;
        SDL_Renderer    *_ren;
        SDL_Texture     *_tex;
        SDL_Event       _eve;

        unsigned int    _winSizeX;
        unsigned int    _winSizeY;
        uint32_t        *_pixels;

        bool            _isRunning;
        Poolium::Thread _thread;
        std::map<int, KeyCode>  _keyMap;
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
