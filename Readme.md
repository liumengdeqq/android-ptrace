## 说明

* ptrace_target 是 ptrace_attach 的目标程序
* ptrace_hook_target 是 ptrace_hook 的目标程序

---
* ptrace_attach 演示了 attach 一个目标程序，并且监听系统调用，改变其参数（反转）
* ptrace_hook 演示了 hook 一个自定义的方法sub，并改变其执行流程（屏蔽了 printf 语句，并改变了返回值），方法是反汇编之后找到要 hook 的函数的地址，直接改变对应地址的代码块的机器码来达到目的



---
参考：

* http://hi.baidu.com/harry_lime/item/cce5161e4af86d4a71d5e8ff
* http://hi.baidu.com/harry_lime/item/0109173da8c3909d633affa5

