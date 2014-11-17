// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "Log.h"
#include "MSG.h"
#include "EWORK.h"

#include <stdio.h>
#include <memory>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <list>

#include "c:\\Frameworks\mpi.h"

#ifdef _DEBUG
#define LOG(type, message) Log::getInstance().info(type, message);
#define LOG_LN() Log::getInstance().ln();
#else
#define LOG(type, message)
#define LOG_LN()
#endif

