<img width="1360" height="397" alt="image" src="https://github.com/user-attachments/assets/578e5bb8-86c4-450c-a1c2-f85b6f0ecbd5" />\n
BaseDelegate:内部持有委托实例指针,绑定时会根据情况构建不同的委托实例(普通函数、成员函数、lambda)
IDelegateInstance:可调用对象的上层接口向外提供执行委托的API
具体实例：FunDelegateInst、MebFunDelegateInst、LambdaDelegateInst，实现具体的调用逻辑
MultiDelegate：存储多个BaseDelegate来实现单播
