#include <thread>

#include <faabric/util/logging.h>

#include <faaslet/FaasletPool.h>

#include <faabric/endpoint/FaabricEndpoint.h>
#include <faabric/executor/FaabricMain.h>

using namespace faabric::executor;
using namespace faaslet;

int main()
{
    const std::shared_ptr<spdlog::logger>& logger = faabric::util::getLogger();

    // Determine maximum concurrency on this node, int cast because FaasletPool constructor
    // requires int. If cast overflows or the number of supported threads is undefined we
    // set them to 0 to suppor the default MPI world size.
    int nAvailableThreads = (int) std::thread::hardware_concurrency();
    if (nAvailableThreads <= 0) {
        nAvailableThreads = 5;
    }

    // Start the worker pool
    logger->info("Starting faaslet pool in the background");
    logger->info("Faaslet pool size is " + std::to_string(nAvailableThreads));
    FaasletPool p(nAvailableThreads);
    FaabricMain w(p);
    w.startBackground();

    // Start endpoint (will also have multiple threads)
    logger->info("Starting endpoint");
    faabric::endpoint::FaabricEndpoint endpoint;
    endpoint.start();

    logger->info("Shutting down endpoint");
    w.shutdown();

    return EXIT_SUCCESS;
}
