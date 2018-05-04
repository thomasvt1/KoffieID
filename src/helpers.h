#pragma once

boolean arrayIncludeElement(int array[], int element)
{
    int max = sizeof(&array) - 1;

    for (int i = 0; i < max; i++)
    {
        if (array[i] == element)
            return true;
    }
    return false;
}