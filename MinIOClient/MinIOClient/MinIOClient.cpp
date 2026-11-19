#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentials.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <random>
#include "MinIOManager.h"

std::string getEnvironmentVariable(const char* name)
{
    char* value = nullptr;
    size_t length = 0;

    if (_dupenv_s(&value, &length, name) != 0)
    {
        return {};
    }

    std::string result = value ? value : "";
    std::free(value);

    return result;
}
#include <optional>

struct MinIOConfiguration
{
    Aws::Auth::AWSCredentials credentials;
    Aws::Client::ClientConfiguration clientConfig;
};

std::optional<MinIOConfiguration> configureMinIO()
{
    const std::string accessKey = getEnvironmentVariable("MINIO_ACCESS_KEY");

    const std::string secretKey = getEnvironmentVariable("MINIO_SECRET_KEY");

    if (accessKey.empty() || secretKey.empty())
    {
        std::cerr << "Missing MinIO credentials.\n";
        return std::nullopt;
    }

    MinIOConfiguration configuration;

    configuration.credentials =Aws::Auth::AWSCredentials(accessKey, secretKey);

    configuration.clientConfig.region = "us-east-1";
    configuration.clientConfig.endpointOverride = "http://127.0.0.1:9000";
    configuration.clientConfig.scheme = Aws::Http::Scheme::HTTP;

    return configuration;
}

int main()
{
    Aws::SDKOptions options;
    Aws::InitAPI(options);

    int exitCode = 0;

    {
        auto configuration = configureMinIO();

        if (!configuration)
        {
            exitCode = 1;
        }
        else
        {
            MinIOManager manager(
                configuration->credentials,
                configuration->clientConfig
            );

            const std::string bucketName = "cpp-training-bucket";

            std::random_device rd;
            std::mt19937 generator(rd());
            std::uniform_int_distribution<int> distribution(1, 4);

            int operation = distribution(generator);
            bool success = false;

            switch (operation)
            {
            case 1:
                std::cout << "Creating bucket...\n";
                success = manager.createBucket(bucketName);
                break;

            case 2:
                std::cout << "Listing buckets...\n";
                success = manager.listBuckets();
                break;

            case 3:
            {
                std::string fileName;

                std::cout << "Enter file name to upload: ";
                std::cin >> fileName;

                const std::string uploadPath =
                    "D:\\MinIO\\TestFiles\\" + fileName;

                success = manager.uploadFile(bucketName, fileName, uploadPath);
                break;
            }

            case 4:
            {
                std::string fileName;

                std::cout << "Enter file name to download: ";
                std::cin >> fileName;

                const std::string downloadPath =
                    "D:\\MinIO\\TestFiles\\downloaded_" + fileName;

                success = manager.downloadFile(bucketName, fileName, downloadPath);
                break;
            }
            }

            if (!success)
            {
                exitCode = 1;
            }
        }
    }

    Aws::ShutdownAPI(options);
    return exitCode;
}
