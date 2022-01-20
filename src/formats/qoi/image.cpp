#include "image.hpp"

// #include "SDL.h"
#include "chunks.hpp"
#include "decode_exception.hpp"

void QOI::Image::encode(OutputStream& out)
{
    header.encode(out);

    Color previously_seen_pixels[64];
    Color previous_color(0, 0, 0, 255);
    uint8_t run_count = 0;

    for (long i = 0; i < pixels.size(); ++i)
    {
        const Color& current_color = pixels[i];
        if (current_color == previous_color)
        {
            ++run_count;
            if (run_count == 62)
            {
                Run(run_count).encode(out);
                run_count = 0;
            }
        }
        else
        {
            if (run_count != 0)
            {
                Run(run_count).encode(out);
                run_count = 0;
            }

            uint8_t index = current_color.hash();
            if (previously_seen_pixels[index] == current_color)
            {
                Index(index).encode(out);
            }
            else if (current_color.alpha == previous_color.alpha)
            {
                int8_t diff_red = current_color.red - previous_color.red;
                int8_t diff_green = current_color.green - previous_color.green;
                int8_t diff_blue = current_color.blue - previous_color.blue;

                int8_t diff_red_green = diff_red - diff_green;
                int8_t diff_blue_green = diff_blue - diff_green;

                if (-2 <= diff_red && diff_red <= 1 &&
                    -2 <= diff_green && diff_green <= 1 &&
                    -2 <= diff_blue && diff_blue <= 1)
                {
                    Diff(diff_red, diff_green, diff_blue).encode(out);
                }
                else if (-8 <= diff_red_green && diff_red_green <= 7 &&
                         -32 <= diff_green && diff_green <= 31 &&
                         -8 <= diff_blue_green && diff_blue_green <= 7)
                {
                    Luma(diff_green, diff_red_green, diff_blue_green).encode(out);
                }
                else
                {
                    Pixel(current_color, Channels::RGB).encode(out);
                }
            }
            else
            {
                Pixel(current_color, Channels::RGBA).encode(out);
            }
            previously_seen_pixels[index] = current_color;
            previous_color = current_color;
        }
    }

    if (run_count != 0)
        Run(run_count).encode(out);

    Footer().encode(out);
}

void QOI::Image::decode(InputStream& in)
{
    long size = in.size() - 14 - 8;

    header.decode(in);

    pixels.clear();
    pixels.reserve(get_width() * get_height());

    Color previously_seen_pixels[64];
    Color previous_color(0, 0, 0, 255);

    long i = 0;
    while (i < size)
    {
        uint8_t tag_8 = in.peek_8();
        uint8_t tag_2 = (tag_8 & 0xc0);

        if (tag_8 == Pixel::RGB_TAG || tag_8 == Pixel::RGBA_TAG)
        {
            previous_color = Pixel(in).get_color();
            previously_seen_pixels[previous_color.hash()] = previous_color;
            pixels.push_back(previous_color);
            i += tag_8 == Pixel::RGB_TAG ? Pixel::RGB_SIZE : Pixel::RGBA_SIZE;
        }
        else if (tag_2 == Index::TAG)
        {
            uint8_t index = Index(in).get_index();
            previous_color = previously_seen_pixels[index];
            pixels.push_back(previous_color);
            i += Index::SIZE;
        }
        else if (tag_2 == Diff::TAG)
        {
            Diff diff(in);

            previous_color.red += diff.get_diff_red();
            previous_color.green += diff.get_diff_green();
            previous_color.blue += diff.get_diff_blue();

            previously_seen_pixels[previous_color.hash()] = previous_color;
            pixels.push_back(previous_color);
            i += Diff::SIZE;
        }
        else if (tag_2 == Luma::TAG)
        {
            Luma luma(in);

            previous_color.red += luma.get_diff_green() + luma.get_diff_red_green();
            previous_color.green += luma.get_diff_green();
            previous_color.blue += luma.get_diff_green() + luma.get_diff_blue_green();

            previously_seen_pixels[previous_color.hash()] = previous_color;
            pixels.push_back(previous_color);
            i += Luma::SIZE;
        }
        else if (tag_2 == Run::TAG)
        {
            uint8_t repeat = Run(in).get_run();
            for (uint8_t j = 0; j < repeat; ++j)
                pixels.push_back(previous_color);
            i += Run::SIZE;
        }
        else
        {
            throw DecodeException("Tag not recognized.");
        }
    }
}

// void QOI::Image::show()
// {
//     SDL_Init(SDL_INIT_VIDEO);
//     SDL_Window* window = SDL_CreateWindow(
//         "Visualizer",
//         SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
//         (int)width, (int)height, SDL_WINDOW_SHOWN);
//
//     SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
//
//     SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//     SDL_RenderClear(renderer);
//
//     for (int i = 0; i < pixels.size(); ++i)
//     {
//         SDL_SetRenderDrawColor(renderer, pixels[i].red, pixels[i].green, pixels[i].blue, pixels[i].alpha);
//         SDL_RenderDrawPoint(renderer, i % width + 1, i / width + 1);
//     }
//
//     SDL_RenderPresent(renderer);
//
//     SDL_Event event;
//     bool exit = false;
//     while (!exit)
//         while (SDL_PollEvent(&event))
//             if (event.type == SDL_QUIT) exit = true;
//
//     SDL_DestroyWindow(window);
//     SDL_Quit();
// }
