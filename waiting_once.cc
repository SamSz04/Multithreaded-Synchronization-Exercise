#include "waiting_once_api.hpp"

void waiting_once::call_once_waiting(init_function f) {
    std::lock_guard<std::mutex> lock(mtx);
    if (!initialized) {
        f();
        initialized = true;
    }
}

void optimized_waiting_once::call_once_waiting(init_function f) {
    // 第一次检查，避免不必要的锁争用
    // memory_order_acquire 确保之前的写操作对当前线程可见。如果 initialized 已经为 true，则无需获取锁，直接返回。
    if (!initialized.load(std::memory_order_acquire)) {
        std::lock_guard<std::mutex> lock(mtx);
        // 再次检查，防止在第一次检查和获取锁之间有其他线程完成了初始化
        if (!initialized.load(std::memory_order_relaxed)) {
            f();
            // 设置原子变量，并使用 std::memory_order_release 确保所有写操作对其他线程可见
            initialized.store(true, std::memory_order_release);
        }
    }
}
