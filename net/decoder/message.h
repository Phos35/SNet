#ifndef SNET_MESSAGE_H
#define SNET_MESSAGE_H
#include <memory>
#include <string>

class Message
{
public:
    typedef Message *Ptr;
    typedef std::unique_ptr<Message> UPtr;

    // 报文解析的最终结果
    enum class DeocdeResult
    {
        SUCCESS,    // 解析成功
        FAILURE     // 解析失败
    };

    Message(const std::string& data);
    Message(DeocdeResult result);
    virtual ~Message();

    /// @brief 设置解析结果
    void set_result(DeocdeResult result);

    /// @brief 获取解析结果
    DeocdeResult get_result();

    /// @brief 获取报文原始数
    /// @return 原始数据
    std::string raw_data();

private:
    DeocdeResult    result_;        // 报文解析结果
    std::string     raw_data_;      // 原始数据
};

#endif