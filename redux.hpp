#include <functional>
#include <vector>

namespace redux {
template <typename State, typename Action, typename Reducer>
class Store {
 public:
  using Next = std::function<void(Action)>;
  using Middleware =
      std::function<void(Store& store, Next next, Action action)>;
  using Subsciber = std::function<void(State)>;

  Store(const Store& other) = delete;

  Store(Reducer reducer, State state)
      : state(state), next(nextWrapper(reducer)) {
  }

  State getState() const {
    return state;
  }

  void dispatch(Action action) {
    next(action);
  }

  void subscribe(Subsciber subscriber) {
    subscribers.push_back(subscriber);
  }

  Store& applyMiddleware(Middleware middleware) {
    next = [this, oldNext = next, middleware](Action action) {
      middleware(*this, oldNext, action);
    };
    return *this;
  }

 private:
  Next                   next;
  State                  state;
  std::vector<Subsciber> subscribers;

  Next nextWrapper(Reducer reducer) {
    return [this, reducer](Action action) {
      state = reducer(state, action);
      for (auto subscriber : subscribers) {
        subscriber(state);
      }
    };
  }
};

template <typename Reducer, typename State, typename Action>
auto createStore(Reducer reducer, State state = State()) {
  return Store<State, Action, Reducer>(reducer, state);
}

}  // namespace redux
