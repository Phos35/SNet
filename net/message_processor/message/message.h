#ifndef SNET_MESSAGE_H
#define SNET_MESSAGE_H
#include <memory>
#include <string>

class Message : public std::enable_shared_from_this<Message>
{
public:
    typedef Message *Ptr;
    typedef std::unique_ptr<Message> UPtr;
    typedef std::shared_ptr<Message> SPtr;

    // 报文解析的最终结果
    enum class DeocdeResult
    {
        SUCCESS,    // 解析成功
        FAILURE     // 解析失败
    };

    Message();
    Message(const std::string &data);
    Message(DeocdeResult result);
    virtual ~Message();

    /// @brief 设置解析结果
    void set_result(DeocdeResult result);

    /// @brief 获取解析结果
    DeocdeResult get_result();

    /// @brief 获取报文字符数据
    /// @return 原始数据
    virtual std::string data();

    /// @brief 获取原始数据的大小
    /// @return 原始数据大小
    size_t data_size();

    // TODO 或许可以使用移动语义做优化
    /// @brief 用于派生类设置数据
    /// @param data 待设置的数据
    void set_data(const std::string &data);

    /// @brief 清空数据
    void clear_data();

private:
    DeocdeResult    result_;        // 报文解析结果
    std::string     data_;          // 字符串数据
};

#endif