#include "rfmgr_cl.h"

using namespace ScoutBot_Client;

static BLEUUID             service_uuid;
static BLEAdvertisedDevice service_device;
static bool                service_found;

void RFManager_Client::setup(std::string_view client_name) {
    BLEDevice::init(client_name.data());

    m_client = BLEDevice::createClient();
    m_client->setClientCallbacks(&m_client_callback_obj);

    m_adv_scan_obj = BLEDevice::getScan();
    m_adv_scan_obj->setAdvertisedDeviceCallbacks(&m_adv_callback_obj);
}

void RFManager_Client::adv_scan_start(BLEUUID serv_uuid, bool active_scan, uint16_t scan_interval, uint16_t scan_window, uint32_t scan_duration) {
    service_uuid = serv_uuid;

    m_adv_scan_obj->setActiveScan(active_scan);
    m_adv_scan_obj->setInterval(scan_interval);
    m_adv_scan_obj->setWindow(scan_window);

#ifdef CLIENT_DBG
    g_log_mgr.println("dbg: (re)starting scan for device");
    g_log_mgr.print("dbg: search param:\n\tservice uuid: ");
    g_log_mgr.println(service_uuid.toString());
#endif

    m_adv_scan_obj->start(scan_duration, false);
}

void RFManager_Client::adv_scan_stop() {
    m_adv_scan_obj->stop();

#ifdef CLIENT_DBG
    g_log_mgr.println("dbg: scan aborted");
#endif
}

bool RFManager_Client::found() {
    return service_found;
}

void RFManager_Client::pair() {
    m_client->connect(&service_device); 
    m_client->setMTU(517);
}

bool RFManager_Client::paired() {
    return m_client->isConnected();
}

BLERemoteService *RFManager_Client::pair_service(BLEUUID service_uuid) {
#ifdef CLIENT_DBG
    g_log_mgr.println("dbg: returning peer service: " + service_uuid.toString());
#endif
    return m_client->getService(service_uuid);
}
       
BLERemoteCharacteristic *RFManager_Client::pair_characteristic(BLERemoteService *service, BLEUUID characteristic_uuid) {
#ifdef CLIENT_DBG
    g_log_mgr.println("dbg: from service: " + service->toString() + " returning peer characteristic: " + characteristic_uuid.toString());
#endif
    return service->getCharacteristic(characteristic_uuid);
}

RFManager_Client::peer_char_attributes_t RFManager_Client::pair_char_attributes(BLERemoteCharacteristic *characteristic) {
    peer_char_attributes_t attr = 0;

    /**
     * @brief layout of peer_char_attributes_t
     * bit 0: read?
     * bit 1: write?
     * bit 2: notify?
     * bit 3: broadcast?
     * bit 4: indicate?
     * bit 5: write no response?
     */

    if (characteristic->canRead())
        attr |= 1 << 0;

    if (characteristic->canWrite())
        attr |= 1 << 1;

    if (characteristic->canNotify())
        attr |= 1 << 2;

    if (characteristic->canBroadcast())
        attr |= 1 << 3;

    if (characteristic->canIndicate())
        attr |= 1 << 4;

    if (characteristic->canWriteNoResponse())
        attr |= 1 << 5;

    return attr;
}

void RFManager_Adv_Callbacks::onResult(BLEAdvertisedDevice adv_device) {
/* #ifdef CLIENT_DBG
    g_log_mgr.println("dbg: found advertising device " + adv_device.toString());
#endif */
    if (adv_device.haveServiceUUID() && adv_device.isAdvertisingService(service_uuid)) {
        auto adv_scan_obj = BLEDevice::getScan();
        adv_scan_obj->stop();

#ifdef CLIENT_DBG
        g_log_mgr.println("dbg: found device: " + adv_device.getName());
        g_log_mgr.println("dbg: peer params: \n\tpeer address: " + adv_device.getAddress().toString() + "\n\tservice uuid: " + service_uuid.toString());
#endif
        service_device = adv_device;
        service_found  = true;
    }
}

void RFManager_Client_Callbacks::onConnect(BLEClient *client) {
#ifdef CLIENT_DBG
    g_log_mgr.println("dbg: paired with peer address: " + client->getPeerAddress().toString());
#endif
}

void RFManager_Client_Callbacks::onDisconnect(BLEClient *client) {
#ifdef CLIENT_DBG
    g_log_mgr.println("dbg: unpaired with peer address: " + client->getPeerAddress().toString());
#endif
}