#include <iostream>
#include <fstream>
#include <string>
#include <curl/curl.h>
#include <stdio.h>
#include <archive.h>
#include <archive_entry.h>

bool isFileExists (const std::string& name);

size_t writeData(void *ptr, size_t size, size_t nmemb, void *stream);

void downloadFile(const std::string& fileName, const std::string& url, CURL* curl);

int main () {
    std::ofstream log;
    curl_global_init(CURL_GLOBAL_ALL);
    CURL *curl = curl_easy_init();
    log.open("retro-manager.log", std::ios::out | std::ios::trunc );
    
    
    log << "Checking java availability" << std::endl;
    if(!isFileExists("java/bin/java.exe")) {
        log << "Java not found, dowloading it..." << std::endl;
        downloadFile("java.tar.gz", "https://bitbucket.org/yildiz-engine-team/build-application-binaries/downloads/java_jre_win64.tar.gz", curl);
        log << "Java download complete." << std::endl;
        log << "Unpacking java.tar.gz..." << std::endl;
        //@tar -zxvf java.tar.gz
        
        struct archive *a;
	struct archive_entry *entry;
	a = archive_read_new();
	int r;
        archive_read_support_filter_gzip(a);
        archive_read_support_format_raw(a);
	r = archive_read_open_filename(a, "java.tar.gz", 10240);
        if (r != ARCHIVE_OK) {
		
	}
        while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
            printf("%s\\n",archive_entry_pathname(entry));
            archive_read_data_skip(a);  // Note 2
        }
        r = archive_read_free(a);  // Note 3
        if (r != ARCHIVE_OK) {
	
	}
        
        log << "Unpack java.tar.gz complete." << std::endl;
    } else {
        log << "Java found, checking version..." << std::endl;
        downloadFile("expected-release", "https://bitbucket.org/yildiz-engine-team/build-application-binaries/downloads/release_jre_win64", curl);  
       //@fc /b java\release expected-release > nul
       //if errorlevel 1 (
        //echo Java version not matching, downloading the correct one... >> launch-retro.log
        //@RD /S /Q "java\"
        //@curl.exe -s -Lo java.tar.gz https://bitbucket.org/yildiz-engine-team/build-application-binaries/downloads/java_jre_win64.tar.gz
        //echo Java download complete. >> launch-retro.log
        //echo Unpacking java.tar.gz... >> launch-retro.log
        //@tar -zxvf java.tar.gz
        //echo Unpack java.tar.gz complete. >> launch-retro.log
    //) else (
        //log << "echo Java version is correct." << std::endl;
    }
//)
log << "Downloading last version of the launcher..." << std::endl;
//@curl.exe -s -Lo launcher.jar http://play50hz-data.yildiz-games.be/launcher.jar
log << "Download last version of the launcher complete." << std::endl;
log <<  "Starting play50hz launcher..." << std::endl;
//@%~dp0java/bin/java.exe -jar launcher.jar
    
    return 0;
}

void downloadFile(const std::string& fileName, const std::string& url, CURL* curl) {
    FILE* file;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    file = fopen(fileName.c_str(), "wb");
    if(file) {
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
        curl_easy_perform(curl);
        fclose(file);
    }   
}

inline bool isFileExists (const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }   
}

size_t writeData(void *ptr, size_t size, size_t nmemb, void *stream) {
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}
