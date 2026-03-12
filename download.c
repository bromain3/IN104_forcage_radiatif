#include <stdio.h>
#include <curl/curl.h>

void check_curl_init(CURL *curl) {
    // if the pointer *curl == 0 (NULL pointer), it means that
    // the CURL object is not pointing to any valid location, so that it is not defined
    if (!curl) {
        // printing in the standard error pipe (terminal by default) that the curl init failed
        fprintf(stderr, "CURL init failed, the library may be unavailable\n");
    }
}

int check_file_init(FILE *file, CURL *curl) {
    // if the pointer *file == 0 (NULL pointer), it means that
    // the CURL object is not pointing to any valid location, so that it is not defined
    if (!file) {
        // printing in the standard error pipe (terminal by default) that the file init failed
        fprintf(stderr, "FILE init failed\n");
        // cleaning the internet connection
        curl_easy_cleanup(curl);
        // returning a flag to know what happened during the check
        return 1;
    }
    return 0;
}

void check_download_success(CURLcode res) {
    // checking if the curl return code is equal the the CURL builtin variable CURLE_OK
    if(res != CURLE_OK) {
        // if not, print in the standard error pipe the returned error
        fprintf(stderr, "curl error: %s\n", curl_easy_strerror(res));
    } else {
        printf("Download successfull\n");
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

    // Curl initialisation
    CURL *curl = curl_easy_init();
    check_curl_init(curl);

    // Output file init
    FILE *file = fopen(url, "w");
    int retcode = check_file_init(file, curl);
    if (retcode) {return 1;};

    // Curl request setup
    curl_easy_setopt(curl, CURLOPT_URL, url);  // setting request URL
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);  // setting non default stdout (setting output file)

    // Making the request
    CURLcode res = curl_easy_perform(curl);
    check_download_success(res);

    // closing all created objects for preventing conflicts whene using internet or opening the file
    fclose(file);
    curl_easy_cleanup(curl);
    return 0;

}
