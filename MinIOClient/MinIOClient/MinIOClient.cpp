
#include "MinIOManager.h"

#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentials.h>

#include <cstdlib>
#include <iostream>
#include <string>

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

int main()
{
    Aws::SDKOptions options;
    Aws::InitAPI(options);

    int exitCode = 0;

    {
        const std::string accessKey =
            getEnvironmentVariable("MINIO_ACCESS_KEY");

        const std::string secretKey =
            getEnvironmentVariable("MINIO_SECRET_KEY");

        if (accessKey.empty() || secretKey.empty())
        {
            std::cerr << "Missing MinIO credentials.\n";
            exitCode = 1;
        }
        else
        {
            Aws::Client::ClientConfiguration config;
            config.region = "us-east-1";
            config.endpointOverride = "http://127.0.0.1:9000";
            config.scheme = Aws::Http::Scheme::HTTP;

            Aws::Auth::AWSCredentials credentials(
                accessKey,
                secretKey
            );

            MinIOManager manager(credentials, config);

            const std::string bucketName = "cpp-training-bucket";

            if (!manager.createBucket(bucketName))
            {
                exitCode = 1;
            }
            else
            {
                if (!manager.listBuckets())
                {
                    exitCode = 1;
                }

                const bool uploaded = manager.uploadFile(
                    bucketName,
                    "hello.txt",
                    "D:\\MinIO\\TestFiles\\hello.txt"
                );

                if (!uploaded)
                {
                    exitCode = 1;
                }
                else if (!manager.downloadFile(
                    bucketName,
                    "hello.txt",
                    "D:\\MinIO\\TestFiles\\downloaded_hello.txt"
                ))
                {
                    exitCode = 1;
                }
            }
        }
    }

    Aws::ShutdownAPI(options);
    return exitCode;
}
