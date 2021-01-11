#pragma once

#include <base/gh.h>
#include "CH_Base.h"
#include <base/list/ListDL.h>
#include <chrono>

namespace base {
namespace crashHandler {
class CH_Entries : public CH_Base {priv typedef CH_Base super;pub dCtor(CH_Entries);
    prot class CrashLogEntry {pub dCtor(CrashLogEntry);
        pub enum Type { Info, CriticalException };

        pub Type type;

        pub std::chrono::time_point<std::chrono::system_clock> timestamp;

        pub const char* fileName;
        pub int lineIndex;

        pub std::string msg;

        pub explicit CrashLogEntry(
            Type type, std::chrono::time_point<std::chrono::system_clock> timestamp,
            const char* fileName, int lineIndex, std::string& msg)
            : type(type), timestamp(timestamp), fileName(fileName), lineIndex(lineIndex), msg(msg)
        {
            //void
        }
    };

    prot std::string appVersion;
    prot std::chrono::time_point<std::chrono::system_clock> startTimestamp;

    prot ListDL<CrashLogEntry> entriesList{};

    pub explicit CH_Entries();

    pub void pushEntry_Info(const char* fileName, int lineIndex, std::string msg) final;
    pub void pushEntry_CriticalException(const char* fileName, int lineIndex, std::string msg) final;

    pub void create() override;

    pub ~CH_Entries() override;
};
};
};
