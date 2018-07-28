//
// Created by xobtah on 27/02/17.
//

#include <cstring>

#include "Window.hpp"

namespace ium
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
        _keyMap[SDLK_ESCAPE] = KeyCode::ESCAPE;
        _keyMap[SDLK_SPACE] = KeyCode::SPACE;
        _keyMap[SDLK_a] = KeyCode::a;
        _keyMap[SDLK_b] = KeyCode::b;
        _keyMap[SDLK_c] = KeyCode::c;
        _keyMap[SDLK_d] = KeyCode::d;
        _keyMap[SDLK_e] = KeyCode::e;
        _keyMap[SDLK_f] = KeyCode::f;
        _keyMap[SDLK_g] = KeyCode::g;
        _keyMap[SDLK_h] = KeyCode::h;
        _keyMap[SDLK_i] = KeyCode::i;
        _keyMap[SDLK_j] = KeyCode::j;
        _keyMap[SDLK_k] = KeyCode::k;
        _keyMap[SDLK_l] = KeyCode::l;
        _keyMap[SDLK_m] = KeyCode::m;
        _keyMap[SDLK_n] = KeyCode::n;
        _keyMap[SDLK_o] = KeyCode::o;
        _keyMap[SDLK_p] = KeyCode::p;
        _keyMap[SDLK_q] = KeyCode::q;
        _keyMap[SDLK_r] = KeyCode::r;
        _keyMap[SDLK_s] = KeyCode::s;
        _keyMap[SDLK_t] = KeyCode::t;
        _keyMap[SDLK_u] = KeyCode::u;
        _keyMap[SDLK_v] = KeyCode::v;
        _keyMap[SDLK_w] = KeyCode::w;
        _keyMap[SDLK_x] = KeyCode::x;
        _keyMap[SDLK_y] = KeyCode::y;
        _keyMap[SDLK_z] = KeyCode::z;
        _keyMap[SDLK_LEFT] = KeyCode::LEFT;
        _keyMap[SDLK_RIGHT] = KeyCode::RIGHT;
        _keyMap[SDLK_UP] = KeyCode::UP;
        _keyMap[SDLK_DOWN] = KeyCode::DOWN;
        _keyMap[SDL_BUTTON_LEFT] = KeyCode::MOUSELEFT;
        _keyMap[SDL_BUTTON_RIGHT] = KeyCode::MOUSERIGHT;
    }

    void    Window::HandleEvents()
    {
        while (SDL_PollEvent(&_eve))
            switch (_eve.type)
            {
                case SDL_QUIT:
                    this->Stop();
                    break;
                case SDL_KEYDOWN:
                    if (_keyMap.find(_eve.key.keysym.sym) != _keyMap.end())
                        this->OnKeydown(_keyMap[_eve.key.keysym.sym]);
                    break;
                case SDL_KEYUP:
                    if (_keyMap.find(_eve.key.keysym.sym) != _keyMap.end())
                        this->OnKeyup(_keyMap[_eve.key.keysym.sym]);
                    break;
                case SDL_MOUSEMOTION:
                    this->OnMouseMotion(_eve.motion.x, _eve.motion.y);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (_keyMap.find(_eve.button.button) != _keyMap.end())
                        this->OnMouseDown(_keyMap[_eve.button.button], _eve.button.x, _eve.button.y);
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (_keyMap.find(_eve.button.button) != _keyMap.end())
                        this->OnMouseUp(_keyMap[_eve.button.button], _eve.button.x, _eve.button.y);
                    break;
                default:
                    break;
            }
    }
}