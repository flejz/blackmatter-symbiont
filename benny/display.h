#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>

class Display
{
    public:
        Display(int width, int height, const std::string& title);
        virtual ~Display();

    protected:
    private:
        Display(const Display& other) {}
};

#endif
