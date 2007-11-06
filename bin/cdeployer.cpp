#include <rtt/os/main.h>
#include <rtt/RTT.hpp>
#include <deployment/DeploymentComponent.hpp>
#include <rtt/corba/ControlTaskServer.hpp>
#include <iostream>

using namespace std;

void usage(const char* name)
{
    cout << "usage: " << name << " [DeployerName] [--start config-file.xml] [ -- TAO options]"<<endl;
    cout << endl;
}

int ORO_main(int argc, char** argv)
{
    // deployer [DeployerName] [ -- TAO options ]
    char* name = 0;
    char* script = 0;
    int i = 1;
    while ( i < argc && std::string(argv[i]) != "--" ) {
        std::string arg = argv[i];
        if ( arg == "--help" ) {
            usage(argv[0]);
            return 0;
        } else
        if ( arg == "--start" ) {
            ++i;
            if ( i < argc ) {
                script = argv[i];
            } else {
                cerr << "Please specify a filename after --start." <<endl;
                usage(argv[0]);
                return 0;
            }
        } else
            if (name == 0)
                name = argv[i];
            else {
                cerr << "Please specify only one name for the Deployer." <<endl;
                usage(argv[0]);
                return 0;
            }
        ++i;
    }

    if (name == 0)
        name = "Deployer";
    OCL::DeploymentComponent dc( name );

    if (script)
        dc.kickStart( script );

    /**
     * If CORBA is enabled, export the DeploymentComponent
     * as CORBA server.
     */
    using namespace RTT::Corba;
    ControlTaskServer::InitOrb( argc, argv );

    ControlTaskServer::Create( &dc );

    ControlTaskServer::RunOrb();

    ControlTaskServer::ShutdownOrb();

    ControlTaskServer::DestroyOrb();

    return 0;
}