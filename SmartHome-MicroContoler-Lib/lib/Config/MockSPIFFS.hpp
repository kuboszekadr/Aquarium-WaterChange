#include <fakeit.hpp>

class SPIFFS {
public:
    virtual File open(const char* path, const char* mode) = 0;
};

Mock<SPIFFS> SPIFFS;

When(Method(SPIFFS, read)).AlwaysReturn(123);
When(Method(SPIFFS, open)).AlwaysReturn(mockFile.get());