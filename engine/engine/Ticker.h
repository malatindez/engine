#pragma once
#include <chrono>
#include <thread>
namespace engine::core {
class Ticker {
 public:
  /// <summary>
  /// Ticker class constructor.
  /// If tickrate is zero, 
  /// </summary>
  /// <param name="tickrate">frequency with which we should call the Update function</param>
  /// <param name="thread_id">thread id if class should be bound to thread</param>
  Ticker(const uint32_t tickrate, std::thread::id const& thread_id)
      : tickrate_(tickrate),
        thread_id_(std::make_shared<std::thread::id>(thread_id)) {}

  explicit Ticker(const uint32_t tickrate) : tickrate_(tickrate) {}

  virtual ~Ticker() = default;

  /// <summary>
  ///
  /// </summary>
  /// <param name="tick"></param>
  /// <param name="time_delta"></param>
  void UpdateExecutionTime(const uint64_t tick) {
    if ( (tick % tickrate() != 0) || !needs_update_) {
      return;
    }
    auto start = std::chrono::high_resolution_clock::now();
    Update(tick);
    double exec_time =
        (double(std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::high_resolution_clock::now() - start)
                    .count())) /
        10e9;
    this->average_update_time_ =
        average_update_time_ * double(calls_counter_) + exec_time;
    this->average_update_time_ /= ++calls_counter_;
  }
  /// <summary>
  /// 
  /// </summary>
  /// <param name="tick">current engine tick</param>
  /// <param name="time_delta"></param>
  virtual void Update(const uint64_t tick) {
    // Intentionally unimplemented
  }

  [[nodiscard]] uint32_t tickrate() const noexcept { return tickrate_; }

  // If the thread_id() is not equal to nullptr, then we should update this
  // object only in the thread with this id
  [[nodiscard]] std::weak_ptr<std::thread::id> thread_id() const {
    return std::weak_ptr<std::thread::id>(thread_id_);
  }

  /// <summary>
  /// Returns average Update execution time(in seconds).
  /// </summary>
  /// <returns>Average Update execution time.</returns>
  [[nodiscard]] double average_update_time() const noexcept {
    return average_update_time_;
  }
  [[nodiscard]] int calls_counter() const noexcept { return calls_counter_; }

  [[nodiscard]] bool needs_update() const noexcept { return needs_update_; }

 protected:
  void SetTickrate(uint32_t tickrate) { tickrate_ = tickrate; }
  void SetThreadID(std::thread::id &id) {
    thread_id_ = std::make_shared<std::thread::id>(id);
  }

  void DisableUpdating() { needs_update_ = false; }
  void EnableUpdating() { needs_update_ = true; }

 private:
  uint32_t tickrate_;
  int calls_counter_ = 0;
  double average_update_time_ = 0;

  bool needs_update_ = true;

  std::shared_ptr<std::thread::id> thread_id_ =
      std::shared_ptr<std::thread::id>(nullptr);
};
}  // namespace engine::core