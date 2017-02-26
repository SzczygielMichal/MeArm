//---------------------------------------------------
//                   version.h
//---------------------------------------------------

// Plik testowy dla SubWCRev: Version_tmpl.h

#ifndef VERSION_H
  #define VERSION_H
  
#define REVISION	22
#define MODIFIED	"Yes"
#define DATE_REV	"16-03-2015"
#define TIME_REV	"16:05:34"
#define DATE_NOW	"23-03-2015"
#define TIME_NOW	"12:09:14"
#define RANGE		"5:22"
#define MMIXED		"Mixed revision WC"
#define URL			"svn://192.168.2.121/home/svn/KR-UNR1/trunk/Program"

struct sSoftInfo
{
	unsigned int BuildNumber;
	char RevisionDate[12];
	char RevisionTime[10];
	char CompilationDate[12];
	char CompilationTime[10];
};

extern struct sSoftInfo SoftInfo;

extern unsigned int Get_BuildNumberFromMainProg(void);
extern unsigned int GetBuildNumber();


//#if 1
//#error Source is modified
//#endif

  #endif // VERSION_H
