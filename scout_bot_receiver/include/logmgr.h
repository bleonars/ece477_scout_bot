#ifndef __LOGMGR_H__
#define __LOGMGR_H__

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "driver/uart.h"

#include <string_view>
#include <string>

namespace Utils {
     /**
     * @brief thread-safe wrapper for uart logging
     */
    class LogManager {
    public:
        /**
         * @brief allocate data for uart log manager
         * 
         * @param port_num uart number
         * @param tx_io_num uart tx io port number
         * @param rx_io_num uart rx io port number
         * @param rx_buf_size uart rx buffer size
         * @param tx_buf_size uart tx buffer size
         * @param rate uart rate
         * @param data_bits uart data bits
         * @param parity uart parity
         * @param stop_bits uart stop bits
         * @return true, allocation successful
         * @return false, allocation unsuccessful
         */
        bool allocate(uart_port_t port_num, int tx_io_num, int rx_io_num, int rx_buf_size, int tx_buf_size, int rate, 
            uart_word_length_t data_bits = UART_DATA_8_BITS, uart_parity_t parity = UART_PARITY_DISABLE, uart_stop_bits_t stop_bits = UART_STOP_BITS_1);

        /**
         * @brief free log manager data
         * 
         */
        void release();

        /**
         * @brief read data from uart
         * 
         * @param data data buffer
         * @param buf_size size of data buffer
         * @return true, successful read or failure to acquire mutex
         * @return false, unsuccessful read or failure to acquire mutex
         */
        bool read(uint8_t *data, size_t buf_size);

        /**
         * @brief write data to uart
         * 
         * @param data data buffer
         * @param len length of data to write from buffer
         * @return true, successful write or failure to acquire mutex
         * @return false, unsuccessful write or failure to acquire mutex
         */
        bool write(const uint8_t *data, size_t len);

        /**
         * @brief print string to uart
         * 
         * @param str string view literal
         */
        void print(std::string_view str);

        /**
         * @brief print string to uart followed by newline
         * 
         * @param str string view litearl
         */
        void println(std::string_view str);
    private:
        static constexpr int UART_SEMAPHORE_MS_WAIT_TIME = 20; /* time to wait to acquire semaphore */

        SemaphoreHandle_t    m_mutex;                          /* uart buffer mutex */
        uart_port_t          m_uart_num;                       /* uart number */
        uart_config_t        m_uart_cfg;                       /* uart config obj */
    };
}

extern Utils::LogManager g_log_mgr;

#endif