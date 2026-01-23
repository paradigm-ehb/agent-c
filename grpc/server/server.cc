#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

/* needed for building the server */
#include <grpcpp/server_builder.h>

#include "grpc/grpc_include.h"
#include "base/base.h"
#include "grpc/generated/resources/v3/deviceresources.grpc.pb.h"
#include "grpc/generated/resources/v3/deviceresources.pb.h"

#include <memory>

class ResourcesServiceImpl final
    : public resources::v3::ResourcesService::Service
{
public:
  grpc::Status
  GetSystemResources(
    grpc::ServerContext                            *ctx,
    const resources::v3::GetSystemResourcesRequest *req,
    resources::v3::GetSystemResourcesResponse      *resp
  ) override;
};

grpc::Status
ResourcesServiceImpl::GetSystemResources(
  grpc::ServerContext                            *ctx,
  const resources::v3::GetSystemResourcesRequest *req,
  resources::v3::GetSystemResourcesResponse      *resp
)
{
  return get_system_resources_impl(
    ctx,
    const_cast<resources::v3::GetSystemResourcesRequest *>(req),
    resp
  );
}

local_internal void
grpc_server_intercept_logger(void)
{
  /*
    TODO(nasr): every grpc call contains meta data we can inspect
    based on this we can give better responses
   */
}

/*
 * grpc server setup
 */
local_internal void
grpc_server_setup(void)
{
  ResourcesServiceImpl resources_service;

  /*
    TODO(nasr): provide a way for defining this?
   */
  const char         *server_address = "0.0.0.0:5000";
  grpc::ServerBuilder sb;

  /*
    enabled for first initial check to see if a server is online or not
   */
  grpc::EnableDefaultHealthCheckService(true);

  grpc::reflection::InitProtoReflectionServerBuilderPlugin();

  sb.AddListeningPort(
    server_address,
    grpc::InsecureServerCredentials()
  );

  sb.RegisterService(&resources_service);

  printf("gRPC service registered: %s\n", resources::v3::ResourcesService::service_full_name());

  std::unique_ptr<grpc::Server> server(sb.BuildAndStart());

  server->Wait();
}
