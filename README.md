# vk_api
Простая надстройка над libcurl для упрощённого взаимодействия с апишкой вконтакта. Умеет вызывать методы с произвольными параметрами в query. Больше она ничего не умеет. Для использования подключите `vk_api.h`. Так же следует предварительно собрать libcurl, так как он тянется как зависимость. Далее собирать всегда с флагом `-lcurl`.

# Usage
```c
#include "vk_api.h"
```

Создать массив параметров:
```c
// pair -- простая пара byte_str* key и value.
pair params[3] = {
    *(mpair("access_token", "token")),
    *(mpair("group_id", "123")),
    *(mapir("v", API_VER))
};
```

Вызвать метод:
```c
// 3 -- кол-во элементов (пар) массива параметров.
byte_str_ptr response = call_method("messages.send", params, 3);
printf("%s\n", response->data); // byte_str_ptr - указатель на byte_str.
