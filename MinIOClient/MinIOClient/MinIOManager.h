#pragma once

#include <aws/core/auth/AWSCredentials.h>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/s3/S3Client.h>

#include <string>

class MinIOManager
{
private:
    Aws::S3::S3Client client;

public:
    MinIOManager(
        const Aws::Auth::AWSCredentials& credentials,
        const Aws::Client::ClientConfiguration& config
    );

    bool listBuckets() const;

    bool bucketExists(std::string_view bucketName) const;

    bool createBucket(std::string_view bucketName) const;

    bool uploadFile(std::string_view bucketName,
    std::string_view objectKey,
    std::string_view filePath) const;

    bool downloadFile(std::string_view bucketName,
    std::string_view objectKey,
    std::string_view destinationPath) const;
};
