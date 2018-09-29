
#pragma once

#include "ElementApp.h"

/** Representation of a version number */
struct Version
{
    Version();
    ~Version();
    
    /** Split a version string into an array of segments */
    static StringArray segments (const String& versionString);
    
    /** Converts version segments into an integer, good for version comparison */
    static int asHexInteger (const String& versionString);
};

class CurrentVersion : private Timer,
                       public DeletedAtShutdown,
                       public Thread
{
public:
    CurrentVersion();
    ~CurrentVersion();
    
    /** Delay version check. Handles presenting AlertWindow */
    static void checkAfterDelay (const int milliseconds, const bool showUpToDate = false);
    
    /** Returns true if a newer version is available for download */
    bool isNewerVersionAvailable();
    
    void run() override;
    
private:
    String permalink, version;
    bool hasChecked;
    bool shouldShowUpToDateMessage;
    bool result = false;
    int timeout = 0;
    
    friend class Timer;
    void timerCallback() override;
};