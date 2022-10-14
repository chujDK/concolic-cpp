#pragma once
#include <iostream>
#include <memory>

#include "z3++.h"
#include "util.h"

class Ast {
 public:
  virtual std::ostream& dump(std::ostream& o) const;
  [[nodiscard]] virtual z3::expr _z3expr() const = 0;

 private:
};

using AstPtr = std::shared_ptr<Ast>;

AstPtr make_intvar();
AstPtr make_intvar(const char* var_name);
AstPtr make_intconst(const int x);

class AstUnaryOp : public Ast {
 public:
  [[nodiscard]] z3::expr _z3expr() const override = 0;

  AstUnaryOp(z3::expr opr) : operand_(std::move(opr)) {}

 protected:
  z3::expr operand_;
};

class AstConst : public Ast {
 public:
  [[nodiscard]] z3::expr _z3expr() const override;

  AstConst(z3::expr const_val) : const_(std::move(const_val)) {}

 protected:
  z3::expr const_;
};

class AstConstInt : public AstConst {
 public:
  AstConstInt(int const_val) : AstConst(z3ctx->int_val(const_val)) {}

 protected:
};

class AstInt : public Ast {
 public:
  [[nodiscard]] z3::expr _z3expr() const override;

  static AstPtr make_int() {
    auto var_name = generate_uniq_varname();
    return make_int(var_name.c_str());
  }

  static AstPtr make_int(const char* var_name) {
    return std::make_shared<AstInt>(z3ctx->int_const(var_name));
  }

  AstInt(z3::expr id) : id_(std::move(id)) {}

 protected:
  z3::expr id_;
};

class AstBinaryOp : public Ast {
 public:
  [[nodiscard]] z3::expr _z3expr() const override = 0;

  AstBinaryOp(z3::expr lhs, z3::expr rhs)
      : lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

 protected:
  z3::expr lhs_;
  z3::expr rhs_;
};

class AstAdd : public AstBinaryOp {
 public:
  [[nodiscard]] z3::expr _z3expr() const override;

  static AstPtr make_add(const AstPtr& a, const AstPtr& b) {
    return std::make_shared<AstAdd>(a->_z3expr(), b->_z3expr());
  }

  using AstBinaryOp::AstBinaryOp;

 protected:
};

std::ostream& operator<<(std::ostream& o, const Ast& ast);
