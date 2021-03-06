/***********************************************************************/
/*
** 2018 March 23
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
**    Author: taozuhong@gmail.com (Andy Tao)
**
*************************************************************************/

#include "voa_common.h"
#include <unistd.h>
#include <algorithm>

string GetCurrentTime(const string & fmt)
{
    time_t now = time(0);
    struct tm tstruct = {0};
    char szBuf[128]  = {0};
    tstruct = *localtime(&now);
    
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(szBuf, sizeof(szBuf), fmt.c_str(), &tstruct);
    
    return szBuf;
}

string GetFullPath(const string & localFile)
{
    string strFilePath = localFile;
    if (string::npos == strFilePath.find("/"))
    {
        char * pstrWorkDir = get_current_dir_name();        
        if (strFilePath.empty())
        {
            strFilePath = pstrWorkDir;
            strFilePath += "/";
            strFilePath += GetCurrentTime();
            strFilePath += ".tmp";
        }
        else
        {
            strFilePath = pstrWorkDir;
            strFilePath += "/";
            strFilePath = strFilePath + localFile;        
        }
        
        // free the system alloced memory
        free(pstrWorkDir);        
    }
    else if (strFilePath.length() - 1 == strFilePath.find_last_of("/"))
    {
         strFilePath += GetCurrentTime();
         strFilePath += ".tmp";
    }
    
    return strFilePath;
}

string GetFullPathFromUrl(const string & url)
{
    char * pstrWorkDir = get_current_dir_name(); 
    string strFilePath = pstrWorkDir;
    size_t start_index = url.find_last_of('/') + 1;
    size_t end_index = (string::npos == url.find('?')) ? url.length() : url.find('?');
    strFilePath += "/";
    strFilePath += url.substr(start_index, end_index - start_index);
    
    // free the system alloced memory
    free(pstrWorkDir);
    
    return strFilePath;    
}

string GetFullUrl(const string & relativeUrl, const string & fullUrl)
{
    string strTemp = relativeUrl;
    // std::transform(strTemp.begin(), strTemp.end(),strTemp.begin(), ::toupper);
    if(string::npos == strTemp.find("http"))
    {
        // https://learningenglish.voanews.com/z/1689
        strTemp = fullUrl.substr(0, fullUrl.find_first_of('/', 8));
        strTemp += relativeUrl;
    }

    return  strTemp;
}

bool IsUrlMatchPattern(const string & url, const string & extension, const string & filter)
{
    bool isMatch = true;
    size_t char_index = url.find('?');
    if (! extension.empty()) {
        isMatch = (string::npos == char_index) ? \
              (string::npos != url.find(extension)) : \
              (string::npos != url.rfind(extension, char_index));
    }

    if (isMatch && (!filter.empty())) {
        isMatch = isMatch && ((string::npos == char_index) ? \
                              (string::npos != url.find(filter)) : \
                              (string::npos != url.rfind(filter, char_index)));
    }

    return  isMatch;
}
