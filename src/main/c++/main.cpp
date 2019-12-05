#include <iostream>
#include <fstream>
#include <string>
#include <curl/curl.h>
#include <stdio.h>
#include <archive.h>
#include <archive_entry.h>
#include <windows.h>

std::ofstream log;

bool isFileExists (const std::string& name);

size_t writeData(void *ptr, size_t size, size_t nmemb, void *stream);

void downloadFile(const std::string& fileName, const std::string& url, CURL* curl);

int compareFiles(const std::string& file1, const std::string file2);

void runApp();

static void	errmsg(const char *);
static void	extract(const char *filename, int do_extract, int flags);
static void	fail(const char *, const char *, int);
static int	copy_data(struct archive *, struct archive *);

static void	usage(void);
static void	warn(const char *, const char *);

static int verbose = 0;

int main () {
    
    curl_global_init(CURL_GLOBAL_ALL);
    CURL *curl = curl_easy_init();
    log.open("retro-manager.log", std::ios::out | std::ios::trunc );
    
    
    log << "Checking java availability" << std::endl;
    if(!isFileExists("java/bin/java.exe")) {
        log << "Java not found, dowloading it..." << std::endl;
        downloadFile("java.tar.gz", "https://bitbucket.org/yildiz-engine-team/build-application-binaries/downloads/java_jre_win64.tar.gz", curl);
        log << "Java download complete." << std::endl;
        log << "Unpacking java.tar.gz..." << std::endl;
        //extract( "java.tar.gz", 1, 0);
        log << "Unpack java.tar.gz complete." << std::endl;
    } else {
        log << "Java found, checking version..." << std::endl;
        downloadFile("expected-release", "https://bitbucket.org/yildiz-engine-team/build-application-binaries/downloads/release_jre_win64", curl);  
        if(!compareFiles("java/release", "expected-release")) {
	    log << "Java version not matching, downloading the correct one..." << std::endl;
            downloadFile("java.tar.gz", "https://bitbucket.org/yildiz-engine-team/build-application-binaries/downloads/java_jre_win64.tar.gz", curl);
            log << "Java download complete." << std::endl;
            log << "Unpacking java.tar.gz..." << std::endl;
            //extract( "java.tar.gz", 1, 0);
            log << "Unpack java.tar.gz complete." << std::endl;
        } else { 
	    log << "echo Java version is correct." << std::endl;
	}
    }
    log << "Downloading last version of the launcher..." << std::endl;
    downloadFile("launcher.jar", "http://play50hz-data.yildiz-games.be/launcher.jar", curl);  
    log << "Download last version of the launcher complete." << std::endl;
    log <<  "Starting play50hz launcher..." << std::endl;
    runApp();
    
    return 0;
}

void runApp() {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    CONST wchar_t* commandLine = TEXT("-jar launcher.jar");

    CreateProcessW(
        L"java/bin/java.exe",      // app path
        (LPWSTR)commandLine,     // Command line 
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory
        &si,            // Pointer to STARTUPINFO structure
        &pi)           // Pointer to PROCESS_INFORMATION structure
        );
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

static void
extract(const char *filename, int do_extract, int flags)
{
	struct archive *a;
	struct archive *ext;
	struct archive_entry *entry;
	int r;

	a = archive_read_new();
	ext = archive_write_disk_new();
	archive_write_disk_set_options(ext, flags);

	archive_read_support_format_tar(a);

	if (filename != NULL && strcmp(filename, "-") == 0)
		filename = NULL;
	if ((r = archive_read_open_filename(a, filename, 10240)))
		fail("archive_read_open_filename()",
		    archive_error_string(a), r);
	for (;;) {
		r = archive_read_next_header(a, &entry);
		if (r == ARCHIVE_EOF)
			break;
		if (r != ARCHIVE_OK)
			fail("archive_read_next_header()",
			    archive_error_string(a), 1);
		if (verbose && do_extract)
			log << "x ";
		if (verbose || !do_extract)
			log << archive_entry_pathname(entry);
		if (do_extract) {
			r = archive_write_header(ext, entry);
			if (r != ARCHIVE_OK)
				warn("archive_write_header()",
				    archive_error_string(ext));
			else {
				copy_data(a, ext);
				r = archive_write_finish_entry(ext);
				if (r != ARCHIVE_OK)
					fail("archive_write_finish_entry()",
					    archive_error_string(ext), 1);
			}

		}
		if (verbose || !do_extract)
			log << std::endl;
	}
	archive_read_close(a);
	archive_read_free(a);
	
	archive_write_close(ext);
  	archive_write_free(ext);
	exit(0);
}

static int
copy_data(struct archive *ar, struct archive *aw)
{
	int r;
	const void *buff;
	size_t size;
#if ARCHIVE_VERSION_NUMBER >= 3000000
	int64_t offset;
#else
	off_t offset;
#endif

	for (;;) {
		r = archive_read_data_block(ar, &buff, &size, &offset);
		if (r == ARCHIVE_EOF)
			return (ARCHIVE_OK);
		if (r != ARCHIVE_OK)
			return (r);
		r = archive_write_data_block(aw, buff, size, offset);
		if (r != ARCHIVE_OK) {
			warn("archive_write_data_block()",
			    archive_error_string(aw));
			return (r);
		}
	}
}


static void
errmsg(const char *m)
{
	write(2, m, strlen(m));
}

static void
warn(const char *f, const char *m)
{
	errmsg(f);
	errmsg(" failed: ");
	errmsg(m);
	errmsg("\n");
}

static void
fail(const char *f, const char *m, int r)
{
	warn(f, m);
	exit(r);
}

static void
usage(void)
{
	const char *m = "Usage: untar [-tvx] [-f file] [file]\n";
	errmsg(m);
	exit(1);
}

int compareFiles(const std::string& file1, const std::string file2) {
    std::fstream f1, f2;
    char name[20], c1, c2;
    int flag=3;

    f1.open(file1, std::ios::in);
    f2.open(file2, std::ios::in);

    while(1){
        c1=f1.get();
        c2=f2.get();
        if(c1!=c2){
            flag=0;
            break;
        }
        if((c1==EOF)||(c2==EOF))
            break;
    }
    f1.close();
    f2.close();
    return flag;
}
