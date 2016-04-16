#pragma once

#include <iostream>
#include <cstdlib>
#include <thread>
#include <future>
#include <locale>
#include <codecvt>
#include <string>

// Rapidjson
#define RAPIDJSON_HAS_STDSTRING 1

#include <rapidjson/document.h>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "Log.hpp"
