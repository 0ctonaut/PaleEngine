#pragma once
#include <string>
#include "PaleRenderer/Core/PaleRendererExport.h"
namespace PaleRdr
{
    struct PALE_API SCompName
    {
        SCompName(const std::string& name) : _Name(name) {};
        std::string _Name;
    };
}