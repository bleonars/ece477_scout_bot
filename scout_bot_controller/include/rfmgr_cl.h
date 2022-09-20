#ifndef __RF_MANAGER_CL_H__
#define __RF_MANAGER_CL_H__

#include "main.h"
#include "BLEDevice.h"

#include <memory>

namespace ScoutBot_Client {
    /**
     * @brief rf manager advertising callback override
     */
    class RFManager_Adv_Callbacks : public BLEAdvertisedDeviceCallbacks {
    private:
        /**
         * @brief virtual override of BLEAdvertisedDeviceCallbacks::onResult
         * 
         * @param adv_device advertised device scan object
         */
        void onResult(BLEAdvertisedDevice adv_device);
    };

    /**
     * @brief rf manager client callback override
     * 
     */
    class RFManager_Client_Callbacks : public BLEClientCallbacks {
    private:
        /**
         * @brief virtual override of BLEClientCallbacks::onConnect
         * 
         * @param client ble client object
         */
        void onConnect(BLEClient *client);

        /**
         * @brief virtual override of BLEClientCallbacks::onDisconnect
         * 
         * @param client ble client object
         */
        void onDisconnect(BLEClient *client);
    };

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
        void setup(std::string_view client_name);

        /**
         * @brief start an advertising scan based on scan parameters
         * 
         * @param serv_uuid uuid of ble service to scan for
         * @param active_scan perform active scan
         * @param scan_interval interval of scan (how long we scan)
         * @param scan_window window of scan (how long to wait between scans)
         * @param scan_duration total duration of scan
         */
        void adv_scan_start(BLEUUID serv_uuid, bool active_scan, uint16_t scan_interval, uint16_t scan_window, uint32_t scan_duration);

        /**
         * @brief stop an in progress advertising scan
         */
        void adv_scan_stop();

        /**
         * @brief check if correct advertising device was found
         * 
         * @return true, successfully found
         * @return false, unsuccessfully found
         */
        bool found();

        /**
         * @brief pair to device found through completed scan
         */
        void pair();

        /**
         * @brief check if pairing with server device completed
         * 
         * @return true, successfuly paired
         * @return false, unsuccessfully paired
         */
        bool paired();

    private:
        BLEClient                  *m_client;              /* client obj */
        BLEScan                    *m_adv_scan_obj;        /* advertising scan obj */
        RFManager_Client_Callbacks  m_client_callback_obj; /* client callback obj */
        RFManager_Adv_Callbacks     m_adv_callback_obj;    /* advertising callback obj */
    };
}

extern ScoutBot_Client::RFManager_Client g_rfmgr_cl;

#endif
