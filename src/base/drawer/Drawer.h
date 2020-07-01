//
// Created by Mugur Tomita Jr. on 16/03/2020.
// But there is code here, not written by me
//

#ifndef ENGINE_DRAWER_H
#define ENGINE_DRAWER_H

#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <algorithm>
#include "../../options/Path.h"
#include "../../math/BiVector.h"
#include "../../options/UIValues.h"
#include "../../support/Color.h"
#include "../../math/MeshTriangle.h"

TTF_Font *fontRobotoRegular;
TTF_Font *fontRobotoBlack;


TTF_Font *fontSpaceMonoRegular;
TTF_Font *fontSpaceMonoBold;

bool InitDrawer() {
    TTF_Init();

    fontRobotoRegular = TTF_OpenFont((PATH_RESOURCES + "fonts/roboto/Roboto-Regular.ttf").c_str(), 50);
    fontRobotoBlack = TTF_OpenFont((PATH_RESOURCES + "fonts/roboto/Roboto-Black.ttf").c_str(), 50);

    fontSpaceMonoRegular = TTF_OpenFont((PATH_RESOURCES + "fonts/spacemono/SpaceMono-Regular.ttf").c_str(), 50);
    fontSpaceMonoBold = TTF_OpenFont((PATH_RESOURCES + "fonts/spacemono/SpaceMono-Bold.ttf").c_str(), 50);

    return true;
}

BiVector worldportToViewport(const BiVector &point) {
    double x = ((double)point.x * ((double)USED_SCREEN_WIDTH) / ((double) WORLD_WIDTH));
    double y = ((double)USED_SCREEN_HEIGHT) - ((double)point.y * ((double)USED_SCREEN_HEIGHT) / ((double) WORLD_HEIGHT));
    return BiVector(x, y);
}

BiVector viewportToWorldport(const BiVector &point) {
    double x = ((double)point.x * ((double)WORLD_WIDTH) / ((double) USED_SCREEN_WIDTH));
    double y = ((double)WORLD_HEIGHT) - ((double)point.y * ((double) WORLD_HEIGHT) / ((double) USED_SCREEN_HEIGHT));
    return BiVector(x, y);
}

double viewportToWorldport_X(const int &p_x) {
    return fmax(fmin((((double)p_x - USED_WIDTH_OFFSET) * ((double)WORLD_WIDTH) / ((double) USED_SCREEN_WIDTH)), WORLD_WIDTH), 0);
}

double viewportToWorldport_Y(const int &p_y) {
    return fmax(fmin(((double)WORLD_HEIGHT) - (((double)p_y - USED_HEIGHT_OFFSET) * ((double) WORLD_HEIGHT) / ((double) USED_SCREEN_HEIGHT)), WORLD_HEIGHT), 0);
}

BiVector worldsizeToViewsize(const BiVector &point) {
    //return BiVector(point.x, point.y);
    double x = ((double)point.x * ((double)USED_SCREEN_WIDTH) / ((double) WORLD_WIDTH));
    double y = ((double)point.y * ((double)USED_SCREEN_HEIGHT) / ((double) WORLD_HEIGHT));
    return BiVector(x, y);
}

// TODO: OPTIMIZATION -> new BiVectors not necessary, operations on double
void drawLine(SDL_Renderer *renderer, const BiVector &start, const BiVector &end) {
    BiVector viewStart = worldportToViewport(start);
    BiVector viewEnd = worldportToViewport(end);

    SDL_RenderDrawLine(renderer, viewStart.intX(), viewStart.intY(), viewEnd.intX(), viewEnd.intY());
}

void drawLineCanvas(SDL_Renderer *renderer, const BiVector &start, const BiVector &end) {
    SDL_RenderDrawLine(renderer, start.intX()-1, start.intY()-1, end.intX()-1, end.intY()-1);
    SDL_RenderDrawLine(renderer, start.intX(), start.intY(), end.intX(), end.intY());
    SDL_RenderDrawLine(renderer, start.intX()+1, start.intY()+1, end.intX()+1, end.intY()+1);
}

void drawLine(SDL_Renderer *renderer, const BiVector &start, const BiVector &end, const Color &color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    drawLine(renderer, start, end);
}

void fillRectanglePixel(SDL_Renderer *renderer, const BiVector &start, const BiVector &dimensions) {
    BiVector viewStart = worldportToViewport(start);
    BiVector viewDimensions = worldsizeToViewsize(dimensions);
    SDL_Rect rect = {viewStart.intX(), viewStart.intY() - viewDimensions.intY(), viewDimensions.intX(), viewDimensions.intY()};
    //SDL_RenderDrawRect(renderer, &rect);
    if(rect.w > 0 && rect.h > 0) {
        SDL_Point *points = new SDL_Point[rect.w * rect.h];
        for (int i = rect.x; i < rect.x + rect.w; ++i) {
            for (int j = rect.y; j < rect.y + rect.h; ++j) {
                //SDL_RenderDrawPoint(renderer, i, j);
                int position = (i-rect.x) * rect.h + (j-rect.y);
                points[position].x = i;
                points[position].y = j;
            }
        }
        SDL_RenderDrawPoints(renderer, points, rect.w * rect.h);
        delete[] points;
    }
}

void drawRectangle(SDL_Renderer *renderer, const BiVector &start, const BiVector &dimensions) {
    BiVector viewStart = worldportToViewport(start);
    BiVector viewDimensions = worldsizeToViewsize(dimensions);

    SDL_Rect rect = {viewStart.intX(), viewStart.intY() - viewDimensions.intY(), viewDimensions.intX(), viewDimensions.intY()};
    SDL_RenderDrawRect(renderer, &rect);
}

void drawRectangle(SDL_Renderer *renderer, const BiVector &start, const BiVector &dimensions, const Color &color, int stroke) {
    if(color.a > 0) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
        double currentStroke = 0;

        while(currentStroke < stroke) {
            drawRectangle(renderer, start + currentStroke, dimensions - (currentStroke * 2));
            currentStroke += 1.0f;
        }
    }
}

void fillRectangle(SDL_Renderer *renderer, const BiVector &start, const BiVector &dimensions) {
    BiVector viewStart = worldportToViewport(start);
    BiVector viewDimensions = worldsizeToViewsize(dimensions);

    SDL_Rect rect = {viewStart.intX(), viewStart.intY() - viewDimensions.intY(), viewDimensions.intX(), viewDimensions.intY()};
    SDL_RenderFillRect(renderer, &rect);
}

void fillRectangle(SDL_Renderer *renderer, const BiVector &start, const BiVector &dimensions, const Color &color) {
    if(color.a > 0) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
        fillRectangle(renderer, start, dimensions);
        //fillRectanglePixel(renderer, start, dimensions);
    }
}

BiVector prepareText(SDL_Renderer *renderer, SDL_Texture *&texture, double size, std::string text, const Color &color) {
    if(texture != nullptr) {
        SDL_DestroyTexture(texture);
    }
    SDL_Surface* textSurface = TTF_RenderText_Solid(fontRobotoRegular, text.c_str(), color.toSDL());
    texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    BiVector newDimensions = BiVector(textSurface->w, textSurface->h);

    SDL_FreeSurface(textSurface);

    BiVector dimensions = BiVector(((double) newDimensions.x) * size / ((double) newDimensions.y), size);
    return dimensions;
}

void drawText(SDL_Renderer *renderer, SDL_Texture *texture, const BiVector &start, const BiVector &dimensions) {
    if(texture != nullptr) {
        BiVector viewStart = worldportToViewport(start);
        BiVector viewDimensions = worldsizeToViewsize(dimensions);

        SDL_Rect rect = {viewStart.intX(), viewStart.intY() - viewDimensions.intY(), viewDimensions.intX(), viewDimensions.intY()};

        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }
}

static void DrawLine(SDL_Renderer* renderer, int fromX, int fromY, int toX, int toY) {
    SDL_RenderDrawLine(renderer, fromX, fromY, toX, toY);
}


// NOT WRITTEN BY MUGUR TOMITA - I DON'T HAVE THE PATIENCE
void FillTriangleImportedVersion(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int x3, int y3)
{
    auto SWAP = [](int &x, int &y) { int t = x; x = y; y = t; };

    int t1x, t2x, y, minx, maxx, t1xp, t2xp;
    bool changed1 = false;
    bool changed2 = false;
    int signx1, signx2, dx1, dy1, dx2, dy2;
    int e1, e2;
    // Sort vertices
    if (y1>y2) { SWAP(y1, y2); SWAP(x1, x2); }
    if (y1>y3) { SWAP(y1, y3); SWAP(x1, x3); }
    if (y2>y3) { SWAP(y2, y3); SWAP(x2, x3); }

    t1x = t2x = x1; y = y1;   // Starting points
    dx1 = (int)(x2 - x1); if (dx1<0) { dx1 = -dx1; signx1 = -1; }
    else signx1 = 1;
    dy1 = (int)(y2 - y1);

    dx2 = (int)(x3 - x1); if (dx2<0) { dx2 = -dx2; signx2 = -1; }
    else signx2 = 1;
    dy2 = (int)(y3 - y1);

    if (dy1 > dx1) {   // swap values
        SWAP(dx1, dy1);
        changed1 = true;
    }
    if (dy2 > dx2) {   // swap values
        SWAP(dy2, dx2);
        changed2 = true;
    }

    e2 = (int)(dx2 >> 1);
    // Flat top, just process the second half
    if (y1 == y2) goto next;
    e1 = (int)(dx1 >> 1);

    for (int i = 0; i < dx1;) {
        t1xp = 0; t2xp = 0;
        if (t1x<t2x) { minx = t1x; maxx = t2x; }
        else { minx = t2x; maxx = t1x; }
        // process first line until y value is about to change
        while (i<dx1) {
            i++;
            e1 += dy1;
            while (e1 >= dx1) {
                e1 -= dx1;
                if (changed1) t1xp = signx1;//t1x += signx1;
                else          goto next1;
            }
            if (changed1) break;
            else t1x += signx1;
        }
        // Move line
        next1:
        // process second line until y value is about to change
        while (1) {
            e2 += dy2;
            while (e2 >= dx2) {
                e2 -= dx2;
                if (changed2) t2xp = signx2;//t2x += signx2;
                else          goto next2;
            }
            if (changed2)     break;
            else              t2x += signx2;
        }
        next2:
        if (minx>t1x) minx = t1x; if (minx>t2x) minx = t2x;
        if (maxx<t1x) maxx = t1x; if (maxx<t2x) maxx = t2x;
        DrawLine(renderer, minx, y, maxx, y);
        //drawline(minx, maxx, y);    // Draw line from min to max points found on the y
        // Now increase y
        if (!changed1) t1x += signx1;
        t1x += t1xp;
        if (!changed2) t2x += signx2;
        t2x += t2xp;
        y += 1;
        if (y == y2) break;

    }
    next:
    // Second half
    dx1 = (int)(x3 - x2); if (dx1<0) { dx1 = -dx1; signx1 = -1; }
    else signx1 = 1;
    dy1 = (int)(y3 - y2);
    t1x = x2;

    if (dy1 > dx1) {   // swap values
        SWAP(dy1, dx1);
        changed1 = true;
    }
    else changed1 = false;

    e1 = (int)(dx1 >> 1);

    for (int i = 0; i <= dx1; i++) {
        t1xp = 0; t2xp = 0;
        if (t1x<t2x) { minx = t1x; maxx = t2x; }
        else { minx = t2x; maxx = t1x; }
        // process first line until y value is about to change
        while (i<dx1) {
            e1 += dy1;
            while (e1 >= dx1) {
                e1 -= dx1;
                if (changed1) { t1xp = signx1; break; }//t1x += signx1;
                else          goto next3;
            }
            if (changed1) break;
            else                t1x += signx1;
            if (i<dx1) i++;
        }
        next3:
        // process second line until y value is about to change
        while (t2x != x3) {
            e2 += dy2;
            while (e2 >= dx2) {
                e2 -= dx2;
                if (changed2) t2xp = signx2;
                else          goto next4;
            }
            if (changed2)     break;
            else              t2x += signx2;
        }
        next4:

        if (minx>t1x) minx = t1x; if (minx>t2x) minx = t2x;
        if (maxx<t1x) maxx = t1x; if (maxx<t2x) maxx = t2x;
        DrawLine(renderer, minx, y, maxx, y);
        //drawline(minx, maxx, y);
        if (!changed1) t1x += signx1;
        t1x += t1xp;
        if (!changed2) t2x += signx2;
        t2x += t2xp;
        y += 1;
        if (y>y3) return;
    }
}
BiVector maxi = BiVector(-10000000, -10000000);
BiVector mini = BiVector(10000000, 10000000);
void drawTriangle(SDL_Renderer *renderer, const MeshTriangle &triangle, bool filled) {
    for(int i = 0; i < 3; ++ i) {
        if(triangle.point[i]->x > maxi.x) {
            maxi.x = triangle.point[i]->x;
        }
        if(triangle.point[i]->y > maxi.y) {
            maxi.y = triangle.point[i]->y;
        }

        if(triangle.point[i]->x < mini.x) {
            mini.x = triangle.point[i]->x;
        }
        if(triangle.point[i]->y < mini.y) {
            mini.y = triangle.point[i]->y;
        }
    }
    if(filled) {
        FillTriangleImportedVersion(renderer, triangle.point[0]->intX(), triangle.point[0]->intY(),
                                    triangle.point[1]->intX(), triangle.point[1]->intY(), triangle.point[2]->intX(),
                                    triangle.point[2]->intY());
    } else {
        DrawLine(renderer, triangle.point[0]->x, triangle.point[0]->y, triangle.point[1]->x, triangle.point[1]->y);
        DrawLine(renderer, triangle.point[0]->x, triangle.point[0]->y, triangle.point[2]->x, triangle.point[2]->y);
        DrawLine(renderer, triangle.point[1]->x, triangle.point[1]->y, triangle.point[2]->x, triangle.point[2]->y);
    }
}

#endif //ENGINE_DRAWER_H
