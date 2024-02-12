template <typename... Args>
auto g(Args&&... args) -> decltype(f(std::forward<Args>(args)...)) {
  return f(std::forward<Args>(args)...);
}