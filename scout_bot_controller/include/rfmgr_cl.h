#ifndef __RF_MANAGER_CL_H__
#define __RF_MANAGER_CL_H__

#include "BLEDevice.h"
#include "Arduino.h"
#include "main.h"

#include <memory>

namespace ScoutBot_Controller {
    class RFManager_Adv_Callbacks;

     /**
     * @brief rf manager implementation for all client ble functionality
     */
    class RFManager_Client {
    public:
        /**
         * @brief initializes ble device
         * 
         * @param client_name string name for ble client
         */
        void setup(std::string client_name);

        /**
         * @brief start an advertising scan based on scan parameters
         * 
         * @param active_scan perform active scan
         * @param scan_interval interval of scan (how long we scan)
         * @param scan_window window of scan (how long to wait between scans)
         * @param scan_duration total duration of scan
         */
        void adv_scan_start(BLEUUID srv_uuid, bool active_scan, uint16_t scan_interval, uint16_t scan_window, uint32_t scan_duration);

        /**
         * @brief stop an in progress advertising scan
         */
        void adv_scan_stop();

        /**
         * @brief connect to device found through completed scan
         */
        void connect();

    protected:
        BLEUUID                                   m_srv_uuid;         // remote server uuid
        BLEAdvertisedDevice                       m_conn_device;      // connected device advertising obj

    private:
        BLEScan                                  *m_adv_scan_obj;     // advertising scan obj
        std::unique_ptr<RFManager_Adv_Callbacks>  m_adv_callback_obj; // advertising callback obj
    };

    /**
     * @brief rf manager advertising callback override
     */
    class RFManager_Adv_Callbacks : public BLEAdvertisedDeviceCallbacks, protected RFManager_Client {
    public:
        /**
         * @brief virtual override of BLEAdvertisedDeviceCallbacks::onResult
         * 
         * @param advertisedDevice advertised device scan object
         */
        void onResult(BLEAdvertisedDevice advertisedDevice) {
#ifdef CLIENT_DBG
            Serial.print("dbg: found advertising device ");
            Serial.println(advertisedDevice.toString().c_str());
#endif
            if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(m_srv_uuid)) {
                adv_scan_stop();

#ifdef CLIENT_DBG
                Serial.print("dbg: found advertising device with matching uuid ");
                Serial.println(advertisedDevice.toString().c_str());
#endif
                m_conn_device = advertisedDevice;
            }
        }
    };

}

#endif