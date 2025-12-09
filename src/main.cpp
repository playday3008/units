#include <example/example.hpp>
#include <print>

auto main() noexcept(false) -> int {
  std::print("hello {}!\n", example::who);
  return 0;
}
