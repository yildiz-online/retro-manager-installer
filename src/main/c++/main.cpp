#include <iostream>
#include <fstream>
#include <string>
#include <curl/curl.h>
#include <stdio.h>

bool isFileExists (const std::string& name);

size_t writeData(void *ptr, size_t size, size_t nmemb, void *stream);

int main () {
    std::ofstream log;
    curl_global_init(CURL_GLOBAL_ALL);
    CURL *curl = curl_easy_init();
    log.open("retro-manager.log", std::ios::out | std::ios::trunc );
    
    
    log << "Checking java availability" << std::endl;
    if(!isFileExists("java/bin/java.exe")) {
        log << "Java not found, dowloading it..." << std::endl;
        FILE *javaFile;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
        curl_easy_setopt(curl, CURLOPT_URL, "https://bitbucket.org/yildiz-engine-team/build-application-binaries/downloads/java_jre_win64.tar.gz");
        javaFile = fopen("java.tar.gz", "wb");
        if(javaFile) {
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, javaFile);
            curl_easy_perform(curl);
            fclose(javaFile);
        }
        log << "Java download complete." << std::endl;
        log << "Unpacking java.tar.gz..." << std::endl;
        //@tar -zxvf java.tar.gz
        log << "Unpack java.tar.gz complete." << std::endl;
    } else {
        log << "Java found, checking version..." << std::endl;
        FILE *javaVersionFile;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
        curl_easy_setopt(curl, CURLOPT_URL, "https://bitbucket.org/yildiz-engine-team/build-application-binaries/downloads/release_jre_win64");
        javaVersionFile = fopen("expected-release", "wb");
        if(javaVersionFile) {
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, javaVersionFile);
            curl_easy_perform(curl);
            fclose(javaVersionFile);
        }    
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
