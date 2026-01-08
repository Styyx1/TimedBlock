#pragma once


#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>
#include <REX/REX/Singleton.h>
#include <REX/REX/TOML.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <styyx-utils.h>
#include <CLIBUtil/hotkeys.hpp>

using namespace std::literals;
namespace logger = SKSE::log;
using namespace StyyxUtil;



#ifdef SKYRIM_SUPPORT_AE
#	define REL_ID(se, ae) REL::ID(ae)
#	define OFFSET(se, ae) ae
#	define OFFSET_3(se, ae, vr) ae
#elif SKYRIM_SUPPORT_VR
#	define REL_ID(se, ae) REL::ID(se)
#	define OFFSET(se, ae) se
#	define OFFSET_3(se, ae, vr) vr
#else
#	define REL_ID(se, ae) REL::ID(se)
#	define OFFSET(se, ae) se
#	define OFFSET_3(se, ae, vr) se
#endif

#define DLLEXPORT __declspec(dllexport)