/**
 * The majority of this code was adapted from:
 *
 * https://gist.github.com/rioki/1290004d7505380f2b1d
 * https://gist.github.com/martinfinke/a636dcddbcf112344b59
 *
 **/

#pragma once

#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <string>

namespace lve {
class EventEmitter {
 public:
  EventEmitter() {}
  ~EventEmitter() {}

  template <typename... Args>
  std::string addListener(const std::string &name, std::function<void(Args...)> cb) {
    std::string listener_name = name + "_" + std::to_string(++lastListener);
    listeners.insert(std::make_pair(name, std::make_shared<Listener<Args...>>(listener_name, cb)));
    std::cout << "listeners: " << listeners.size() << std::endl;
    return listener_name;
  }

  std::string addListener(const std::string &name, std::function<void()> cb) {
    std::string listener_name = name + "_" + std::to_string(++lastListener);
    listeners.insert(std::make_pair(name, std::make_shared<Listener<>>(listener_name, cb)));
    return listener_name;
  }

  template <typename Lambda>
  std::string addListener(const std::string &name, Lambda cb) {
    return addListener(name, make_function(cb));
  }

  void removeListener(const std::string &listener_name) {
    auto i = std::find_if(
        listeners.begin(),
        listeners.end(),
        [&](std::pair<std::string, std::shared_ptr<ListenerBase>> p) {
          return p.second->name == listener_name;
        });

    if (i != listeners.end()) {
      listeners.erase(i);
    } else {
      throw std::invalid_argument("EventEmitter::remove_listener: Invalid listener name.");
    }
  }

  template <typename... Args>
  void emit(const std::string &name, Args... args) {
    std::cout << "emit name: " << name << ", listeners size: " << listeners.size() << std::endl;

    std::list<std::shared_ptr<Listener<Args...>>> handlers;
    auto range = listeners.equal_range(name);
    handlers.resize(std::distance(range.first, range.second));

    std::transform(
        range.first,
        range.second,
        handlers.begin(),
        [](std::pair<std::string, std::shared_ptr<ListenerBase>> p) {
          std::cout << "transform p: " << p.first << std::endl;
          auto l = std::dynamic_pointer_cast<Listener<Args...>>(p.second);
          if (l) {
            return l;
          } else {
            throw std::logic_error("EventEmitter::emit: Invalid event signature.");
          }
        });

    std::cout << "handlers size: " << handlers.size() << std::endl;

    for (auto &h : handlers) {
      std::cout << "emit: handler.name: " << h->name << std::endl;
      h->callback(args...);
    }
  }

 private:
  struct ListenerBase {
    ListenerBase(const std::string &n) : name(n) {}
    virtual ~ListenerBase() {}
    const std::string &name;
  };

  template <typename... Args>
  struct Listener : public ListenerBase {
    Listener() {}
    Listener(const std::string &name, std::function<void(Args...)> cb)
        : ListenerBase(name), callback(cb) {}
    std::function<void(Args...)> callback;
  };

  unsigned int lastListener{0};
  std::multimap<std::string, std::shared_ptr<ListenerBase>> listeners;

  template <typename T>
  struct function_traits : public function_traits<decltype(&T::operator())> {};

  template <typename ClassType, typename ReturnType, typename... Args>
  struct function_traits<ReturnType (ClassType::*)(Args...) const> {
    typedef std::function<ReturnType(Args...)> f_type;
  };

  template <typename L>
  typename function_traits<L>::f_type make_function(L l) {
    return (typename function_traits<L>::f_type)(l);
  }
};
}  // namespace lve