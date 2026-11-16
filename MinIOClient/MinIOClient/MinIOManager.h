
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

    bool bucketExists(const std::string& bucketName) const;

    bool createBucket(const std::string& bucketName) const;

    bool uploadFile(
        const std::string& bucketName,
        const std::string& objectKey,
        const std::string& filePath
    ) const;

    bool downloadFile(
        const std::string& bucketName,
        const std::string& objectKey,
        const std::string& destinationPath
    ) const;
};
