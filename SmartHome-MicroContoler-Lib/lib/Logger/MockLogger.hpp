#include <fakeit.hpp>
#include "Logger/Logger.h"

using namespace fakeit;

Mock<Logger> mockLogger;

When(Method(mockLogger, log)).AlwaysDo([](const char *msg) {
    // You can put your test code here.
    // For example, you can check if the msg is what you expect.
});

When(Method(mockLogger, logf)).AlwaysDo([](const char *msg_format, ...) {
    // You can put your test code here.
    // For example, you can check if the msg_format is what you expect.
});

When(Method(mockLogger, _stream)).AlwaysDo([](const char *module_name, const char *log_level, const char *msg, const char *timestamp) {
    // You can put your test code here.
    // For example, you can check if the parameters are what you expect.
});

// Since addStream is a static method, we can't mock it directly.
// Instead, we can create a separate method to add a stream and mock that.
void addStreamMock(stream s) {
    Logger::addStream(s);
}
When(Method(mockLogger, addStream)).AlwaysDo([](stream s) {
    addStreamMock(s);
});