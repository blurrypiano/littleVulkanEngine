#pragma once

#include "lve_utils.hpp"

// std
#include <any>
#include <bitset>
#include <cassert>
#include <cstddef>  // For std::ptrdiff_t
#include <iostream>
#include <iterator>  // For std::forward_iterator_tag
#include <memory>
#include <string>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace lve {

using EntId = unsigned int;
using EntQueryId = int;
using ComponentType = int;  // // how to cast any type?

constexpr EntId NullEntId = 0;
constexpr EntQueryId NullQueryId = 0;

class EntManager;
class EntQuery;
class EntQueryResult;
class EntQuery__internal {
 public:
  const size_t hashValue;

  bool operator==(const EntQuery__internal &other) const {
    return hashValue == other.hashValue && allOf == other.allOf && anyOf == other.anyOf &&
           noneOf == other.noneOf;
  }

 private:
  EntQuery__internal(
      size_t hash,
      const std::unordered_set<ComponentType> allOfTypes,
      const std::unordered_set<ComponentType> anyOfTypes,
      const std::unordered_set<ComponentType> noneOfTypes)
      : hashValue{hash}, allOf{allOfTypes}, anyOf{anyOfTypes}, noneOf{noneOfTypes} {}

  const std::unordered_set<ComponentType> allOf;
  const std::unordered_set<ComponentType> anyOf;
  const std::unordered_set<ComponentType> noneOf;

  friend class EntManager;
  friend class EntQuery;
};
}  // namespace lve

namespace std {
template <>
struct hash<lve::EntQuery__internal> {
  size_t operator()(lve::EntQuery__internal const &query) const { return query.hashValue; }
};
}  // namespace std

namespace lve {

template <typename K>
class PackedSet {
 public:
  void add(K key) {
    assert(!contains(key) && "PackedSet::Add failed: key already in index");

    indexFromKey[key] = keyFromIndex.size();
    keyFromIndex.push_back(key);
  }

  // returns the position of key that was removed and replaced by the last element
  size_t removeAndPack(K key) {
    assert(contains(key) && "PackedSet::Remove failed: key not in index");

    size_t availableIndex = indexFromKey[key];

    // move last element into newly available space
    if (availableIndex != keyFromIndex.size() - 1) {
      size_t lastElementK = keyFromIndex.back();
      keyFromIndex[availableIndex] = lastElementK;
      indexFromKey[lastElementK] = availableIndex;
    }

    // then remove last element
    keyFromIndex.pop_back();
    indexFromKey.erase(key);

    return availableIndex;
  }

  // returns the position of the key that was removed and replaced by the last element
  size_t tryRemoveAndPack(K key) {
    if (!contains(key)) return -1;
    return removeAndPack(key);
  }

  void entDestroyed(K entId) {
    if (contains(entId)) {
      removeAndPack(entId);
    }
  }

  bool contains(K key) const { return indexFromKey.find(key) != indexFromKey.end(); }

  const std::vector<K> &getKeys() const { return keyFromIndex; }

  size_t getIndexFromEnt(K key) const { return indexFromKey.at(key); }

 private:
  std::vector<K> keyFromIndex{};
  std::unordered_map<K, size_t> indexFromKey{};
};

template <typename K, typename V>
class PackedMap {
 public:
  V &get(K key) {
    assert(keySet.contains(key) && "Get Component called with invalid ent ID");
    return values[keySet.getIndexFromEnt(key)];
  }

  void add(K key) {
    assert(!keySet.contains(key) && "Component already added to entity");
    keySet.add(key);
    values.push_back(V{});
  }

  void remove(K key) {
    assert(keySet.contains(key) && "Cannot remove non-existant component from entity");

    size_t availableIndex = keySet.removeAndPack(key);

    // move last element into newly available space if not already last position
    if (availableIndex != values.size() - 1) {
      values[availableIndex] = values.back();
    }

    // then remove last element
    values.pop_back();
  }

  bool contains(K key) const { return keySet.contains(key); }

  const std::vector<K> &getKeys() const { return keySet.getKeys(); }

  const PackedSet<K> &getKeySet() const { return keySet; }

  std::vector<V> &getValues() { return values; }

 private:
  std::vector<V> values{};
  PackedSet<K> keySet{};
};

class EntManager;
class Ent {
 public:
  EntId getId() const { return id; };
  template <typename T>
  T &get();

 private:
  Ent(EntId objId, EntManager &manager) : id{objId}, entManager{manager} {}

  // store ComponentType -> T* const (pointer cannot change, but object it points to can
  // be modified)
  EntId id;
  EntManager &entManager;

  friend class EntManager;
  friend class EntQueryResult;
};

// a query is an index, as its results reference
// needs to stick around and be valid so long as it exists
class EntQueryResult {
 public:
  const EntQueryId id;

  ~EntQueryResult();
  EntQueryResult(const EntQueryResult &);
  EntQueryResult(EntQueryResult &&) noexcept;
  EntQueryResult &operator=(const EntQueryResult &) = delete;
  EntQueryResult &operator=(EntQueryResult &&) = delete;

  struct Iterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = Ent;
    using pointer = Ent *;    // or also value_type*
    using reference = Ent &;  // or also value_type&

    Iterator(std::vector<EntId>::const_iterator iterator, EntManager &manager, bool isValid = true)
        : it{iterator}, entManager{manager}, ent{isValid ? *iterator : NullEntId, manager} {}

    reference operator*() { return ent; }
    pointer operator->() { return &ent; }

    // Prefix increment
    Iterator &operator++() {
      it++;
      ent.id = *it;
      return *this;
    }

    // Postfix increment
    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    friend bool operator==(const Iterator &a, const Iterator &b) { return a.it == b.it; };
    friend bool operator!=(const Iterator &a, const Iterator &b) { return a.it != b.it; };

   private:
    Ent ent;  // <- store pointer to an ent
    EntManager &entManager;
    std::vector<EntId>::const_iterator it;
  };

  // needs a signature?
  // allOf, anyOf, noneOf, etc.
  // register/deregister on construction desctruction
  EntQueryResult(
      EntManager &manager, const std::vector<EntId> &entIds, EntQueryId queryId = NullQueryId);

  const std::vector<EntId> &ids() { return results; }

  Iterator begin() { return Iterator{results.begin(), entManager, results.size() > 0}; }
  Iterator end() { return Iterator{results.end(), entManager, false}; }
  size_t size() const { return results.size(); }

 private:
  const std::vector<EntId> &results;
  EntManager &entManager;

  friend class EntManager;
};

class EntManager {
 public:
  EntManager() = default;
  ~EntManager() = default;
  EntManager(const EntManager &) = delete;
  EntManager &operator=(const EntManager &) = delete;
  EntManager(EntManager &&) = delete;
  EntManager &operator=(EntManager &&) = delete;

  EntId createEnt() {
    auto entId = nextEntId;
    nextEntId += 1;
    allEntIds.add(entId);
    return entId;
  }

  // would require either virtual function
  template <typename T>
  const std::vector<EntId> &getAllIds() {
    PackedMap<EntId, T> &componentMap = getComponentMap<T>();
    return componentMap.getKeys();
  }

  template <typename T>
  EntQueryResult getKeys() {
    const auto &results = getAllIds<T>();
    return EntQueryResult{*this, results};
  }

  template <typename T>
  EntQueryResult allOf();

  template <typename... Ts>
  typename std::enable_if<sizeof...(Ts) != 1, EntQueryResult>::type allOf();

  template <typename T>
  EntQueryResult anyOf();

  template <typename... Ts>
  typename std::enable_if<sizeof...(Ts) != 1, EntQueryResult>::type anyOf();

  template <typename... Ts>
  EntQueryResult noneOf();

  void retain(EntQueryId queryId) {
    if (queryId == NullQueryId) return;
    if (queryRefCount.find(queryId) != queryRefCount.end()) {
      ++queryRefCount.at(queryId);
    } else {
      queryRefCount[queryId] = 1;
    }
  }

  void release(EntQueryId queryId) {
    if (queryId == NullQueryId) return;
    if (queryRefCount.find(queryId) == queryRefCount.end()) {
      return;
    }
    auto &refCount = queryRefCount.at(queryId);

    // decrement ref count and erase if count = 0
    if (--refCount == 0) {
      queryRefCount.erase(queryId);
      queryIndexes.erase(queryId);
      // TODO not very efficient (especially if many open indexes)
      for (auto &kv : queryIdFromQuery) {
        if (kv.second == queryId) {
          queryIdFromQuery.erase(kv.first);
          break;
        }
      }
    }
  }

  // TODO rename EntId to ent
  // rename Ent To EntComponentAccessor?

  template <typename... Ts>
  typename std::enable_if<sizeof...(Ts) == 0>::type add(EntId entId) {
    updateTrackedQueries(entId);
  }

  template <typename T, typename... Ts>
  void add(EntId entId) {
    PackedMap<EntId, T> &componentMap = getComponentMap<T>();
    componentMap.add(entId);
    add<Ts...>(entId);
  }

  template <typename... Ts>
  typename std::enable_if<sizeof...(Ts) == 0>::type remove(EntId entId) {
    updateTrackedQueries(entId);
  }

  template <typename T, typename... Ts>
  void remove(EntId entId) {
    PackedMap<EntId, T> &componentMap = getComponentMap<T>();
    componentMap.remove(entId);
    remove<Ts...>(entId);
  }

  template <typename T>
  T &get(EntId entId) {
    PackedMap<EntId, T> &componentMap = getComponentMap<T>();
    return componentMap.get(entId);
  }

  template <typename T>
  ComponentType getComponentType() {
    const auto &typeIndex = std::type_index(typeid(T));

    // register this type of component if first time being observed
    if (componentTypeFromTypeIndex.find(typeIndex) == componentTypeFromTypeIndex.end()) {
      componentTypeFromTypeIndex[typeIndex] = nextComponentType;
      componentMaps.emplace(nextComponentType, PackedMap<EntId, T>{});
      PackedMap<EntId, T> &componentMap =
          std::any_cast<PackedMap<EntId, T> &>(componentMaps[nextComponentType]);
      componentSets.emplace(nextComponentType, componentMap.getKeySet());
      return nextComponentType++;
    }

    return componentTypeFromTypeIndex.at(typeIndex);
  }

  template <typename... Ts>
  typename std::enable_if<sizeof...(Ts) == 0>::type insertComponentTypes(
      std::unordered_set<ComponentType> &set) {}

  template <typename T, typename... Ts>
  void insertComponentTypes(std::unordered_set<ComponentType> &set) {
    auto componentType = getComponentType<T>();
    set.insert(componentType);
    insertComponentTypes<Ts...>(set);
  }

  template <typename T>
  PackedMap<EntId, T> &getComponentMap() {
    const auto componentType = getComponentType<T>();
    return std::any_cast<PackedMap<EntId, T> &>(componentMaps[componentType]);
  }

  EntQuery query();

  EntQueryResult getAllEnts() { return EntQueryResult{*this, allEntIds.getKeys()}; }

 private:
  EntId nextEntId = NullEntId + 1;
  ComponentType nextComponentType = 0;
  EntQueryId nextQueryId = NullQueryId + 1;

  const std::vector<EntId> emptyQueryResult{};
  PackedSet<EntId> allEntIds{};

  // track existing queries
  std::unordered_map<EntQuery__internal, EntQueryId> queryIdFromQuery{};
  std::unordered_map<EntQueryId, int> queryRefCount{};
  std::unordered_map<EntQueryId, PackedSet<EntId>> queryIndexes{};

  // tracking component types and registered components
  std::unordered_map<std::type_index, ComponentType> componentTypeFromTypeIndex{};
  std::unordered_map<ComponentType, std::any> componentMaps{};
  std::unordered_map<ComponentType, const PackedSet<EntId> &> componentSets{};

  void updateTrackedQueries(EntId entId) {
    for (auto &kv : queryIdFromQuery) {
      auto &query = kv.first;
      EntQueryId queryId = kv.second;
      auto &querySet = queryIndexes[queryId];
      bool wasMatch = querySet.contains(entId);
      bool isMatch = matches(query, entId);
      if (wasMatch && !isMatch) {
        querySet.removeAndPack(entId);
      } else if (!wasMatch && isMatch) {
        querySet.add(entId);
      }
    }
  }

  bool matches(const EntQuery__internal &query, const EntId entId) {
    for (const auto componentType : query.allOf) {
      auto &index = componentSets.at(componentType);
      if (!index.contains(entId)) {
        return false;
      }
    }
    for (const auto componentType : query.noneOf) {
      auto &index = componentSets.at(componentType);
      if (index.contains(entId)) {
        return false;
      }
    }

    if (query.anyOf.size() == 0) return true;

    for (const auto componentType : query.anyOf) {
      auto &index = componentSets.at(componentType);
      if (index.contains(entId)) {
        return true;
      }
    }
    return false;
  }

  // slow process, should be pre-down when possible
  EntQueryId getQueryId(const EntQuery__internal &query) {
    if (query.hashValue == 0) {
      return NullQueryId;
    }

    // build index for query if one does not already exist
    if (queryIdFromQuery.find(query) == queryIdFromQuery.end()) {
      const EntQueryId queryId = nextQueryId;
      nextQueryId += 1;

      queryIdFromQuery[query] = queryId;
      queryIndexes[queryId] = PackedSet<EntId>{};
      PackedSet<EntId> &index = queryIndexes[queryId];

      // build index
      for (const auto entId : allEntIds.getKeys()) {
        if (matches(query, entId)) {
          index.add(entId);
        }
      }
    }

    return queryIdFromQuery[query];
  }

  EntQueryResult getQueryResult(const EntQuery__internal &query) {
    auto queryId = getQueryId(query);

    // return null query result for null query
    if (queryId == 0) {
      return EntQueryResult(*this, emptyQueryResult, 0);
    }

    auto &index = queryIndexes[queryId];
    return EntQueryResult(*this, index.getKeys(), queryId);
  }

  friend class EntQuery;
};

class EntQuery {
 public:
  template <typename... Ts>
  EntQuery &allOf() {
    allOfTypes.clear();
    entManager.insertComponentTypes<Ts...>(allOfTypes);
    return *this;
  }

  template <typename... Ts>
  EntQuery &anyOf() {
    anyOfTypes.clear();
    entManager.insertComponentTypes<Ts...>(anyOfTypes);
    return *this;
  }

  template <typename... Ts>
  EntQuery &noneOf() {
    noneOfTypes.clear();
    entManager.insertComponentTypes<Ts...>(noneOfTypes);
    return *this;
  }

  EntQueryResult result() {
    EntQuery__internal queryInternal = build();
    return entManager.getQueryResult(queryInternal);
  }

 private:
  EntManager &entManager;
  std::unordered_set<ComponentType> allOfTypes;
  std::unordered_set<ComponentType> anyOfTypes;
  std::unordered_set<ComponentType> noneOfTypes;

  EntQuery(EntManager &manager) : entManager{manager} {}

  EntQuery__internal build() {
    // calculate hashcode
    size_t hashValue = 0;
    for (const auto componentType : allOfTypes) lve::hashCombine(hashValue, componentType);
    for (const auto componentType : anyOfTypes) lve::hashCombine(hashValue, componentType);
    for (const auto componentType : noneOfTypes) lve::hashCombine(hashValue, componentType);
    return EntQuery__internal{hashValue, allOfTypes, anyOfTypes, noneOfTypes};
  }

  friend class EntManager;
};

template <typename T>
T &Ent::get() {
  return entManager.get<T>(id);
}

template <typename T>
EntQueryResult EntManager::allOf() {
  const PackedMap<EntId, T> &componentMap = getComponentMap<T>();
  return EntQueryResult(*this, componentMap.getKeys());
}

template <typename... Ts>
typename std::enable_if<sizeof...(Ts) != 1, EntQueryResult>::type EntManager::allOf() {
  EntQuery__internal query = EntQuery(*this).allOf<Ts...>().build();
  return getQueryResult(query);
}

template <typename T>
EntQueryResult EntManager::anyOf() {
  const PackedMap<EntId, T> &componentMap = getComponentMap<T>();
  return EntQueryResult(*this, componentMap.getKeys());
}

template <typename... Ts>
typename std::enable_if<sizeof...(Ts) != 1, EntQueryResult>::type EntManager::anyOf() {
  EntQuery__internal query = EntQuery(*this).anyOf<Ts...>().build();
  return getQueryResult(query);
}

template <typename... Ts>
EntQueryResult EntManager::noneOf() {
  EntQuery__internal query = EntQuery(*this).noneOf<Ts...>().build();
  return getQueryResult(query);
}

inline EntQuery EntManager::query() { return EntQuery{*this}; }

inline EntQueryResult::EntQueryResult(
    EntManager &manager, const std::vector<EntId> &entIds, EntQueryId queryId)
    : entManager{manager}, results{entIds}, id{queryId} {
  entManager.retain(id);
}
inline EntQueryResult::~EntQueryResult() { entManager.release(id); }

// copy constructor
inline EntQueryResult::EntQueryResult(const EntQueryResult &q)
    : entManager{q.entManager}, results{q.results}, id{q.id} {
  entManager.retain(id);
}

// move assignment (deleted)
// copy assignment (deleted)

// move constructor
inline EntQueryResult::EntQueryResult(EntQueryResult &&q) noexcept
    : entManager{q.entManager}, results{q.results}, id{q.id} {
  entManager.retain(id);
}

}  // namespace lve
