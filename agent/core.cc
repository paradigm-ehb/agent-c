#include <grpcpp/server_builder.h>
#include <unistd.h>

#include "agent_grpc/agent_grpc.h"

int
main(void)
{

  grpc_server_setup();
  return 0;
}
