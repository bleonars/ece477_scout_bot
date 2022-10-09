#include "rfmgr_srv.h"

using namespace ScoutBot_Server;

void RFManager_Server::setup(std::string_view server_name) {
    BLEDevice::init(server_name.data());
    
    m_server = BLEDevice::createServer();
    m_server->setCallbacks(&m_server_callback_obj);
    
    m_adv_obj = BLEDevice::getAdvertising();
}

RFManager_Service *RFManager_Server::add_service(BLEUUID serv_uuid) {
    RFManager_Service *created_service = new RFManager_Service;

    created_service->setup(m_server, serv_uuid);
    m_services.insert({serv_uuid.toString(), created_service});
    
    m_adv_obj->addServiceUUID(serv_uuid);

#ifdef SERVER_DBG
    g_log_mgr.println("dbg: added service to host server");
    g_log_mgr.print("dbg: search param:\n\tservice uuid: ");
    g_log_mgr.println(serv_uuid.toString());
#endif

    return m_services.at(serv_uuid.toString());
}

void RFManager_Server::adv_start() {
#ifdef SERVER_DBG
    g_log_mgr.println("dbg: (re)starting advertising");
#endif
    for (auto &service : m_services)
        service.second->get()->start();

    /* set GATT appearance */
    // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.gap.appearance.xml
    m_adv_obj->setScanResponse(true);
    m_adv_obj->setMinPreferred(0x6);
    m_adv_obj->setMinPreferred(0x12);

    BLEDevice::startAdvertising();
}

void RFManager_Server::adv_stop() {
    BLEDevice::stopAdvertising();
#ifdef SERVER_DBG
    g_log_mgr.println("dbg: stopped advertising");
#endif
}

RFManager_Service *RFManager_Server::get_service(BLEUUID serv_uuid) {
    return m_services.at(serv_uuid.toString());
}

void RFManager_Service::setup(BLEServer *server, BLEUUID serv_uuid) {
    m_service = server->createService(serv_uuid);
}

BLEService *RFManager_Service::get() {
    return m_service;
}

BLECharacteristic *RFManager_Service::add_characteristic(BLEUUID char_uuid, peer_char_attributes_t char_attributes) {
    /**
         * @brief layout of peer_char_attributes_t
         * bit 0: read?
         * bit 1: write?
         * bit 2: notify?
         * bit 3: broadcast?
         * bit 4: indicate?
         * bit 5: write no response?
     */
    m_characteristics.insert({char_uuid.toString(), m_service->createCharacteristic(char_uuid, char_attributes)});
    return m_characteristics.at(char_uuid.toString());
}

BLECharacteristic *RFManager_Service::get_characteristic(BLEUUID char_uuid) {
    return m_characteristics.at(char_uuid.toString());
}

void RFManager_Server_Callbacks::onConnect(BLEServer *server) {
#ifdef SERVER_DBG
    g_log_mgr.println("dbg: paired with client, connection count: " + std::to_string(server->getConnectedCount()));
#endif
} 

void RFManager_Server_Callbacks::onDisconnect(BLEServer *server) {
#ifdef SERVER_DBG
    g_log_mgr.println("dbg: paired with client, connection count: " + std::to_string(server->getConnectedCount()));
#endif
} 
