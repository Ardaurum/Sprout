#pragma once

#include <string>

static int strcicmp(char const* a, char const* b)
{
    for (;; a++, b++)
    {
        int distance = tolower((unsigned char)*a) - tolower((unsigned char)*b);
        if (distance != 0 || !*a)
        {
            return distance;
        }
    }
}