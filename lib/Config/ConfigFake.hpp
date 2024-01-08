#include <fakeit.hpp>
#include "Config.h"

using namespace fakeit;

void setupMockConfig() {
    Mock<Config> mockConfig;

    When(Method(mockConfig, load)).AlwaysReturn(CONFIG_LOADED);
    When(Method(mockConfig, save)).AlwaysReturn(CONFIG_SAVED);
}