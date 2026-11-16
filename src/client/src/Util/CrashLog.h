#pragma once

/// On a crash, writes crash.txt in the game folder: the exception, and the
/// stack as module+offset, which the PDBs turn into source lines
/// (llvm-symbolizer --obj=TRose.exe 0x140000000+offset). For development.
void InstallCrashLog();

/// For development: appends a line to client.log in the game folder (the
/// Release build has no LogString). printf-style.
void DevLog(const char* fmt, ...);
