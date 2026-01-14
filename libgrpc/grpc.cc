/* ========================================================================
   $File: grpc.c $
   $Date: 2026-01-14 $
   $Revision: $
   $Author: nsrddyn@gmail.com $
   $Notice: (C) Copyright 2026 $
   ======================================================================== */

#include <cstdio>
#include <grpcpp/grpcpp.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>

void
run_server()
{
  const char *server_address = "0.0.0.0:5000";

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();

  grpc::ServerBuilder builder;
  builder.AddListeningPort(
    server_address,
    grpc::InsecureServerCredentials());

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

  std::printf("running");

  server->Wait();
}
