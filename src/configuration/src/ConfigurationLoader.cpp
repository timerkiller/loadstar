#include "configuration/ConfigurationLoader.hpp"
#include <boost/program_options.hpp>
#include <cstdint>
#include <fstream>
#include <utility>
#include "configuration/ApplicationConfiguration.hpp"
#include "configuration/ConfigurationKeys.hpp"
#include "logger/logger.hpp"

namespace configuration
{
namespace
{
auto addStandAloneOptionsDescription(boost::program_options::options_description& description)
{
    using boost::program_options::value;
    description.add_options()(keys::amfAddress, value<std::string>(), "AMF address");
}

boost::program_options::options_description createProgramOptionsDescription()
{
    auto description = boost::program_options::options_description();
    using boost::program_options::value;
    using namespace keys;
    // clang-format off
    description.add_options()
        (mmePort, value<unsigned>()->default_value(36412), "MME port")
        (f1apPort, value<unsigned>()->default_value(38472), "F1AP SCTP port")
        (f1apPpid, value<unsigned>()->default_value(30162), "F1AP SCTP Protocol ppid (depends on ASN1 shadow spec version)")
        (x2apPort, value<unsigned>()->default_value(36422), "X2AP SCTP port")
        (x2apPpid, value<unsigned>()->default_value(30127), "X2AP SCTP Protocol ppid (depends on ASN1 shadow spec version)")
        (cpCellRndCommandAddress, value<std::string>(), "CP-Cell R&D Command Address")
        (cpUeRndCommandAddress, value<std::string>(), "CP-UE R&D Command Address")
        (cpIfRndCommandAddress, value<std::string>(), "CP-IF R&D Command Address")
        (cpNbRndCommandAddress, value<std::string>(), "CP-NB R&D Command Address")
        (cpUeAddress, value<std::string>()->required(), "ZMQ address of Cplane Ue instance")
        (cpCellConfAddress, value<std::string>()->required(), "ZMQ address of Cplane Cell configuration instance")
        (cpCellAddress, value<std::string>()->required(), "ZMQ address of Cplane Cell instance")
        (cpCellTfAddress, value<std::string>()->required(), "ZMQ address of Cplane TF Cell instance")
        (internalMmeAddress, value<std::string>()->required(), "ZMQ address of Internal CPlane MME instance")
        (internalX2Address, value<std::string>()->required(), "ZMQ address of Internal CPlane X2 Service instance")
        (cplaneIfF1apServiceAddress, value<std::string>()->required(), "ZMQ address of F1AP service address")
        (cplaneFsHLAPAdress, value<std::string>()->required(), "ZMQ address of FsHLAP gateway")
        (cplaneSCTPServiceAddress, value<std::string>()->required(), "ZMQ address of Cplane SCTP Service instance")
        (cplaneNbAddress,  value<std::string>()->required(), "ZMQ address of Cplane NB instance")
        (oamAddress, value<std::string>()->required(), "Syscom GW Address")
        (sysComProxyAddress, value<std::string>()->required(), "Syscom proxy Address")
        (enableNSAX2AP, value<bool>()->default_value(false), "R&D parameter for NSA X2 AP")
        (enableMMEService, value<bool>()->default_value(false), "R&D parameter for MME service")
        (enableL2HiInterfaceSplit, value<std::string>()->default_value("False"), "R&D parameter for L2_HI split")
        (f1apAddress, value<std::string>()->required(), "F1AP service IP address")
        (ueSubscriberAddress, value<std::string>()->required(), "ZMQ address of Internal CPlane MME UE instance")
        (x2apUeSubscriberAddress, value<std::string>()->required(), "ZMQ address of Internal CPlane X2 Service UE instance")
        (f1apUeSubscriberAddress, value<std::string>()->required(), "ZMQ address of Internal CPlane F1 Service UE instance")
        (cpCellF1DataAddress, value<std::string>()->required(), "ZMQ address of Internal CPlane F1 Service Cell instance")
        (cpIfNbAddress, value<std::string>()->required(), "cpIf and cpNb bindable address")
        (shutdownNoX2TrafficTmr, value<std::uint16_t>()->default_value(60), "NoX2TrafficTmr for NSA X2 AP")
        (x2SetupProcedureTmr, value<std::uint16_t>()->default_value(1), "X2SetupProcedureTmr for NSA X2 AP")
        (sctpSendBufferSize, value<unsigned>()->default_value(0), "R&D parameter for sctp send buffer size")
        (sctpBlockedAssociationResetTime, value<std::uint32_t>()->default_value(25000), "R&D parameter for sctp blocked association reset time")
        (supportedMaximumSctpAssociation, value<unsigned>()->default_value(136), "R&D parameter for supported maximum sctp association")
        (alarmClearTimeInterval, value<std::uint32_t>()->default_value(86400000), "When timeInterval arrived, clear related alarm")
        (databaseFile, value<std::string>()->required(), "RnD database file used for testing purpose")
        (cpCellFshlapAddress, value<std::string>()->required(), "ZMQ address of FsHLAP messages handler on CP-Cell")
        (pmProxyAddress, value<std::string>()->required(), "PM proxy address");
    // clang-format on

    addStandAloneOptionsDescription(description);

    for (int i = 0; i < 2; ++i)
    {
        description.add_options()(
            (configuration::keys::f1UAddressGroupPrefix + std::to_string(i)).c_str(),
            value<std::string>()->required(),
            "F1-U Address for dedicated group")(
            (configuration::keys::s1UAddressGroupPrefix + std::to_string(i)).c_str(),
            value<std::string>()->required(),
            "S1-U Address for dedicated group")(
            (configuration::keys::x2UAddressGroupPrefix + std::to_string(i)).c_str(),
            value<std::string>()->required(),
            "X2-U Address for dedicated group");
    }
    return description;
}
}

namespace
{
boost::program_options::variables_map parseProgramOptions(std::ifstream& boostOptionsStream)
{
    namespace po = boost::program_options;

    po::variables_map enbConfig;
    po::store(po::parse_config_file(boostOptionsStream, createProgramOptionsDescription()), enbConfig);
    try
    {
        po::notify(enbConfig);
    }
    catch (po::error& e)
    {
        LOG_ERROR_MSG("Parsing the config file failed: {}", e.what());
        throw;
    }
    return enbConfig;
}
}

ApplicationConfiguration loadFromIniFile(const std::string& configFilename)
{
    std::ifstream configFileStream{configFilename};
    ApplicationConfiguration configuration{{parseProgramOptions(configFileStream)}};

    return configuration;
}
}
