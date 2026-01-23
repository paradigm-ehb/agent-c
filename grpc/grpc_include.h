#ifndef GRPC_H
#define GRPC_H

#include "base/base.h"

#include <grpcpp/server_context.h>
#include <grpcpp/support/status.h>
#include "grpc/generated/resources/v3/deviceresources.pb.h"

/* server bootstrap */
local_internal void
grpc_server_setup(void);

local_internal grpc::Status
get_system_resources_impl(
  grpc::ServerContext* ctx,
  resources::v3::GetSystemResourcesRequest* req,
  resources::v3::GetSystemResourcesResponse* resp
);

#endif /* GRPC_H */
