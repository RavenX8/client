#ifndef __SYSINFO_H__
#define __SYSINFO_H__

#include <map>
#include <string>
#include "dispinfo.h"
#include "systeminfo.h"

static LPSTINFO m_system;
static DPINFO m_display1;

class CSysInfo
{
private:
	std::map<std::string, std::string> m_mapCPU;
	std::map<std::string, std::string> m_mapOS;
	std::map<std::string, std::string> m_mapVideo;
	std::map<std::string, std::string> m_mapDisc;
	std::map<std::string, std::string> m_mapMem;
	std::vector<DisplayInfo*> m_DisplayInfo;
	LPSTINFO  m_SystemInfo;

	// HWND m_hWnd;
private:
	void GetOSInfo (void);
	void GetCpuInfo (void);
	void GetVideoInfo (void);
	void GetDiskInfo (void); // 
	void GetMemInfo (void); // 
	
	
public:
	CSysInfo ();
	~CSysInfo ();
	std::string GetAllInfo (void);
	std::string MakeWebString (void);
    bool GetDXInfo();

};

#endif