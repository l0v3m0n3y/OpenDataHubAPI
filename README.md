# OpenDataHubAPI
api for opendatahub.com site provides access to environmental monitoring data, including key parameters such as humidity, temperature, and carbon dioxide levels, from selected areas within NOI Techpark Bolzano and Brunico
# main
```cpp
#include "OpenDataHubAPI.h"
#include <iostream>

int main() {
   OpenDataHubAPI api;
    auto categories = api.get_categories().then([](json::value result) {
        std::cout << result<< std::endl;
    });
    categories.wait();
    
    return 0;
}
```

# Launch (your script)
```
g++ -std=c++11 -o main main.cpp -lcpprest -lssl -lcrypto -lpthread -lboost_system -lboost_chrono -lboost_thread
./main
```
