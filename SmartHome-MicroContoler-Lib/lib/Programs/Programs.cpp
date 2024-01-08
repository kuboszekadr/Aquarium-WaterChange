#include "Programs.h"

bool Programs::Program::isActive()
{
    return _is_active;
}

void Programs::Program::deactivate()
{
    _is_active = false;
    
}

void Programs::Program::activate()
{
    _is_active = true;
}