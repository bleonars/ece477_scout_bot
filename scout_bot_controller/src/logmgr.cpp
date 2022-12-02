#include "logmgr.h"

using namespace Utils;

bool LogManager::allocate(uart_port_t port_num, int tx_io_num, int rx_io_num, int rx_buf_size, int tx_buf_size, 
    int rate, uart_word_length_t data_bits, uart_parity_t parity, uart_stop_bits_t stop_bits) {
    m_mutex    = xSemaphoreCreateMutex();
    if (!m_mutex)
        return false;

    m_uart_num = port_num;

    m_uart_cfg.baud_rate  = rate;
    m_uart_cfg.data_bits  = data_bits;
    m_uart_cfg.parity     = parity;
    m_uart_cfg.stop_bits  = stop_bits;
    m_uart_cfg.flow_ctrl  = UART_HW_FLOWCTRL_DISABLE;
    m_uart_cfg.source_clk = UART_SCLK_APB;

    if (uart_driver_install(port_num, rx_buf_size, tx_buf_size, 0, nullptr, 0) != ESP_OK)
        return false;

    if (uart_param_config(port_num, &m_uart_cfg) != ESP_OK)
        return false;

    if (uart_set_pin(port_num, tx_io_num, rx_io_num, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE) != ESP_OK)
        return false;

    return true;
}

void LogManager::release() {
    uart_driver_delete(m_uart_num);
}

bool LogManager::read(uint8_t *data, size_t buf_size) {
    int bytes_read = 0;

    if (xSemaphoreTake(m_mutex, UART_SEMAPHORE_MS_WAIT_TIME / portTICK_PERIOD_MS) == pdTRUE) {
        bytes_read = uart_read_bytes(m_uart_num, data, buf_size - 1, UART_SEMAPHORE_MS_WAIT_TIME / portTICK_PERIOD_MS);

        xSemaphoreGive(m_mutex);
    }

    return bytes_read != 0;
}

bool LogManager::write(const uint8_t *data, size_t buf_size) {
    int bytes_written = 0;

    if (xSemaphoreTake(m_mutex, UART_SEMAPHORE_MS_WAIT_TIME / portTICK_PERIOD_MS) == pdTRUE) {
        bytes_written = uart_write_bytes(m_uart_num, data, buf_size);

        xSemaphoreGive(m_mutex);
    }

    return bytes_written == buf_size;
}

void LogManager::print(std::string_view str) {
    write((const uint8_t *) str.data(), str.size());
}

void LogManager::println(std::string_view str) {
    auto str_n = std::string(str) + "\n";
    write((const uint8_t *) str_n.data(), str_n.size());
}
