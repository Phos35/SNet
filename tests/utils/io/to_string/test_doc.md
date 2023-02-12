# IO模块测试

## 一、to_string 测试

- 将对象转化为字符串提出2种方法：

  1. 重载`<<`操作符，并实现一个`SNet::IO::to_string(const T&)`函数，在函数中利用`<<`和`ostringstream`实现对象的字符串化
  2. 在对象中直接实现`to_string()`函数，将对象的内容转换成字符串

- 测试目的
  方法1更具备通用性，但从实现逻辑上看，其效率可能低于类成员`to_string`函数。因此，对这两种方法进行测试，检验方法1的效率是否低于方法2，若低于，则是否在一个可接受的范围内

- 测试数据

  - 实现测试类`TestClass`

    ```c++
    class TestClass
    {
    public:
        std::string to_string()
        {
            return std::string(256, 'a');
        }
    
        friend std::ostream& operator <<(std::ostream& os, const TestClass& t)
        {
            return os << std::string(256, 'a');
        }
    };
    ```

  - 测试规模
    使用`vector<TestClass> tests(100000);`循环输出所有对象, 重复测试1000次

- 测试结果

  - 使用类成员`to_string()`

    ```
    average time:0.0007594893260000003
    ```

  - 利用重载`<<`操作符的`to_string()`

    ```
    0.0007776993080000001
    ```

- 测试结论
  从最终的测试结果来看，方法1相较于方法2有多余23%左右的时间开销，因此在之后的程序中还是尽可能使用类内`to_string`方法会更好

- 疑问
  从直觉上来看，明显类成员函数的方法比利用重载运算符的方法要快，因为少了创建一次函数调用（`SNet::IO::to_string()`），但实际结果却违反了这种直觉。其中的原因尚未明确

