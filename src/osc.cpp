#include "osc.hpp"
#include <cmath>
#include <algorithm>

float pi = 3.14159265358979323846;
void Osc::step() {
}

void Osc::start(int key)
{
}

void Osc::release()
{
    state = RELEASING;
    time = 0;
}
