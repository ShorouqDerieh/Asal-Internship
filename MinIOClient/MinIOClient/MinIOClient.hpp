#pragma once

#include <aws/core/auth/AWSCredentials.h>
#include <aws/core/client/ClientConfiguration.h>

#include <optional>
#include <string>

struct MinIOConfiguration
{
    Aws::Auth::AWSCredentials credentials;
    Aws::Client::ClientConfiguration clientConfig;
};

std::string getEnvironmentVariable(const char* name);

std::optional<MinIOConfiguration> configureMinIO();
