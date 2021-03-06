#ifndef LGPP_TOK_HPP
#define LGPP_TOK_HPP

#include <deque>
#include <ostream>

#include "lgpp/env.hpp"
#include "lgpp/pos.hpp"

namespace lgpp {
  struct Parser;
  struct Thread;
  struct Tok;

  using Toque = deque<Tok>;

  namespace toks {
    template <typename T>
    void compile(const Tok& tok, const T& imp, Toque& in, Thread& out, Env& env) {}

    template <typename T>
    void dump(const Tok& tok, const T& imp, ostream& out);
  }

  struct Tok {
    struct Imp {
      virtual ~Imp() = default;
      virtual void compile(const Tok& tok, Toque& in, Thread& out, Env& env) const = 0;
      virtual void dump(const Tok& tok, ostream& out) const = 0;
    };

    template <typename T>
    struct TImp: Imp {
      TImp(T imp): imp(move(imp)) { }

      void compile(const Tok& tok, Toque& in, Thread& out, Env& env) const override {
	toks::compile(tok, imp, in, out, env);
      }
      
      void dump(const Tok& tok, ostream& out) const override { toks::dump(tok, imp, out); }

      T imp;
    };

    template <typename T>
    Tok(Pos pos, T imp): pos(pos), imp(make_shared<TImp<T>>(move(imp))) { } 

    template <typename T>
    const T& as() const { return dynamic_cast<const TImp<T>&>(*imp).imp; }

    template <typename T>
    const T* try_as() const {
      auto timp = dynamic_cast<const TImp<T> *>(imp.get());
      return timp ? &timp->imp : nullptr;
    }

    Pos pos;
    shared_ptr<const Imp> imp;
  };

  inline void compile(const Tok &tok, Toque& in, Thread& out, Env& env) { return tok.imp->compile(tok, in, out, env); }

  inline void dump(const Tok &tok, ostream& out) { return tok.imp->dump(tok, out); }

  inline ostream &operator<<(ostream &out, const Tok &t) {
    t.imp->dump(t, out);
    return out;
  }

  template <typename T, typename...Args>
  const Tok& push(Toque& in, Pos pos, Args&&...args) {
    return in.emplace_back(pos, T(forward<Args>(args)...));
  }
  
  inline optional<Tok> peek(const Toque& in) {
    return in.empty() ? nullopt : make_optional(in.front());
  }

  inline Tok pop(Toque& in) {
    if (in.empty()) { throw runtime_error("Missing token"); }
    Tok t = in.front();
    in.pop_front();
    return t;
  }

  inline Tok pop_back(Toque& in) {
    if (in.empty()) { throw runtime_error("Missing token"); }
    Tok t = in.back();
    in.pop_back();
    return t;
  }
}

#endif
