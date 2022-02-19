/*
    Simple libcurl based library to interract with vk api methods.
        @ default api version = 5.130.

    Main interaface function -- call_method(const char*, pair*, size_t);
        @ where first argument = method name,
        @       second argument = array of params: (key, value) pairs.
        @       third argument = length of array (unline size in bytes, it's length).

    
    Simple usage example can be found in usage_example.c;
    To use this library you should get the libcurl lib first.
        @ compile with -lcurl flag.
*/

#ifndef VK_API_H
#include "vk_api.h"
#endif


// --------- base string reslisation;

// -* constructor;

byte_str_ptr init_empty_byte_str()
{
    byte_str_ptr base = (byte_str_ptr)malloc(sizeof(struct byte_str));
    base->data = NULL;
    base->size = 0;
    return base;
}

// ---------- base string interface;

void byte_str_destructor(byte_str_ptr target)
{
    free(target->data);
    free(target);
}

void byte_str_append(byte_str_ptr current, const char* data)
{
    current->data = (char*)realloc(current->data,
                                   current->size + strlen(data) + 1);
    memcpy(&(current->data[current->size]), data, strlen(data));

    current->size                += strlen(data);
    current->data[current->size] = 0;
}

// -* will also call destructor for tail arg.
void byte_str_merge(byte_str_ptr dest, byte_str_ptr tail)
{
    dest->data = (char*)realloc(dest->data,
                                dest->size + tail->size + 1);
    memcpy(&(dest->data[dest->size]), tail->data, tail->size);

    dest->size             += tail->size;
    dest->data[dest->size] = 0;
    
    byte_str_destructor(tail);
}

// --------- request params realisation;

pair_ptr mpair(const char* key, const char* value)
{
    byte_str_ptr byte_key = init_empty_byte_str(),
                 byte_val = init_empty_byte_str();

    byte_str_append(byte_key, key);
    byte_str_append(byte_val, value);

    pair_ptr _pair = (pair_ptr)malloc(sizeof(pair));
    _pair->key = byte_key;
    _pair->value = byte_val;
}

byte_str_ptr form_query(pair_ptr p_arr, size_t size)
{
    byte_str_ptr q_link = init_empty_byte_str();
    byte_str_append(q_link, "?");

    for (int i = 0; i < size; ++i) {
        byte_str_append(q_link, p_arr[i].key->data);
        byte_str_append(q_link, "=");
        byte_str_append(q_link, p_arr[i].value->data);

        if (i+1 != size)
            byte_str_append(q_link, "&");
    }

    return q_link;
}

// --------- curl realisation;

static size_t write_callback(char* data, size_t size, size_t bytes, void* uresp)
{
    byte_str_ptr mem = (byte_str_ptr)uresp;
    char* data_null = (char*)malloc(size * bytes + 1);
    memcpy(data_null, data, size * bytes);
    data_null[size * bytes] = 0;

    byte_str_append(mem, data_null);
    return size * bytes;
}

static byte_str_ptr send_request(const char* request_url)
{
    CURL* curl = curl_easy_init();
    if (!curl) {
        perror("Failed to initialize curl.");
        return NULL;
    }

    byte_str_ptr response = init_empty_byte_str();

    curl_easy_setopt(curl, CURLOPT_URL,
                           request_url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
                           write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA,
                           response);

    curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return response;
}

// ------- vk-api interface;

byte_str_ptr call_method(const char* method, pair_ptr params, size_t size)
{
    byte_str_ptr q_params = form_query(params, size);
    byte_str_ptr api_request = init_empty_byte_str();

    byte_str_append(api_request, base_api); // api.vk.com/method/
    byte_str_append(api_request, method); // api.vk.com/method/{method_name}
    byte_str_merge(api_request, q_params);

    byte_str_ptr response = send_request(api_request->data);

    byte_str_destructor(api_request);
    return response;
}

