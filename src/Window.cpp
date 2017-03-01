//
// Created by xobtah on 27/02/17.
//

#include <cstring>

#include "Window.hpp"

namespace Pixium
{
    /*
     *  Ctor & Dtor
     */

    Window::Window(unsigned int sizeX, unsigned int sizeY, std::string title, int positionFlag)
            : _win(NULL), _ren(NULL), _tex(NULL),
              _winSizeX(sizeX), _winSizeY(sizeY), _isRunning(true),
              _maxFps(60), _lastTick(SDL_GetTicks()), _fpsCounter(0), _fpsTimeCounter(0), _lastSecondFps(0)
    {
        if (SDL_Init(SDL_INIT_VIDEO))
            throw WindowException("SDL failed to init");
        if (!(_win = SDL_CreateWindow("Untitled", positionFlag, positionFlag, sizeX, sizeY, SDL_WINDOW_SHOWN)))
            throw WindowException("Failed to create window");
        if (!(_ren = SDL_CreateRenderer(_win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)))
            throw WindowException("Failed to create renderer");
        if (!(_tex = SDL_CreateTexture(_ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, _winSizeX, _winSizeY)))
            throw WindowException("Failed to create texture");
        _pixels = new uint32_t[_winSizeX * _winSizeY];
        std::memset(_pixels, 0, _winSizeX * _winSizeY * sizeof(uint32_t));
        this->InitKeyMap();
        _thread.Set([&]() { while (this->_isRunning) this->HandleEvents(); }).Run();
    }

    Window::~Window()
    {
        delete[] _pixels;
        SDL_DestroyTexture(_tex);
        SDL_DestroyRenderer(_ren);
        SDL_DestroyWindow(_win);
        SDL_Quit();
        _thread.Join().Free();
    }

    /*
     *  Public member functions
     */

    Window  &Window::Display()
    {
        if (SDL_GetTicks() - _fpsTimeCounter >= 1000)
        {
            _lastSecondFps = _fpsCounter;
            _fpsCounter = 0;
            _fpsTimeCounter = SDL_GetTicks();
        }
        if (SDL_GetTicks() - _lastTick >= 1000 / _maxFps)
        {
            _fpsCounter++;
            SDL_RenderClear(_ren);
            SDL_UpdateTexture(_tex, NULL, reinterpret_cast<void*>(_pixels), _winSizeX * sizeof(uint32_t));
            SDL_RenderCopy(_ren, _tex, NULL, NULL);
            SDL_RenderPresent(_ren);
            _lastTick = SDL_GetTicks();
        }
        return (*this);
    }

    Window  &Window::Draw(unsigned int posX, unsigned int posY, uint32_t color)
    {
        if (posX >= _winSizeX || posY >= _winSizeY)
            throw WindowException("Pixel position out of range");
        _pixels[posX + posY * _winSizeX] = color;
        return (*this);
    }

    uint32_t    Window::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t opacity)
    {
        uint32_t    color = opacity;

        color <<= 8;
        color += red;
        color <<= 8;
        color += green;
        color <<= 8;
        color += blue;
        return (color);
    }

    Window  &Window::Stop()
    {
        _isRunning = false;
        return (*this);
    }

    bool    Window::IsRunning() const { return (_isRunning); }

    Window  &Window::SetMaxFps(unsigned int maxFps)
    {
        _maxFps = maxFps;
        return (*this);
    }

    unsigned int    Window::GetFps() const { return (_lastSecondFps); }

    /*
     *  Private member functions
     */

    void    Window::InitKeyMap()
    {
        _keyMap[SDLK_ESCAPE] = "Escape";
        _keyMap[SDLK_SPACE] = "Space";
        _keyMap[SDLK_z] = "a";
        _keyMap[SDLK_z] = "b";
        _keyMap[SDLK_z] = "c";
        _keyMap[SDLK_z] = "d";
        _keyMap[SDLK_z] = "e";
        _keyMap[SDLK_z] = "f";
        _keyMap[SDLK_z] = "g";
        _keyMap[SDLK_z] = "h";
        _keyMap[SDLK_z] = "i";
        _keyMap[SDLK_z] = "j";
        _keyMap[SDLK_z] = "k";
        _keyMap[SDLK_z] = "l";
        _keyMap[SDLK_z] = "m";
        _keyMap[SDLK_z] = "n";
        _keyMap[SDLK_z] = "o";
        _keyMap[SDLK_z] = "p";
        _keyMap[SDLK_z] = "q";
        _keyMap[SDLK_z] = "r";
        _keyMap[SDLK_z] = "s";
        _keyMap[SDLK_z] = "t";
        _keyMap[SDLK_z] = "u";
        _keyMap[SDLK_z] = "v";
        _keyMap[SDLK_z] = "w";
        _keyMap[SDLK_z] = "x";
        _keyMap[SDLK_z] = "y";
        _keyMap[SDLK_z] = "z";
        _keyMap[SDL_BUTTON_LEFT] = "ButtonLeft";
        _keyMap[SDL_BUTTON_RIGHT] = "ButtonRight";
    }

    void    Window::HandleEvents()
    {
        while (SDL_PollEvent(&_eve))
            switch (_eve.type)
            {
                case SDL_QUIT:
                    this->Emit("Quit");
                    this->Stop();
                    break;
                case SDL_KEYDOWN:
                    this->Emit("Keydown");
                    if (_keyMap.find(_eve.key.keysym.sym) != _keyMap.end())
                        this->Emit("d_" + _keyMap[_eve.key.keysym.sym]);
                    break;
                case SDL_KEYUP:
                    this->Emit("Keyup");
                    if (_keyMap.find(_eve.key.keysym.sym) != _keyMap.end())
                        this->Emit("u_" + _keyMap[_eve.key.keysym.sym]);
                    break;
                case SDL_MOUSEMOTION:
                    this->Emit("MouseMotion", _eve.motion.x, _eve.motion.y);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    this->Emit("MouseButtonDown", _eve.motion.x, _eve.motion.y);
                    if (_keyMap.find(_eve.button.button) != _keyMap.end())
                        this->Emit("d_" + _keyMap[_eve.button.button], _eve.button.x, _eve.button.y);
                    break;
                case SDL_MOUSEBUTTONUP:
                    this->Emit("MouseButtonUp", _eve.motion.x, _eve.motion.y);
                    if (_keyMap.find(_eve.button.button) != _keyMap.end())
                        this->Emit("u_" + _keyMap[_eve.button.button], _eve.button.x, _eve.button.y);
                    break;
                default:
                    break;
            }
    }
}