#include <stdio.h>  // prints
#include <stdlib.h>  // using exit()
#include <curl/curl.h>  // using curl
#include <sys/stat.h>  // creating folder and having system infos

void check_curl_init(CURL *curl) {
    // if the pointer *curl == 0 (NULL pointer), it means that
    // the CURL object is not pointing to any valid location, so that it is not defined
    if (!curl) {
        // printing in the standard error pipe (terminal by default) that the curl init failed
        fprintf(stderr, "CURL init failed, the library may be unavailable\n");
        // stopping the code with exit code 1
        exit(1);
    }
}

void check_file_init(FILE *file, CURL *curl) {
    // if the pointer *file == 0 (NULL pointer), it means that
    // the CURL object is not pointing to any valid location, so that it is not defined
    if (!file) {
        // printing in the standard error pipe (terminal by default) that the file init failed
        fprintf(stderr, "FILE init failed\n");
        // cleaning the internet connection
        curl_easy_cleanup(curl);
        // stopping the code with exit code 1
        exit(1);
    }
}

void check_download_success(CURLcode res, CURL *curl) {
    // get http return code
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    if (http_code != 200) {  // 200 is the return code for saying that everything went well
        fprintf(stderr, "Erreur HTTP %ld\n", http_code);
        exit(1);
    }
}

int main(void) {

    // Variables definition (to be replaced by comande line args
    const char *specie = "CO2";
    const char *iso_ids_list = "1,2,3";
    unsigned int nu_min = 0, nu_max = 2100;

    // URL formating, snprintf copies formated url to rhe url var and prevents char url overflow
    char url[512]; // max size of the URL
    snprintf(url, sizeof(url), "https://hitran.org/lbl/api?iso_ids_list=%s&numin=%u&numax=%u", iso_ids_list, nu_min, nu_max);

    // saving path formating
    char save_path[512]; // max size of the saving path
    snprintf(save_path, sizeof(save_path), "data/bandes_%s.par", specie);

    // creating saving folder if it does not exist
    #ifdef _WIN32  // on a windows computer
        mkdir("data");
    #else  // on a linux computer
        mkdir("data", 0777);  // giving the right access
    #endif

    // Curl initialisation
    CURL *curl = curl_easy_init();
    check_curl_init(curl);

    // Output file init
    FILE *file = fopen(save_path, "w");
    check_file_init(file, curl);

    // Curl request setup
    curl_easy_setopt(curl, CURLOPT_URL, url);  // setting request URL
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);  // setting non default stdout (setting output file)
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);  // activating progressbar
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 600);  // global timout of 10 minutes
    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 30);  // timout set to 30 secondes if there is a really low volume of data
    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 1);  // the "low volume of data" threshold in byte/s

    // Making the request
    CURLcode res = curl_easy_perform(curl);
    check_download_success(res, curl);

    // closing all created objects for preventing conflicts whene using internet or opening the file
    fclose(file);
    curl_easy_cleanup(curl);
    return 0;

}
