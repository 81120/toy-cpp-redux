#include <functional>
#include <iostream>

#include "./redux.hpp"

int main(int argc, char** argv) {
  using State   = int;
  using Action  = std::string;
  using Reducer = std::function<State(State, Action)>;

  auto reducer = [](State state, Action action) {
    if (action == "Inc") {
      return state + 1;
    }
    if (action == "Dec") {
      return state - 1;
    }
    return state;
  };

  auto store = redux::createStore<Reducer, State, Action>(reducer);
  store.subscribe([](auto state) {
    std::cout << "In the subscriber, the state: " << state << std::endl;
  });

  auto loggerMiddleware = [](auto&& store, auto next, auto action) {
    std::cout << "Dispatching " << action << ", "
              << "the state is: " << store.getState() << std::endl;
    next(action);
    std::cout << "Finished! New state is: " << store.getState() << std::endl;
  };

  store.applyMiddleware(loggerMiddleware);

  store.dispatch("Inc");
  store.dispatch("Dec");

  return 0;
}
