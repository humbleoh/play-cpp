#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <variant>

template<typename... Ts>
class polycomp3 {
public:
  template<typename T>
  bool operator()(const T* e1, const T* e2) const
  {
    auto v = visitor{};
    e1->accept(v);
    e2->accept(v);
    return v.result;
  }

  template<typename T>
  bool operator()(const std::shared_ptr<T> e1, const std::shared_ptr<T> e2) const
  {
    auto v = visitor{};
    e1->accept(v);
    e2->accept(v);
    return v.result;
  }

  // std::monostate 在这里的作用是作为模版参数占位符，因为 c++ 不允许嵌套类模版
  // 的显式全特化，而我们这里的嵌套类模版的结束类是一个全特化的类模版。
  template<std::monostate, typename... TDs>
  struct visitor_impl;

  template<std::monostate PH, typename TD, typename... TDs>
  struct visitor_impl<PH, TD, TDs...> : visitor_impl<PH, TDs...> {
    using base = visitor_impl<PH, TDs...>;
    using base::first;
    using base::rank;
    using base::result;
    using base::visit;

    void visit(const TD& e)
    {
      if (!first) {
        first = &e;
        rank = sizeof...(TDs);
      } else if (rank > sizeof...(TDs)) {
        result = false;
      } else if (rank < sizeof...(TDs)) {
        result = true;
      } else {
        auto ptr = static_cast<const TD*>(first);
        result = (*ptr < e);
      }
    }
  };

  template<std::monostate PH>
  struct visitor_impl<PH> {
    void visit(); // 无需定义，只是用来 workaround 上面的 base::visit 形式问题。
    const void* first { nullptr }; // 二元比较操作的左元素
    int rank { 0 }; // 左元素的优先级
    bool result { false }; // 比较操作结果
  };

  using visitor = visitor_impl<std::monostate{}, Ts...>;

  class visit_dispatcher_interface {
  public:
    virtual void accept(visitor& v) const = 0;
  };

  // 很难解耦元素访问 interface 与 dispatcher  interface，因为 set 指定的是元素
  // 访问 interface，而 set 的 comparator 要求 dispatcher interface。所以，我们
  // 必须强制元素访问 interface 也必须是 dispatcher interface，进而引入了虚继承
  template<typename Acceptor>
  class visit_dispatcher : virtual public visit_dispatcher_interface {
  public:
    void accept(visitor& v) const override
    {
      auto ptr = static_cast<const Acceptor*>(this);
      v.visit(*ptr);
    }
  };
};

class teacher;
class student;

using staff_comp = polycomp3<teacher, student>;
using staff_visitor = staff_comp::visitor;
template<typename Acceptor>
using staff_visit_dispatcher = staff_comp::visit_dispatcher<Acceptor>;
using staff_visit_dispatcher_interface = staff_comp::visit_dispatcher_interface; 

class staff : virtual public staff_visit_dispatcher_interface {
public:
  virtual void print() const = 0;
};

class teacher
  : public staff
  , public staff_visit_dispatcher<teacher> {
public:
  teacher(int id)
    : m_id { id }
  {
  }

  bool operator<(const teacher& r) const
  {
    return m_id < r.m_id;
  }

  void print() const override
  {
    std::cout << "teacher:" << m_id << std::endl;
  }

private:
  int m_id;
};

class student
  : public staff
  , public staff_visit_dispatcher<student> {
public:
  student(const std::string& s)
    : m_name { s }
  {
  }

  bool operator<(const student& r) const
  {
    return m_name < r.m_name; 
  }

  void print() const override
  {
    std::cout << "student:" << m_name << std::endl;
  }

private:
  std::string m_name;
};

int main(int argc, char* argv[])
{
  std::set<std::shared_ptr<staff>, staff_comp> s2;
  s2.emplace(new teacher{1});
  s2.emplace(new teacher{3});
  s2.emplace(new teacher{2});
  s2.emplace(new student{"a"});
  s2.emplace(new student{"c"});
  s2.emplace(new student{"b"});
  for (auto&& e: s2) {
    e->print();
  }
  return 0;
}
