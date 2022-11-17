#ifndef __RF_MANAGER_SRV_H__
#define __RF_MANAGER_SRV_H__

#include "main.h"
#include "BLEDevice.h"

namespace ScoutBot_Server {
    class RFManager_Service {
    public:
        /**
         * @brief initializes ble service
         * 
         * @param server host server for service
         * @param service_uuid string advertising service uuid
         */
        void setup(BLEServer *server, BLEUUID serv_uuid);
        
        /**
         * @brief get service object pointer
         * 
         * @return BLEService* service object pointer
         */
        BLEService *get();
        
        /**
         * @brief add characteristic to serivce
         * 
         * @param char_uuid string advertising characteristic uuid
         * @param char_attributes property attributes of characteristic
         */
        using peer_char_attributes_t = uint8_t;
        BLECharacteristic *add_characteristic(BLEUUID char_uuid, peer_char_attributes_t char_attributes);
        
        /**
         * @brief get the characteristic object pointer by uuid
         * 
         * @param char_uuid characteristic advertising uuid
         * @return BLECharacteristic* characteristic object pointer
         */
        BLECharacteristic *get_characteristic(BLEUUID char_uuid);
    
    private:
        BLEService                                 *m_service;         /* host service obj */
        std::map<std::string, BLECharacteristic *>  m_characteristics; /* advertising characteristics of service */
    };
    
    class RFManager_Server_Callbacks : public BLEServerCallbacks {
    private:
        /**
         * @brief virtual override of BLEServerCallbacks::onConnect
         * 
         * @param server ble server object
         */
        void onConnect(BLEServer *server);

        /**
         * @brief virtual override of BLEServerCallbacks::onDisconnect
         * 
         * @param server ble server object
         */
        void onDisconnect(BLEServer *server);
    };
    
    class RFManager_Server {
    public:
        /**
         * @brief initializes ble device
         * 
         * @param server_name string name for ble client
         */
        void setup(std::string_view server_name);
        
        /**
         * @brief adds advertising service to host server
         * 
         * @param service_uuid string advertising service uuid
         */
        RFManager_Service *add_service(BLEUUID service_uuid);
        
        /**
         * @brief start advertising
         */
        void adv_start();

        /**
         * @brief stop advertising
         */
        void adv_stop();
        
        /**
         * @brief get advertising service data by uuid
         * 
         * @param serv_uuid service advertising uuid
         * @return RFManager_Service service object
         */
        RFManager_Service *get_service(BLEUUID serv_uuid);

    private:
        BLEServer                                 *m_server;              /* server obj */
        BLEAdvertising                            *m_adv_obj;             /* advertising obj */
        std::map<std::string, RFManager_Service*>  m_services;            /* advertising services */
        RFManager_Server_Callbacks                 m_server_callback_obj; /* server callback obj */
    };
}


extern ScoutBot_Server::RFManager_Server g_rfmgr_srv;

#endif