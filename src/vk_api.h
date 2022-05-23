
#ifndef VK_API
#define VK_API
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

#define API_VER "5.130"

#define base_api "https://api.vk.com/method/"

// -* this is main string class used in this library.
// -* basic C_string with size and some interface.

typedef struct byte_str
{
    char*  data;
    size_t size;
}
byte_str;

typedef struct byte_str* byte_str_ptr;

// -* main pair class used in call_method() function;
// -* make single pair -- mpair(const char*, const char*);
//
//    you should use an array of those in call_method(), see usage_example.c;

typedef struct pair
{
    byte_str_ptr key, value;
}
pair;

typedef pair* pair_ptr;

// *------ base string interface:

byte_str_ptr init_empty_byte_str(); // constructor;
void byte_str_destructor(byte_str_ptr target); // destructor;

void byte_str_merge(byte_str_ptr dest, byte_str_ptr tail);

void byte_str_append(byte_str_ptr current, const char* data);

// *------ request params interface:
pair_ptr mpair(const char* key, const char* value); // constructor;

byte_str_ptr form_query(pair_ptr p_arr, size_t size); // vk-api query;

// *------ main lib interaface:

byte_str_ptr call_method(const char* method, pair_ptr params, size_t size);
#endif

