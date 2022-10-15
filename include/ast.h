#pragma once
#include <iostream>
#include <memory>

#include "z3++.h"
#include "util.h"

class Ast {
 public:
  virtual std::ostream& dump(std::ostream& o) const;
  [[nodiscard]] virtual z3::expr _z3expr() const = 0;
  virtual ~Ast()                                 = default;

 private:
};

using AstPtr = std::shared_ptr<Ast>;

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
  // TODO: change int to bit vector
  AstConstInt(int const_val) : AstConst(z3ctx->int_val(const_val)) {}

  static AstPtr make_const_int(int x) {
    return std::make_shared<AstConstInt>(x);
  }

 protected:
};

class AstConstBool : public AstConst {
 public:
  AstConstBool(bool b) : AstConst(z3ctx->bool_val(b)) {}

  static AstPtr make_const_bool() { return make_const_bool(true); }
  static AstPtr make_const_bool(bool b) {
    return std::make_shared<AstConstBool>(b);
  }
};

class AstId : public Ast {
 public:
  [[nodiscard]] z3::expr _z3expr() const override;
  AstId(z3::expr id) : id_(std::move(id)) {}

 protected:
  z3::expr id_;
};

class AstInt : public AstId {
 public:
  using AstId::AstId;
  static AstPtr make_int(const char* var_name) {
    // TODO: change int to bit vector
    return std::make_shared<AstInt>(z3ctx->int_const(var_name));
  }
};

class AstBool : public AstId {
 public:
  static AstPtr make_bool() {
    auto var_name = generate_uniq_varname();
    return make_bool(var_name.c_str());
  }

  using AstId::AstId;
  static AstPtr make_bool(const char* var_name) {
    return std::make_shared<AstBool>(z3ctx->bool_const(var_name));
  }
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

class AstLogicAnd : public AstBinaryOp {
  // &&
 public:
  [[nodiscard]] z3::expr _z3expr() const override;
  static AstPtr make_logic_and(const AstPtr& a, const AstPtr& b) {
    return std::make_shared<AstLogicAnd>(a->_z3expr(), b->_z3expr());
  }

  using AstBinaryOp::AstBinaryOp;

 private:
};

class AstEq : public AstBinaryOp {
 public:
  [[nodiscard]] z3::expr _z3expr() const override;

  static AstPtr make_eq(const AstPtr& a, const AstPtr& b) {
    return std::make_shared<AstEq>(a->_z3expr(), b->_z3expr());
  }

  using AstBinaryOp::AstBinaryOp;

 protected:
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
