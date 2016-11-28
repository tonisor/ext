#pragma once

#include <vector>
#include <map>
#include <algorithm>
#include <functional>

template <typename key_t, typename value_t>
class LRUCache
{
public:
	LRUCache(std::size_t capacity) : c(capacity), t(0) {}
	void set(const key_t& key, const value_t& value)
	{
		auto it = m.lower_bound(key);
		if (it != m.end() && !(key < it->first))
		{
			it->second.value = value;
			make_mru(it->second);
		}
		else
		{
			if (v.size() < c)
			{
				Node n{ key, value, t++ };
				it = m.emplace_hint(it, std::move(key), std::move(n));
				v.emplace_back(it->second);
			}
			else
			{
				m.erase(v.begin()->get().key);
				Node n{ key, value, 0 };
				auto itx = m.emplace(std::move(key), std::move(n));
				*v.begin() = itx.first->second;
				make_mru(v.begin());
			}
		}
	}
	const value_t& get(const key_t& key)
	{
		auto it = m.find(key);
		if (it != m.end())
		{
			make_mru(it->second);
			return it->second.value;
		}
		else
		{
			return value_t();
		}
	}

protected:
	struct Node
	{
		key_t key;
		value_t value;
		int timestamp;
	};
	struct sc { bool operator()(const Node& lhs, const Node& rhs) const { return lhs.key < rhs.key; } };
	struct vc {
		bool operator()(const std::reference_wrapper<Node> lhs, const Node& rhs) const {
			return lhs.get().timestamp < rhs.timestamp;
		}
	};
	std::vector<std::reference_wrapper<Node>> v;
	std::map<key_t, Node> m;
	std::size_t c;
	int t;

	void make_mru(const Node& n)
	{
		auto it = std::lower_bound(v.begin(), v.end(), n, vc());
		make_mru(it);
	}

	void make_mru(decltype(v.begin()) it)
	{
		it->get().timestamp = t++;
		std::rotate(it, it + 1, v.end());
	}
};

template <typename key_t, typename value_t>
class VectorLRUCache
{
public:
	VectorLRUCache(std::size_t capacity) : c(capacity) {}
	void set(const key_t& key, const value_t& value)
	{
		auto it = std::find_if(v.begin(), v.end(), [&key](const auto& node) { return node.key == key; });
		if (it != v.end())
		{
			it->value = value;
			return;
		}
		if (v.size() < c)
		{
			v.emplace_back(key, value);
		}
		else
		{
			std::move(v.begin() + 1, v.end(), v.begin());
			auto it = v.end() - 1;
			it->key = key;
			it->value = value;
		}
	}
	const value_t& get(const key_t& key)
	{
		auto it = std::find_if(v.begin(), v.end(), [&key](const auto& node) { return node.key == key; });
		if (it != v.end())
		{
			std::rotate(it, it + 1, v.end());
			return (v.cend() - 1)->value;
		}
		return value_t();
	}

protected:
	struct node_t
	{
		node_t(const key_t& k, const value_t& v) : key(k), value(v) {}
		key_t key;
		value_t value;
	};
	std::vector<node_t> v;
	std::size_t c;
};

template <typename key_t, typename value_t>
class VectorLRUCache2
{
public:
	VectorLRUCache2(std::size_t capacity) : c(capacity) { s.reserve(c); }
	void set(const key_t& key, const value_t& value)
	{
		auto it = std::find_if(v.begin(), v.end(), [&key](const auto& node) { return node.key == key; });
		if (it != v.end())
		{
			it->value_ref.get() = value;
			return;
		}
		if (v.size() < c)
		{
			s.emplace_back(value);
			v.emplace_back(key, *(s.end() - 1));
		}
		else
		{
			auto ref = v.begin()->value_ref;
			ref.get() = value;
			std::move(v.begin() + 1, v.end(), v.begin());
			auto it = v.end() - 1;
			it->key = key;
			it->value_ref = ref;
		}
	}
	const value_t& get(const key_t& key)
	{
		auto it = std::find_if(v.begin(), v.end(), [&key](const auto& node) { return node.key == key; });
		if (it != v.end())
		{
			std::rotate(it, it + 1, v.end());
			return (v.cend() - 1)->value_ref.get();
		}
		return value_t();
	}

protected:
	struct node_t
	{
		node_t(const key_t& k, value_t& v) : key(k), value_ref(v) {}
		key_t key;
		std::reference_wrapper<value_t> value_ref;
	};
	std::vector<node_t> v;
	std::vector<value_t> s;
	std::size_t c;
};

template <typename key_t, typename value_t>
class VectorLRUCache3
{
public:
	VectorLRUCache3(std::size_t capacity) : c(capacity) {}
	void set(const key_t& key, const value_t& value)
	{
		auto it = std::find_if(v.begin(), v.end(), [&key](const auto& node) { return node.key == key; });
		if (it != v.end())
		{
			*it->value_ptr = value;
			return;
		}
		if (v.size() < c)
		{
			if (s.size() < s.capacity())
			{
				s.emplace_back(value);
			}
			else
			{
				auto s_old_ptr = &s[0];
				s.emplace_back(value);
				auto ptr_diff = &s[0] - s_old_ptr;
				if (ptr_diff != 0)
				{
					for (auto& node : v)
					{
						node.value_ptr += ptr_diff;
					}
				}
			}
			v.emplace_back(key, &*(s.end() - 1));
		}
		else
		{
			auto ptr = v.begin()->value_ptr;
			*ptr = value;
			std::move(v.begin() + 1, v.end(), v.begin());
			auto it = v.end() - 1;
			it->key = key;
			it->value_ptr = ptr;
		}
	}
	const value_t& get(const key_t& key)
	{
		auto it = std::find_if(v.begin(), v.end(), [&key](const auto& node) { return node.key == key; });
		if (it != v.end())
		{
			std::rotate(it, it + 1, v.end());
			return *(v.cend() - 1)->value_ptr;
		}
		return value_t();
	}

protected:
	struct node_t
	{
		node_t(const key_t& k, value_t* v) : key(k), value_ptr(v) {}
		key_t key;
		value_t* value_ptr;
	};
	std::vector<node_t> v;
	std::vector<value_t> s;
	std::size_t c;
};
