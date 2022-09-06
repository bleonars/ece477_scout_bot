#include "rfmgr_cl.h"

using namespace ScoutBot_Controller;

void RFManager_Client::setup(std::string client_name) {
    BLEDevice::init(client_name);
}

void RFManager_Client::adv_scan_start(BLEUUID srv_uuid, bool active_scan, uint16_t scan_interval, uint16_t scan_window, uint32_t scan_duration) {
    if (!m_adv_scan_obj) {
        m_adv_scan_obj = BLEDevice::getScan();

        m_adv_callback_obj = std::unique_ptr<RFManager_Adv_Callbacks>(new RFManager_Adv_Callbacks());
        m_adv_scan_obj->setAdvertisedDeviceCallbacks(m_adv_callback_obj.get());
    }

    m_srv_uuid = srv_uuid;

    m_adv_scan_obj->setActiveScan(active_scan);
    m_adv_scan_obj->setInterval(scan_interval);
    m_adv_scan_obj->setWindow(scan_window);

#ifdef CLIENT_DBG
    Serial.print("dbg: starting scan for uuid ");
    Serial.println(m_srv_uuid.toString().c_str());
#endif

    m_adv_scan_obj->start(scan_duration, false);
}

void RFManager_Client::adv_scan_stop() {
    m_adv_scan_obj->stop();

#ifdef CLIENT_DBG
    Serial.println("dbg: scan ended");
#endif
}