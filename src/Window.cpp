//
// Created by xobtah on 27/02/17.
//

#include "Window.hpp"

namespace Pixium
{
    /*
     *  Ctor & Dtor
     */

    Window::Window(int sizeX, int sizeY, std::string title, int positionFlag) : _win(NULL), _ren(NULL)
    {
        if (SDL_Init(SDL_INIT_VIDEO))
            throw WindowException("SDL failed to init");
        if (!(_win = SDL_CreateWindow("Untitled", positionFlag, positionFlag, sizeX, sizeY, SDL_WINDOW_SHOWN)))
            throw WindowException("Failed to create window");
        if (!(_ren = SDL_CreateRenderer(_win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)))
            throw WindowException("Failed to create renderer");
    }

    Window::~Window()
    {
        SDL_DestroyWindow(_win);
        SDL_Quit();
    }

    /*
     *  Public member functions
     */

    Window  &Window::HandleEvents()
    {
        while (SDL_PollEvent(&_eve))
            switch (_eve.type)
            {
                case SDL_QUIT:
                    this->Emit("Quit");
                    break;
                case SDL_KEYDOWN:
                    this->Emit("Keydown");
                    switch (_eve.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            this->Emit("Escape");
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        return (*this);
    }
}