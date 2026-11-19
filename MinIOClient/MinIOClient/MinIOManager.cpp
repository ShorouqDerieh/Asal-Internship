#include <aws/s3/model/ListBucketsRequest.h>
#include <aws/s3/model/HeadBucketRequest.h>
#include <aws/s3/model/CreateBucketRequest.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/core/utils/memory/stl/AWSStreamFwd.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <format>
#include "MinIOManager.h"

MinIOManager::MinIOManager(const Aws::Auth::AWSCredentials& credentials,
const Aws::Client::ClientConfiguration& config)
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
        std::cerr << std::format("ListBuckets failed: {}\n",outcome.GetError().GetMessage());
    }

    std::cout << "Buckets:\n";

    for (const auto& bucket : outcome.GetResult().GetBuckets())
    {
        std::cout << std::format("- {}\n", bucket.GetName());
    }

    return true;
}

bool MinIOManager::bucketExists(std::string_view bucketName) const
{
    Aws::S3::Model::HeadBucketRequest request;
    request.SetBucket(std::string(bucketName));
    auto outcome = client.HeadBucket(request);
    return outcome.IsSuccess();
}

bool MinIOManager::createBucket(std::string_view bucketName) const
{
    Aws::S3::Model::HeadBucketRequest headRequest;
    headRequest.SetBucket(std::string(bucketName));

    auto headOutcome = client.HeadBucket(headRequest);

    if (headOutcome.IsSuccess())
    {
        std::cerr << std::format("Cannot verify bucket: {}\n",headOutcome.GetError().GetMessage());
        return true;
    }

    if (headOutcome.GetError().GetResponseCode()!= Aws::Http::HttpResponseCode::NOT_FOUND)
    {
        std::cerr << std::format("Cannot verify bucket: {}\n", headOutcome.GetError().GetMessage());
        return false;
    }

    Aws::S3::Model::CreateBucketRequest request;
    request.SetBucket(bucketName);
    auto outcome = client.CreateBucket(request);

    if (!outcome.IsSuccess())
    {
        std::cerr << std::format("CreateBucket failed: {}\n", outcome.GetError().GetMessage());
        return false;
    }

    std::cout << std::format("Bucket created: {}\n", bucketName);
    return true;
}

bool MinIOManager::uploadFile(const std::string& bucketName,
const std::string& objectKey,
const std::string& filePath) const
{
    auto file = Aws::MakeShared<Aws::FStream>("MinIOUpload",
    filePath.c_str(),
    std::ios::in | std::ios::binary
    );

    if (!file->is_open())
    {
        std::cerr << std::format("Cannot open file: {}\n", filePath);
        return false;
    }

    Aws::S3::Model::PutObjectRequest request;
    request.SetBucket(bucketName);
    request.SetKey(objectKey);
    request.SetBody(file);
    auto outcome = client.PutObject(request);

    if (!outcome.IsSuccess())
    {
        std::cerr << std::format("PutObject failed: {}\n", outcome.GetError().GetMessage());
        return false;
    }

    std::cout << std::format("File uploaded: {}\n", objectKey);
    return true;
}

bool MinIOManager::downloadFile(std::string_view bucketName,
std::string_view objectKey,
std::string_view destinationPath) const
{
    Aws::S3::Model::GetObjectRequest request;
    request.SetBucket(std::string(bucketName));
    request.SetKey(std::string(objectKey));
    auto outcome = client.GetObject(request);

    if (!outcome.IsSuccess())
    {
        std::cerr << std::format("GetObject failed: {}\n", outcome.GetError().GetMessage());
        return false;
    }

    std::ofstream output(std::string(destinationPath),std::ios::binary);

    if (!output.is_open())
    {
        std::cerr << std::format("Cannot create file: {}\n", destinationPath);
        return false;
    }

    output << outcome.GetResult().GetBody().rdbuf();
    output.flush();

    if (!output.good())
    {
        std::cerr << std::format("Failed to write downloaded file: {}\n", destinationPath);
        return false;
    }

    std::cout << std::format("File downloaded: {}\n", destinationPath);
    return true;
}
