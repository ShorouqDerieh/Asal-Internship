
#include "MinIOManager.h"

#include <aws/s3/model/ListBucketsRequest.h>
#include <aws/s3/model/HeadBucketRequest.h>
#include <aws/s3/model/CreateBucketRequest.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/core/utils/memory/stl/AWSStreamFwd.h>

#include <fstream>
#include <iostream>
#include <memory>

MinIOManager::MinIOManager(
    const Aws::Auth::AWSCredentials& credentials,
    const Aws::Client::ClientConfiguration& config
)
    : client(
        credentials,
        config,
        Aws::Client::AWSAuthV4Signer::PayloadSigningPolicy::Never,
        false
    )
{
}

bool MinIOManager::listBuckets() const
{
    Aws::S3::Model::ListBucketsRequest request;
    auto outcome = client.ListBuckets(request);

    if (!outcome.IsSuccess())
    {
        std::cerr << "ListBuckets failed: "
            << outcome.GetError().GetMessage() << '\n';
        return false;
    }

    std::cout << "Buckets:\n";

    for (const auto& bucket : outcome.GetResult().GetBuckets())
    {
        std::cout << "- " << bucket.GetName() << '\n';
    }

    return true;
}

bool MinIOManager::bucketExists(
    const std::string& bucketName
) const
{
    Aws::S3::Model::HeadBucketRequest request;
    request.SetBucket(bucketName);

    auto outcome = client.HeadBucket(request);

    return outcome.IsSuccess();
}

bool MinIOManager::createBucket(
    const std::string& bucketName
) const
{
    Aws::S3::Model::HeadBucketRequest headRequest;
    headRequest.SetBucket(bucketName);

    auto headOutcome = client.HeadBucket(headRequest);

    if (headOutcome.IsSuccess())
    {
        std::cout << "Bucket already exists and is accessible: "
            << bucketName << '\n';
        return true;
    }

    if (headOutcome.GetError().GetResponseCode()
        != Aws::Http::HttpResponseCode::NOT_FOUND)
    {
        std::cerr << "Cannot verify bucket: "
            << headOutcome.GetError().GetMessage() << '\n';
        return false;
    }

    Aws::S3::Model::CreateBucketRequest request;
    request.SetBucket(bucketName);

    auto outcome = client.CreateBucket(request);

    if (!outcome.IsSuccess())
    {
        std::cerr << "CreateBucket failed: "
            << outcome.GetError().GetMessage() << '\n';
        return false;
    }

    std::cout << "Bucket created: " << bucketName << '\n';
    return true;
}

bool MinIOManager::uploadFile(
    const std::string& bucketName,
    const std::string& objectKey,
    const std::string& filePath
) const
{
    auto file = Aws::MakeShared<Aws::FStream>(
        "MinIOUpload",
        filePath.c_str(),
        std::ios::in | std::ios::binary
    );

    if (!file->is_open())
    {
        std::cerr << "Cannot open file: " << filePath << '\n';
        return false;
    }

    Aws::S3::Model::PutObjectRequest request;
    request.SetBucket(bucketName);
    request.SetKey(objectKey);
    request.SetBody(file);

    auto outcome = client.PutObject(request);

    if (!outcome.IsSuccess())
    {
        std::cerr << "PutObject failed: "
            << outcome.GetError().GetMessage() << '\n';
        return false;
    }

    std::cout << "File uploaded: " << objectKey << '\n';
    return true;
}

bool MinIOManager::downloadFile(
    const std::string& bucketName,
    const std::string& objectKey,
    const std::string& destinationPath
) const
{
    Aws::S3::Model::GetObjectRequest request;
    request.SetBucket(bucketName);
    request.SetKey(objectKey);

    auto outcome = client.GetObject(request);

    if (!outcome.IsSuccess())
    {
        std::cerr << "GetObject failed: "
            << outcome.GetError().GetMessage() << '\n';
        return false;
    }

    std::ofstream output(
        destinationPath,
        std::ios::binary
    );

    if (!output.is_open())
    {
        std::cerr << "Cannot create file: "
            << destinationPath << '\n';
        return false;
    }

    output << outcome.GetResult().GetBody().rdbuf();
    output.flush();

    if (!output.good())
    {
        std::cerr << "Failed to write downloaded file.\n";
        return false;
    }

    std::cout << "File downloaded: "
        << destinationPath << '\n';
    return true;
}
