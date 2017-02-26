//---------------------------------------------------
//                   version.h
//---------------------------------------------------

// Plik testowy dla SubWCRev: Version_tmpl.h

#ifndef VERSION_H
  #define VERSION_H
  
#define REVISION	$WCREV$
#define MODIFIED	"$WCMODS?Yes:Not$"
#define DATE_REV	"$WCDATE=%d-%m-%Y$"
#define TIME_REV	"$WCDATE=%H:%M:%S$"
#define DATE_NOW	"$WCNOW=%d-%m-%Y$"
#define TIME_NOW	"$WCNOW=%H:%M:%S$"
#define RANGE		"$WCRANGE$"
#define MMIXED		"$WCMIXED?Mixed revision WC:Not mixed$"
#define URL			"$WCURL$"

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


//#if $WCMODS?1:0$
//#error Source is modified
//#endif

  #endif // VERSION_H
