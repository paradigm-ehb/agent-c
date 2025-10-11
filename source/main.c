#include <pthread.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <lib.h>
#include <stdlib.h>
#include <stdio.h>

cJSON convert_to_json(info info) {

  cJSON *root = cJSON_CreateObject();
  cJSON_AddStringToObject(root, "cpu", info.cpu->name);
  cJSON_AddNumberToObject(root, "ram", info.ram->total);
  cJSON_AddStringToObject(root, "disk", info.disk->name);
  cJSON_AddStringToObject(root, "device", info.device->name);

  return *root;
}

void handler(char *url, cJSON object) {
  CURL *curl;
  CURLcode res;

  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();

  if (curl) {
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, object);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
          curl_easy_strerror(res));

    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
}

void setup_mt() {
  pthread_t cpu_t,ram_t; // disk_t , device_t;

  pthread_create(&cpu_t, NULL, cpu_i, NULL);
  pthread_create(&ram_t, NULL, ram_i, NULL);
  //    pthread_create(&disk_t, NULL, disk_i, NULL);
  //    pthread_create(&device_t, NULL, device_i, NULL);

  pthread_join(cpu_t, NULL);
  pthread_join(ram_t, NULL);
  //   pthread_join(disk_t, NULL);
  //   pthread_join(device_t, NULL);
}

#ifdef __APPLE__

int main(int argc, char** argv) {

  setup_mt();
  handler("api.nsrddyn.com");

  return 0;
}

#endif

#ifdef __gnu_linux



int main(int argc, char** argv) {

  setup_mt();

  return 0;
}

#endif

