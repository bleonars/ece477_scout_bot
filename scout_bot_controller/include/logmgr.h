#ifndef __LOGMGR_H__
#define __LOGMGR_H__

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "driver/uart.h"

#include <string_view>
#include <string>

namespace Utils {
    class LogManager {
    public:
        bool allocate(uart_port_t port_num, int tx_io_num, int rx_io_num, int rx_buf_size, int tx_buf_size, int rate, 
            uart_word_length_t data_bits = UART_DATA_8_BITS, uart_parity_t parity = UART_PARITY_DISABLE, uart_stop_bits_t stop_bits = UART_STOP_BITS_1);

        void release();

        bool read(uint8_t *data, size_t buf_size);
        bool write(const uint8_t *data, size_t len);

        void print(std::string_view str);
        void println(std::string_view str);
    private:
        static constexpr int UART_SEMAPHORE_MS_WAIT_TIME = 20;

        SemaphoreHandle_t m_mutex;
        uart_port_t       m_uart_num;
        uart_config_t     m_uart_cfg;
    };
}

extern Utils::LogManager g_log_mgr;

#endif